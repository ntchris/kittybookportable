

#include <ConfigWindow.h>
#include <ColorPad.h>
#include <PSP_FileUtil.h>
#include <stdio.h>
#include <PSP_Window.h>


PSP_Window TextViewPadConfigWindow::fontColorBar, TextViewPadConfigWindow::backgroundColorBar ;

Label *TextViewPadConfigWindow:: label_ttfFilePath ,
      *TextViewPadConfigWindow:: label_title,
      *TextViewPadConfigWindow:: label_helpText1;
//      *TextViewPadConfigWindow:: label_helpText2,
//      *TextViewPadConfigWindow:: label_fontSize;

NumberBox * TextViewPadConfigWindow::numbox_fontSize ;
TextViewPadConfig * TextViewPadConfigWindow::config_p;

Button *TextViewPadConfigWindow::btn_setFontColor , 
       *TextViewPadConfigWindow::btn_setBackgroundColor , 
       *TextViewPadConfigWindow::btn_setTTFFilePath,
       *TextViewPadConfigWindow::btn_OK;

BoolBox * TextViewPadConfigWindow::boolBox_usingTTF;


RGBColor TextViewPadConfigWindow::tempRGBFontColor, 
         TextViewPadConfigWindow::tempRGBBackGroundColor;
  

// config file only should be loaded after Power on 
bool TextViewPadConfig :: configFileWasLoaded = false;

static const bool debugConfig = false;



//====================================================================================================


const char TextViewPadConfig ::configFileName[] = {"KityConf.dat"};

TextViewPadConfig ::TextViewPadConfig () 
{
    this->fontRGBColor.setColor( rgbBlack );
    backGroudRGBColor.setColor ( rgbWhite );
   
    this->setIsUsingTTF( false );
     //this->setIsUsingTTF( true );
    //this->fontSize = 25;
    onePressLineMoveCount = 1;
    pageTurnKeepOneLine = false;
    spaceBetweenLine = 1;
    memset ( trueTypeFontFilePath, 0,  pathMaxLen );
    strcpy ( trueTypeFontFilePath, "ms0:/simsun.ttc");
   
}


TextViewPadConfig ::~TextViewPadConfig () 
{



}

void TextViewPadConfig ::setIsUsingTTF( bool useTTF)
{
    isUsingTrueTypeFont = useTTF;
    if ( !useTTF )
    {
        fontSize = dotFontHanZiFontWidth;	
    }

}





void TextViewPadConfig ::save()
{   
    const int filelen = sizeof( TextViewPadConfig ) ;
    char buffer[filelen  ];
    memset ( buffer, 0, filelen  );
    
    //PSP_Window:: showMessage( this->trueTypeFontFilePath     );
    
    FILE *fp;
    fp = fopen( configFileName, "wb");    
    
    fseek( fp, 0, 0 );        
    fwrite( buffer, 1, filelen,  fp );
    
    fseek( fp, 0, 0 );        
    fwrite( this, 1, filelen,  fp );
    fclose( fp);
#ifdef __PSPSDK__
    if( debugConfig)
    PSP_Window::showMessage ( "config save done.");
#endif


}

void TextViewPadConfig ::load()
{
    // config file only should be loaded after Power on  
    if( configFileWasLoaded  )
    {
       //PSP_Window::showMessage(" skipLoad config ");
       return;
    }
 
    FILE *fp;
    TextViewPadConfig config;
    fp = fopen(  configFileName, "rb");
    if( !fp )
    {
#ifdef __PSPSDK__
        if( debugConfig )
        PSP_Window::showMessage ( "config file not found, creating one...\n 正在新建配置文件  \n ○ ×");
#else
        printf( "can not load config file\n");
#endif
        this->save();
        // because the save file is just default values,
        // useless to load again.        
        // config file only should be loaded after Power on 
        configFileWasLoaded  = true;
        //just use default value
        return;
    }

    fread( &config, 1, sizeof( TextViewPadConfig ),  fp );
    fclose( fp);

    this->backGroudRGBColor.setColor ( config.backGroudRGBColor );
    this->fontRGBColor.setColor (  config.fontRGBColor );

    this->isUsingTrueTypeFont = config.isUsingTrueTypeFont;
    this->fontSize = config.fontSize ;
    //!!!!
    this->pageTurnKeepOneLine = config.pageTurnKeepOneLine;
    //if not using ttf, fontSize is 16
   
   
   
   
    
    this->onePressLineMoveCount = config.onePressLineMoveCount;

    memcpy ( this->trueTypeFontFilePath , config.trueTypeFontFilePath ,pathMaxLen );
#ifdef __PSPSDK__
    if( debugConfig)
    PSP_Window::showMessage ( "config load OK.");
#endif

   

}
	
	
	
