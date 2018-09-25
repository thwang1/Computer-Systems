// Simulates a mama kangaroo waiting for her joeys using UNIX signaling.

#include	"kangarooHeaders.h"

//  PURPOSE:  To hold the random number seed the user requested.
int		randomNumSeed;

//  PURPOSE:  To hold the number of joeys the user requested to make.
int		numJoeys		= MIN_NUM_LEGAL_JOEYS - 1;

//  PURPOSE:  To hold the number of joeys that have not yet found their way
//	back to their mama's pouch.
int		numActiveJoeys		= 0;

//  PURPOSE:  To hold the process ids of the joey processes.
pid_t*		joeyPidArray		= NULL;

//  PURPOSE:  To hold the process id of the mall process.
pid_t		mallPid			= -1;

//  PURPOSE:  To initialize 'numJoeys' from the 'argc' commamnd line
//	parameters give in 'argv[]'.  No return value.
void	initializeNumJoeys	(int		argc,
				 char*		argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Initialize 'numJoeys':
  char		line[LINE_LEN];

  if  (argc > MAMAS_NUM_JOEYS_CMD_LINE_INDEX)
    numJoeys	= atoi(argv[MAMAS_NUM_JOEYS_CMD_LINE_INDEX]); // <-- REPLACE with code that gets integer from 'argv[MAMAS_NUM_JOEYS_CMD_LINE_INDEX]'

  while  (numJoeys < MIN_NUM_LEGAL_JOEYS)
  {
    printf("Please enter the number of joeys that"
    	   " get lost in the shopping mall: "
	  );
    fgets(line,LINE_LEN,stdin);
    numJoeys	= atoi(line); // <-- REPLACE with code that gets integer from 'line'
  }

  //  III.  Finished:
}


//  PURPOSE:  To initialize 'randomNumSeed' from the 'argc' commamnd line
//	parameters give in 'argv[]'.  No return value.
void		initializeRandomNumSeed	(int		argc,
					 char*		argv[]
					)
{
  //  I.  Application validity check:

  //  II.  Initialize 'randomNumSeed':
  char		line[LINE_LEN];

  if  (argc > MAMAS_RAND_SEED_CMD_LINE_INDEX)
    randomNumSeed	= atoi(argv[MAMAS_RAND_SEED_CMD_LINE_INDEX]); // <-- REPLACE with code that gets integer from 'argv[MAMAS_RAND_SEED_CMD_LINE_INDEX]'
  else
  {
    printf("Please enter the random number seed: ");
    fgets(line,LINE_LEN,stdin);
    randomNumSeed	= atoi(line); // <-- REPLACE with code that gets integer from 'line'
  }

  //  III.  Finished:
}


//  PURPOSE:  To handle signal 'SIGCHLD'.  'sig' is ignored because it is known
//	to be 'SIGCHLD'.  No return value.
void		sigChldHandler		(int		sig
					)
{
  //  I.  Application validity check:

  //  II.  Search for the joey with process id equal to 'childPid':
  int	status;
  pid_t	childPid;
  int	joey;


  //  YOUR CODE HERE that uses a 'while()' loop testing 'waitpid()' to
  //   get *all* finished children.  Each should be tested against
  //   'joeyPidArray[joey]' to see if it is that particular joey.
  //  Decrements 'numActiveJoeys' for each stopped joey.
  for(joey = 0; joey < numJoeys; joey++)
  {
	while( (childPid=waitpid(joeyPidArray[joey],&status,0)) > 0) {
		if(WIFEXITED(status) != 0) {
			printf("Mama: Joey #%d just entered my pouch!\n", joey);
			numActiveJoeys--;
		}
	}
  }

  //  III.  Finished:
}


