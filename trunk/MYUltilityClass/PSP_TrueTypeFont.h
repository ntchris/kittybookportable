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
   

    struct TTFBitMapDrawData
    {
        //The whole TTF font bitmap width and heigh
        unsigned short bitMapWidth; 
        unsigned short bitMapHeigh;  
      //  unsigned short antiCropX; 
        unsigned short antiCropY;
    };


//    static const unsigned short  maxFontSizeCount = 18;
    static FT_Library             TTFlibrary;
    static bool                isInited   ;
    static char               ttfFilePath[ 255 ];
    static unsigned short     TTFFontSize ;   
    static FT_Face              face  ;
    static bool               isFontFileLoadOK  ;
    //unsigned short       fontSize ;
 //   static unsigned short      goodSizeCount ;
    

   
  //  static unsigned char fontSizeList_am[  maxFontSizeCount ];

    //check if the size is in the list.
    bool isTheFontSizeValid ( unsigned short );
    
    // not every size is useful.
    // so ...
    void setDefaultFontSize( void ) ;
    void clear ( void );
     // FreeType API getChar 得到的bmp是缩小了的, 而且它在整个字体框中的位置是不确定的.
// 所以需要计算中它复员的大小并且要知道放置的为止. 例如句号. 要放在下面. "要放在上面.
//Input glyph, new font width and heigh ( the whole bitmap )
//OUTPUT  antiCropX, antiCrop Y
   static void getDrawData ( unsigned short unicode ,    FT_GlyphSlot *glyph,  
                       struct TTFBitMapDrawData  * drawData) ;
 
   void  preloadEnglishASC ( void );
public:
    
     PSP_TrueTypeFont();
    ~ PSP_TrueTypeFont();
   // static PSP_TrueTypeFont * getInstance( void );
    
   //draw a hanzi to Bitmap.
   // use free type api
   // let this function to new space for the bmp
   //static bool drawHanZiTTFToBMP( unsigned short hanzi,  
   //              ucharBitMap * hanzibmp  );
        
   static bool drawHanZiTTFToBMPMono(  HanZi &hanzi,  
                                           ucharBitMap * hanzibmp  );
                                           
                                           
  //  void getAllAvailableFontSize( void ); 
 //   static void init( void );
    bool loadFontFile  ( const char * ttfpath );
    bool  setTTFFontSize ( unsigned short size );
    unsigned short getTTFFontSize ( void );

};


//One item takes  bytes
struct TTF_HanZiCachedItem
{  

    //each HanZi has two char, so it's A Short.
    //eg. HanZi "我" is CE D2 , so it's 53966 ( D2CE )
    unsigned short hanZi;

    //How freq is it used 
    unsigned short frequency;

    // must store bitmap and width and heigh
    // because width and heigh are not the same for each bmp
    ucharBitMap hanZiTTFBitMap;

};



//store the freq used HanZi bit map.
class TTF_HanZiCache
{

    // store up to 2000 HanZiCachedItem, so 2000*16*16 = 512 KBytes
    static const unsigned maxHanZiStorageSize = 2500;
    //from 33 - {   to  126 ~
    static const unsigned char maxASCStorageFrom = 33 , maxASCStorageTo = 126;    
    static const unsigned short maxASCStorageSize = maxASCStorageTo - maxASCStorageFrom + 1;

    // array, data member.
    static TTF_HanZiCachedItem  bufferedHanZi_am [ maxHanZiStorageSize ];
    static TTF_HanZiCachedItem  bufferedASC_am [ maxASCStorageSize  ];
    //the size can change
     
      unsigned short currentSize;
      //unsigned short currentASCCacheSize;
      bool isInited ; 
      void increaseFrequency( TTF_HanZiCachedItem * );
    // static unsigned short getLeastFreqIndex ( void );
    inline   TTF_HanZiCachedItem * getCachedItem( unsigned short index )
    { 
        if( index<  TTF_HanZiCache ::currentSize )
        {
            return  &TTF_HanZiCache ::bufferedHanZi_am[index];
        }
        return 0;
    }

    inline   TTF_HanZiCachedItem * getASCCachedItem( unsigned short index )
    { 
        if( index<  TTF_HanZiCache :: maxASCStorageSize )
        {
            return  &TTF_HanZiCache ::bufferedASC_am [ index ];
        }
        return 0;
    }


    unsigned short getLeastFreqIndex ( void );
    //unsigned short getASCLeastFreqIndex ( void );

    //unsigned short getRoomForNewASC( void  );
    unsigned short getRoomForNewHanZi( void  );  
    inline   bool getIsInited( void )
    {
        return isInited;
    }

      void init ( void );   

   
    ucharBitMap * getASCTTFBitMap ( unsigned short asc );
    
public:


  
    TTF_HanZiCache ();
    ~TTF_HanZiCache();
    
    //remove the freq == 0  hanzi bmp if full
    //  static unsigned short addHanZiBMP 
    //         ( unsigned short hanzi,  const HanZiFontBitMap &hanzibmp );



    inline bool isInASCCache( unsigned short asc )
    {
        if (  (  asc >= maxASCStorageFrom )//33 
            &&  
            (  asc <= maxASCStorageTo   ) // 126;    
            )
        {
           return true;
        }
        return false;
    }

      void clearAllCachedItem ( void );
      void clearCachedItem ( unsigned short index  );
      
    //  void clearASCCachedItem( unsigned short index  );
      void clearAllASCCachedItems ( void  );
      
      ucharBitMap * getHanZiTTFBitMap (    HanZi &hanZi );

    //static TTF_HanZiCache * getInstance ( void );
     
    //set all freq = 0; hanzi  = 0 ; everything = 0;
      void selfTest ( void );

};

/*
class TTF_Utility
{
 
    
 public:
   static  TTF_HanZiCache TTFcache;
   static  PSP_TrueTypeFont ttfFont;
    static ucharBitMap * getHanZiTTFBitMap ( unsigned short hanzi   );

    static void  setTTFSize ( unsigned short size );

};
*/
/*

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
*/

#endif     //__PSPTTF__