//====================================================================================================

TextViewPadConfigWindow ::TextViewPadConfigWindow (   TextViewPadConfig * _config_p )
{
    setRectangle(0, 0, PSP_SCREEN_WIDTH_Visible_cg , PSP_SCREEN_HEIGH_cg );
    this->setColor( 0xffff, 0x1);
    config_p = _config_p;
    btn_setFontColor = 0;
    btn_setBackgroundColor = 0 ;
    btn_setTTFFilePath = 0;
    addComponents();
  
}



TextViewPadConfigWindow ::~TextViewPadConfigWindow (   )
{
    delete  btn_setFontColor;
    
    
    	
}

 
void TextViewPadConfigWindow :: applyConfigToWindow( void )
{

    fontColorBar.setColor( config_p->fontRGBColor.getColor() );
    tempRGBFontColor.setColor ( config_p->fontRGBColor );
    
    
    backgroundColorBar.setColor( config_p->backGroudRGBColor.getColor() );
    tempRGBBackGroundColor.setColor( config_p->backGroudRGBColor );
    
    label_ttfFilePath ->setText( this->config_p->trueTypeFontFilePath );    
    boolBox_usingTTF ->setValue ( this->config_p->getIsUsingTrueTypeFont() );    
    this->numbox_fontSize->setValue( this->config_p->fontSize );
          
}


void TextViewPadConfigWindow ::setComponents( Win_Component *component_a [] )
{
    if ( *  component_a == NULL ) return;
//    component_a[ componentCount ];

    component_a [0 ] = this->btn_setFontColor;
    //**************************************************************** 
    // the first one , set focused
    component_a[0] ->setFocused( true );
    //**************************************************************** 

    component_a [1] = this->btn_setBackgroundColor;
    component_a [1] ->setFocused( false);

    component_a [2 ] = this->btn_setTTFFilePath;
    component_a [2] ->setFocused( false);

    component_a [3] = this->boolBox_usingTTF;
    component_a [3] ->setFocused( false);

    component_a [4] = this->numbox_fontSize;
    component_a [4] ->setFocused( false);
    
    component_a [5] = this->btn_OK;
    component_a [5] ->setFocused( false);

    


}

