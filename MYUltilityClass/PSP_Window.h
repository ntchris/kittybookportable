
#ifndef __PSP_WINDOW__
#define __PSP_WINDOW__


#include <PSP_Global.h>
#include <CommonString.h>
//#include <PSP_GRAPHICS.h>
//#include <PSP_CHINESEHANDLER.h>

//Window, to contain information like Text,bmp.
// Contents will not draw outside the window.

using namespace PSP_Constants;

class PSP_Window
{
  protected  :       
    const static unsigned short borderColor1 = colorWhite2;//
    const static unsigned short borderColor2 = colorGray;
    
    const static unsigned short borlderLine = 2;
    
    //static 
    unsigned  short spaceSpareLineEnd;  
    unsigned short fontColor;
    const static unsigned short titleHeigh = DotFont16ASCfont_heigh + 1 ;
//    const static unsigned short spaceBetweenChar = 1 ;
    const static unsigned short spaceBetweenLine = 1 ;
    const static unsigned short winXStart = 3;
    const static unsigned short winYStart = DotFont16ASCfont_heigh ;
    // this describes the window size
    Rectangle  rectangle;
    // so rightBot X and Y is determined !
    unsigned short backgroudColor ;
    bool showWindowBorder;
    DrawAttribute drawAttWin;


    //this line is full, go to next line
    //if after wrap line , it is getting out of window return false
    bool wrapLine ( unsigned short &winX , unsigned short &winY, 
        unsigned short charSize, unsigned short lineHeigh   );
    bool newLine (  unsigned short &winX , unsigned short &winY, 
        unsigned short lineHeigh                     );
    void drawAscChar8_16Window ( unsigned short x, unsigned short y,  unsigned short color, char ch); 
    void drawHanZi16Window ( unsigned short winX, unsigned short winY,  
        unsigned short color, const HanZi &hanzi) ;
    CommonString  title;
    bool ifAllowWrapLine;
    
    
    void drawTTFHanZiWindow ( unsigned short winX, unsigned short winY,  
                                     unsigned short color,  const HanZi &hanzi, BitMapSize *bmpsize ) ;
                                     
                                     
public :  
    PSP_Window ( const Rectangle &rec );
    PSP_Window (  );
    /* PSP_Window ( unsigned short _x , unsigned short _y , 
    unsigned short _width,  unsigned short _heigh  );
    */                          
    void setColor ( unsigned short backgroundColor ,  unsigned short _fontColor = 0  );
    unsigned short getBackGroundColor( void );
    // the x and y is the position in this window !  NOT physical screen!!!
    //==========================================================================
    //  print Asc (pure english/1234 ? #*$) and Chinese HanZi in graphic mode.
    //  return the how many char has been processed.
    //  
    //========================================================================== 
    unsigned long   printAscHan16ZiString ( unsigned short winx, unsigned short winy, unsigned short color,
         const char *str  , unsigned long bufferSize = 0 ) ;

    void drawBitMap (  unsigned short winX, unsigned short winY, 
        const ushortBitMap & bitmap ) ;

    void drawBitMap ( unsigned short winX, unsigned short winY,  
        unsigned short color, const ucharBitMap & bitmap ) ;
    
/*
    unsigned long printUnicodeAscHan16ZiString( unsigned short winx, unsigned short winy, 
                                                      unsigned short color, const unsigned  short *unicodeString ,
                                                      unsigned long bufferSize
                                                      ) ;
 */
	

    unsigned long printTTFHanZiString
                     ( unsigned short winx, unsigned short winy, 
                       unsigned short color, const unsigned  char *str, unsigned long bufferSize = 0 );

                                     
    // draw a HanZi to a Window. x y is the position of the window
    //inline  static bool drawHanZi ( unsigned short x, unsigned y, unsigned short hanZi );                         
    void showWindow( void );
    inline bool isInsideWindow( unsigned short x, unsigned short y );
    inline bool isInsideWindow( const Rectangle &rec  );
    void setRectangle( unsigned short x, unsigned short y, unsigned short wid, unsigned short heigh  );
    void getRectangle ( Rectangle *rec ) const ;
    inline void setPositionY ( unsigned short y )
    {
        this->rectangle.y = y;
    }

    


    void setTitle ( const unsigned char * title );
    void drawTitle ( void );
    unsigned short getMaxItemCountInWindow ( void );
    static void showMessage( const    char * textMessage  );
    static void showMessage( const    char * textMessage , int value );
    
    unsigned short getWindowHigh( void );
    void selfTest ( ) ;
    void   selfTestTTF ( ) ;
};

// draw a HanZi to a Window. x y is the position of the window

#endif


