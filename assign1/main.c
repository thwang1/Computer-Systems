/*-------------------------------------------------------------------------*
 *---									---*
 *---		main.c							---*
 *---									---*
 *---	    This file defines the functions swap(),			---*
 *---	obtainNumberBetween() and main() needed for the program of	---*
 *---	assignment 1.							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2017 April 3		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"header.h"


//  PURPOSE:  To hold the lowest allowed random number. 
int		low		= 0;


//  PURPOSE:  To hold the highest allowed random number. 
int		high		= 32767;


//  PURPOSE:  To return another randomly-generated number.
int		getNextNumber	()
{
  return( (rand() % (high - low + 1)) + low );
}


//  PURPOSE:  To exchange the integers at indices &i& and &j& in &array&.  No
//	return value.
void		swap		(int*		array,
				 int		i,
				 int		j
				)
{
	int	 temp;
	temp	 = 	array[i];
	array[i] =	array[j];
	array[j] =	temp; 
}


//  PURPOSE:  To repeatedly ask the user the text "Please enter ", followed
//	by the text in &descriptionCPtr&, followed by the numbers &low& and
//	&high&, and to get an entered integer from the user.  If this entered
//	integer is either less than &low&, or is greater than &high&, then
//	the user is asked for another number.  After the user finally enters
//	a legal number, this function returns that number.
int		obtainNumberBetween
				(const char*	descriptionCPtr,
				 int   		low,
				 int		high
				)
{
	int input;
	while(1) {
		printf("Please enter %s (%d-%d): ", descriptionCPtr, low, high);
		scanf("%d", &input);
		if(low <= input && input <= high) return(input);
	}
}


//  PURPOSE:  To create and return an array of &numNums& random integers.
int*		createArray	(int		numNums
				)
{
  int*	array	= (int*)malloc(numNums * sizeof(int));
  int	i;

  for  (i = 0;  i < numNums;  i++)
    array[i] = getNextNumber();

  return(array);
}


//  PURPOSE:  To print the &arrayLen& integers in &array&.  No return value.
void		printArray	(int*		array,
				 int		arrayLen
				)
{
	int i;
	for(i = 0; i < arrayLen; i++) {
		printf("%d\n", array[i]);
	}
}


//  PURPOSE:  To use the function obtainNumberBetween() to obtain the values
//  	for global variable &numNums& (which must be between RANGE_LOWEST and
//  	 RANGE_HIGHEST).
//  	 Then it enters a loop asking the user what they want to do.  If the
//  	 user chooses integer 1 then the program runs bubbleSort(array,numNums).
//  	 If the user chooses integer 2 then the program runs
//  	 quickSort(array,numNums).  Returns &EXIT_SUCCESS& to OS.
int		main		()
{
  int		numNums	= obtainNumberBetween
				("number of numbers to sort",
  		  		 RANGE_LOWEST,
	  		  	 RANGE_HIGHEST
				);

  const char*	msgCPtr	= "What would you like to do?\n"
			  "(1) Sort with bubble-sort\n"
			  "(2) Sort with quick-sort\n"
			  "Your choice ";

  int		choice	= obtainNumberBetween(msgCPtr,1,2);
  int*		array	= createArray(numNums);

  switch  (choice)
  {
  case 1 :
    bubbleSort(array,numNums);
    break;

  case 2 :
    quickSort(array,numNums);
    break;
  }

  printArray(array,numNums);
  free(array);
  return(EXIT_SUCCESS);
}
