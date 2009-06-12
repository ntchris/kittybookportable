


 
//#define __PSPSDK__
#define printf pspDebugScreenPrintf
#include <PSP_Global.h>
#include <pspkernel.h> 
//#include <pspdebug.h> 

//#include <string.h> 
#include <pspdisplay.h>  //    sceDisplaySetMode(0,SCREEN_WIDTH,SCREEN_HEIGHT);
//#include <pspctrl.h>    //   ctrl_data_t pad;
#include <PSP_GRAPHICS.h>

//#include <PSP_Window.h> 
#include <PSP_ExitToBrowser.h>
//#include <PSP_ChineseUtil.h>
//
//#include <PSP_FileUtil.h>
//#include <TextViewPad.h>
#include <KittyBook.h>


PSP_MODULE_INFO("PSPGraphic", 0, 1, 1);



PSP_MAIN_THREAD_ATTR( THREAD_ATTR_USER );
 

void init( void )
{
    PSP_ExitToBrowser::registerExitToBrowser();
}


  

int main ( void )
{
    init( );
    PSP_GRAPHICS::setScreenFrameMode( screenModeWriteAndBuffered_cg );
    KittyBook *kittyreaderp;
    kittyreaderp = new KittyBook ;
    
   // scePowerSetCpuClockFrequency (20);
    //scePowerSetBusClockFrequency ( 20); 
    
    if( kittyreaderp )
    { 
	     kittyreaderp->start();    
         delete kittyreaderp;
    }    
	    
	      
  //  PSP_GRAPHICS::selfTestGetImage (  );
   
  
    
    //draw to back buffer
  
   // PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP );        
    //PSP_GRAPHICS::flipScreenBuffer(); 
    //getch();
   //=============================================================================
    while ( true )
    {
           
        if( PSP_ExitToBrowser::shouldIExitToBrowser() )
        {
            break;
        }  
   
        //this is important for [ HOME ] to quit.
        sceDisplayWaitVblankStart();

    }
   
    return 0;
}



    
   // pspDebugScreenInit();
    //pspDebugScreenPrintf("cn testing ! \n"); 
 /*   
    PSP_GRAPHICS::setScreenFrameMode( screenModeWriteAndShow_cg );
    
    Rectangle rec;
    rec.x = 10; rec.y=20; rec.width=360; rec.heigh = 185;
    PSP_Window win( rec );
    win.setColor( 1 );
    win.show();
    win.selfTestTTF ();
    
   //PSP_GRAPHICS::selfTestPrintStringABC_8_16 (  );
   getch();getch();
   
*/
   
  
    
     
// TextViewPad  textViewPad;
    

    //    textViewPad.readFile(  ( const char *)fname.getChar_p_readonly()   );
 //   textViewPad.start ();
    //PSP_GRAPHICS::setScreenFrameMode( screenModeWriteAndShow_cg );
    //Rectangle rec ( 0,0, 290, 160 );
    //Rectangle rec ( 0,0, 200, 160 );
//    PSP_FileManager filemgr( rec  );
 
 /*   PSP_FileManager filemgr;   
      filemgr.explore();
  */
  
 
  
    
    //PSP_GRAPHICS::selfTestGetImage();
    //PSP_GRAPHICS::setScreenFrameMode( screenModeWriteAndShow_cg );
  //  PSP_GRAPHICS::setScreenFrameMode( screenModeWriteAndBuffered_cg );
    	
    //PSP_ChineseUtil::  selfTestTTFGraphicMode (  );
   
   //PSP_GRAPHICS::setScreenFrameMode( screenModeWriteAndShow_cg );
   //PSP_ChineseUtil ::selfTestHanZi(); 	
    	



