
 

#include <stdio.h>
 

#include <PSP_AscCnFont.h>
#include <FontData_Asc8_16.h>
#include <FontData_HZK16.h>
//#define printf pspDebugScreenPrintf
 
#include <string.h> 


using namespace PSP_Constants;

//============================================================================
//   class PSP_ASCFONTS8_16
//
//============================================================================

 

//One ASC Char takes 16 bytes from the dot matrix

const unsigned int PSP_ASCFONTS8_16::maxIndex = sizeof(good_asc_8_16_font )/oneAscCharUseByte_LIB -1;
const unsigned char * const PSP_ASCFONTS8_16::font_dot_matrix_p  
                                           = good_asc_8_16_font;

ASCFontBitMap PSP_ASCFONTS8_16:: ascFontBitMap [ PSP_ASCFONTS8_16::maxIndex  ] ;
bool PSP_ASCFONTS8_16::ascBMPinited = false;

void PSP_ASCFONTS8_16::loadAllASCBitMap( void )
{
    // Do not init more than once !!!!!!!
    if ( ascBMPinited )
    {  return ;
    }
    ascBMPinited  = true;
    
    unsigned int i; 
    for ( i =0;i< maxIndex ; i++)
    {
         drawASC8_16ToBMP( i, ascFontBitMap[i] );    
    }
        
}

ASCFontBitMap * PSP_ASCFONTS8_16::getASCBmp(   char ascchar )
{
    int i;
    i = ascchar;
    if( !ascBMPinited ) 
    {
       loadAllASCBitMap ();
    }   
    //If the asc char is invalid, then return a [](empty) bitmap.
  
    if ( ascchar < 0 ) 
    {
       return ascFontBitMap;
    } 
    if ( (unsigned int )i >= maxIndex ) 
    {
       return ascFontBitMap;
    } 
      
    return  &(ascFontBitMap[ i ]);
    
    
    


}


bool PSP_ASCFONTS8_16::drawASC8_16ToBMP( unsigned char _char,  ASCFontBitMap &ascbitmap )
{

    //sanity checking
    if (  ascbitmap.data == NULL )return false;
    if (  ascbitmap.size < width * heigh )
    {
      return false;
    }


    unsigned char *vptr0,  *vptr;
    const unsigned char *cfont;	
    unsigned long xi, yi;
    unsigned char b;    

    //if (printChar > 255) return;
    Dot_Font_Data dotFontData;

    //cfont = font + printChar*8;
    PSP_ASCFONTS8_16::getFontData( _char , &dotFontData);
    //cfont = good_asc_8_16_font + printChar*16;
    cfont = dotFontData.fontData;
    

    vptr0 = ascbitmap.data;
    for (yi=0; yi<dotFontData.heigh; yi++) 
    {   
        vptr=vptr0;
        b=0x80;
        for ( xi=0; xi<dotFontData.width; xi++) 
        {
            if ((*cfont&b)!=0) 
            {
                *vptr= defaultColor;

            } else 
            {
                *vptr = 0;
            }
            vptr++;
            b=b>>1;
        }
        vptr0 += width ;
        cfont++;
    }
    
    
    return true;
}



void  PSP_ASCFONTS8_16::getFontData( unsigned index, Dot_Font_Data * font_data )
{
    
    if( font_data == 0 //NULL
       )
    {  
        return ;
    }
    
    if(  index >= maxIndex )
    {
        // the char is out of range so just return empty dot data.
        font_data -> fontData = font_dot_matrix_p ;
        font_data -> width = width;
        font_data -> heigh = heigh; 
        font_data -> sizeByte = oneAscCharUseByte_LIB;
        font_data -> dotsPerByte = dotsPerByte;
        return;
    }   
    
    font_data -> fontData = font_dot_matrix_p + index * oneAscCharUseByte_LIB ;
    font_data -> width = width;
    font_data -> heigh = heigh; 
    font_data -> sizeByte = oneAscCharUseByte_LIB;
    font_data -> dotsPerByte = dotsPerByte;
    return ;
}






//============================================================================
//  Class PSP_CNFONTS16_16;
//============================================================================



const unsigned int PSP_CNFONTS16_16::maxIndex = sizeof(hanZiFont16_cg) /oneHanZiUseByte -1;

const unsigned char * const PSP_CNFONTS16_16 ::font_dot_matrix_p = hanZiFont16_cg;

void  PSP_CNFONTS16_16::getFontData( unsigned index, Dot_Font_Data * font_data )
{
    if( font_data == 0 //NULL
       )
    {  
        return ;
    }
    
    if(  index >= maxIndex )
    {
        // the char is out of range so just return empty dot data.
        font_data -> fontData = font_dot_matrix_p ;
        font_data -> width = width;
        font_data -> heigh = heigh; 
        font_data -> sizeByte = oneHanZiUseByte;
        font_data -> dotsPerByte = dotsPerByte;
        return;
    }   
    
    font_data -> fontData = font_dot_matrix_p + index * oneHanZiUseByte;
    font_data -> width = width;
    font_data -> heigh = heigh; 
    font_data -> sizeByte = oneHanZiUseByte;
    font_data -> dotsPerByte = dotsPerByte;
    return ;
}


