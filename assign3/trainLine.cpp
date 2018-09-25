// Defines code the implements 2 Station instances, and some number of Train objects that transit between them.
// Compile with: $ g++ trainLine.cpp -o trainLine -lpthread -g

#include        <cstdlib>
#include		<string>
#include		<iostream>
#include        <unistd.h>
#include        <pthread.h>
#include        "Train.h"
#include        "Station.h"

const int	NUM_TRAINS		= 4; 
const int	NUM_NORTHERNLY_TRAINS	= NUM_TRAINS / 2;
const int	NUM_SOUTHERNLY_TRAINS	= NUM_TRAINS - NUM_NORTHERNLY_TRAINS;
const int       NUM_LOOPS		= 16;
const char*	TRAIN_NAME_ARRAY[NUM_TRAINS]
					= { "Thomas the Tank-Engine",
					    "Engine No. 9",
					    "California Zephyr",
					    "Tokaido Shinkansen"
					  };
const int	TRAIN_TRANSIT_TIME_MSECS[NUM_TRAINS]
					= { 10000,	// Thomas
					    10000,	// Engine no. 9
					     5000,	// CA Zephyr
					     1000	// Bullet train
					   };

Station		northTerminal("North Station");
Station		southTerminal("South Station");

//  PURPOSE:  To make the Train object pointed to by 'vPtr' do 'NUM_LOOP' times:
//	* arrive at 'northTerminal'
//	* leave at 'northTerminal'
//	* pause
//	* arrive at 'southTerminal'
//	* leave at 'southTerminal'
//	* pause
//	Returns a pointer to the Train it used.
void*   initiallyNorthernly (void*  vPtr)
{     
  // YOUR CODE HERE
  Train* train = (Train*) vPtr;
  int i;
  for(i=0;i<NUM_LOOPS;i++) {
	northTerminal.arrive(train);
  	northTerminal.leave();
  	train->pause();
  	southTerminal.arrive(train);
  	southTerminal.leave();
  	train->pause();
  }
  return (void*)train;
}   
    

//  PURPOSE:  To make the Train object pointed to by 'vPtr' do 'NUM_LOOP' times:
//	* arrive at 'southTerminal'
//	* leave at 'southTerminal'
//	* pause
//	* arrive at 'northTerminal'
//	* leave at 'northTerminal'
//	* pause
//	Returns a pointer to the Train it used.
void*   initiallySouthernly (void*  vPtr)
{     
  // YOUR CODE HERE
  Train* train = (Train*) vPtr;
  int i;
  for(i=0;i<NUM_LOOPS;i++) {
	southTerminal.arrive(train);
  	southTerminal.leave();
  	train->pause();
  	northTerminal.arrive(train);
  	northTerminal.leave();
  	train->pause();
  }	
  return (void*)train;
}   


int     main    (int		argc,
		 char*		argv[]
		)
{
  if  (argc > 1)
    srand(strtol(argv[1],NULL,10));

  pthread_t     tidArray[NUM_TRAINS];
  Train*	trainArray[NUM_TRAINS];

  for  (int i = 0;  i < NUM_TRAINS;  i++)
    trainArray[i] = new Train(TRAIN_NAME_ARRAY[i],TRAIN_TRANSIT_TIME_MSECS[i]);

  int trainInd	  = 0;

  //  Please make 'NUM_NORTHERNLY_TRAINS' threads (tidArray[0] to
  //  tidArray[NUM_NORTHERNLY_TRAINS-1]) run 'initiallyNorthernly()'.
  //  Each thread should get a pointer to its own Train instance
  //  (trainArray[0] to trainArray[NUM_NORTHERNLY_TRAINS-1], this is
  //   an array of POINTERS to trains, not trains themselves).
  for (trainInd = 0;  trainInd < NUM_NORTHERNLY_TRAINS; trainInd++)
    // YOUR CODE HERE
    pthread_create(&tidArray[trainInd],NULL,initiallyNorthernly,(void *)trainArray[trainInd]);

  //  Please make 'NUM_SOUTHERNLY_TRAINS' threads
  //  (tidArray[NUM_NORTHERNLY_TRAINS] to tidArray[NUM_TRAINS-1]) run
  //   'initiallySouthernly()'.  Each thread should get a pointer to its own
  //  Train instance (trainArray[NUM_NORTHERNLY_TRAINS] to
  //   trainArray[NUM_TRAINS-1], this is an array of POINTERS to trains, not
  //   trains themselves).
  for ( ;  trainInd < NUM_TRAINS; trainInd++)
    // YOUR CODE HERE
    pthread_create(&tidArray[trainInd],NULL,initiallySouthernly,(void *)trainArray[trainInd]);
    

  //  Leave this loop here.  I want to make sure you get the Train pointers
  //  back from initiallyNorthernly and initiallySouthernly().
  for  (int i = 0;  i < NUM_TRAINS;  i++)
    trainArray[i] = NULL;

  //  Wait for all Train threads.  Also, get the pointers to the Train objects
  //  and delete() them because they were created by 'new'
  // YOUR CODE HERE
  Train* trArr[NUM_TRAINS];
  pthread_join(tidArray[0],(void**)&trArr[0]);
  pthread_join(tidArray[1],(void**)&trArr[1]);
  pthread_join(tidArray[2],(void**)&trArr[2]);
  pthread_join(tidArray[3],(void**)&trArr[3]);
  
  delete trArr[0];
  delete trArr[1];
  delete trArr[2];
  delete trArr[3];
  return(EXIT_SUCCESS);
}