void TextViewPadConfigWindow :: startConfig(  void  )
{
    

    static const unsigned short componentCount = 6;
    static const unsigned short numbox_fontSizeIndex = 4;
    static const unsigned short btnSaveDoneIndex = 5;

    Win_Component *component_a[ componentCount ];
    /*
    component_a [0 ] = this->btn_setFontColor;
    component_a [1 ] = this->btn_setBackgroundColor;
    component_a [2 ] = this->btn_setTTFFilePath;
    component_a [3 ] = this->boolBox_usingTTF;
    component_a [4 ] = this->numbox_fontSize;
    component_a [5 ] = this->btn_OK;
    
    */
    setComponents( component_a );
    unsigned short selectedIndex = 0;
        
    applyConfigToWindow();
    this->drawAllItems();
    
    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode( PSP_CTRL_MODE_ANALOG ); 
    skipCurrentKey();  
    bool needSave = false;
    do
    {   
        
        sceCtrlReadBufferPositive(&pad, 1); 

        if ( !pad.Buttons )
        continue;   

        if (pad.Buttons & PSP_CTRL_UP)
        {
             
             if ( selectedIndex > 0  )
             {  component_a[ selectedIndex ] ->setFocused ( false );
                selectedIndex --;
                component_a[ selectedIndex ] ->setFocused ( true );
             }   
        } 
        if (pad.Buttons & PSP_CTRL_DOWN)
        {          
             if ( selectedIndex <  componentCount-1  )
             {
                component_a[ selectedIndex ] ->setFocused ( false );
                selectedIndex ++;    
                component_a[ selectedIndex ] ->setFocused ( true );
             }   
        }
        
        if ( pad.Buttons & PSP_CTRL_CIRCLE )
        {
           component_a[ selectedIndex ]->onClick( );
           if( selectedIndex != btnSaveDoneIndex &&
               selectedIndex != numbox_fontSizeIndex )
           {
              needSave = true;
           }   
        }   
        
        if ( pad.Buttons & PSP_CTRL_CROSS )
        {
            bool r ;
            if ( needSave  )
            {
               ConfirmWindow *confwinp;
               confwinp = new ConfirmWindow;
               if ( !confwinp )
               {   //memory alloc failed
	               break;
	           }
	           r = confwinp->show( "Cancel and Quit ?   \n\n 不保存就直接退出?" );
	           delete confwinp;
               if ( r ) 
               { 
                  break;
               }
                   
            }else
            {
                 break;   
            }

        }   
           
        if ( selectedIndex == numbox_fontSizeIndex )
        {  int upStep = 1, downStep = 1; 
           static const int minsize = minTTFFontSize, maxsize = maxTTFFontSize;
           if ( pad.Buttons & PSP_CTRL_LEFT )
           {
               numbox_fontSize->decrease(  );
               if( numbox_fontSize->getValue() < minsize )
               numbox_fontSize->setValue( minsize );
               
           }else if ( pad.Buttons & PSP_CTRL_RIGHT  )
           {
                    
               numbox_fontSize->increase(  );
               if( numbox_fontSize->getValue() > maxsize )
               numbox_fontSize->setValue( maxsize );
           }
           if ( pad.Buttons & PSP_CTRL_RIGHT  ||
                pad.Buttons & PSP_CTRL_LEFT 
               )
           {               
               needSave = true;
    //           this->config_p->fontSize = numbox_fontSize->getValue();            
               this->drawAllItems();
               wait( 8 );
               continue;
           }            
        }//if ( selectedIndex == numbox_fontSizeIndex )        
       
        if ( selectedIndex == btnSaveDoneIndex )
        {
            if ( pad.Buttons & PSP_CTRL_CIRCLE )
            { 
                // save and quit
                if ( needSave )
                {                    
                    applyWindowToConfig();
                    this->config_p->save();                    
                }
                break;    
            }
        }        
        this->drawAllItems();              
        wait(   );

    }while( true );

}
 


void TextViewPadConfigWindow :: applyWindowToConfig( void )
{
    this->config_p->fontRGBColor.setColor( tempRGBFontColor );
    this->config_p->backGroudRGBColor.setColor( tempRGBBackGroundColor);
    strcpy( config_p->trueTypeFontFilePath, 
        (const char*)label_ttfFilePath->getText() );
    config_p->setIsUsingTTF( boolBox_usingTTF->getValue() );                            
    this->config_p->fontSize = numbox_fontSize->getValue();            
}


