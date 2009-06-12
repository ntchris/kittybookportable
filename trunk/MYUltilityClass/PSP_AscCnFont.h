

#ifndef __PSP_ASCFONTS__
#define __PSP_ASCFONTS__

 

#include <PSP_Global.h>


using namespace PSP_Constants;

struct Dot_Font_Data
{   
    //dot , pixel
    unsigned short width;
    //dot , pixel
    unsigned short heigh;
    const unsigned char * fontData;
    unsigned short sizeByte;
    unsigned short dotsPerByte ;
};

// get the font data
class PSP_ASCFONTS8_16 
{

    static ASCFontBitMap ascFontBitMap [   ] ;
    static bool ascBMPinited;
    static const unsigned char * const font_dot_matrix_p;

    //one byte offer 8 dots data
    static const unsigned short dotsPerByte = 8 ; 
    //================================================================
    //   
    //   Every ASC takes 16 bytes, 
    //   so the 2nd ASC should return fontp + (2-1)*16 = 16
    //   3rd ASC :  fontp+ (3-1)* 16 = 32.
    //   The input index is just the ASC. 
    //   eg. index = 'a' get 'a's data.
    //================================================================ 
    static void getFontData( unsigned index, Dot_Font_Data * font_data );
    static bool drawASC8_16ToBMP ( unsigned char _char,  ASCFontBitMap &bitmap );
    static const unsigned short defaultColor =1;

    static void loadAllASCBitMap ( void );
    //private constructor to prevent creating instance
    PSP_ASCFONTS8_16 ();
    ~PSP_ASCFONTS8_16();

public:
    static const unsigned char oneAscCharUseByte_LIB = PSP_Constants::oneAscCharUseByte_LIB;
    static const unsigned int maxIndex;

    static const unsigned short width = DotFont16ASCfont_width ;
    static const unsigned short heigh = DotFont16ASCfont_heigh;
    static ASCFontBitMap * getASCBmp(   char ascchar );


};






//For Chinese 16X16 DOT FONT
class PSP_CNFONTS16_16   
{
   //private:
   
     const static unsigned short defaultIndex = 0;
     //this index has no font,  I can use it.
     const static unsigned short noFontHZKIndex =  6520;
     const static unsigned short noFontHanZi =  63137 ;//f6 a1
     
      //==========================================================================
    //    Chinese HanZi to HanZi LIB index
    //==========================================================================                                 
     static unsigned long HanZiToHZKLibIndex ( const HanZi &hanzi );                          
     const static unsigned char defaultColor = 1; 
          //================================================================
     //    index is the font's position in the HZK lib.
     //    eg. if the HZK contains 10000 HanZi,
     //    so getFontData( 1 ) gets the 1st HanZi's data.
     //       getFontData( 10 ) gets the 10th HanZi's data.
     //    every HanZi takes 32 bytes, 
     //    so the 2nd HZ should return fontp+ (2-1)*32 = 32
     //    3rd HZ :  fontp+ (3-1)* 32 = 64.
     //    To get the index of a perticular HanZi,
     //    you must use cnToHZKIndex ( const HanZi &hanzi ).
     //    HanZi contains the HanZi char * data. ( two chars )
     //================================================================ 

     static void  getFontData( unsigned index, Dot_Font_Data * font_data );
     static const unsigned char * const font_dot_matrix_p ;
     static bool drawKouHuBitMap ( HanZi16FontBitMap &bitmap );
      
   public:
     //Font width  : 16 dots
     static const unsigned short width = 16;
     //Font heigh  : 16 dots
     static const unsigned short heigh = 16;
     //One HanZi uses 32 bytes
     static const unsigned char oneHanZiUseByte = 32;
     static const unsigned short dotsPerByte = 8 ;
    
     static const unsigned int maxIndex;      
     
     static bool drawHanZi16ToBMP ( unsigned short hanZi,  HanZi16FontBitMap &bitmap ) ;
     static bool getBitMapHZKLibIndex ( unsigned short index , HanZi16FontBitMap &bitmap ) ;
     static void  selfTest( void );

};

 

#endif   //  __PSP_ASCFONTS__






