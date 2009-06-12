
#include <ft2build.h>
#include FT_FREETYPE_H

#include <PSP_TrueTypeFont.h>


//#include <PSP_GLOBAL.h> 
#include <PSP_ChineseUtil.h>

const bool debugCache_cg = false;
const bool debug_ttf_cg =  false;


#include <PSP_Window.h>



#ifdef __VC__

#include <unicodeToGBKTable.h>

#endif






using namespace PSP_Constants;

//===================================================================
//   Class PSP_TrueTypeFont 
//
//===================================================================
FT_Library           PSP_TrueTypeFont::  TTFlibrary;
bool                PSP_TrueTypeFont::isInited   ;
char               PSP_TrueTypeFont::ttfFilePath[ 255 ];
unsigned short     PSP_TrueTypeFont::TTFFontSize ;   
FT_Face              PSP_TrueTypeFont::face  ;
bool               PSP_TrueTypeFont::isFontFileLoadOK  ;
//unsigned short       fontSize ;
//unsigned short     PSP_TrueTypeFont:: goodSizeCount = 0;



//unsigned char PSP_TrueTypeFont:: fontSizeList_am[ PSP_TrueTypeFont:: maxFontSizeCount ];




extern PSP_TrueTypeFont * pspTrueTypeFont_cgp ;
extern TTF_HanZiCache * TTF_HanZiCache_cgp ; 




//===================================================================
//  prevent create more than 1 instance.
//===================================================================
/*PSP_TrueTypeFont * PSP_TrueTypeFont :: getInstance( void )
{
static PSP_TrueTypeFont pspTTFintance;
if ( pspTrueTypeFont_cgp == 0 )
{
pspTrueTypeFont_cgp = &pspTTFintance;       
}
return pspTrueTypeFont_cgp;


}
*/



PSP_TrueTypeFont::  PSP_TrueTypeFont()
{
     
     clear ();    
     isInited = false ;
  
}



PSP_TrueTypeFont:: ~ PSP_TrueTypeFont()
{
    FT_Done_Face    ( face );
    FT_Done_FreeType( TTFlibrary );
}



void PSP_TrueTypeFont ::clear ( void )
{
  /*  for ( int i = 0;i<maxFontSizeCount;i++)
    {
        fontSizeList_am [i] = 0;
    }   
*/
    isFontFileLoadOK = false;
    // clear the path string
    ttfFilePath[0] = 0;

    isFontFileLoadOK = false;
//    goodSizeCount = 0;


    TTFFontSize =  0;   

  //  isFontFileLoadOK = false;
    //fontSize = 0;
//    goodSizeCount = 0;


} 

 



void PSP_TrueTypeFont ::setDefaultFontSize( void )
{ 

    const unsigned short defaultFontSize = 14 ;
  /*  for( int i=0; i< goodSizeCount ; i++)
    {
        testingSize =  fontSizeList_am[ i ]; 
        if (  testingSize >20 && testingSize < 30 )
        {

            break;
        }   
    }


    if ( testingSize == 0 )
    {
        testingSize = fontSizeList_am [ 0 ];
    }
    setTTFFontSize ( testingSize );
    */
    setTTFFontSize ( defaultFontSize );

}



bool PSP_TrueTypeFont ::isTheFontSizeValid ( unsigned short size )
{
     
    if ( size >= minTTFFontSize  && size <= maxTTFFontSize )
       return true;
    else
       return false;

}

bool PSP_TrueTypeFont ::loadFontFile ( const char * ttfpath )
{
    FT_Error      error;
    if ( !  isInited ) 
    {          
        error = FT_Init_FreeType( &TTFlibrary ); 
        if ( error ) 
        {
            return false;
        }
        else 
        { 
            isInited = true;
        }   
    }

    if ( ttfpath == 0 ) return false;    

    if ( isFontFileLoadOK )
    {
        FT_Done_Face  ( face );
        isFontFileLoadOK  = false;

    }   
    error = FT_New_Face( TTFlibrary, ttfpath, 0, &face );
    if ( error )
    {   char *msg;
        msg = new char[ pathMaxLen + 80 ];
        
        sprintf ( msg , "can't load font file:%s please go to Config Window to select one.\n\
 TTF字体文件找不到, 请先去配置窗口设置" , ttfpath );
        
#ifdef __PSPSDK__
        PSP_Window::showMessage( msg );
#else
        printf(msg);        
#endif
        delete msg;   

        return false;
    }
    else
    {
        if( ttfFilePath != ttfpath )
        {  
           ttfFilePath [0] = 0;
           strcpy ( ttfFilePath , ttfpath );
        }
        isFontFileLoadOK = true;
    }
    TTFFontSize = 0;
    //getAllAvailableFontSize();
    setDefaultFontSize();

    return true;
}