void TextViewPadConfigWindow :: addComponents  ( void )
{
 
    const static int y = 40, ystep = 38;
    btn_setFontColor = new Button (  this, 20, y );
    btn_setFontColor ->setCaption ( "Font Color");
  
    btn_setFontColor ->setFocused();
      
    
    btn_setBackgroundColor = new Button (  this, 20, y + ystep  );
    btn_setBackgroundColor ->setCaption ( "BackGd Color");
   
    //fontColorBar.setColor( this->config_p->fontRGBColor.getColor());
    fontColorBar.setRectangle( 180, y , 48, 30 );
    //backgroundColorBar.setColor ( this->config_p->backGroudRGBColor.getColor());
    backgroundColorBar.setRectangle( 180, y + ystep, 48 , 30 ); 


    btn_setTTFFilePath = new Button ( this, 20, y + ystep*2 );
    btn_setTTFFilePath ->setCaption ( "TTF File...");
    
    label_ttfFilePath = new Label ( this, 180, y + ystep*2 + 5 );
    //label_ttfFilePath ->setText( this->config_p->trueTypeFontFilePath );
    
    
    label_title = new Label ( this, 150, 1 );
    label_title ->setColor( RGBColor::getShortColor( 0,0,255 ) ) ; 
    label_title ->setText( " == Config Window ==" );
    
    label_helpText1 = new Label ( this, 1, 251 );
    label_helpText1 ->setColor( RGBColor::getShortColor( 0, 0, 255 ) ) ; 
    label_helpText1->setText ( "UP/Down/Right/Left/O to set options. X to cancel");
    
   /* label_helpText2 = new Label ( this, 7, 250 );
    label_helpText2 ->setColor( RGBColor::getShortColor( 0,0 , 255 ) ) ; 
    label_helpText2->setText ( "X to quit and save");
    */
    
    
    boolBox_usingTTF = new BoolBox ( this, 20, y + ystep*3 );
    boolBox_usingTTF ->setLabel( "use TrueTypeFont" );
    //boolBox_usingTTF ->setValue ( this->config_p->getIsUsingTrueTypeFont() );
    
    
    //label_fontSize = new Label ( this, 20, y + ystep*4 - 8 );
    //label_fontSize->setText( "TTF Font Size" );
    numbox_fontSize = new NumberBox( this, 20, y + ystep*4 - 8 );    
    numbox_fontSize->setValue ( this->config_p->fontSize );
    numbox_fontSize->setText("TTF Font Size");

    btn_setFontColor->setAction ( setFontColor );
    btn_setBackgroundColor->setAction( setBackGroundColor  );
    btn_setTTFFilePath->setAction(  setTTFFontFilePath );
    boolBox_usingTTF->setAction (  setIsUseTTF );
    
    btn_OK = new Button (  this, 20, y + 181 );
    btn_OK->setCaption ( "DONE and Save");;
}


void TextViewPadConfigWindow :: setIsUseTTF ( void )
{
    boolBox_usingTTF->click();
    //config_p->setIsUsingTTF( boolBox_usingTTF->getValue() );
    
}



void TextViewPadConfigWindow :: setFontColor ( void )
{
    ColorPad::show( tempRGBFontColor);
    fontColorBar.setColor( tempRGBFontColor.getColor() ); 
     
      
    
}




void TextViewPadConfigWindow :: setBackGroundColor ( void )
{
     ColorPad::show( tempRGBBackGroundColor );
     backgroundColorBar.setColor( tempRGBBackGroundColor.getColor() );
     

}



void TextViewPadConfigWindow ::setTTFFontFilePath( void )
{

   
   CommonString fileNameString, ext;
   bool r;
   r = PSP_FileManager ::fileSelector ( fileNameString );
   skipCurrentKey();  
   if ( r == false )
   return;
   
   ext.getFileExtention( fileNameString.getChar_p_readonly() );
   ext.toLowerCase();
   if ( ext == ttfFileExt || ext == ttcFileExt )
   {
       
       /*strcpy( config_p->trueTypeFontFilePath, 
               (char*)fileNameString.getChar_p_readonly() );       
               */
       //label_ttfFilePath->setText( config_p->trueTypeFontFilePath );
       label_ttfFilePath->setText( (char*)fileNameString.getChar_p_readonly() );
       
       showMessage( " TTF font file selected.");
   }else
   {
       showMessage( "Only *.ttf and *.ttc are True Type Font files.\n 请选择*.ttf 和 *.ttc 作为True Type字体文件. \n\
 并且, 如果要显示True Type汉字, 请选择汉字字库文件如宋体 仿宋等.");
   
   }
   
   

}



void TextViewPadConfigWindow ::drawAllItems( void )
{
    
    PSP_Window :: showWindow( );
   


    btn_setFontColor->show();
    
    btn_setBackgroundColor ->show();
    
    btn_setTTFFilePath->show();
    label_ttfFilePath->show();
    label_title ->show();
    
    label_helpText1->show();
   // label_helpText2->show();
    
    //backgroundColorBar.setColor( config_p->backGroudRGBColor.getColor() );
    backgroundColorBar.showWindow(); 
    
    
    //fontColorBar.setColor( config_p->fontRGBColor.getColor() );
    fontColorBar.showWindow();

    boolBox_usingTTF->show();
    //label_fontSize->show();
     
    numbox_fontSize->show();
    btn_OK->show();
    
    PSP_GRAPHICS::flipScreenBuffer();

}





