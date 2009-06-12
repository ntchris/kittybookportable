

#include<KittyBook.h>
//#include <TextViewPad.h>
#ifdef __PSPSDK__
#include<PSP_FileUtil.h>
#endif


//TextViewPad              FireStarReader :: viewPad;
TextViewPadConfigWindow  * KittyBook :: configWin_p ;
    


KittyBook :: KittyBook ( )
{
    TextViewPad::config. load();
    configWin_p = new TextViewPadConfigWindow (  & TextViewPad::config  );
   

}

KittyBook :: ~KittyBook ( )
{
    delete configWin_p ;

}



void  KittyBook :: browseAndRead ( void )
{
      CommonString textFile;
      Rectangle recMgr ( 10, 10, 450, 255 );
      PSP_FileManager *fileMgrp;
      fileMgrp = new PSP_FileManager( recMgr  );
//      PSP_FileManager fileMgr;
      //filemgr.explore();
      /*bool r;
      r = PSP_FileManager ::fileSelector( textFile );
      if ( !r ) return;

      if ( textFile.getSize() > 0 )
      viewPad.readFile( (char*)textFile.getChar_p_readonly() );
      */
      
     
      fileMgrp->explore ();
      delete fileMgrp;
      
      
}




void  KittyBook  :: start ( void )
{

     
    //=====================================================================
    PSP_FileManager:: registerFileHandler ( textFileExt, 
                           this->viewPad.readMoreFileFromFileMgr );
    PSP_FileManager:: registerFileHandler ( htmFileExt , 
                           this->viewPad.readMoreFileFromFileMgr );
    PSP_FileManager:: registerFileHandler ( htmlFileExt ,
                           this->viewPad.readMoreFileFromFileMgr );
    
    //=====================================================================
    
    makeTitleScreen();
    showTitleScreen();    
    
    //=====================================================================
    // Get Key input 
    static const unsigned short componentCount = 4;
    Win_Component *component_a[ componentCount ];
    component_a [0 ] = this->btn_Browsefile;
    component_a [1 ] = this->btn_BookMark;
    component_a [2 ] = this->btn_Config;
    component_a [3 ] = this->btn_About;
    
    unsigned short selectedIndex = 0;
    
    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);    
    
    skipCurrentKey();  
    midSpeedCPU();
    do
    {           
        sceCtrlReadBufferPositive(&pad, 1);         

        if ( !pad.Buttons )
        continue;   
      
         midSpeedCPU();

        if (pad.Buttons & PSP_CTRL_CIRCLE)
        {
             component_a[ selectedIndex ]->onClick();
             //after read/browse, must redraw the title screen
             showTitleScreen();    
            
             

        }else if (pad.Buttons & PSP_CTRL_CROSS)
        {

        } else if (pad.Buttons & PSP_CTRL_UP)
        {

        }else if (pad.Buttons & PSP_CTRL_DOWN)
        {   

        }else if (pad.Buttons & PSP_CTRL_LEFT)
        {   
            component_a[ selectedIndex ] ->setFocused ( false );
            if ( selectedIndex > 0  )
               selectedIndex --;            
            else 
               selectedIndex = componentCount -1;
           
            component_a[ selectedIndex ] ->setFocused ( true );
            showTitleScreen();    
            
        }else if (pad.Buttons & PSP_CTRL_RIGHT )
        { 
          
            component_a[ selectedIndex ] ->setFocused ( false );
            if ( selectedIndex < ( componentCount -1 ) )
               selectedIndex ++ ;
            else
               selectedIndex = 0;
               
            component_a[ selectedIndex ] ->setFocused ( true );
            showTitleScreen();
        }else if(pad.Buttons & PSP_CTRL_TRIANGLE)
        {

        } 
        
        skipCurrentKey( false );     
    }while( true );


}





void KittyBook  ::makeTitleScreen( void )
{
    
    setRectangle(0, 0, PSP_SCREEN_WIDTH_Visible_cg , PSP_SCREEN_HEIGH_cg );
    
    setColor(  0xffff );
   
    addMainComponents ( this );
    
     
}

void KittyBook  ::showTitleScreen( void )
{
    
   
    showWindow();
    //printAscHan16ZiString ( 10, 10, 0,  "Developed by ..." );
    PSP_GRAPHICS:: drawKitty ( 220, 20  );
    btn_Browsefile->show();
    btn_Config ->show();
    btn_About->show();
    label_Title ->show();
    btn_BookMark ->show();
    PSP_GRAPHICS::flipScreenBuffer();
 
}


void KittyBook  ::addMainComponents ( PSP_Window * win )
{
    const static int y = 200;
    btn_Browsefile = new Button( win , 20, y );
    btn_Browsefile->setCaption( "Browse" );
    btn_Browsefile->setFocused (   );
    
    btn_Browsefile->setAction ( browseAndRead   );
    
    btn_BookMark = new Button ( win, 138, y );
    btn_BookMark->setCaption( "Bookmark" );
    btn_BookMark ->setAction( showBookMarkWin );
 //   btn_Readfile->show();
    
    btn_Config =  new Button( win , 250, y );
    btn_Config ->setCaption( "Config");
    //btn_Config ->show();
    btn_Config ->setAction ( showConfigWindow );
    
    
    btn_About =  new Button( win , 400, y );;
    btn_About->setCaption( "About" );
    btn_About->setAction(  showHelp );
    
    label_Title = new Label( win, 10, 20 );
    label_Title ->setText( "Kitty Book Portable ver 1.3" );
    label_Title ->setColor( 1 );
    //btn_About->show();

}

void KittyBook  :: showConfigWindow ( void )
{
   if( configWin_p )
   configWin_p->startConfig(); 
   else 
   showMessage ( "configWin_p is NULL " ); 
   


}

void KittyBook  :: showBookMarkWin( void )
{
    int action = 0 ;
    action = BookMarkWindow::show(   );   
    if ( action == BookMarkWindow::doneReturn )
    {
        return ;
    }else if ( action == BookMarkWindow::loadActionDone )
    {
        
        if ( BookMarkHandler::currentBookMark.hasData )
        {
	         
	        //showMessage ( BookMarkHandler::currentBookMark.filename );
	        //showMessage ( "posi is ", BookMarkHandler::currentBookMark.position );
            KittyBook::viewPad.readMoreFileFromBookMark();
        }
        return;
        
    }
    /*
     // here is the main menu, should not save
     else if ( action == BookMarkWindow::saveAction )
    {
    
       return;
    }
    */
    else 
    {
       return;
    }
     


}



void KittyBook :: showHelp( void )
{
	
	
  //  PSP_Window::showMessage ( "bus speed",getBufFreq(  ) );
//    PSP_Window::showMessage ( "CPU speed",getCPUFreq(  ) );
    
//	char msg[]={"This tool can read normal text file, unicode text file, and unicode BigEndian text file. TrueTypeFont is supported, And you also can only use built-in font. \nChange these options in Config Menu.\nO - confirm    X - cancel/quit"};
	//PSP_Window::showMessage( msg );
      viewPad.readMoreFileFromFileMgr ("ReadmeC_E.txt");
	
	
}