unsigned short PSP_TrueTypeFont ::getTTFFontSize (  void )
{
    return this->TTFFontSize;
}


bool  PSP_TrueTypeFont :: setTTFFontSize ( unsigned short size )
{
   // const int mustBeGoodSize = 1;
    //const int insaneSize  = 200;

    if ( size == TTFFontSize )
    {
        if ( debugCache_cg )
            printf(  "new font size == old font size \n");
        return true;
    }

     

    FT_Error error ;
    if (  isTheFontSizeValid( size ) )  
    {   
        error = FT_Set_Pixel_Sizes ( face, 0, size  ); 
        if( !error )
        {   
            if ( debugCache_cg )
            {  
                printf(  "set new font size:  %u done\n" , size );
            } 
            TTFFontSize = size ;
            TTF_HanZiCache_cgp ->clearAllCachedItem ();
            TTF_HanZiCache_cgp ->clearAllASCCachedItems ();
            
            // only in this case , clear the TTF cache 
            
            ////for speed , preload a-z, A-Z
             //preloadEnglishASC();
            return true;       
        }

    }
    else
    {
        if ( debugCache_cg )
        {
            printf(  "new font size is not valid so still use old size \n");
        } 	
        return false;
    }
       
    return false;

}


/*
//for speed , preload a-z, A-Z
void  PSP_TrueTypeFont :: preloadEnglishASC ( )
{
 
    ucharBitMap * ucharbitmap = 0; 
    HanZi hanzitemp; 
    unsigned short  i;
    //use this 'w' to get the ASC width
    for( i='a';i<='z' ;i ++ )
    { 
       hanzitemp.ushort = i;
       ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap ( hanzitemp    );
    }   
    for( i='A';i<='Z' ;i ++ )
    { 
       hanzitemp.ushort = i;
       ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap ( hanzitemp    );
    }   
    


}
*/

//=================================================================
//class TTF_HanZiTTFCache
//
//===================================================================
//static member for class HanZiCache
TTF_HanZiCachedItem  TTF_HanZiCache::bufferedHanZi_am [ maxHanZiStorageSize ];
TTF_HanZiCachedItem  TTF_HanZiCache::bufferedASC_am [ maxASCStorageSize ];


TTF_HanZiCache ::TTF_HanZiCache()
{
    this->isInited = false;
    init();
}

TTF_HanZiCache:: ~TTF_HanZiCache()
{
    clearAllCachedItem();
    clearAllASCCachedItems();
    if (  debugCache_cg )
    {   printf( "TTF_HanZiTTFCache destructor \n" );    
    }
}


/*TTF_HanZiTTFCache * TTF_HanZiTTFCache :: getInstance ( void )
{
static TTF_HanZiTTFCache ttfCache;
if ( TTF_HanZiTTFCache_cgp  == 0 )
{
TTF_HanZiTTFCache_cgp  = &ttfCache;
}
return TTF_HanZiTTFCache_cgp ;


}

*/




void TTF_HanZiCache :: init ( void )
{

    if (  debugCache_cg )
    {   printf( "initing \n" );    
    }


    if( TTF_HanZiCache::isInited ) return ;

    isInited = true;
    unsigned short i ;

    TTF_HanZiCachedItem *p;
    for ( i=0 ; i< maxHanZiStorageSize ;i++)
    {
        p = & bufferedHanZi_am[i];
        p->frequency = 0;
        p->hanZi = 0;    
        p->hanZiTTFBitMap.data_p = 0;
        p->hanZiTTFBitMap.width  = 0;
        p->hanZiTTFBitMap.heigh  = 0;

    }

    for ( i=0 ; i< maxASCStorageSize ;i++)
    {
        p = & bufferedASC_am[i];
    
        p->hanZi = 0;    
        p->hanZiTTFBitMap.data_p = 0;
        p->hanZiTTFBitMap.width  = 0;
        p->hanZiTTFBitMap.heigh  = 0;

    }


    currentSize = 0;
  
}  



