// Defines a C program that represents positive decimal 
// integers as linked lists of digits. It asks the user 
// for 2 integers, computes the sum, and outputs it.

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

#define		NUM_TEXT_LEN	256

//  PURPOSE:  To represent one digit of a decimal number.
struct			DigitNode
{
  int			digit_;		// I suggest you range this in [0..9]
  struct DigitNode*	nextPtr_;	// I suggest you make this point to
  	 				//  the next most significant digit.
};


//  PURPOSE:  To obtain the text of a decimal integer into array 'numberCPtr'
//	of length 'numberTextLen'.  'descriptionCPtr' is printed because it
//	tells the user the integer that is expected.   Ending '\n' from
//	'fgets()' is replaced with '\0'.  No return value.
void		obtainPostiveInt(char*		numberCPtr,
				 int		numberTextLen,
				 const char*	descriptionCPtr
				)
{
  //  YOUR CODE HERE
  int cont = 1;

  do{
	printf("Please enter the %s positive integer: ", descriptionCPtr);
	fgets(numberCPtr,numberTextLen,stdin);
	int i;
	for(i=0; numberCPtr[i]!='\n';i++) {
		if(isdigit(numberCPtr[i])!=0 && (numberCPtr[i]-'0')>=0)
			cont = 0;
		else {
			cont = 1;
			break;
		}
	}
  }
  while(cont);

  char* cPtr = strchr(numberCPtr,'\n');
  if(cPtr != NULL) *cPtr = '\0';
}


//  PURPOSE:  To build and return a linked list IN LITTLE ENDIAN ORDER
//	of the decimal number whose text is pointed to by 'numberCPtr'.
//	If 'numberCPtr' points to the string "123" then the linked list
//	returned is 'digit_=3' -> 'digit_=2' -> 'digit_=1' -> NULL.
struct	DigitNode*
		numberList	(const char*	numberCPtr
				)
{
  //  YOUR CODE HERE
  int i;
  struct DigitNode* list = NULL;  

  for(i=0;numberCPtr[i]!='\0';i++) {
	struct DigitNode* nodePtr = (struct DigitNode*) malloc(sizeof(struct DigitNode));
	nodePtr->digit_ 	= numberCPtr[i] - '0';
	nodePtr->nextPtr_ 	= list;
	list 	= nodePtr;	  	
  }

  return list;
}


//  PURPOSE:  To build and return a linked list IN LITTLE ENDIAN ORDER
//	of the decimal number that results from adding the decimal numbers
//	whose digits are pointed to by 'list0' and 'list1'.
struct	DigitNode*
		add		(const struct DigitNode*	list0,
				 const struct DigitNode*	list1
				)
{
  //  YOUR CODE HERE
  struct DigitNode* tmp0 	= (struct DigitNode*)list0;
  struct DigitNode* tmp1 	= (struct DigitNode*)list1;
  struct DigitNode* prev 	= NULL;
  struct DigitNode* sumList 	= NULL;
  int num0 	= 0;
  int num1 	= 0;
  int carry 	= 0;
  int sum 	= 0;
 
  while(tmp0 != NULL || tmp1 != NULL) {
	num0 	= tmp0 == NULL ? 0 : tmp0->digit_;
	num1 	= tmp1 == NULL ? 0 : tmp1->digit_;
	sum 	= num0 + num1 + carry;
	if(sum 	>= 10) {
		sum = sum % 10;
		carry = 1;
	}
	else 	carry = 0;
	struct 	DigitNode* sumPtr = (struct DigitNode*) malloc(sizeof(struct DigitNode));
	sumPtr->digit_	 = sum;
	sumPtr->nextPtr_ = NULL;
	if(prev == NULL) {
		prev = sumPtr;
		sumList = sumPtr;
	}
	else {
		prev->nextPtr_ = sumPtr;
		prev = sumPtr;
	}
	if(tmp0 != NULL) tmp0 = tmp0->nextPtr_;
	if(tmp1 != NULL) tmp1 = tmp1->nextPtr_;
	if(tmp0 == NULL && tmp1 == NULL && carry == 1) {
		struct DigitNode* carryPtr = (struct DigitNode*) malloc(sizeof(struct DigitNode));
		carryPtr->digit_ = carry;
		carryPtr->nextPtr_ = NULL;
		prev->nextPtr_ = carryPtr;
	}
  }
  return sumList;	 	
}


//  PURPOSE:  To print the decimal number whose digits are pointed to by 'list'.
//	Note that the digits are IN LITTLE ENDIAN ORDER.  No return value.
void		printList	(const struct DigitNode*	list
				)
{
  //  YOUR CODE HERE
  struct DigitNode* post = NULL;
  struct DigitNode* tmp;
  while(1) {
	tmp = (struct DigitNode*)list;
	for(;tmp!=post;tmp=tmp->nextPtr_){
		if((tmp->nextPtr_)==post){
			printf("%d",tmp->digit_);
			post=tmp;
			break;
		}
	}
	if(list==post) break;
  }
}


//  PURPOSE:  To print the nodes of 'list'.  No return value.
void		freeList	(struct DigitNode*		list
				)
{
  //  YOUR CODE HERE
  free(list);
}


//  PURPOSE:  To coordinate the running of the program.  Ignores command line
//	arguments.  Returns 'EXIT_SUCCESS' to OS.
int		main		()
{
  char			numberText0[NUM_TEXT_LEN];
  char			numberText1[NUM_TEXT_LEN];
  struct DigitNode*	operand0List	= NULL;
  struct DigitNode*	operand1List	= NULL;
  struct DigitNode*	sumList		= NULL;

  obtainPostiveInt(numberText0,NUM_TEXT_LEN,"first");
  obtainPostiveInt(numberText1,NUM_TEXT_LEN,"second");

  operand0List	= numberList(numberText0);
  operand1List	= numberList(numberText1);
  sumList	= add(operand0List,operand1List);
  
  printList(operand0List);
  printf(" + ");
  printList(operand1List);
  printf(" = ");
  printList(sumList);
  printf("\n");

  freeList(sumList);
  freeList(operand1List);
  freeList(operand0List);

  return(EXIT_SUCCESS);
}
