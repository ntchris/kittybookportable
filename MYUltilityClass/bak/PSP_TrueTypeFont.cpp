
//#include "stdafx.h"


#include <ft2build.h>
#include FT_FREETYPE_H

#include <PSP_TrueTypeFont.h>
//#include <unicodeGBKTable.h>
#include <PSP_GLOBAL.h> 
#include <PSP_ChineseUtil.h>

const bool debugCache_cg = true;



using namespace PSP_Constants;

//===================================================================
//   Class PSP_TrueTypeFont 
//
//===================================================================


static PSP_TrueTypeFont * pspTrueTypeFont_cgp = new  PSP_TrueTypeFont ();
static HanZiTTFCache * hanZiTTFCache_cgp = new HanZiTTFCache();// ::getInstance();;

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
    for ( int i = 0;i<maxFontSizeCount;i++)
    {
        fontSizeList_am [i] = 0;
    }   

    isFontFileLoadOK = false;
    // clear the path string
    ttfFilePath[0] = 0;
     
    isFontFileLoadOK = false;
    goodSizeCount = 0;
     
     
    TTFFontSize =  0;   
    
    isFontFileLoadOK = false;
    //fontSize = 0;
    goodSizeCount = 0;
    
    
} 

// this method will set all available font size to the fontSizeList_am[]
void PSP_TrueTypeFont :: getAllAvailableFontSize( void )
{
    FT_Error    error;
    unsigned testingSize = 10;
   
    unsigned testUnicode = 29003 ;
    //FT_GlyphSlot slot ;
    FT_Bitmap  *testFtBitmap;
    int maxTry = 30;
    int tryCount = 0;
    clear();
        
    do
    {   
       testingSize ++;      
       tryCount ++;
       if ( tryCount > maxTry ) break;
       if ( goodSizeCount >=  ( maxFontSizeCount  ) ) break;  

       error = FT_Set_Pixel_Sizes ( face, 0, testingSize  );           
       if ( error ) continue;

       error = FT_Load_Char ( face , testUnicode , FT_LOAD_RENDER  );          
       if ( error ) continue;

       testFtBitmap = & face->glyph->bitmap;

       if ( testFtBitmap->pitch == testFtBitmap->width )
       {   
           fontSizeList_am[ goodSizeCount ]  = testingSize;     
           goodSizeCount ++;
          
       }

    }while( true );
    
    setDefaultFontSize();


}

void PSP_TrueTypeFont ::setDefaultFontSize( void )
{ 

     unsigned short testingSize = 0 ;
     for( int i=0; i< goodSizeCount ; i++)
     {
        testingSize =  fontSizeList_am[ i ]; 
        if (  testingSize >16 && testingSize < 24 )
        {
             
            break;
        }   
     }


    if ( testingSize == 0 )
    {
        testingSize = fontSizeList_am [ 0 ];
    }
    setTTFFontSize ( testingSize );

}



bool PSP_TrueTypeFont ::isTheFontSizeValid ( unsigned short size )
{
    for ( int i =0;i<maxFontSizeCount ;i++)
    {
        if ( size == fontSizeList_am [i] ) 
            return true;

    }
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
    {       
        return false;
    }
    else
    {
        strcpy ( ttfFilePath , ttfpath );
        isFontFileLoadOK = true;
    }
    TTFFontSize = 0;
    getAllAvailableFontSize();
    return true;
}


