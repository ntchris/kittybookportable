

#ifndef __KittyBook__
#define __KittyBook__



#include <PSP_Window.h>
#include <WinComponent.h>
#include <TextViewPad.h>

class KittyBook :PSP_Window
{
    static TextViewPad viewPad;    
    static TextViewPadConfigWindow  *configWin_p ;
    
    Button *btn_Browsefile, *btn_BookMark, *btn_Config, *btn_About;
    Label * label_Title;
    void addMainComponents ( PSP_Window *win );
    void makeTitleScreen( void );
    void showTitleScreen( void );
    static void browseAndRead  ( void );
    static void showConfigWindow( void );
    static void showBookMarkWin( void );    
    static void showHelp( void );

  public:
     
    void start ( void );
    KittyBook ();
   ~KittyBook ();

  

};


#endif //__KittyBook__