unsigned short TTF_HanZiCache :: getRoomForNewHanZi( void  )
{
    unsigned short roomIndex;     

    if ( this->currentSize < maxHanZiStorageSize )
    {  
        //There is enough room, let's add one        
        
        roomIndex = currentSize;
        currentSize ++;
    }   
    else
    {   // There is NOT enough ROOM, let's find a least freq item to replace
        roomIndex = this-> getLeastFreqIndex(  );
    } 
    return  roomIndex;   
}

 


unsigned short TTF_HanZiCache :: getLeastFreqIndex ( void )
{
    unsigned short leastFreq = 0xffff;
    unsigned short index = 0;
    unsigned short i;
    TTF_HanZiCachedItem *tempCachedItem_P = NULL;
    for( i =0;i< this-> currentSize;i++ )
    {
        tempCachedItem_P = this->  getCachedItem(i);

        if( tempCachedItem_P == NULL)
        {    
            if ( debugCache_cg )
            {   printf( "impossible ! tempCachedItem_P = NULL !!! \n ");
            } 	
            return 0;
        }        

        if( tempCachedItem_P->frequency < leastFreq )
        {            
            leastFreq =  tempCachedItem_P->frequency ;
            index = i;
            if( tempCachedItem_P->frequency == 0 )
            {   
                // if freq = 0 , that's it.
                break;
            }
        }

    }
    return index;

}



void TTF_HanZiCache ::clearAllCachedItem ( void )
{
    if ( ! TTF_HanZiCache ::isInited  )
    {
       init();
    }   
    // nothing to clear
    if( currentSize <= 0 )
    {
        return ;
    }	
    if( debugCache_cg )
    {
        printf("clear all old cache space  total: %u cached items \n", currentSize );	
    }
    for ( unsigned int i=0 ; i< currentSize ; i++ )
    {
        clearCachedItem ( i );   
    }  

    currentSize = 0;
}





void TTF_HanZiCache :: clearAllASCCachedItems ( void  )
{
    

    TTF_HanZiCachedItem *p ;	
    unsigned i;
    for( i =0;i< maxASCStorageSize;i++)
    {
        p = & bufferedASC_am[ i ];
        //p->frequency = 0;
        p->hanZi = 0;    

        p->hanZiTTFBitMap.width  = 0;
        p->hanZiTTFBitMap.heigh  = 0;

        if( p->hanZiTTFBitMap.data_p  )
        {
            if( debugCache_cg )
            {
                printf ( "trying to delete %u \n",  p->hanZi  );
            }
            delete p->hanZiTTFBitMap.data_p ;
            //after delete must reset data_p to 0
            p->hanZiTTFBitMap.data_p = 0;
            
        }	   
     }

}

void TTF_HanZiCache :: clearCachedItem ( unsigned short index  )
{
    if( debugCache_cg )
    {
       printf ( "clearCachedItem clearing index: %u \n", index );
    
    }
    if ( index >= maxHanZiStorageSize ) 
    {
        return;
    }

    TTF_HanZiCachedItem *p ;	
    p = & bufferedHanZi_am[ index ];
    p->frequency = 0;
    p->hanZi = 0;    

    p->hanZiTTFBitMap.width  = 0;
    p->hanZiTTFBitMap.heigh  = 0;

    if( p->hanZiTTFBitMap.data_p  )
    {
         if( debugCache_cg )
         {
            //printf ( "trying to delete %u \n",  p->hanZiTTFBitMap.data_p  );
         }
        delete p->hanZiTTFBitMap.data_p ;
        //after delete must reset data_p to 0
        p->hanZiTTFBitMap.data_p = 0;

        if( debugCache_cg )
        {
            printf("delete old cache space index : %u \n", index );	
        }
    }	   


}


void TTF_HanZiCache :: increaseFrequency( TTF_HanZiCachedItem * cacheItem_p )
{
    if (  debugCache_cg )
    {
        printf( "increaseFrequency %u \n", cacheItem_p->hanZi );
    }    

    if( cacheItem_p ->frequency < (0xffff-1 ) )
    {
        cacheItem_p ->frequency ++;
    }   
    //after increase,  the position of this item should be move forward!
    //to make the whole item list sorted


}

 