//also allocate memory for hanzibmp->data_p[];
bool PSP_TrueTypeFont :: drawHanZiTTFToBMP( unsigned short hanzi,  
                                           ucharBitMap * hanzibmp  )
{
    // 1 use free type to get the glyph        
    // 2 new space for hanzibmp
    // 3 copy the glyph bmp to hanzibmp
    
    // the font is not loaded and the font size is not set !!!   
    if ( this->TTFFontSize == 0 ) return false;
    
    
    unsigned short unicode ; 
    unicode =  PSP_ChineseUtil::fullGBKToUnicode (  hanzi  );


    FT_Error      error;
    error = FT_Load_Char ( face , unicode , FT_LOAD_RENDER  );          
    if ( error )  return false;
    
    FT_Bitmap  *testFtBitmap;
    
    testFtBitmap = & face->glyph->bitmap;
    unsigned bufferSize;
    bufferSize = testFtBitmap ->width * testFtBitmap ->rows;
    if ( hanzibmp->data_p )
    {
    	if ( debugCache_cg )
    	printf(  "delete old cache space %d\n", (int) hanzibmp->data_p );
    	
        delete 	hanzibmp->data_p;
        hanzibmp->data_p = 0;
        
        if ( debugCache_cg )
        printf(  "After delete old cache space \n");
        
    }
    hanzibmp->data_p = new unsigned char [ bufferSize ];
    // allocate memory for new font failed
    if ( hanzibmp->data_p  == 0 )
    {
    	  if ( debugCache_cg )
       	{
       		  printf(  " not enough memory for new font\n" );
        }		  
    	
    	  return false;
    }	
    hanzibmp->width  = testFtBitmap ->width;
    hanzibmp->heigh  = testFtBitmap ->rows;
    memcpy ( hanzibmp->data_p, testFtBitmap->buffer, bufferSize );
    // no need to set size again because set fontsize ( ) did it.
    // error = FT_Set_Pixel_Sizes ( face, 0, TTFFontSize  );   
    return true;   
}


void  PSP_TrueTypeFont :: setTTFFontSize ( unsigned short size )
{
    const int mustBeGoodSize = 30;
    const int insaneSize  = 200;
    
    if ( size == TTFFontSize )
    {
    	if ( debugCache_cg )
        printf(  "new font size == old font size \n");
        return;
    }

    if ( size >= insaneSize ) 
    {
    	  if ( debugCache_cg )
        printf(  "new font size is too large , insane \n");
        return ;
    } 	
    	
    FT_Error error ;
    if (  isTheFontSizeValid( size ) || size > mustBeGoodSize )
    {   
        error = FT_Set_Pixel_Sizes ( face, 0, size  ); 
        if( !error )
        {   
             if ( debugCache_cg )
           {  
               printf(  "set new font size:  %u done\n" , size );
           } 
           TTFFontSize = size ;
           hanZiTTFCache_cgp ->clearAllCachedItem ();
               
        }
        
    }
    else
    {
    	if ( debugCache_cg )
        {
           printf(  "new font size is not valid so still use old size \n");
        } 	
        return;
    }   

}


//=================================================================
//class HanZiTTFCache
//
//===================================================================
//static member for class HanZiCache
HanZiTTFCachedItem  HanZiTTFCache::bufferedHanZi_am [ maxHanZiStorageSize ];
 

 HanZiTTFCache ::HanZiTTFCache()
 {
      init();
 }
 
 HanZiTTFCache:: ~HanZiTTFCache()
{
    clearAllCachedItem();
    if (  debugCache_cg )
    {   printf( "HanZiTTFCache destructor \n" );    
    }
}
    
    
/*HanZiTTFCache * HanZiTTFCache :: getInstance ( void )
{
    static HanZiTTFCache ttfCache;
    if ( hanZiTTFCache_cgp  == 0 )
    {
        hanZiTTFCache_cgp  = &ttfCache;
    }
    return hanZiTTFCache_cgp ;
	
	
}

*/




void HanZiTTFCache :: init ( void )
{

    if (  debugCache_cg )
    {   printf( "initing \n" );    
    }
    
    
    if( HanZiTTFCache::isInited ) return ;
    
    isInited = true;
    unsigned short i ;
    
    HanZiTTFCachedItem *p;
    for ( i=0 ; i< maxHanZiStorageSize ;i++)
    {
       p = & bufferedHanZi_am[i];
       p->frequency = 0;
       p->hanZi = 0;    
       p->hanZiTTFBitMap.data_p = 0;
       p->hanZiTTFBitMap.width  = 0;
       p->hanZiTTFBitMap.heigh  = 0;
       
    }
    
    currentSize = 0;

}  





