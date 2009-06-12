
//#define __VC__
#ifdef __VC__

#include <conio.h>
#endif

 

#include <PSP_ChineseUtil.h>
#include <CommonString.h>
#include <PSP_Global.h>
#include <GBKToUnicodeTable.h>

#include <unicodeJISTable.h>
#include <unicodeToGBKTable.h>
#include <PSP_TrueTypeFont.h>
#include <PSP_Window.h>



#ifdef __PSPSDK__

#include <PSP_GRAPHICS.h>


//#define printf pspDebugScreenPrintf
#else
//#include <stdio.h>
#endif


#include <stdio.h>
#include <PSP_AscCnFont.h>

const bool debugCache_cg = true;

//static member for class HanZiCache
HanZi16CachedItem  HanZiCache::bufferedHanZi_am [ maxHanZiStorageSize ];
unsigned short HanZiCache::currentSize = 0;
bool HanZiCache::isInited = false;
 
  
  
  
  


PSP_TrueTypeFont * pspTrueTypeFont_cgp = new  PSP_TrueTypeFont ();
TTF_HanZiCache * TTF_HanZiCache_cgp  = new TTF_HanZiCache();// ::getInstance();;
  
  
  
  
  
  
  
  

void HanZiCache ::selfTest ( void )
{
    unsigned short i,c = HanZiCache::currentSize;
    HanZi16CachedItem *p;
    for ( i=0;i<c;i++)
    {  p = HanZiCache ::getCachedItem ( i );
       printf ( " hanzi: %u  freq: %u \n", p->hanZi, p->frequency );
    }   


}
 
 /*
//move the newly increased item forward sort by freq
unsigned short  HanZiCache ::sortCachedItem ( unsigned short index )
{
   if (  debugCache_cg )
   {
        printf( "sortCachedItem %u \n", index );
   }
   
   if ( index <= 0 )
   {   // this is the first item 
       // no need to sort 
       return index;
   }   
      
   HanZiCachedItem *incItem_p, *testItem_p;
   incItem_p = getCachedItem( index );
   testItem_p = getCachedItem( index - 1 );   
   
   
   // eg 1 1 1  3   make sure find the first index of 1.
   unsigned testItemIndex = index -1 , foundIndex = 0;
   while( true )
   {
       
       
       testItem_p = getCachedItem( testItemIndex );
       if ( testItem_p->frequency >= incItem_p->frequency )
       {
          // = should not happen, but just in case.
          foundIndex = testItemIndex + 1;
          break;
       }
      
       if ( testItemIndex == 0 )
       {
           foundIndex = testItemIndex ;
           break;
       }  
       testItemIndex --;  
   }
   //switch tempItem_p and incItem_p;
   HanZiCachedItem tempSwitch;
   testItem_p = getCachedItem( foundIndex  );
   
   tempSwitch.frequency = testItem_p->frequency;
   tempSwitch.hanZi = testItem_p->hanZi;
   memcpy( &tempSwitch.hanZiBitMap, 
           &testItem_p->hanZiBitMap , oneHanZiUseByte_BMP );
   
   testItem_p->frequency = incItem_p->frequency;
   testItem_p->hanZi = incItem_p->hanZi;
   memcpy( &testItem_p->hanZiBitMap, &incItem_p->hanZiBitMap, oneHanZiUseByte_BMP );
   
   incItem_p->frequency = tempSwitch.frequency;
   incItem_p->hanZi = tempSwitch.hanZi; 
   memcpy( &incItem_p->hanZiBitMap, &tempSwitch.hanZiBitMap, oneHanZiUseByte_BMP );
   return foundIndex;
}
*/


  


 
void HanZiCache :: increaseFrequency( HanZi16CachedItem * cacheItem_p )
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