ucharBitMap * TTF_HanZiCache::getASCTTFBitMap ( unsigned short asc )
{

	if ( debugCache_cg )
	{
		printf("ucharBitMap * TTF_HanZiCache::getASCTTFBitMap ( unsigned short %u ) \n", asc );
	}     
    ucharBitMap *temp_ASCBMP_p = NULL;
    TTF_HanZiCachedItem *tempASCCachedItem_P = NULL;
    //bool found = false;
    //unsigned int i = 0;
    if ( !isInASCCache (  asc ) ) return NULL;
    

    if ( debugCache_cg )
	{
		printf(" isInASCCache \n");
	}     

	    
    unsigned short cacheIndex  = asc - maxASCStorageFrom;
      
    tempASCCachedItem_P = getASCCachedItem( cacheIndex );

    if( tempASCCachedItem_P == NULL )
    {   //impossible!!!!
        return NULL;
     }

    //found and done
     if ( tempASCCachedItem_P->hanZi ==   asc  )
    {   
	    if ( debugCache_cg )
	    {
		   printf( "found !! \n");    
		}
        temp_ASCBMP_p = &( tempASCCachedItem_P->hanZiTTFBitMap );            
        return temp_ASCBMP_p ;
    }   
    
    ucharBitMap *AddASCBMP  ; 
    //not found in the cache ???
    // let's add it to the cache !!!!!
    //PSP_Window::showMessage( "Not found");

    if (  debugCache_cg )
    {
        printf( "not found in ASC cache : %u \n",  asc  );
    } 

    tempASCCachedItem_P ->hanZi = asc;
    AddASCBMP = & tempASCCachedItem_P->hanZiTTFBitMap;
    HanZi hanziAsc;
    hanziAsc.ushort = asc; hanziAsc.isUnicode = false;
    PSP_TrueTypeFont::drawHanZiTTFToBMPMono( hanziAsc, AddASCBMP  );

    

    return AddASCBMP;

}

ucharBitMap * TTF_HanZiCache::getHanZiTTFBitMap ( HanZi &hanzi )
{
 
    if(   isInASCCache ( hanzi.ushort )  ) 
    {
       //this is a ASC, ASC use another  cache to enhance the speed
       return getASCTTFBitMap ( hanzi.ushort);
    }
    
    ucharBitMap *temp_hanZiBMP_p = NULL;
    TTF_HanZiCachedItem *tempCachedItem_P = NULL;
    bool found = false;
    unsigned int i = 0;
    
    for(i=0;i<currentSize;i++)
    {    
        tempCachedItem_P = getCachedItem( i );

        if( tempCachedItem_P == NULL )
        {   //impossible!!!!
            return NULL;
        }

//为什么会找到呢??? !!!
        if ( tempCachedItem_P->hanZi == hanzi.ushort )
        {   //increase and resort
            increaseFrequency( tempCachedItem_P );
            //newIndex = HanZiCache ::sortCachedItem ( i );
            //tempCachedItem_P = getCachedItem( newIndex );
            temp_hanZiBMP_p = &( tempCachedItem_P->hanZiTTFBitMap );            
            found = true;
            break;
        }   
    }    
    
    
    if ( found )
    {
        
        return temp_hanZiBMP_p;
    }else
    {
        
    }

    ucharBitMap *AddhanZiBMP  ; 
    //not found in the cache ???
    // let's add it to the cache !!!!!
    //PSP_Window::showMessage( "Not found");
    if( !found  )
    {
        if (  debugCache_cg )
        {
            printf( "not found in cache hanzi: %u \n", hanzi.ushort  );
        } 


        bool r;       
        unsigned short roomIndex;
        roomIndex = this->getRoomForNewHanZi( ); 
        tempCachedItem_P = this->getCachedItem ( roomIndex );        
        this->clearCachedItem ( roomIndex ) ;

        tempCachedItem_P ->frequency = 0;
        /*if( show36974 )
        {
           PSP_Window::showMessage(" TTF_HanZiCache::getHanZiTTFBitMap(): tempCachedItem_P ->hanZi = hanzi.ushort; " );
        }*/
        tempCachedItem_P ->hanZi = hanzi.ushort;
        AddhanZiBMP = & tempCachedItem_P->hanZiTTFBitMap;
        //r = PSP_TrueTypeFont::drawHanZiTTFToBMP( hanZi, AddhanZiBMP  );
        r = PSP_TrueTypeFont::drawHanZiTTFToBMPMono( hanzi, AddhanZiBMP  );
        if( !r)
        {
           //PSP_Window::showMessage( "PSP_TrueTypeFont::drawHanZiTTFToBMPMn() returns false");
        }
    }


    if (  debugCache_cg )
    {
        if( found )printf( "found in cache index %u \n", i );
    } 

    return AddhanZiBMP;

}


