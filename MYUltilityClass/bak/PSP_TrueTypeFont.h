#ifndef __PSPTTF__
#define __PSPTTF__




#include <ft2build.h>
#include FT_FREETYPE_H


#include <PSP_GLOBAL.h> 


/*
struct ucharBitMap
{
    unsigned short width;
    unsigned short heigh;	
    unsigned char  *data_p;

};
*/


using namespace PSP_Constants;

class PSP_TrueTypeFont
{
    static const unsigned short  maxFontSizeCount = 18;
    FT_Library             TTFlibrary;
    bool                isInited   ;
    char               ttfFilePath[ 255 ];
    unsigned short     TTFFontSize ;   
    FT_Face              face  ;
    bool               isFontFileLoadOK  ;
    //unsigned short       fontSize ;
    unsigned short      goodSizeCount ;
    

   
    unsigned char fontSizeList_am[  maxFontSizeCount ];

    //check if the size is in the list.
    bool isTheFontSizeValid ( unsigned short );
    
    // not every size is useful.
    // so ...
    void setDefaultFontSize( void ) ;
    void clear ( void );
     
public:
    
     PSP_TrueTypeFont();
    ~ PSP_TrueTypeFont();
    static PSP_TrueTypeFont * getInstance( void );
    //draw a hanzi to Bitmap.
    // use free type api
    // let this function to new space for the bmp
    bool drawHanZiTTFToBMP( unsigned short hanzi,  
        ucharBitMap * hanzibmp  );
    void getAllAvailableFontSize( void ); 
 //   static void init( void );
    bool loadFontFile  ( const char * ttfpath );
    void  setTTFFontSize ( unsigned short size );

};


//One item takes  bytes
struct HanZiTTFCachedItem
{  

    //each HanZi has two char, so it's A Short.
    //eg. HanZi "Œ“" is CE D2 , so it's 53966 ( D2CE )
    unsigned short hanZi;

    //How freq is it used 
    unsigned short frequency;

    // must store bitmap and width and heigh
    // because width and heigh are not the same for each bmp
    ucharBitMap hanZiTTFBitMap;

};



//store the freq used HanZi bit map.
class HanZiTTFCache
{

    // store up to 2000 HanZiCachedItem, so 2000*16*16 = 512 KBytes
    static const unsigned maxHanZiStorageSize =  3;

    // array, data member.
    static HanZiTTFCachedItem  bufferedHanZi_am [ maxHanZiStorageSize ];
    //the size can change
     
      unsigned short currentSize;
      bool isInited ; 
      void increaseFrequency( HanZiTTFCachedItem * );
    // static unsigned short getLeastFreqIndex ( void );
    inline   HanZiTTFCachedItem * getCachedItem( unsigned short index )
    { 
        if( index<  HanZiTTFCache ::currentSize )
        {
            return  &HanZiTTFCache ::bufferedHanZi_am[index];
        }
        return 0;
    }

      unsigned short getLeastFreqIndex ( void );


       unsigned short getRoomForNewHanZi( void  );  
    inline   bool getIsInited( void )
    {
        return isInited;
    }

      void init ( void );   

  
    
public:


  
    HanZiTTFCache ();
    ~HanZiTTFCache();
    
    //remove the freq == 0  hanzi bmp if full
    //  static unsigned short addHanZiBMP 
    //         ( unsigned short hanzi,  const HanZiFontBitMap &hanzibmp );

    
      void clearAllCachedItem ( void );
      void clearCachedItem ( unsigned short index  );
      ucharBitMap * getHanZiTTFBitMap ( unsigned short hanZi );

    static HanZiTTFCache * getInstance ( void );
     
    //set all freq = 0; hanzi  = 0 ; everything = 0;
      void selfTest ( void );

};






class  TestPSP_ChineseUtil
{

    //unsigned short TTFFontSize ;

public:

    //Return the pointer of a TTF BitMap. (unsigned char bitmap)
    // the instance is in the cache.
    // get the bitmap from TTF bitmap cache.
    static ucharBitMap * getHanZiTTFBitMap ( unsigned short hanzi   );


    //set the ttf font size, 
    //SimSun
    //working 15 17  19 20 21  22  23  ...26
    //not working 16   18
    // once set a new size, clear the cache.
     static void selfTest( void );
     static void printBitMapInTextMode (  const ucharBitMap &hanzibitmap ) ;
     void  selfTestGraphicMode( void );
     void  selfTestTextMode( void );
};


#endif     //__PSPTTF__


