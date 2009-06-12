



#ifndef __PSP_CHINESEUTIL__
#define __PSP_CHINESEUTIL__



//#ifdef  __PSPSDK__
//#include <PSP_GRAPHICS.h>


#include <PSP_AscCnFont.h>

using namespace PSP_Constants;



//One item takes 32+2=34 bytes
struct HanZi16CachedItem
{  

    //each HanZi has two char, so it's A Short.
    //eg. HanZi "Œ“" is CE D2 , so it's 53966 ( D2CE )
    unsigned short hanZi;

    //How freq is it used 
    unsigned short frequency;

    // for 16*16 hanZiBMP   32 bytes  
    HanZi16FontBitMap hanZi16BitMap;

};


//store the freq used HanZi bit map.
class HanZiCache
{

    // store up to 2000 HanZiCachedItem, so 2000*16*16 = 512 KBytes
    static const unsigned maxHanZiStorageSize =  2000;

    // array, data member.
    static HanZi16CachedItem  bufferedHanZi_am [ maxHanZiStorageSize ];
    static const unsigned width = PSP_CNFONTS16_16 ::width;
    static const unsigned heigh = PSP_CNFONTS16_16 ::heigh;

    static unsigned short currentSize;
    static bool isInited ; 
    static void increaseFrequency( HanZi16CachedItem * );
    // static unsigned short getLeastFreqIndex ( void );
    inline static HanZi16CachedItem * getCachedItem( unsigned short index )
    { 
        if( index<  HanZiCache ::currentSize )
        {
            return  &HanZiCache ::bufferedHanZi_am[index];
        }
        return 0;
    }
    
    static unsigned short getLeastFreqIndex ( void );

    //do not sort use getleastfreqitem.
    //static unsigned short sortCachedItem ( unsigned short index );

    static  unsigned short getRoomForNewHanZi( void  );  
    inline static bool getIsInited( void )
    {
        return isInited;
    }

    static void init ( void );   

    //private constructor to prevent creating instance
    HanZiCache ();
    ~HanZiCache();
public:

    //remove the freq == 0  hanzi bmp if full
    //  static unsigned short addHanZiBMP ( unsigned short hanzi,  const HanZiFontBitMap &hanzibmp );

    //get the bitmap   hanzi;
    static HanZi16FontBitMap * getHanZiBMP ( const HanZi & hanzi );

    //set all freq = 0; hanzi  = 0 ; everything = 0;
    static void selfTest ( void );

};










class PSP_ChineseUtil
{
    //class to get HanZi 16*16 font data.
    // static PSP_CNFONTS16_16 CN16Font;

    //class to get ASC 8*16 font data.
    // static PSP_ASCFONTS8_16 ASCfont;

    //How many dots can a byte offer ?
    static const unsigned char CnCharDotsPerByte = 8;

    //static unsigned long printAscHanZiString( unsigned short x, unsigned short y, 
    //   unsigned short color, const char *str );	

    static void printASC_dotMatrix(  unsigned short x, unsigned short y, 
        unsigned short color, char _char);

    //==========================================================================
    //  print HanZi ( CN ) in graphic mode.  
    //  
    //========================================================================== 
    //static void printHanZi_dotmatrix(  unsigned short x, unsigned short y, 
    //unsigned short color,unsigned short cn);

    //========================================================================== 
    // in Text mode print the HanZi in Dot matrix
    //========================================================================== 
    //static void printHanZiTextMode( unsigned short cn );


    //==========================================================================
    //    Chinese HanZi to HanZi LIB index
    //==========================================================================                                 
    //   static unsigned long HanZiToHZKLibIndex ( const HanZi &hanzi );
    //   static const unsigned char defaultColor = 1;


    static void  printBitMapInTextMode ( const HanZi16FontBitMap &hanzibitmap );

    static void  printBitMapInTextMode ( const ASCFontBitMap &ascbitmap );



    //private constructor to prevent creating instance 
    PSP_ChineseUtil();
    ~PSP_ChineseUtil();
     static unsigned short TTFFontSize ;




public :
    static unsigned short getTTFSize( void );
    static void   jis2cjk( const unsigned char *jis, unsigned char *cjk);
    static void  printBitMapInTextMode (  const ucharBitMap &hanzibitmap ) ;    
   static void  printBitMapInTextMode ( unsigned int fontSize, int x, int y,  const ucharBitMap &hanzibitmap ) ;    

    static bool  isThisAHanZi(  const unsigned short HZbyte1 );

    static HanZi16FontBitMap * getHanZiBitMap ( const HanZi &hanzi );

    //Return the pointer of a TTF BitMap. (unsigned char bitmap)
    // the instance is in the cache.
    // get the bitmap from TTF bitmap cache.
    // accept hanzi char code and ASC
    static ucharBitMap * getHanZiTTFBitMap ( HanZi & hanzi   );


    //draw one hanzi
    static void   drawHanZi16 ( DrawAttribute &drawAtt, unsigned short color, 
        char * hanZi2Byte     ) ;
    static void   drawHanZi16 ( DrawAttribute &drawAtt, unsigned short color, 
          const HanZi &hanzi ) ;

    static void   drawHanZi16Turn ( DrawAttribute &drawAtt, unsigned short color, 
                                    const HanZi &hanzi     ) ;

    static void selfTestHanZi ( void );
   
    
    static void drawTTFHanzi ( DrawAttribute &drawAtt, unsigned short color, 
                                 HanZi hanzi , BitMapSize *  bmpsize_p = 0  ) ;
    


    static unsigned short fullGBKToUnicode ( unsigned short gbc );
    static unsigned short UnicodeToGBK ( unsigned short unicode );

    static void setTTFSize ( unsigned short size );

    static void selfTestTextModeDrawAllIndex ( void );
    static void selfTestTextModeDrawAllHanzi ( void );
    static void  selfTestTTFGraphicMode( void );
    static void  selfTestTTFTextMode( void );    
    static void selfTestTextMode( void );

    static void selfTestPSPGraphicMode( void ); 
    static bool  isEmpty( HanZi16FontBitMap *bmpp);
    static void  convertUnicodeBigEnd ( unsigned short * unicodeTextBuffer, unsigned long UshortSize   );

   static void drawAscChar8_16( const DrawAttribute &drawAtt, unsigned short color,  char ascChar );
	 
};






#endif  // __PSP_CHINESEHANDLER__



