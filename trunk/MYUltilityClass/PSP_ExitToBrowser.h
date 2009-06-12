


//=====================================================================================
//
// When in PSP GAME, press [HOME] button to exit to PSP Browser.
// In main ( ) 
// use registerExitToBrowser(void) when the game start up. 
// check bool shouldIExitToBrowser( void ) to see if should exit.
//=====================================================================================



#ifndef __PSP_EXITTOBROWSER__
#define __PSP_EXITTOBROWSER__


#include <pspthreadman.h>


class PSP_ExitToBrowser
{
   private:
    
    static bool ifExitToBrowser ;
//    static bool ifHaveRegistered;
    static int exit_CallbackThread ( SceSize  args, void *argp );
    static int exit_callback ( int arg1, int arg2, void *common );
   

   public:
    static bool shouldIExitToBrowser( void );
    static void registerExitToBrowser( void );
    //static void registerExceptionHandler( void );
    static void testNULLError( void );
    
    
	
};





//=====================================================================================================
//
//
//
//=====================================================================================================


//#include <PSP_ExitToBrowser.h>
//#include <pspthreadman.h>
//#include <psploadexec.h>

bool PSP_ExitToBrowser::ifExitToBrowser = false ;
//bool PSP_ExitToBrowser::ifHaveRegistered = false ;



int PSP_ExitToBrowser:: exit_callback(int arg1, int arg2, void *common )
{
    normalSpeedCPU();
    ifExitToBrowser = true;
   	sceKernelExitGame();


   //这里要不要exitGame()跟主函数有关系 
   //如果主函数是 PSP_MAIN_THREAD_ATTR(0);
   //那么需要  sceKernelExitGame();
   
   //如果主函数没有设定, 则不要// sceKernelExitGame(); 否则按Yes后会黑屏关机.
   
   
   // sceKernelExitGame();
   
    // 在这里退出是没有用滴!!!  导致黑屏死机
    // 必须要在主程序使用该语句退出
    // Useless Exit GAME here, it just causes black screen and hang up.
    // sceKernelExitGame();    
    return 0;
}

/*
int exit_callback(int arg1, int arg2, void *common )
{
	
   	sceKernelExitGame();
    return 0;
}
*/

// Callback thread 
int PSP_ExitToBrowser:: exit_CallbackThread(SceSize args, void *argp)
{
    int cbid;
//    cbid = sceKernelCreateCallback("Exit Callback",PSP_ExitToBrowser:: exit_callback, NULL );
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL );
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();

    return 0;
}


// Sets up the callback thread and returns its thread id 
void PSP_ExitToBrowser::registerExitToBrowser ( void )
{
    
   // if( ifHaveRegistered )
    {
   // 	return ;
    } 	
    
   // registerExceptionHandler();
    
    int thid = 0;    
    thid = sceKernelCreateThread("update_thread", PSP_ExitToBrowser::exit_CallbackThread,0x11, 0xFA0, 0, 0 );
    // IF this main.cpp uses this: PSP_MAIN_THREAD_ATTR( THREAD_ATTR_USER );
     //Then here must be   THREAD_ATTR_USER   otherwise, can not quit the HOME menu YES/NO
   /*
     main()                         here                   result
   THREAD_ATTR_USER                 0                       fail
   THREAD_ATTR_USER                 THREAD_ATTR_USER        fail    (use	sceKernelExitGame(); success )
   
     not set                        0                       fail
      
    主函数要 		sceDisplayWaitVblankStart();

   */
       
    if(thid >= 0)
    {
       sceKernelStartThread(thid, 0, 0);
    }
    
  
}


bool PSP_ExitToBrowser::shouldIExitToBrowser( void )
{
     return ifExitToBrowser;
 
}





// Example custom exception handler 
// PSP_MODULE_INFO("ExitToBrowser", 0x1000, 1, 1);
// 必须要load到0x1000,  
// 才可以用 void PSP_ExitToBrowser::myExceptionHandler( PspDebugRegBlock *regs )
/*
void PSP_ExitToBrowser::myExceptionHandler( PspDebugRegBlock *regs )
{
	// Do normal initial dump, setup screen etc 
	pspDebugScreenInit();

	// I always felt BSODs were more interesting that white on black 
	pspDebugScreenSetBackColor(0x00FF0000);
	pspDebugScreenSetTextColor(0xFFFFFFFF);
	pspDebugScreenClear();

	pspDebugScreenPrintf("I regret to inform you your psp has just crashed\n");
	pspDebugScreenPrintf("Please contact Sony technical support for further information\n\n");
	pspDebugScreenPrintf("Exception Details:\n");
 	pspDebugDumpException(regs);
	pspDebugScreenPrintf("\nBlame the 3rd party software, it cannot possibly be our fault!\n");

 
}


*/


 /*
 void PSP_ExitToBrowser::registerExceptionHandler( void )
{
   pspDebugInstallErrorHandler(  NULL );
}

*/
 

void PSP_ExitToBrowser::testNULLError( void )
{
	
    char *p = 0;
    p[100]=0;
    	
}



 







//

/*
Demo  How to use class PSP_ExitToBrowser

#include <PSP_ExitToBrowser.h>

 
int main(void)
{
    
   

    init();
    pspDebugScreenInit();
    pspDebugScreenPrintf("Exit To Browser Demo started ... \n");
    PSP_ExitToBrowser::registerExitToBrowser();

       
    sceCtrlSetSamplingCycle( 0 );
    sceCtrlSetSamplingMode( 0 );
    
    
    pspDebugScreenPrintf("Program is working ....... \n");
    pspDebugScreenPrintf("Press [HOME] to quit to PSP Browser\n");
    
    while (true)
    {

       if( PSP_ExitToBrowser::shouldIExitToBrowser() )
       {  
          break;
       }
        
    }
     
    sceKernelExitGame();
    return 0;
}
 
*/







#endif //__PSP_EXITTOBROWSER__