/*

ucharBitMap *  TTF_Utility::getHanZiTTFBitMap ( unsigned short hanzi   )
{
ucharBitMap * bitmap;
//hanZiTTFCache_cgp 
bitmap = TTFcache. getHanZiTTFBitMap (   hanzi );

return bitmap;       	
}

void  TTF_Utility::setTTFSize ( unsigned short size )
{
bool r;
r =  ttfFont.setTTFFontSize( size );
if ( r )
TTFcache.clearAllCachedItem ();  

}

*/
/*

// the instance is in the cache.
// get the bitmap from TTF bitmap cache.
ucharBitMap * TestPSP_ChineseUtil::getHanZiTTFBitMap ( unsigned short hanzi   )
{
ucharBitMap * bitmap;
//TTF_HanZiTTFCache_cgp 
bitmap = TTF_HanZiTTFCache_cgp-> getHanZiTTFBitMap (   hanzi );

return bitmap;                 
}
*/

/*
void TestPSP_ChineseUtil:: printBitMapInTextMode (  const ucharBitMap &hanzibitmap ) 
{
unsigned xi, yi, xc, yc;
xc =  hanzibitmap.width ;
yc =  hanzibitmap.heigh ;
unsigned index =0;

printf( "\n");
printf( "\n");
for( yi=0; yi< yc ; yi++)
{
for( xi=0; xi< xc ; xi++)
{
if ( hanzibitmap.data_p[index] )
{
printf("O");
}else   
{
printf(" ");
}
index ++;

}
printf("\n");
}    


}




void  TestPSP_ChineseUtil:: selfTestGraphicMode( void )
{


//pspTTF->init();
pspTrueTypeFont_cgp ->loadFontFile("c:\\winnt\\fonts\\simsun.ttc");
pspTrueTypeFont_cgp  ->setTTFFontSize ( 24 );
HanZi hanzi("煋") ,  hanzi2 ("事"), hanzi3("新"), hanzi4("替");

ucharBitMap *bitmap ;

//pspTrueTypeFont_cgp  ->setTTFFontSize ( 2* (i%10) + 25 );

bitmap = getHanZiTTFBitMap( hanzi.ushort );  





bitmap = getHanZiTTFBitMap( hanzi.ushort );
printBitMapInTextMode ( *bitmap ) ;



bitmap = getHanZiTTFBitMap ( hanzi2.ushort );
printBitMapInTextMode ( *bitmap ) ;

bitmap = getHanZiTTFBitMap ( hanzi2.ushort );
printBitMapInTextMode ( *bitmap ) ;


bitmap = getHanZiTTFBitMap ( hanzi3 .ushort );
printBitMapInTextMode ( *bitmap ) ;
//delete bitmap->data_p;

bitmap = getHanZiTTFBitMap ( hanzi4.ushort );
printBitMapInTextMode ( *bitmap ) ;


bitmap = getHanZiTTFBitMap ( hanzi2.ushort );
printBitMapInTextMode ( *bitmap ) ;

bitmap = getHanZiTTFBitMap ( hanzi3 .ushort );
printBitMapInTextMode ( *bitmap ) ;
}



}



void  TestPSP_ChineseUtil:: selfTestTTFTextMode( void )
{


//pspTTF->init();
pspTrueTypeFont_cgp ->loadFontFile("c:\\winnt\\fonts\\simsun.ttc");
pspTrueTypeFont_cgp  ->setTTFFontSize ( 24 );
HanZi hanzi("煋") ,  hanzi2 ("事"), hanzi3("新"), hanzi4("替");


ucharBitMap *bitmap ;

for ( int i = 0;i < 1000; i++)  
{ 


pspTrueTypeFont_cgp  ->setTTFFontSize ( 2* (i%10) + 25 );

bitmap = getHanZiTTFBitMap( hanzi.ushort );  
printBitMapInTextMode ( *bitmap ) ;




bitmap = getHanZiTTFBitMap( hanzi.ushort );
printBitMapInTextMode ( *bitmap ) ;



bitmap = getHanZiTTFBitMap ( hanzi2.ushort );
printBitMapInTextMode ( *bitmap ) ;

bitmap = getHanZiTTFBitMap ( hanzi2.ushort );
printBitMapInTextMode ( *bitmap ) ;


bitmap = getHanZiTTFBitMap ( hanzi3 .ushort );
printBitMapInTextMode ( *bitmap ) ;
//delete bitmap->data_p;

bitmap = getHanZiTTFBitMap ( hanzi4.ushort );
printBitMapInTextMode ( *bitmap ) ;


bitmap = getHanZiTTFBitMap ( hanzi2.ushort );
printBitMapInTextMode ( *bitmap ) ;

bitmap = getHanZiTTFBitMap ( hanzi3 .ushort );
printBitMapInTextMode ( *bitmap ) ;
}

}

*/