// before 1st time use, must use HanZiCache ::init() !!!!!!!
HanZi16FontBitMap * HanZiCache :: getHanZiBMP ( const HanZi & hanzi )
{
    if ( ! HanZiCache::getIsInited()  )
   {
        HanZiCache::init();
   
   }
   
   
    HanZi16FontBitMap *temp_hanZiBMP_p = NULL;
    HanZi16CachedItem *tempCachedItem_P = NULL;
    bool found = false;
    unsigned int i = 0;
    for(i=0;i<currentSize;i++)
    {    
        tempCachedItem_P = getCachedItem( i );
        
        if( tempCachedItem_P == NULL )
        {   //impossible!!!!
            return NULL;
        }
        
        if ( tempCachedItem_P->hanZi == hanzi.ushort )
        {   //increase and resort
            increaseFrequency( tempCachedItem_P );
            //newIndex = HanZiCache ::sortCachedItem ( i );
            //tempCachedItem_P = getCachedItem( newIndex );
            temp_hanZiBMP_p = &(tempCachedItem_P->hanZi16BitMap);            
            found = true;
            break;
        }   
    }    
        
    if ( found )
    {
       return temp_hanZiBMP_p;
    }
     
    HanZi16FontBitMap *AddhanZiBMP  ; 
    //not found in the cache ???
    // let's add it to the cache !!!!!
    if( !found  )
    {
        if (  debugCache_cg )
        {
            printf( "not found in cache hanzi: %u \n", hanzi.ushort  );
        } 

       
        bool r;       
        unsigned short roomIndex;
        roomIndex = HanZiCache::getRoomForNewHanZi( ); 
        tempCachedItem_P = HanZiCache::getCachedItem ( roomIndex );
        tempCachedItem_P ->frequency = 0;
        tempCachedItem_P ->hanZi = hanzi.ushort;
        AddhanZiBMP = & tempCachedItem_P->hanZi16BitMap;
        r = PSP_CNFONTS16_16::drawHanZi16ToBMP( hanzi.ushort , *AddhanZiBMP  );
        if( !r )        
        {
            printf(" HanZiFontBitMap tempAddhanZiBMP size not enough for width*heigh!!\n");
        }
    }
    
       
    if (  debugCache_cg )
    {
        if( found )printf( "found in cache index %u \n", i );
    } 

    return AddhanZiBMP;
    
}

