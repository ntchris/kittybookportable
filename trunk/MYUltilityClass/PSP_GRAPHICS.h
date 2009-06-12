
#ifndef __PSP_GRAPHICS__
#define __PSP_GRAPHICS__


//#define __PSPSDK__

#ifdef  __PSPSDK__

#include <pspdisplay.h>

#include <PSP_AscCnFont.h>
//#include <CommonString.h>

#endif

#include <PSP_Global.h>



// PSP Vedio RAM base address
// cg = const global




using namespace PSP_Constants;

class PSP_GRAPHICS
{
    /*struct ASC8_16_BitMap
    {   unsigned char ASC8_16_Bitmap[ PSP_ASCFONTS8_16:: oneAscCharUseByte ];
    };

    static const unsigned short ASCfont_size_x = 8;
    static const unsigned short ASCfont_size_y = 16;
    static const unsigned char maxASCSize = 128 ;

    static ASC8_16_BitMap[ maxASCSize ];//[ PSP_ASCFONTS8_16::maxIndex ];
    static void initASCBitMaps ( void );
    */

    static PSP_SCREEN_MODE screenMode;
    //Active buffer address
    static unsigned short *frameBufferA_p;
    //Inactive buffer address
    static unsigned short *frameBufferB_p;

    //Draw whatever to this;
    static bool  isUsingBufferA;

    static const Rectangle  defaultRec;// full screen

    //If the Image hits the border , use this.
    //Image parts outside border doesn't show
    //slower.  ( because need check border every time.
    /* static void graphicBitBlt_CheckBorder( const DrawAttribute &drawAtt, 
    const ushortBitMap & ushortbitmap );
    static void graphicBitBlt_CheckBorder( const DrawAttribute &drawAtt, 
    const ucharBitMap & ucharbitmap,
    unsigned short color   );
    */
    //Draw a enlarged image
    //If the Image hits the border , use this.
    //Image parts outside border doesn't show
    //slower.  ( because need check border every time.
    static void graphicBitBltEnlarge_CheckBorder( const DrawAttribute &drawAtt, 
        const ushortBitMap & ushortbitmap    );
    static void graphicBitBlt_CheckBorder( const DrawAttribute &drawAtt, 
        const ushortBitMap & ushortbitmap    );
        
    static void graphicBitBltEnlarge_CheckBorder( const DrawAttribute &drawAtt, 
        const ucharBitMap & ucharbitmap, unsigned short color   );      

    static void   graphicBitBltEnlarge_CheckBorderTurn( const DrawAttribute &drawAtt, 
        const ucharBitMap & ucharbitmap,
        unsigned short color   ) ;



    //If the Image DOES NOT hit the border , use this.
    //Image parts outside border , will show up somewhere else. 
    // so  do not use this when image does hit border
    //quicker .  ( because no checking is done).
    /*  static void graphicBitBlt_NoCheck(  const DrawAttribute &drawAtt,const ushortBitMap & ushortbitmap );   

    static void graphicBitBlt_NoCheck(  const DrawAttribute &drawAtt,const ucharBitMap & ucharbitmap,
    unsigned short color   );     
    */

    //enlarge.
    //If the Image DOES NOT hit the border , use this.
    //Image parts outside border , will show up somewhere else. 
    // so  do not use this when image does hit border
    //quicker .  ( because no checking is done). 
    /* static void graphicBitBltEnlarge_NoCheck( unsigned short x, unsigned short y, unsigned short width,
    unsigned short heigh, const unsigned short *data , unsigned char  enlarge);

    static void graphicBitBltEnlarge_NoCheck( unsigned short x, unsigned short y, unsigned short width,
    unsigned short heigh, const unsigned char *data , unsigned char  enlarge, unsigned short color );
    */


    //get vram address for point ( x,y)
    static unsigned short *getVRamAddr( unsigned short x, unsigned short y);    

    static unsigned short *getVRamAddrFrontBuffer
                                         (unsigned  short x, unsigned  short y);
 


    //private constructor prevent to be new !;
    PSP_GRAPHICS ( );
public:

    inline static PSP_SCREEN_MODE getScreenMode( void )
    {
        return screenMode;
    }   
    

    // make the whole screen goes that color.
    static void fillVRam( unsigned short color = 0 );
    static void drawKitty (  unsigned short x , unsigned short y );


    // Set the current frame mode
    // 0   write and show
    // 1   write and buffered, not show, later , use flipScreen( ) to show
    static void setScreenFrameMode( PSP_SCREEN_MODE mode );

    // set active frame to back group
    // back group frame to ative.
    static void flipScreenBuffer( void );

    /*static void drawBitMap( const DrawAttribute &drawAtt,   const ushortBitMap & bitmap );

    static void drawBitMap(  const DrawAttribute &drawAtt,  unsigned short color,
    const ucharBitMap  & bitmap    );
    */
    static void drawBitMap (  const DrawAttribute &drawAtt,   
        const ushortBitMap & bitmap    );





    static void drawRectangle ( const Rectangle &rec,  unsigned short color );
    static void fillColorRect ( const Rectangle &rec,  unsigned short color );



    //static void getch( void );
    static void drawHorizonLine ( unsigned short x1 , unsigned short x2, 
        unsigned short y, unsigned short color );
    static void drawVerticalLine ( unsigned short y1 , unsigned short y2, 
        unsigned short x,  unsigned short color );


    //static void wait( unsigned long count);


  //  static void drawAscChar8_16( const DrawAttribute &drawAtt, unsigned short color,  char ascChar );


    static void graphicPrintAscString( unsigned short x, unsigned short y,
        unsigned short color, const char *str) ;
    static void graphicPrintAscString2( unsigned short x, unsigned short y,
        unsigned short color, const char *str);

    static void graphicPrintAscString4( unsigned short x, unsigned short y,
        unsigned short color, const char *str);

    static void drawBitMap ( const DrawAttribute &drawAtt, unsigned short color,  
        const ucharBitMap & bitmap    );

    static void drawBitMapTurn (  const DrawAttribute &drawAtt,   unsigned short color ,
        const ucharBitMap & bitmap     );

    //static void   drawHanZiEnlarge( unsigned short x, unsigned short y, char * hanZi2Byte, 
    //                 unsigned short color, unsigned char enlarge   ) ;

    static const ushortBitMap * getIconBitMap(  const unsigned char * fileExt );
    static const ushortBitMap * getFolderIconBitMap(  void  );
    
    inline static unsigned short RGBTo16BitColor ( unsigned char r ,
        unsigned char g, unsigned char b  )
    {
        return (  ( ( ( b>>3 ) & 0x1F )<< 10 ) +
            ( ( ( g>>3 ) & 0x1F ) <<5 ) +
            ( ( ( r>>3 ) & 0x1F ) <<0 ) + 0x8000 );

    } 


    void static copyFrontBufferToBackBuffer( void );
    // remember to allocate memroy for bitmap.data_p before call this
    static void getScreenImageToBitMap ( ushortBitMap & bitmap );
    //static void putImageBitMap ( unsigned short x, unsigned short y, ushortBitMap & bitmap );
    // simply use drawbmp
    
    static void selfTestSingleFrame( void ) ;
    static void selfTestDoubleFrame( void ) ;
    static void selfTestDoubleFrameMove( void );
    static void selfTestDoubleFrameFlip( void );
    // static void selfTestPrintStringABC ( void );
    static void selfTestAll ( void );
    static void selfTestPrintStringABC_8_16 ( void );
    static void selfTestDrawRect ( void );    
    static void selfTestGetImage ( void );


};

// End OF class PSP_GRAPHICS


#endif //__PSP_GRAPHICS__