void PSP_CNFONTS16_16::selfTest( void )
{
   printf( "oneCharUseByte %u\n", oneHanZiUseByte );
   printf( "sizeof(HZK16_FONTDATA ) is %u \n", sizeof(hanZiFont16_cg ) );
   printf( "maxIndex is %u\n", maxIndex);

}



bool PSP_CNFONTS16_16::getBitMapHZKLibIndex ( unsigned short index , HanZi16FontBitMap &bitmap ) 
{
    const unsigned char *cfont;		//pointer to font
    unsigned long xi, yi;
    unsigned long b;
    unsigned char  *vptr0, *vptr;

    //sanity checking
    //if (  bitmap.data_p == NULL )return false;
    //if (  bitmap.heigh < PSP_CNFONTS16_16::heigh ) return false;
    //if (  bitmap.width < PSP_CNFONTS16_16::width ) return false;
    if( bitmap.size < heigh * width ) return false;
    // make sure the bitmap can contain the whole Hanzi dot matrix.
    
    
    Dot_Font_Data cnDotFontData; 
    PSP_CNFONTS16_16::getFontData( index , &cnDotFontData); 

    cfont = cnDotFontData.fontData;
    vptr0 = bitmap.data ;   
    for ( yi=0; yi<cnDotFontData.heigh; yi++) 
    {   
        vptr = vptr0;
        b = 0x80;
        for ( xi =0; xi<cnDotFontData.width; xi++) 
        {
            if(  xi ==  cnDotFontData.dotsPerByte )
            {
                b = 0x80;
                cfont++;                    
            }
            if (  (*cfont) & b ) 
            {             
                *vptr = defaultColor;
            }else
            {   //important! make sure the background is Zeroed !
                *vptr = 0;
            }
            vptr++;
            b=b>>1;
        }
        vptr0 +=  dotFontHanZiFontWidth;
        cfont++;
    }
    
    return true;



}


bool PSP_CNFONTS16_16::drawKouHuBitMap ( HanZi16FontBitMap &bitmap )
{
    if( bitmap.size < heigh * width ) return false;
    
    //memset( bitmap.data,  0, bitmap.size );
    memcpy( bitmap.data,  kouhuData, width*heigh );
    printf("copy kou hu \n");
    return true;
    

}
   

bool PSP_CNFONTS16_16::drawHanZi16ToBMP ( unsigned short hanZi,   HanZi16FontBitMap &bitmap ) 
                                           
{
   

    //sanity checking
    //if (  bitmap.data_p == NULL )return false;
    //if (  bitmap.heigh < PSP_CNFONTS16_16::heigh ) return false;
    //if (  bitmap.width < PSP_CNFONTS16_16::width ) return false;
    if( bitmap.size < heigh * width ) return false;
    // make sure the bitmap can contain the whole Hanzi dot matrix.
    
    
     
    HanZi hz;
    hz.ushort = hanZi;
    unsigned long HZKLibIndex = HanZiToHZKLibIndex ( hz );    
    //!!!!!!
     //printf( "index is %u\n", HZKLibIndex );
    if ( HZKLibIndex == noFontHZKIndex )
    {
        return drawKouHuBitMap( bitmap  );
    }
    else
    {
       return getBitMapHZKLibIndex( HZKLibIndex , bitmap );
    }   

}
    
   /* PSP_CNFONTS16_16::getFontData( index , &cnDotFontData); 

    cfont = cnDotFontData.fontData;
    vptr0 = bitmap.data ;   
    for ( yi=0; yi<cnDotFontData.heigh; yi++) 
    {   
        vptr = vptr0;
        b = 0x80;
        for ( xi =0; xi<cnDotFontData.width; xi++) 
        {
            if(  xi ==  cnDotFontData.dotsPerByte )
            {
                b = 0x80;
                cfont++;                    
            }
            if (  (*cfont) & b ) 
            {             
                *vptr = defaultColor;
            }else
            {   //important! make sure the background is Zeroed !
                *vptr = 0;
            }
            vptr++;
            b=b>>1;
        }
        vptr0 +=  hanZiFontWidth;
        cfont++;
    }
    
    return true;
    */


//  End of bool PSP_CHINESEHANDLER::drawHanZiToBMP ( unsigned short hanZi, bool *bmp, 
//                                                     unsigned short sizeLimited )






unsigned long PSP_CNFONTS16_16::HanZiToHZKLibIndex ( const HanZi &hanzi )
{
   if ( &hanzi == NULL )
   {
      return 0;
   }	
   
   //if this is is No font 
   if ( hanzi.ushort == noFontHanZi )
   {
       return noFontHZKIndex;
   }
   
   unsigned int quIndex , weiIndex;
   quIndex = hanzi.ch[0];
   weiIndex = hanzi.ch[1];

   // this is the index of CN char in the lib.
   return (unsigned long) ((quIndex-0x81)*(0xFF-0x40)+(weiIndex-0x40))  ;

   

}




