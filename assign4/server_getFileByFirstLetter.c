// Defines a C program that waits for a client to connect, gets a letter 
// from the client, and looks for a file in the current directory that 
// begins with that letter. If it finds such a file then it sends the 
// length of the file back as a network-endian 32-bit unsigned integer, 
// followed by the text	of the file. Sends back the appropriate error 
// integer code	otherwise.

//	Compile with:
//	$ gcc server_getFileByFirstLetter.c -o server -lpthread
//

/*---		Header file inclusion					---*/

#include	"getFileByFirstLetter.h"
#include	<dirent.h>		// For opendir(), readdir(), closedir()
#include	<pthread.h>		// For pthread_create(), etc.

const int	LO_LEGAL_PORT		= 1025;
const int	HI_LEGAL_PORT		= 65535;
const int	ERROR_FD		= -1;
const int	NUM_CLIENTS_TO_SERVE	= 4;


//  PURPOSE:  To attempt to create and return a file-descriptor for listening
//	to the OS telling this server when a client process has connect()-ed
//	to 'port'.  Returns that file-descriptor, or 'ERROR_FD' on failure.
int		getServerFileDescriptor
				(int		port,
				 const char*	progName
				)
{
  //  I.  Application validity check:
  if  (progName == NULL)
  {
    fprintf(stderr,"BUG: NULL ptr to getServerFileDescriptor().\n");
    exit(EXIT_FAILURE);
  }

  //  II.  Attempt to get socket file descriptor and bind it to 'port':
  //  II.A.  Create a socket
  int socketDescriptor = socket(AF_INET, // AF_INET domain
			        SOCK_STREAM, // Reliable TCP
			        0);

  if  (socketDescriptor < 0)
  {
    perror(progName);
    return(ERROR_FD);
  }

  //  II.B.  Attempt to bind 'socketDescriptor' to 'port':
  //  II.B.1.  We'll fill in this datastruct
  struct sockaddr_in socketInfo;

  //  II.B.2.  Fill socketInfo with 0's
  memset(&socketInfo,'\0',sizeof(socketInfo));

  //  II.B.3.  Use TCP/IP:
  socketInfo.sin_family = AF_INET;

  //  II.B.4.  Tell port in network endian with htons()
  socketInfo.sin_port = htons(port);

  //  II.B.5.  Allow machine to connect to this service
  socketInfo.sin_addr.s_addr = INADDR_ANY;

  //  II.B.6.  Try to bind socket with port and other specifications
  int status = bind(socketDescriptor, // from socket()
		    (struct sockaddr*)&socketInfo,
		    sizeof(socketInfo)
		   );

  if  (status < 0)
  {
    perror(progName);
    return(ERROR_FD);
  }

  //  II.B.6.  Set OS queue length:
  listen(socketDescriptor,5);

  //  III.  Finished:
  return(socketDescriptor);
}


//  PURPOSE:  To ask the user which port to attempt to monopolize, and to return
//	entered port number.
int		getPort		()
{
  //  I.  Application validity check:

  //  II.  Get port number
  int	port;

  do
  {
    char	buffer[BUFFER_LEN];

    printf("Please enter port number to monopolize [%d-%d]: ",
	   LO_LEGAL_PORT,HI_LEGAL_PORT
	  );
    fgets(buffer,BUFFER_LEN,stdin);
    port = strtol(buffer,NULL,10);
  }
  while  ( (port < LO_LEGAL_PORT)  ||  (port > HI_LEGAL_PORT) );

  //  III.  Finished:
  return(port);
}



