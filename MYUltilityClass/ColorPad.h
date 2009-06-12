

#ifndef __COLORPAD__
#define __COLORPAD__


#include <PSP_Window.h>
#include <WinComponent.h>



class ColorPad : PSP_Window
{
    
    
     
      NumberBox *numbox_Red, *numbox_Green, *numbox_Blue;
      //Label *labelR, *labelG, *labelB, 
      Label *labelHelpText;
      PSP_Window colorBar;
      void addComponent( void );
      void drawAllItems( void );
      void updateAllItems( void );
      
      
    public:
      
      ColorPad (  ) ;    
      ~ColorPad (  ) ;
      //void setColor( unsigned char r, unsigned char g, unsigned char b );
        void setColor ( RGBColor &rgbColor );
      static void show( RGBColor &rgbColor );
     
    
};






#endif



