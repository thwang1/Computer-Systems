// Declares a class that implements a Train object.

class	Train
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the name of '*this' Train.
  std::string	name_;

  //  PURPOSE:  To hold the time it takes '*this' Train to go from one
  //	Station to another (in microseconds).
  int		pauseTimeUsecs_;

public :
  //  PURPOSE:  To initialize '*this' Train to have name 'newName' and to have
  //  	transit time between platforms 'newPauseTimeUsecs'.  Also notes to
  //	std::cout that '*this' Train is leaving the trainyard.  No return
  //	value.
  Train		(const std::string&	newName,
  		 int   			newPauseTimeUsecs
  		)
  {
    name_		= newName;
    pauseTimeUsecs_	= newPauseTimeUsecs;
    std::cout << getName() << " leaving the trainyard.\n";
  }

  //  PURPOSE:  To release resources.  Also notes to std::cout that '*this'
  //  	Train is arriving back at the trainyard.  No parameters.  No return
  ///	value.
  ~Train	()
  {
    std::cout << getName() << " going back to the trainyard\n";
  }

  //  PURPOSE:  To return the name of '*this' Train.  No parameters.
  const std::string&
		getName	()
			const
  { return(name_); }

  //  PURPOSE:  To make '*this' Train wait while it transits between platforms.
  //	No parameters.  No return value.
  void		pause	()
			const
  {
    usleep(pauseTimeUsecs_);
  }

};