//  PURPOSE:  To install 'sigChldHandler()' as the SIGCHLD handler, which is
//	able to distinguish among which of the children has finished.  No
//	parameters.  No return value.
void		installSigChldHandler	()
{
  //  I.  Application validity check:

  //  II.  Install the handler:
  //  YOUR CODE HERE
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  act.sa_handler = sigChldHandler;
  sigaction(SIGCHLD, &act, NULL);

  //  III.  Finished:
}


//  PURPOSE:  To start the mall process.  No parameters.  No return value.
void		startMallProcess	()
{
  //  I.  Application validity check:

  //  II.  Start the mall process:
  mallPid	= fork(); // <-- REPLACE with code that returns pid of new process

  if  (mallPid == -1)
  {
    fprintf(stderr,"Can't make any new processes! :(\n");
    exit(EXIT_FAILURE);
  }

  if  (mallPid == 0)
  {
    char	numJoeysStr[LINE_LEN];
    char	randNumSeedStr[LINE_LEN];

    snprintf(numJoeysStr,LINE_LEN,"%d",numJoeys);
    snprintf(randNumSeedStr,LINE_LEN,"%d",randomNumSeed);
    //  YOUR CODE HERE that runs 'MALL_PROG_NAME' with command line parameters 'numJoeysStr' and 'randNumSeedStr'.
    execl(MALL_PROG_NAME,MALL_PROG_NAME,numJoeysStr, randNumSeedStr,NULL);
    fprintf(stderr,"Could not find the mall! :(\n");
    exit(EXIT_FAILURE);
  }

  //  III.  Finished:
}


//  PURPOSE:  To start the joey processes.  No parameters.  No return value.
void		startJoeyProcesses	()
{
  //  I.  Application validity check:

  //  II.  Start the joey processes:
  int		joey;
  joeyPidArray	= (pid_t*)calloc(sizeof(pid_t),numJoeys);

  for  (joey = 0;  joey < numJoeys;  joey++)
  {
    joeyPidArray[joey]	= fork(); // <-- REPLACE with code that returns pid of new process

    if  (joeyPidArray[joey] == -1)
    {
      fprintf(stderr,"Can't make any new processes! :(\n");
      exit(EXIT_FAILURE);
    }

    if  (joeyPidArray[joey] == 0)
    {
      char	indexStr[LINE_LEN];
      char	mallPidStr[LINE_LEN];
      char	randNumSeedStr[LINE_LEN];

      snprintf(indexStr,LINE_LEN,"%d",joey);
      snprintf(mallPidStr,LINE_LEN,"%d",mallPid);
      snprintf(randNumSeedStr,LINE_LEN,"%d",randomNumSeed + joey);
      //  YOUR CODE HERE that runs 'JOEY_PROG_NAME' with command line parameters 'indexStr', 'mallPidStr' and 'randNumSeedStr'.
      execl(JOEY_PROG_NAME,JOEY_PROG_NAME,indexStr,mallPidStr,randNumSeedStr,NULL);
      fprintf(stderr,"Could not find the joey! :(\n");
      exit(EXIT_FAILURE);
    }

    numActiveJoeys++;
  }

  //  III.  Finished:
}


int	main	(int		argc,
		 char*		argv[]
		)
{
  //  I.  Application validity check:

  //  II.  Do simulation:

  //  II.A.  Get simulation parameters:
  initializeNumJoeys(argc,argv);
  initializeRandomNumSeed(argc,argv);

  //  II.B.  Setup handlers:
  installSigChldHandler();

  //  II.C.  Initialize processes:
  startMallProcess();
  sleep(1);
  startJoeyProcesses();

  //  II.D.  To do the simulation by waiting until all joeys have returned:
  while  (numActiveJoeys > 0)
    sleep(1);

  //  II.E.  Clean up after game:
  //  II.E.1.  Stop the mall process:
  // YOUR CODE HERE to send 'SIGINT' to 'mallPid'
  kill(mallPid, SIGINT);  

  //  II.E.2.  Release mem:
  free(joeyPidArray);

  //  III.  Finished:
  return(EXIT_SUCCESS);
}
