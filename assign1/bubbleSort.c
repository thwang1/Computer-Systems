#include	"header.h"

//  PURPOSE:  To sort the &arrayLen& integers in array &array& with the
//	bubble-sort algorithm.  No return value.
void		bubbleSort	(int*		array,
				 int		arrayLen
				)
{
  int	haveSwapped;
  int	i;

  do
  {
    haveSwapped	= 0;

    for  (i = 0;  i < arrayLen-1;  i++)
      if  (array[i] > array[i+1])
      {
	swap(array,i,i+1);
	haveSwapped	= 1;
      }
  }
  while  (haveSwapped);
}