/*typedef struct  FT_Glyph_Metrics_
{
FT_Pos  width;
FT_Pos  height;

FT_Pos  horiBearingX;
FT_Pos  horiBearingY;
FT_Pos  horiAdvance;

FT_Pos  vertBearingX;
FT_Pos  vertBearingY;
FT_Pos  vertAdvance;

} FT_Glyph_Metrics;
*/



// FreeType API getChar 得到的bmp是缩小了的, 而且它在整个字体框中的位置是不确定的.
// 所以需要计算中它复员的大小并且要知道放置的为止. 例如句号. 要放在下面. "要放在上面.
//Input glyph, new font width and heigh ( the whole bitmap )
//OUTPUT  antiCropX, antiCrop Y
void PSP_TrueTypeFont :: getDrawData ( unsigned short charword ,   FT_GlyphSlot *glyph, 
                                       TTFBitMapDrawData  * drawData)  
{
     //anti crop for hanzi like "一"

    //对于汉字来说 ,  testFtBitmap->width ->rows  永远等于 
    // metrics.height/64 和 metrics.width /64;  
    // 但是这个bmp可能不是满屏的 ( croped )
    // 例如别字体除外. 17, 19
    const FT_Bitmap  *testFtBitmap;
    testFtBitmap = & face->glyph->bitmap;
  
    //X 宽度不作修改了      这是不对的.   有些字实在太小了
    drawData->bitMapWidth  = testFtBitmap->width ;
    if ( charword <128 )
    {
        drawData->bitMapWidth  = testFtBitmap->width ;
    }        
    else
    {  
        //限制汉字不要太小了, 以fontsize -1 为最小值.
        if (  testFtBitmap->width < TTFFontSize -1  ) 
        {
            drawData->bitMapWidth = TTFFontSize -1 ;
        }else
        {
            drawData->bitMapWidth = testFtBitmap ->width ;
        }
     }

    if ( face->glyph->bitmap_left > 0 )
    {   
      //  if (  drawData->bitMapWidth < testFtBitmap->width + face->glyph->bitmap_left  )
        {
           //drawData->bitMapWidth  = testFtBitmap->width + face->glyph->bitmap_left ;
           
           //reason :  01  0后面的东西 要后移动一些
           drawData->bitMapWidth = drawData->bitMapWidth + face->glyph->bitmap_left ;
        }
    }else    
    {  
    }   
   


  /*  if ( face->glyph->bitmap_left < 0 )
    {
//        drawData->antiCropX    = 0;
        if ( (testFtBitmap->width + face->glyph->bitmap_left) > 0);
        {
           drawData->bitMapWidth =   testFtBitmap->width + face->glyph->bitmap_left;
        }
    }else
    {
       //  drawData-> antiCropX  = face->glyph->bitmap_left ;
    }   
*/

     
     if (  testFtBitmap->rows < TTFFontSize ) 
     {
         drawData->bitMapHeigh = TTFFontSize ;
     }else
     {
         drawData->bitMapHeigh  = testFtBitmap ->rows ;
     }
        //end of HanZi or ASC if 

    if ( debug_ttf_cg )
    {
    printf (" ttf Font size is %u  \n"  , TTFFontSize );	
    printf(" drawData->bitMapWidth  ; %u ",  drawData->bitMapWidth  );
    printf("drawData->bitMapHeigh ; %u \n", drawData->bitMapHeigh );
     }


 //   int startFrom = 0, i=0   ; unsigned char data =0 , mask = 0;
 
    int baseLine;;
  

    baseLine  = TTFFontSize * 9/10  ;

    if ( debug_ttf_cg )
    printf ("bitmap_left : %d \n", face->glyph->bitmap_left );



    // reason: 有时候testFtBitmap ->width 很小(croped),但是有时候比FontSize还大.
    // 当它很大的时候, 如果 face->glyph->bitmap_left 也大, 两者加起来就会比 newWidth大了. 
    // 而newWidth 等于 fontSize 或者 testFtBitmap ->width 之中的大者.
    /*if ( (  drawData->antiCropX + testFtBitmap ->width ) >=  drawData->bitMapHeigh )
    {
        drawData->antiCropX = 0;    
    }*/

    if ( debug_ttf_cg ) printf ("bitmap_top : %d \n", face->glyph->bitmap_top );

    //prevent negative result  - 1 = 4777777777 insane big number
    if( baseLine  >  face->glyph->bitmap_top  )
    {
        drawData->antiCropY  = baseLine - face->glyph->bitmap_top ;
    }else
    {   baseLine = face->glyph->bitmap_top  ;
        drawData->antiCropY   = 0;
    }
   
    if ( debug_ttf_cg )
    { 
       printf(" testFtBitmap->rows = %u ",  testFtBitmap->rows );
       printf(" testFtBitmap->width = %u \n",  testFtBitmap->width );
       printf(" testFtBitmap->pitch = %u \n ",  testFtBitmap->pitch );
       printf(" baseLine = %u \n" , baseLine );
    }
    //有时候 比较混乱....
    //free type裁减后的字体, 跟原字体(Font Size一样)的时候, 不要进行位移运算.

    if ( testFtBitmap->rows >= TTFFontSize )
    {   //这种情况下 不需要复原
        drawData->antiCropY = 0;	

    }
/*    if ( testFtBitmap->width >= TTFFontSize )
    {   //这种情况下 不需要复原
        drawData->antiCropX  = 0;	
    }*/

    if ( ( drawData->antiCropY  + testFtBitmap->rows ) >=  drawData->bitMapHeigh  )
    {
        if(  drawData->bitMapHeigh > testFtBitmap->rows  )
        {
            drawData->antiCropY = ( drawData->bitMapHeigh - testFtBitmap->rows );
        }   
        else
        {
            drawData->antiCropY = ( testFtBitmap->rows - drawData->bitMapHeigh  );
        }   

    }  

}


