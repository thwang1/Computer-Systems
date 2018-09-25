// Declares a class that implements a Station object at which Train objects arrive, pause, and then leave.

class   Station
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the name of '*this' Station.
  std::string		name_;

  //  PURPOSE:  To point to the Train object currently at '*this' Station
  //	(if there is one).
  Train*		trainPtr_;

  pthread_mutex_t 	myLock;
  pthread_cond_t	notOccupied;
  pthread_cond_t	notEmpty;

public :
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' Station to have name 'newName' and
  //	and no Train.  No return value.
  Station		(const std::string&	newName
			)
  {
    name_	= newName;
    trainPtr_	= NULL;
    pthread_mutex_init(&myLock,NULL);
    pthread_cond_init(&notOccupied,NULL);
    pthread_cond_init(&notEmpty,NULL);
  }

  //  PURPOSE:  To release resources.  No parameters.  No return value.
  ~Station		()
  {
    pthread_mutex_destroy(&myLock);
    pthread_cond_destroy(&notOccupied);
    pthread_cond_destroy(&notEmpty);
  }

  //  PURPOSE:  To return the name of '*this' Station.  No parameters.
  const std::string&
		getName	()
  			const
  {
    return(name_);
  }

  //  PURPOSE:  To return a pointer to the Train object currently at '*this'
  //	Station (if there is one).
  Train*	getTrainPtr
			()
			const
  {
    return(trainPtr_);
  }

  //  PURPOSE:  To let '*newTrainPtr' arrive at '*this' Station.  No return
  //	value.
  void		arrive	(Train*	newTrainPtr
			)
  {
    pthread_mutex_lock(&myLock);

    while  (getTrainPtr() != NULL)
    {
      std::cout << getTrainPtr()->getName()
		<< " is at " << getName()
		<< ", "	     << newTrainPtr->getName()
		<< " must wait.\n";
      usleep(10 + rand() % 10);
      pthread_cond_wait(&notOccupied,&myLock);
    }

    std::cout << newTrainPtr->getName() << " arriving at " << getName() << "\n";

    if  (getTrainPtr() != NULL)
      std::cout << newTrainPtr->getName() << " and "
		<< getTrainPtr()->getName() << " CRASHED at "
		<< getName() << std::endl;

    trainPtr_	= newTrainPtr; 
    usleep(10 + rand() % 10); 
    pthread_mutex_unlock(&myLock);
    pthread_cond_signal(&notEmpty);
  }

  //  PURPOSE:  To let the Train object currently at '*this' Station leave.
  //	Returns pointer to departing Train.
  Train*	leave	()
  {
    pthread_mutex_lock(&myLock);

    while  (getTrainPtr() == NULL)
    {
      std::cout << "No train at " << getName() << "!\n";
      usleep(10 + rand() % 10);
      pthread_cond_wait(&notEmpty,&myLock);
    }

    Train*	toReturn	= getTrainPtr();
     
    std::cout << toReturn->getName() << " leaving " << getName() << "\n";

    usleep(10 + rand() % 10);
    trainPtr_ = NULL;
    pthread_mutex_unlock(&myLock);
    pthread_cond_signal(&notOccupied); 

    return(toReturn);
  }

};
