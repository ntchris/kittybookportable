


#ifndef __PSPArray__
#define  __PSPArray__


// a common purpose array class 
//Array only remember the pointer, so do not pass local var to it.
// if local var get destroy, the array(element) is destroy too
class PSPArray
{
    
   static const unsigned long maxArraySize = 100*1024;
    void *object_p[ maxArraySize ] ;
    unsigned long size;
   
   public:
    PSPArray();
    ~PSPArray ();
    
    //if full, fail
    //Array only remember the pointer, so do not pass local var to it.
    // if local var get destroy, the array(element) is destroy too
    bool add ( void * newObject_p   );
    void * get( unsigned long index );    	
    unsigned long getSize( void );
    void clear ( void );
    	
};


#endif