bool PSP_TrueTypeFont :: drawHanZiTTFToBMPMono( HanZi &hanzi,  
                                               ucharBitMap * hanzibmp  )
{
    if( hanzibmp == NULL)   return  false;

    // 1 use free type to get the glyph        
    // 2 new space for hanzibmp
    // 3 copy the glyph bmp to hanzibmp

    // the font is not loaded and the font size is not set !!!   
    if ( PSP_TrueTypeFont::TTFFontSize == 0 ) return false;
    if ( !PSP_TrueTypeFont::isFontFileLoadOK )
    {
        const char msg[]={ "font file not loaded !! \n " };
#ifdef __PSPSDK__         
        PSP_Window::showMessage( msg );
#else
        printf( msg );
        char temp; scanf("%c", &temp  );        
#endif        
        return false;
    }

    unsigned short unicode ; 
    /*if( hanzi.ushort == 36974 )
    {
         PSP_Window::showMessage( "drawHanZiTTFToBMPMono  36974 " );
    }*/
    //#ifdef __VC__
    // unicode =  fullGBKToUnicode (  hanzi  );
    //#else
    if ( !hanzi.isUnicode )
    {
    	 unicode =  PSP_ChineseUtil::fullGBKToUnicode (  hanzi.ushort  );
    /*	 if( hanzi.ushort == 36974 )
        {
           PSP_Window::showMessage( "drawHanZiTTFToBMPMono  36974 : is not unicode, change to ", unicode );
        }*/
    
    }else
    {
        unicode = hanzi.ushort;
        /*if( hanzi.ushort == 36974 )
        {
           PSP_Window::showMessage( "drawHanZiTTFToBMPMono  36974 : is unicode ", unicode );
        }*/
    }	 	
    //#endif

    if ( unicode == 0 )
    {

        unicode = hanzi.ushort;
    }




    FT_Error      error;
    error = FT_Load_Char ( face , unicode , FT_LOAD_RENDER   | 
        FT_LOAD_TARGET_MONO);//|FT_LOAD_NO_SCALE   );          
    
    /** =======================================================================
    Why?
    PSP待机(黑屏待机)之后, 会出现乱字/画不出字, 原因是因为 FT_Load_Char ()错误.
    而它的错误的原因是待机之后字库文件的文件信息丢失, 必须要重新打开文件设定字体大小.
    ==========================================================================*/
    if ( error )  
    {
       //PSP_Window::showMessage( "FT_LOAD_Char() error " );
       bool r;
       pspTrueTypeFont_cgp ->isInited = false;
       r  = pspTrueTypeFont_cgp ->loadFontFile( ttfFilePath );
       if( !r  )
       {
          PSP_Window::showMessage( "redo r = loadFontFile( ) but still failed!!" );
          return false;
       }
       pspTrueTypeFont_cgp  ->setTTFFontSize ( pspTrueTypeFont_cgp->TTFFontSize  );
       error = FT_Load_Char ( face , unicode , FT_LOAD_RENDER   | 
                              FT_LOAD_TARGET_MONO);//|FT_LOAD_NO_SCALE   );          
       if ( error )
       PSP_Window::showMessage( "redo r = loadFontFile( ) but still failed!! 2 " );
       return false;
    }   

    FT_Bitmap  *testFtBitmap;

    testFtBitmap = & face->glyph->bitmap;

    //===========================================================
    //There is big chance that the result of font bitmap is empty. so just quit.
    //I use or||  here, because any of x or y is zero , the bitmap is empty.
    if ( testFtBitmap->width == 0 || testFtBitmap->rows == 0 )
    {
        return false;
    }

    TTFBitMapDrawData drawData;
    FT_GlyphSlot *gSlot;
    gSlot = ( FT_GlyphSlot *)&( face->glyph ) ;
    
    //must NOT use unicode here because, !!!!!! some hanzi 's unicode might < 128. so the bitmap size is set wrong
    getDrawData( unicode , gSlot  , &drawData );
  
    unsigned  newBufferSize , newHeigh, newWidth   ;
    newWidth = drawData.bitMapWidth;
    newHeigh = drawData.bitMapHeigh;
   

    newBufferSize = newWidth    * newHeigh ;
    hanzibmp->width  = newWidth;
    hanzibmp->heigh  = newHeigh;    
    if ( hanzibmp->data_p )
    {
        delete 	[]hanzibmp->data_p;
        hanzibmp->data_p = 0;            
    }

    if ( debug_ttf_cg ) printf ("allocating  %u memory \n", newBufferSize   );
    //hanzibmp->data_p = new unsigned char [ newBufferSize  ];
    hanzibmp->data_p = new unsigned char [ newBufferSize ];

   if ( debug_ttf_cg )  printf ("allocating  memory done \n");
    // allocate memory for new font failed
    //if ( hanzibmp->data_p  == 0 )
    if ( hanzibmp->data_p   == 0 )
    {
        
        if ( debug_ttf_cg )
        {
            printf(  " not enough memory for new font\n" );
        }		  
        return false;
    }	

    memset ( hanzibmp->data_p  , 0, newBufferSize );
    
  
    int yi, xi;
    unsigned char mask = 0 , data;
    unsigned long i = 0 , startFrom = 0;
    int //antiCropXStart = 0,
         antiCropYStart = 0;
//    antiCropXStart = drawData.antiCropX ; 
    antiCropYStart = drawData.antiCropY ; 
     
    
    for ( yi=0;yi < testFtBitmap->rows;yi++)
    {  
        unsigned drawIndex;
        for ( xi =0;xi< testFtBitmap->width; xi ++)
        {
            
            if ( mask == 0)
            {
                mask = 0x80;                  
                data = testFtBitmap->buffer[ i + startFrom ];
                //printf("handling %u ", data );
                i++;	
            }
            drawIndex = yi *newWidth + xi + antiCropYStart* newWidth;// + antiCropXStart;
            if ( drawIndex >= newBufferSize     )
            { 
                printf("Wrong ! %u ", drawIndex );
                //char t;
                scanf( " ");
                
            }
            if( xi< drawData.bitMapWidth &&  yi < drawData.bitMapHeigh )
            {  hanzibmp->data_p[ drawIndex ] =  data & mask;
            }
            mask = mask>>1;

             
        }
        //printf("\n");
        startFrom += testFtBitmap->pitch;
        mask = 0;
        i = 0;
    }

    //repair 


    /*   PSP_ChineseUtil::printBitMapInTextMode ( TTFFontSize,face->glyph->bitmap_left , face->glyph->bitmap_top , *hanzibmp ) ;
    */
   // if ( debug_ttf_cg )
     //   PSP_ChineseUtil::printBitMapInTextMode (  *hanzibmp ) ;


    return true;   
}

