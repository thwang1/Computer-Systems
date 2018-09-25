/*-------------------------------------------------------------------------*
 *---									---*
 *---		quickSort.c						---*
 *---									---*
 *---	    This file defines a function that implements the quick-sort	---*
 *---	algorithm.							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2017 April 3		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"header.h"


int		partition	(int*		array,
				 int		pivot,
				 int		lo,
				 int		hi
				)
{
  lo--;
  hi++;

  while  (1)
  {
    do
    {
      lo++;
    }
    while  (array[lo] < pivot);

    do
    {
      hi--;
    }
    while  (array[hi] > pivot);

    if  (lo >= hi)
      break;

    swap(array,lo,hi);
  }

  return(hi);
}


int		pivot		(int*		array,
				 int		lo,
				 int		hi
				)
{
  int	atLo	= array[lo];
  int	atMid	= array[(lo+hi)/2];
  int	atHi	= array[hi];

  if  ( ( (atLo <= atMid) && (atMid <= atHi) )  ||
        ( (atLo >= atMid) && (atMid >= atHi) )
      )
    return(atMid);

  if  ( ( (atMid <= atLo) && (atLo <= atHi) )  ||
	( (atMid >= atLo) && (atLo >= atHi) )
      )
    return(atLo);

  return(atHi);
}


void		quickSort_	(int*		array,
				 int		lo,
				 int		hi
				)
{
  if  (lo < hi)
  {
    int	left;
    int	right;
    int	p	= pivot(array,lo,hi);

    p		= partition(array,p,lo,hi);
    quickSort_(array,lo,p);
    quickSort_(array,p+1,hi);
  }
}


//  PURPOSE:  To sort the &arrayLen& integers in array &array& with the
//	quick-sort algorithm.  No return value.
void		quickSort	(int*		array,
				 int		arrayLen
				)
{
  quickSort_(array,0,arrayLen-1);
}