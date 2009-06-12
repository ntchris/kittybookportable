/*
#include  <PSPArray.h>

#ifndef __PSPSDK__
//#include <memory.h>
#endif

// a common purpose array class 
//Constructor
PSPArray ::PSPArray()
{
    size = 0;
    for ( unsigned int i = 0;i<	maxArraySize ;i++)
    {
        object_p[i] = 0;    
    }
    
}	

// Destructor
PSPArray ::~PSPArray()
{
    clear();
  
}
   

   
//if full, fail
//Array only remember the pointer, so do not pass local var to it.
// if local var get destroy, the array(element) is destroy too
bool PSPArray :: add ( void * newObject_p )

{
     if ( size >=   maxArraySize )
     {
     	return false;
     }	
     
   
   //  void * p;
//     p = new unsigned char [ sizeOfObject ];
     //memcpy ( p , newObject_p, sizeOfObject );
     object_p[ size ] = newObject_p ;
     size ++;
     return true;
}
   
void *  PSPArray :: get( unsigned long index )
{
    if ( index >=   maxArraySize )
    {
     	return 0;
    }
    return object_p[ index ] ;
     
}

unsigned long PSPArray ::getSize( void )
{
    return size;	
}


void PSPArray ::clear ( void )
{
    for ( unsigned int i = 0;i<	maxArraySize ;i++ )
    {
        void * p = 0;
        p = object_p[i];
        if (  p  )
        {
           delete p;
        }	
    }	
	
}
    	*/



  