unsigned short HanZiTTFCache :: getRoomForNewHanZi( void  )
{
    unsigned short roomIndex;     
    
     if ( this->currentSize < maxHanZiStorageSize )
     {  
        //There is enough room, let's add one        
        currentSize ++;
        roomIndex = currentSize -1;
     }   
     else
     {   // There is NOT enough ROOM, let's find a least freq item to replace
         roomIndex = this-> getLeastFreqIndex(  );
         
     } 
    
     
     return  roomIndex;   

}





unsigned short HanZiTTFCache :: getLeastFreqIndex ( void )
{
    unsigned short leastFreq = 0xffff;
    unsigned short index = 0;
    unsigned short i;
    HanZiTTFCachedItem *tempCachedItem_P = NULL;
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
 
 
 
void HanZiTTFCache ::clearAllCachedItem ( void )
{
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
 
 
void HanZiTTFCache :: clearCachedItem ( unsigned short index  )
{
     
     if ( index >= maxHanZiStorageSize ) 
     {
     	return;
     }
     
     HanZiTTFCachedItem *p ;	
     p = & bufferedHanZi_am[ index ];
     p->frequency = 0;
     p->hanZi = 0;    
    
     p->hanZiTTFBitMap.width  = 0;
     p->hanZiTTFBitMap.heigh  = 0;
     
     if( p->hanZiTTFBitMap.data_p  )
     {
         delete p->hanZiTTFBitMap.data_p ;
       	 //after delete must reset data_p to 0
       	 p->hanZiTTFBitMap.data_p = 0;
       	   
         if( debugCache_cg )
         {
             printf("delete old cache space index : %u \n", index );	
         }
     }	   
     
     
}
 
 
 void HanZiTTFCache :: increaseFrequency( HanZiTTFCachedItem * cacheItem_p )
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



ucharBitMap * HanZiTTFCache::getHanZiTTFBitMap ( unsigned short hanZi )
{
   
   /* if ( ! this->getIsInited()  )
   {
        this->init();
   
   }*/
    ucharBitMap *temp_hanZiBMP_p = NULL;
    HanZiTTFCachedItem *tempCachedItem_P = NULL;
    bool found = false;
    unsigned int i = 0;
    for(i=0;i<currentSize;i++)
    {    
        tempCachedItem_P = getCachedItem( i );
        
        if( tempCachedItem_P == NULL )
        {   //impossible!!!!
            return NULL;
        }
        
        if ( tempCachedItem_P->hanZi == hanZi )
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
    }
     
    ucharBitMap *AddhanZiBMP  ; 
    //not found in the cache ???
    // let's add it to the cache !!!!!
    if( !found  )
    {
        if (  debugCache_cg )
        {
            printf( "not found in cache hanzi: %u \n", hanZi  );
        } 

       
        bool r;       
        unsigned short roomIndex;
        roomIndex = this->getRoomForNewHanZi( ); 
        tempCachedItem_P = this->getCachedItem ( roomIndex );        
        this->clearCachedItem ( roomIndex ) ;
        	
        tempCachedItem_P ->frequency = 0;
        tempCachedItem_P ->hanZi = hanZi;
        AddhanZiBMP = & tempCachedItem_P->hanZiTTFBitMap;
        r = pspTrueTypeFont_cgp->drawHanZiTTFToBMP( hanZi, AddhanZiBMP  );
     
    }
    
       
    if (  debugCache_cg )
    {
        if( found )printf( "found in cache index %u \n", i );
    } 

    return AddhanZiBMP;
      
}

 


// the instance is in the cache.
// get the bitmap from TTF bitmap cache.
ucharBitMap * TestPSP_ChineseUtil::getHanZiTTFBitMap ( unsigned short hanzi   )
{
    ucharBitMap * bitmap;
    //hanZiTTFCache_cgp 
    bitmap = hanZiTTFCache_cgp-> getHanZiTTFBitMap (   hanzi );

    return bitmap;                 
}



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



void  TestPSP_ChineseUtil:: selfTestTextMode( void )
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



  