unsigned short HanZiCache :: getRoomForNewHanZi( void  )
{
    unsigned short roomIndex;     
    
     if ( HanZiCache::currentSize < maxHanZiStorageSize )
     {  
        //There is enough room, let's add one        
        currentSize ++;
        roomIndex = currentSize -1;
     }   
     else
     {   // There is NOT enough ROOM, let's find a least freq item to replace
         roomIndex = HanZiCache::getLeastFreqIndex(  );
         
     } 
    
     
     return  roomIndex;   

}

  
unsigned short HanZiCache :: getLeastFreqIndex ( void )
{
    unsigned short leastFreq = 0xffff;
    unsigned short index = 0;
    unsigned short i;
    HanZi16CachedItem *tempCachedItem_P = NULL;
    for( i =0;i< HanZiCache::currentSize;i++ )
    {
        tempCachedItem_P = HanZiCache ::getCachedItem(i);
        
        if( tempCachedItem_P == NULL)
        {    
            printf( "impossible ! tempCachedItem_P = NULL !!! \n ");
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
 

void HanZiCache :: init ( void )
{

    if (  debugCache_cg )
    {   printf( "initing \n" );    
    }
    
    
    if( HanZiCache::isInited ) return ;
    
    isInited = true;
    unsigned short i ;
    
    for ( i=0 ; i< maxHanZiStorageSize ;i++)
    {
       bufferedHanZi_am[i].frequency = 0;
       bufferedHanZi_am[i].hanZi = 0;    
    }
    memset ( bufferedHanZi_am , 0, sizeof( HanZi16CachedItem) * maxHanZiStorageSize ) ;
    currentSize = 0;

}  
 
 
 /*

  //remove the freq == 0  hanzi bmp if full
 unsigned short  HanZiCache::addHanZiBMP ( unsigned short hanzi,  const HanZiFontBitMap &hanzibmp )
{

    unsigned short replaceIndex; 
    if (  debugCache_cg )
    {   printf( "adding %u ", hanzi );    
    }
    
     unsigned short index ;
     if ( HanZiCache::currentSize < maxHanZiStorageSize )
     {  //let's add one        
        currentSize ++;
        replaceIndex = currentSize -1;
     }   
     else
     {
         replaceIndex = HanZiCache::getLeastFreqIndex( void );
         
     }         
     
     HanZiFontBitMap *temp_hanZiBMP_p = NULL;
     HanZiCachedItem *tempCachedItem_P = NULL;
     
     tempCachedItem_P =  HanZiCache::getCachedItem( replaceIndex );
     tempCachedItem_P->frequency = 0;
     tempCachedItem_P->hanZi = hanzi;
     memcpy( tempCachedItem_P->hanZiBitMap.data, hanzibmp.data, oneHanZiUseByte_BMP );
     
     return replaceIndex  ;
}
*/

/*
// draw a HanZi to a Window.
static bool PSP_ChineseUtil:: drawHanZi ( ushortBITMAP &windowBitMap,
                             unsigned short x, unsigned y,
                             unsigned short hanZi          )
{



}                             
                             
                             
  */                           

// use cache  !
 HanZi16FontBitMap * PSP_ChineseUtil::getHanZiBitMap ( const HanZi & hanzi )
{
  
   HanZi16FontBitMap *bmp ; 
   HanZi hanziNotUnicode;;
   
   if ( hanzi.isUnicode )
   {
       hanziNotUnicode.ushort = UnicodeToGBK ( hanzi.ushort  );
   }
   else
   {
       hanziNotUnicode.ushort =  hanzi.ushort  ;
   }	
       
   //    printf("unicode is %u,  ASC code is %u\n" , hanzi.ushort ,   hanziNotUnicode.ushort);
   bmp = HanZiCache :: getHanZiBMP ( hanziNotUnicode );
   
   return bmp ;
   
}



unsigned short PSP_ChineseUtil:: getTTFSize( void )
{
      return pspTrueTypeFont_cgp->getTTFFontSize();
}


void PSP_ChineseUtil:: printBitMapInTextMode (  const HanZi16FontBitMap &hanzibitmap ) 
{
    unsigned xi, yi, xc, yc;
    xc =  dotFontHanZiFontWidth;
    yc =  dotFontHanZiFontHeigh;
    unsigned index =0;
    
    printf( "\n");
    printf( "\n");
    for( yi=0; yi< yc ; yi++)
    {
         for( xi=0; xi< xc ; xi++)
         {
              if ( hanzibitmap.data[index] )
              {
                 printf("O");
              }else   
              {
                 printf("_");
              }
              index ++;
              
         }
         printf("\n");
    }    


}





void PSP_ChineseUtil:: printBitMapInTextMode (  const ucharBitMap &hanzibitmap ) 
{
    unsigned xi, yi, xc, yc;
    xc =  hanzibitmap.width ;
    yc =  hanzibitmap.heigh ;
    unsigned lineNo = 0 ;
    unsigned index =0;
    
    printf( "\n");
    printf( "\n");
    for( yi=0; yi< yc ; yi++)
    {    printf("%2u ", lineNo++ );
         for( xi=0; xi< xc ; xi++)
         {
              if ( hanzibitmap.data_p[index] )
              {
                 printf("W");
              }else   
              {
                 printf("_");
              }
              index ++;
              
         }
         printf("\n");
    }    


}


void PSP_ChineseUtil:: printBitMapInTextMode ( unsigned int fontSize, int bearX, int bearY,  const ucharBitMap &hanzibitmap ) 
{
    
    unsigned xi, yi, xc, yc;
    xc =  hanzibitmap.width ;
    yc =  hanzibitmap.heigh ;
    int baseline=0;
    unsigned index =0;
    int indent=0;
    unsigned lineNo=0;
    printf( "\n");
    printf( "\n");
    
    printf("fontSize %u, bearX %d , bearY %d \n", fontSize, bearX , bearY );
    
    unsigned emptyLine = 0;
    baseline = fontSize*9/10 - 1 ;
    printf("baseline: %d \n",  baseline  );
       emptyLine = baseline   - bearY   ;
       
        for ( yi=0;yi< emptyLine  ;yi++)
       {
             printf("%2u ", lineNo++);
             for ( xi=0;xi< xc + bearX ; xi ++)
             printf ("_");
             
             printf("\n");
             

        }
    
    
    
    for( yi=0; yi< yc ; yi++)
    {
         printf("%2u ", lineNo++);
         for  ( indent =0; indent< bearX;  indent++)
         printf("_");
         
         for( xi=0; xi< xc ; xi++)
         {    
               
              	 if ( hanzibitmap.data_p[index] )
                 {
                    printf("W");
                 }else   
                 {
                    printf("_");
                 }
              
                 index ++;
              
         }
         printf("\n");
    } 
     
    if ( yc + emptyLine < fontSize -1 )
   {    unsigned int count;
   	count = fontSize - yc - emptyLine;
         for ( yi=0;yi< count  ;yi++)
       {
             printf("%2u ", lineNo++);
             for ( xi=0;xi< xc + bearX ; xi ++)
             printf ("_");
             
             printf("\n");
             

        }
   }   


}









void PSP_ChineseUtil:: printBitMapInTextMode ( const ASCFontBitMap &ascbitmap )
{
    unsigned xi, yi, xc, yc;
    xc =  DotFont16ASCfont_width ;
    yc =  DotFont16ASCfont_heigh ;
    unsigned index =0;
    
    printf( "\n");
    printf( "\n");
    for( yi=0; yi< yc ; yi++)
    {
         for( xi=0; xi< xc ; xi++)
         {
              if ( ascbitmap.data[index] )
              {
                 printf("Z");
              }else   
              {
                 printf("0 ");
              }
              index ++;
              
         }
         printf("\n");
    }    


}



#ifdef __PSPSDK__
void PSP_ChineseUtil::printASC_dotMatrix(  unsigned short x, unsigned short y, 
                                 unsigned short color, char _char)
{
    //unsigned short bgcolor = 0;
    //PSP_GRAPHICS::graphicPrintChar8_16( x, y, color, bgcolor, _char, true, false, 1 );

}
#endif 

bool PSP_ChineseUtil::isThisAHanZi(  const unsigned short HZbyte1 )
{
    //GB2312的两个字节的最高位都是1。但符合这个条件的码位只有128*128=16384个。
    //所以GBK和GB18030的低字节最高位都可能不是1。不过这不影响DBCS字符流的解析：
    //在读取DBCS字符流时，只要遇到高位为1的字节，
    //就可以将下两个字节作为一个双字节编码，而不用管低字节的高位是什么。

    if ( HZbyte1 > 128 )
    {
       return true;
       
    }else
    {
       return false;   
    }   


}

#ifdef __PSPSDK__




// return the bitmap size because caller should know the width and heigh
void PSP_ChineseUtil::drawTTFHanzi ( DrawAttribute &drawAtt, unsigned short color, 
                                    HanZi hanzi , BitMapSize *  bmpsize_p ) 
{
    ucharBitMap *bitmap ;
    bitmap = PSP_ChineseUtil:: getHanZiTTFBitMap(  hanzi );

    PSP_GRAPHICS::drawBitMap ( drawAtt, color , *bitmap );

    if( bmpsize_p )
    {
        bmpsize_p->width = bitmap->width;
        bmpsize_p->heigh = bitmap->heigh;
    }

}                               


   
   
void PSP_ChineseUtil::drawHanZi16 ( DrawAttribute &drawAtt, unsigned short color, 
                              char * hanZi2Byte   ) 
  
 
{
    
 
    if( hanZi2Byte == NULL )
    return;    
     
    
    HanZi hanzi;
    hanzi.ch[0] = hanZi2Byte[0];
    hanzi.ch[1] = hanZi2Byte[1];
    PSP_ChineseUtil:: drawHanZi16 ( drawAtt, color,  hanzi );
    
  


}  


     
                     
void   PSP_ChineseUtil::drawHanZi16Turn ( DrawAttribute &drawAtt, unsigned short color, 
                                            const HanZi & hanzi       ) 
{
    
         
    HanZi16FontBitMap *tempHZbitmap;
    tempHZbitmap = PSP_ChineseUtil::getHanZiBitMap ( hanzi  );
    ucharBitMap ucharbmp;
    ucharbmp.data_p = tempHZbitmap->data;
    ucharbmp.width = dotFontHanZiFontWidth ;
    ucharbmp.heigh = dotFontHanZiFontHeigh ;
     
    PSP_GRAPHICS ::drawBitMapTurn ( drawAtt,  color , ucharbmp  );
     
    return;

}        
        
#endif  //__PSPSDK__





 void PSP_ChineseUtil::  drawHanZi16 ( DrawAttribute &drawAtt, unsigned short color, 
          const  HanZi &hanzi ) 
{
    HanZi16FontBitMap *tempHZbitmap;
    tempHZbitmap = PSP_ChineseUtil::getHanZiBitMap ( hanzi );
    ucharBitMap ucharbmp;
    ucharbmp.data_p = tempHZbitmap->data;
    ucharbmp.width = dotFontHanZiFontWidth ;
    ucharbmp.heigh = dotFontHanZiFontHeigh ;

#ifdef __PSPSDK__     
    PSP_GRAPHICS::drawBitMap ( drawAtt, color , ucharbmp );
#else
   //printBitMapInTextMode ( ucharbmp );
   printf( "%c", hanzi.ushort);
#endif
     
    return;
	
	
}
     
     
     
     
//#include <unicodeJIS.h>
//#include <unicodeCJK.h>

//#include <unicodeGBKTable.h>

void PSP_ChineseUtil ::jis2cjk( const unsigned char *jis,unsigned char *cjk)
{
	
	//int iunic;	
	//unsigned char tmp[2];
	int i = 0,len;
	HanZi hanzi, jisHanzi;
	len= strlen( (const char *)jis );
	do
	{
	
	    if(jis[i]<0x81)
		{
			cjk[i]=jis[i];
			i++;
		}		
		else 
		{
			unsigned short unicodeOfJIS;			
			jisHanzi.ch[0] = jis[i];
			jisHanzi.ch[1] = jis[i+1];
			// jis to unicode
			unicodeOfJIS = fullJISToUnicode( jisHanzi.ushort);			
			/*char str[100];
			str [0]=0;
			sprintf ( str, "jis  %u ", jisHanzi.ushort ); 
			PSP_GRAPHICS::graphicPrintAscString ( 0, 0, 0xffff, str );
			str [0]=0;
			sprintf ( str, "jis unicode %u ", unicodeOfJIS ); 
			PSP_GRAPHICS::graphicPrintAscString ( 0, 20, 0xffff, str );
			//
			*/
            hanzi.ushort = PSP_ChineseUtil ::UnicodeToGBK( unicodeOfJIS );
            /*  str [0]=0;
            sprintf ( str, "hanzi gbk %u ",  hanzi.ushort );             
            PSP_GRAPHICS::graphicPrintAscString ( 0, 40, 0xffff, str );
            PSP_GRAPHICS::flipScreenBuffer();
            getch();
            */
            cjk[i] = hanzi.ch[0];
            cjk[i+1] = hanzi.ch[1];
            i += 2;
            
			/*if(cjk[i-1]==0x3f && cjk[i]==0)
			{
				cjk[i-1]=0xa1;
				cjk[i]=0xf6;
			}*/
		
		}
		if ( i >= len ) break;
	}while (true);
}

  
 
 







 
#ifdef __PSPSDK__

void PSP_ChineseUtil::selfTestPSPGraphicMode( void )
{

     
     //printAscHanZiString(0,0, 0xffff, "ABC卧槽!DEK");
    //printAscHanZiString(0,0, 0xffff, "ABC");
    // ABCDE 中文阅读! 测试1测试2测试3测试4编程楼梯走廊");

}
#endif //










/*

void PSP_ChineseUtil::printHanZiTextMode( unsigned short cn )
{    
    const unsigned char *cfont;		//pointer to font
    unsigned long cx,cy;
    unsigned long b;
    char mx,my;
    
    HanZi hz;
    hz.ushort = cn;
    unsigned long index = HanZiToHZKLibIndex ( hz );
    //if (printChar > 255) return;
    Dot_Font_Data cnDotFontData;    
    //cfont = font + printChar*8;
    PSP_CNFONTS16_16::getFontData( index , &cnDotFontData);
    //cfont = good_asc_8_16_font + printChar*16;
    
    cfont = cnDotFontData.fontData;  
     
    for (cy=0; cy<cnDotFontData.heigh; cy++) 
    {   const int mag = 1;
        for (my=0; my<mag; my++) 
        {             
            b=0x80;
            for (cx=0; cx<cnDotFontData.width; cx++) 
            {   
            	//this byte is used up,  drawed half a char, 
            	//now use next byte.
                if( cx == CnCharDotsPerByte )
                { 
                   b = 0x80;
                   cfont ++;
                }                
                for (mx=0; mx<mag; mx++) 
                {
                    if ((*cfont&b)!=0) 
                    {                        
                        printf("O");                        
                    } else 
                    {                        
                        printf(" ");                        
                    }                     
                }
                b=b>>1;                
            }            
            printf("\n");
        }
        cfont++;
    }
    printf("\n"); printf("\n");
}
 
 
*/


bool PSP_ChineseUtil:: isEmpty( HanZi16FontBitMap *bmpp)
{
     int i; 
     int size = bmpp->size;
     for ( i=0; i< size ; i++)
     {
        
         if (   bmpp->data[i]  !=0 )
         {
             return false;
         }
     }
     return true;
}

void PSP_ChineseUtil::selfTestTextModeDrawAllHanzi ( void )
{
    HanZi16FontBitMap hanzibitmap; 
    //unsigned short i;
    HanZi hanzi;
    hanzi.ch[0]= 0;
    hanzi.ch[1]= 128;
    unsigned int emptyCount = 0;
   
    
    for ( hanzi.ushort = 63000 ; hanzi.ushort<65535;hanzi.ushort++)
    {
        
        PSP_CNFONTS16_16::drawHanZi16ToBMP( hanzi.ushort, hanzibitmap ); 
        printf("hanzi is %u \n", hanzi.ushort);
        printBitMapInTextMode ( hanzibitmap );
      
        if (  isEmpty ( &hanzibitmap )  )
        {
           emptyCount  ++ ;
           printf ( "so far emptyCount  is %u \n", emptyCount  );
        }
    }
   
    

}

void PSP_ChineseUtil::selfTestTextModeDrawAllIndex ( void )
{
    HanZi16FontBitMap hanzibitmap; 
    unsigned int i = 6500;
    bool r;
    do
    { 
        printf ("index: %u", i);
        r = PSP_CNFONTS16_16::getBitMapHZKLibIndex ( i , hanzibitmap  );
        if( !r ) break;
        i++ ;
        printBitMapInTextMode ( hanzibitmap );
    }
    while( true );


}











unsigned short PSP_ChineseUtil::fullGBKToUnicode ( unsigned short gbc )

{

	unsigned short unicode  ;//, index ;
//	index = 0;//gbc - GBK_StartFrom;
	 
	//if ( index >= GBKunicodeTableSize  )
	//return 0;
	//unicode = GBKunicodeTable[ index ] ;
	unicode = GBKunicodeTable[ gbc ] ;
	return unicode;
}

    
/*
unsigned short PSP_ChineseUtil::UnicodeToGBK ( unsigned short unicode )

{
     
     unsigned short gbk , unicodeIndexInUnicodeTable;
     unsigned i;
     bool found =false;
     for ( i =0 ;i< GBKunicodeTableSize ; i++)
     {
         if ( GBKunicodeTable[i] == unicode )
         {
            unicodeIndexInUnicodeTable = i;
            found = true;
            break;
         }
        //binarySearch ( GBKunicodeTable, GBKunicodeTableSize , unicode  );
     }
     if ( found )
     {
        gbk = unicodeIndexInUnicodeTable + GBK_StartFrom;
     }
     else
     {
         gbk = GBK_StartFrom;
     }   
     return gbk;
}

*/


unsigned short PSP_ChineseUtil::UnicodeToGBK ( unsigned short unicode )
{
     return unicodeToGBKTable[ unicode ];
     
}







// the instance is in the cache.
// get the bitmap from TTF bitmap cache.
ucharBitMap * PSP_ChineseUtil::getHanZiTTFBitMap ( HanZi &hanzi   )
{

    ucharBitMap * bitmap;
    //hanZiTTFCache_cgp 
       
  /*  if( hanzi.ushort == 36974 )
    {  
       PSP_Window::showMessage(" PSP_ChineseUtil::getHanZiTTFBitMap () unicode:是 ", hanzi.ushort );     
       if ( hanzi.isUnicode )
       {
          PSP_Window::showMessage(" PSP_ChineseUtil::getHanZiTTFBitMap () isUnicode正确" );      
       }else
       PSP_Window::showMessage(" PSP_ChineseUtil::getHanZiTTFBitMap () isUnicode不正确 ");      
       
    }
   */
    bitmap = TTF_HanZiCache_cgp-> getHanZiTTFBitMap (   hanzi );
             
    return bitmap;                 
    
    
    //return TTF_Utility::getHanZiTTFBitMap( hanzi );
}

/*
void PSP_ChineseUtil:: setTTFSize ( unsigned short size )
{
	
     pspTrueTypeFont_cgp -> setTTFFontSize ( size );
    
     
}
*/


 
 


#ifdef __PSPSDK__

void  PSP_ChineseUtil:: selfTestTTFGraphicMode( void )
{
	
    //PSP_TrueTypeFont *pspTrueTypeFont_cgp  = &(TTF_Utility::ttfFont ) ;
    //pspTTF->init();
    bool r;
    r = pspTrueTypeFont_cgp ->loadFontFile("ms0:/simsun.ttc");
    pspTrueTypeFont_cgp  ->setTTFFontSize ( 50  );
    if( !r )
    {
         
         PSP_Window::showMessage(  "TrueTypeFont字库文件读取失败 请检查");
         PSP_GRAPHICS::flipScreenBuffer( );
         getch();
    }
    //pspTrueTypeFont_cgp  ->setTTFFontSize ( 32 );
    HanZi hanzi("煋") ,  hanzi2 ("事"), hanzi3("新"), hanzi4("替");
    
    	
    //pspTrueTypeFont_cgp  ->setTTFFontSize ( 2* (i%10) + 25 );
    
     //
    	
     DrawAttribute drawAtt;
     
     for ( int i=0;i < 9000 ;i++)
     {
     	
     PSP_GRAPHICS::fillVRam( i%100 *500 );   	
     drawAtt.x = 0;  drawAtt.y = i%5 * 40;    
     drawTTFHanzi ( drawAtt,  0xffff, hanzi ) ;
     
     drawAtt.x = 100; // drawAtt.y = 0;
     drawTTFHanzi ( drawAtt,  0xffff, hanzi2   ) ; 
     
     drawAtt.x = 200;  //drawAtt.y = 0;      
     drawTTFHanzi ( drawAtt,  0xffff, hanzi3  ) ; 
     
     drawAtt.x = 300;  //drawAtt.y = 0;      
     drawTTFHanzi ( drawAtt,  0xffff, hanzi4   ) ; 
     
     PSP_GRAPHICS::flipScreenBuffer( );
     wait( 3);
     }
     
          
     	
     getch();
      
 }
	
	
	
#endif
 

void PSP_ChineseUtil::selfTestTextMode( void )
{
     
    unsigned long hzkIndex  ;
              
    // printCN ( 0, 0, 0xffff, ch.i);
   
     hzkIndex = 320;
     
     
     
     printf("\n\n\n");
     /*for ( i =0;i<10000;i++)
     printCNTextMode(i);
     */
    
     HanZi hanzi1, hanzi2, hanzi3, hanzi4, hanzi5;
     char h1[]={"一"};
     hanzi1.ch[0]= h1[0];
     hanzi1.ch[1]= h1[1];
      
     char h2[]={"二"};
     hanzi2.ch[0]= h2[0];
     hanzi2.ch[1]= h2[1];
     
     char h3[]={"三"};
     hanzi3.ch[0]= h3[0];
     hanzi3.ch[1]= h3[1];
     
     char h4[]={"四"};
     hanzi4.ch[0]= h4[0];
     hanzi4.ch[1]= h4[1];
     
      char h5[]={"＠"};
     hanzi5.ch[0]= h5[0];
      hanzi5.ch[1]= h5[1];
     
        
 //    printBitMapInTextMode ( 
     
     HanZi16FontBitMap *hanzibitmap; 
     printf( "hanzi : %u \n", hanzi1.ushort );
     hanzibitmap = getHanZiBitMap ( hanzi1 );
     printBitMapInTextMode( *hanzibitmap );
     
     
     
     hanzibitmap = getHanZiBitMap ( hanzi2   );
     printBitMapInTextMode( *hanzibitmap );

     
     hanzibitmap = getHanZiBitMap ( hanzi3 );
     printBitMapInTextMode( *hanzibitmap );
     
     hanzibitmap = getHanZiBitMap ( hanzi4 );
     printBitMapInTextMode( *hanzibitmap );
     
   
     hanzibitmap = getHanZiBitMap ( hanzi1 );
     printBitMapInTextMode( *hanzibitmap );
       
     
     hanzibitmap = getHanZiBitMap ( hanzi2 );
     printBitMapInTextMode( *hanzibitmap );

     
     hanzibitmap = getHanZiBitMap ( hanzi3 );
     printBitMapInTextMode( *hanzibitmap );
     
     hanzibitmap = getHanZiBitMap ( hanzi4 );
     printBitMapInTextMode( *hanzibitmap );




     hanzibitmap = getHanZiBitMap ( hanzi5 );
     printBitMapInTextMode( *hanzibitmap );
     
     
   
      HanZiCache::selfTest( );

}




#ifdef __PSPSDK__
    


void PSP_ChineseUtil ::selfTestHanZi ( void )
{
    DrawAttribute drawAtt;
    drawAtt.x = 0;
    drawAtt.y = 0;
    drawAtt.enlarge = 1;
    PSP_GRAPHICS::setScreenFrameMode(  screenModeWriteAndShow_cg );
    PSP_GRAPHICS::fillVRam ( 0 );
    drawHanZi16 ( drawAtt ,0xff, "惑"  );
    
    drawAtt.x = 17;
    drawAtt.y = 0;
    
    drawHanZi16 ( drawAtt, 0xff, "煋"   );
    
    drawAtt.x=35;
    drawAtt.y=0;
    drawAtt.enlarge =2;
    
      drawHanZi16  ( drawAtt , 0xff , "湾"     );
    
    drawAtt.x =100;
    drawAtt.y = 0;
    drawAtt.enlarge =4;
      drawHanZi16 ( drawAtt , 0xff  , "蔼" );
    getch();
    drawAtt.x =470;
    
      drawHanZi16 ( drawAtt ,0xff , "惑"  );
    drawAtt.x=17;
    drawAtt.y=260;
      drawHanZi16 ( drawAtt , 0xff , "煋"  );
    getch();
    PSP_GRAPHICS::fillVRam ( 0 );
    
    
    drawAtt.x =460;
    drawAtt.y = 0;
    drawAtt.enlarge =2;
     drawHanZi16  ( drawAtt ,  0xff, "湾");
    
    drawAtt.x =0;
    drawAtt.y = 255;
    drawAtt.enlarge =4;
    
      drawHanZi16 ( drawAtt,   0xff  ,"蔼" );
    
    getch();
    drawAtt.enlarge =1;
    PSP_GRAPHICS::fillVRam ( 0 );
    HanZi hanzi;
    char *p= {"蔼"};
    hanzi.ch[0] = p[0]; 
    hanzi.ch[1] = p[1]; 
     drawHanZi16Turn (  drawAtt, 0xff, hanzi  );
    drawAtt.y = 20;
    drawAtt.enlarge =2;
      drawHanZi16Turn (  drawAtt, 0xff, hanzi );
    getch();
    
    PSP_GRAPHICS::fillVRam ( 0 );
    drawAtt.x =0;
    drawAtt.y = 10;
     drawHanZi16Turn (  drawAtt, 0xff,  hanzi);
    drawAtt.x =30;    
    drawAtt.y = 20;
    drawAtt.enlarge =4;
    drawHanZi16Turn (  drawAtt, 0xff,  hanzi );
    getch();
    
    



}





#endif  // __PSPSDK__






void  PSP_ChineseUtil:: selfTestTTFTextMode( void )
{

    
    //from 18  to ......
    pspTrueTypeFont_cgp ->loadFontFile("c:\\winnt\\fonts\\simsun.ttc");
    pspTrueTypeFont_cgp  ->setTTFFontSize ( 17 );
    
  //  FT_Set_Pixel_Sizes ( face, 0, 10  );           
    
    HanZi hanzi("煋") ,  hanzi2 (","), hanzi3("\""), hanzi4("替");
    HanZi  hanziYi("一") ,  hanziMu ("目") , hanziX("～"),
    hanziASC1("1"),    hanziASC2("_")  , hanziASC3(".") , hanziASC4("~") ;
    int i;
    unsigned char *p;
    unsigned size = 1024;
      
    HanZi hanziTest; 
    
    hanziTest.isUnicode = true;
     ucharBitMap *bitmap ;
     
//     pspTrueTypeFont_cgp  ->setTTFFontSize ( 23 );
     //hanziTest.ushort = 32871 ;
     //bitmap = getHanZiTTFBitMap( hanziTest.ushort );  
    //problem number 9494 =>  23355    

    int fontsize = 22;
  //  for (  fontsize= 22; fontsize< 50; fontsize ++)
    {
         pspTrueTypeFont_cgp  ->setTTFFontSize ( fontsize );
         //23355  这个字( 或者前面几个) 很有问题! 在GCC和VC都死机.
        for ( int i = 0  ;i< 200;i++)//65535;i++)
        {
            hanziTest.ushort = i;
            printf ("testing hanziTest.ushort = %u \n", hanziTest.ushort  );
            bitmap = getHanZiTTFBitMap( hanziTest  );  
            printBitMapInTextMode ( *bitmap );
        }   
        
        pspTrueTypeFont_cgp  ->setTTFFontSize ( 11 );
        
        for ( int i = 0  ;i< 200;i++)//65535;i++)
        {
            hanziTest.ushort = i;
            printf ("testing hanziTest.ushort = %u \n", hanziTest.ushort  );
            bitmap = getHanZiTTFBitMap( hanziTest  );  
            printBitMapInTextMode ( *bitmap );
        }   
        
        
    }  


}




void  PSP_ChineseUtil:: convertUnicodeBigEnd ( unsigned short * unicodeTextBuffer, unsigned long UshortSize  )
{
     unsigned long i ;
     unsigned char ch1,ch2;
     unsigned short tempUshort;
     //unicode has 0 , must not use 0 to end.
     //bufferSize = strlen ( unicodeTextBuffer );
     
     for( i = 0;i< UshortSize ;i++ )
     {
         
        tempUshort  = unicodeTextBuffer [ i ] ;
        ch1 = tempUshort / 0x100;
        ch2 = tempUshort % 0x100;
        unicodeTextBuffer [ i ]  = ch2 * 0x100 + ch1;
        
       // printf( "%x %x\n", ch1, ch2 );
        
        
       // printf( "%x %x\n", ch1, ch2 );
     }

}
 






void PSP_ChineseUtil::drawAscChar8_16( const DrawAttribute &drawAtt, unsigned short color,  char ascChar )
{
    ucharBitMap  bitmap ;
    ASCFontBitMap *ascbitmap_p;

    ascbitmap_p = PSP_ASCFONTS8_16::getASCBmp( ascChar );     


    bitmap.width = PSP_ASCFONTS8_16::width;
    bitmap.heigh = PSP_ASCFONTS8_16::heigh ;
    bitmap.data_p = ascbitmap_p->data;

#ifdef __PSPSDK__
    PSP_GRAPHICS::drawBitMap (  drawAtt,   color , bitmap );
#else
    //PSP_ChineseUtil:: printBitMapInTextMode ( bitmap );
    printf("%c",  ascChar);
#endif



}         