//  PURPOSE:  To do the work of handling the client.  Communication with the
//	client take place using file-descriptor pointed to by '*vPtr'.  Returns
//	'NULL'.
void*		handleClient	(void*	vPtr
				)
{
  //  I.  Application validity check:

  //  II.  Handle the client:
  //  YOUR CODE HERE
  int connfd 	= *((int *)vPtr);
  int tid 	= *((int *)vPtr+1);
  
  free(vPtr);
  
  char buffer[BUFFER_LEN];

  read(connfd,buffer,1);  

  struct dirent *direntPtr;
  DIR 		*dirPtr;

  dirPtr = opendir(".");
  if(dirPtr == NULL) {
	uint32_t netCode = htonl(CANT_READ_DIR_CODE);
	write(connfd,&netCode,sizeof(netCode));
	printf("id %d: Cannot read directory\n", tid);
	close(connfd);
	return NULL;
  }
  
  while((direntPtr = readdir(dirPtr)) != NULL) {
	char* fileName = direntPtr->d_name;
	if(fileName[0] == buffer[0]) {
		int fd = open(fileName,O_RDONLY,0);
		if(fd < 0) {
			uint32_t netCode = htonl(CANT_READ_FILE_CODE);
			write(connfd,&netCode,sizeof(netCode));
			printf("id %d: Cannot read file %s\n",tid,fileName);
			close(connfd);
			closedir(dirPtr);
			return NULL; 
		}

		struct stat fileStat;
		stat(fileName,&fileStat);
		if(S_ISDIR(fileStat.st_mode)) 
			break;
		uint32_t fileSize = htonl((uint32_t)fileStat.st_size);
		printf("id %d: Sending %s, %d bytes\n",tid,fileName,fileSize);
		write(connfd,&fileSize,sizeof(fileSize));

		unsigned int 	totalBytesRead = 0;
		int		bytesRead;
		char		buf[BUFFER_LEN];
		while((totalBytesRead < fileStat.st_size) && 
		      ((bytesRead = read(fd,buf,BUFFER_LEN)) > 0)
		     )
		{
			write(connfd,buf,sizeof(buf));
			buf[bytesRead] = '\0';
			printf("%s",buf);
			totalBytesRead += (unsigned int) bytesRead;
		}
		
		close(direntPtr->d_name);
		closedir(dirPtr);
		close(connfd);
		return NULL;
	} 
  }

  uint32_t netCode = htonl(NO_MATCH_CODE);
  write(connfd,&netCode,sizeof(netCode));
  printf("id %d: No matching file\n", tid);
  closedir(dirPtr);
  close(connfd);

  //  III.  Finished:
  return(NULL);
}


//  PURPOSE:  To serve the clients using file-descriptor 'listenFd' to tell
//	when a client has connected.  Each client is handled by its own child
//	process.  Both the parent and the child processes close unnecesary
//	file-descriptorors.  Serves 'NUM_CLIENTS_TO_SERVE' clients, then quits.
//	No return value.
void		doServer	(int  	 	listenFd
				)
{
  //  I.  Application validity check:
  if  (listenFd < 0)
  {
    fprintf(stderr,"Illegal file-descriptor to doServer()\n");
    exit(EXIT_FAILURE);
  }

  //  II.  Do the work of the server:
  int			i;
  pthread_t		tId;
  pthread_attr_t	tAttr;

  pthread_attr_init(&tAttr);
  pthread_attr_setdetachstate(&tAttr,PTHREAD_CREATE_DETACHED);

  for  (i = 0;  i < NUM_CLIENTS_TO_SERVE;  i++)
  {
    //  YOUR CODE HERE
    socklen_t clientlen 	= sizeof(struct sockaddr_storage);
    struct sockaddr_storage 	clientaddr;
    int* fdpAndTid		= (int *) malloc(sizeof(int));
    fdpAndTid[0]		= accept(listenFd,(struct sockaddr *) &clientaddr,&clientlen);
    fdpAndTid[1]		= i;
    if(i == (NUM_CLIENTS_TO_SERVE-1))
	pthread_create(&tId,NULL,handleClient,fdpAndTid);
    else
	pthread_create(&tId,&tAttr,handleClient,fdpAndTid);
  }

  pthread_join(tId,NULL);
  pthread_attr_destroy(&tAttr);

  //  III.  Finished:
}


//  PURPOSE:  To oversee the main work of the server.  Ignores 'argc' but
//	uses 'argv[0]' as the name of the program.  Returns 'EXIT_SUCCESS' to
//	OS on success or 'EXIT_FAILURE' otherwise.
int		main		(int		argc,
				 char*		argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Do server:
  int	port		= getPort();
  int	socketFd	= getServerFileDescriptor(port,argv[0]);

  doServer(socketFd);

  //  III.  Finished:
  return(EXIT_SUCCESS);
}
