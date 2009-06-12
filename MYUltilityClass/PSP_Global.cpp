



#ifndef __PSP_GLOBAL__
#define __PSP_GLOBAL__

//#define __VC__  

#include <PSP_Global.h>
//only for PSP
#include <stdlib.h> 



#ifdef __PSPSDK__

#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspdebug.h> 
#include <pspkerneltypes.h>
#include <psppower.h>

#endif






#ifndef __VC__
void* operator new( unsigned int size ) 
{
   //printf("operator new: %d Bytes\n", size );
   void *p;
   p = malloc( size );
   if( !p )
   {
#ifdef __PSPSDK__	   
      pspDebugScreenInit();    
      pspDebugScreenPrintf("operator new: %d Bytes\n", size );
      getch();
#endif
      
   }
   return p;
}   


void* operator new[]( unsigned int size ) 
{
   void *p;
   p = malloc( size );
   if( !p )
   {
#ifdef __PSPSDK__	   
      pspDebugScreenInit();    
      pspDebugScreenPrintf("operator new: %d Bytes\n", size );
      getch();
#endif
   }
   return p;
}


void operator delete( void *p ) 
{  //printf("operator new: %d Bytes\n", size ); 
   if( p!=NULL )
   {
      free( p );
   }   
}   

void operator delete[]( void *p ) 
{
   if( p!=NULL )
   {
      free( p );
   }   
}   

#endif  //__VC__



#ifdef __PSPSDK__
 
void   wait( unsigned long count)
{
    slowDownCPU();
	    for (; count>0; --count) 
    {
        sceDisplayWaitVblankStart();
    }
    normalSpeedCPU();
}


void skipCurrentKey( bool powerSave )
{
    SceCtrlData pad; 
    if( powerSave ) 
    {  slowDownCPU();
    }
    int counter = 0;
    const int max = 39;
    do
    {   //直到本次的按键放开之后, 再读取.
        //do not read  until the key is released
        sceCtrlReadBufferPositive(&pad, 1); 
        if (pad.Buttons == 0)
        {    
            break;
        }
        //however, break when max meet
        counter ++;
        if( counter > max ) break;       
    
    }while ( true );
    if( powerSave ) 
    {  normalSpeedCPU();
    }
}


SceCtrlData  getch( void )
{
   
    SceCtrlData pad; 
    
    skipCurrentKey();
    
    while ( true )
    {   
        sceCtrlReadBufferPositive(&pad, 1); 
        if (pad.Buttons != 0)
        {    
            break;
        }
    }    
    skipCurrentKey();
    return pad;
}
 



//cpuNormalSpeed = 222, cpuSlowSpeed = 20,
//busNormalSpeed = 167, busSlowSpeed = 20 , 
void slowDownCPU( void )
{
   scePowerSetCpuClockFrequency ( cpuSlowSpeed );
   scePowerSetBusClockFrequency ( busSlowSpeed ); 
   		
}

void normalSpeedCPU( void )
{
   
    scePowerSetCpuClockFrequency ( cpuNormalSpeed );
    scePowerSetBusClockFrequency ( busNormalSpeed ); 
   // slowDownCPU();
	
}

void midSpeedCPU( void )
{
    scePowerSetCpuClockFrequency ( cpuMidSpeed );
    scePowerSetBusClockFrequency ( busMidSpeed ); 
}

int getBufFreq( void )
{
	return scePowerGetBusClockFrequencyInt ();
}

int getCPUFreq( void )
{
	return scePowerGetCpuClockFrequencyInt ();
}


//bool begin = false;

#endif




#endif  






