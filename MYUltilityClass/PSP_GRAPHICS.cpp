
//=======================================================================
//
//  Chris Jiang
//
//  2005 07 27
//  
//=======================================================================

 

#define __PSPSDK__

#include <PSP_GRAPHICS.h>

#include <CommonString.h>
#include <PSP_AscCnFont.h>
#include <BMP_Kitty.h>
#include <IconLib.h>

#include <PSP_Global.h>
#include <PSP_ChineseUtil.h>
#ifdef __PSPSDK__

#include <pspctrl.h>

#endif

//#include <PSP_ChineseUtil.h>

/*
==========================================================================================
VRAM CACHED !!!!!!!
In a simplistic way on the PSP's CPU if you access memory from 0x00000000 -> 0x3FFFFFFF 
the read and writes are cached. 
That same memory is also mapped to 0x40000000 -> 0x7FFFFFFF 
but in this case it is uncached and all read and writes go directly to memory. 
VRAM access should be made to the address 0x44000000 otherwise some of the pixels 
could get cached and you see artifacts such as missing pixels.
Now if the GE is ever properly utilised this will be totally irrelevant ;P

Screen Size and VRam mapping !!!!!!!
Screen size is 
480 X 272

However
every line takes 512 ( NOT 480 ) dot in MEMORY !!!!!!
(One dot takes two byte, thus one unsigned short)
If draw dot 480 - 512 , nothing shows.

So,  line 0  takes   (0 to 512 )* 2  bytes
line 1  takes   (512 to 512+512 )* 2  bytes
To draw to the screen can only draw  0 to 479. 480 to 511 is invisible.
==========================================================================================
*/




using namespace PSP_Constants;




PSP_SCREEN_MODE PSP_GRAPHICS::screenMode;
unsigned short * PSP_GRAPHICS::frameBufferA_p = psp_vram_base_cg;
unsigned short * PSP_GRAPHICS::frameBufferB_p = psp_vram_base_cg;
//using :  showing this buffer
bool PSP_GRAPHICS::isUsingBufferA = true;
const Rectangle  PSP_GRAPHICS::defaultRec;// full screen

//ucharBitMap PSP_GRAPHICS::ASCBitMap_am[ maxASCSize ];


//private constructor
PSP_GRAPHICS::PSP_GRAPHICS ( )
{

}



/*
void PSP_GRAPHICS::initASCBitMaps ( void )
{
unsigned i = 0;
for (i=0;i< maxASCSize; i++)
{
//ASCBitMap_am

}


}
*/
void PSP_GRAPHICS::setScreenFrameMode( PSP_SCREEN_MODE mode )
{
    sceDisplaySetMode( 0, PSP_SCREEN_WIDTH_Visible_cg   , PSP_SCREEN_HEIGH_cg );    
    screenMode = mode;

    if( mode == screenModeWriteAndBuffered_cg )
    {                                   
        frameBufferA_p   = ( unsigned short *)psp_vram_base_cg ;
        //Use cached, just vrambase; do not add uncached offset.       

        frameBufferB_p = ( unsigned short *)psp_vram_base_cg + screenFramSize_dot_cg;
        isUsingBufferA  = true;
        sceDisplaySetFrameBuf( frameBufferA_p  , PSP_SCREEN_WIDTH_TakeMemoryDot,  1 ,  1 );

    }
    else // mode == screenModeWriteAndShow_cg or other
    {            

        frameBufferA_p   = ( unsigned short *)psp_vram_base_cg ;
        frameBufferB_p = ( unsigned short *)psp_vram_base_cg + screenFramSize_dot_cg ;
        ;
        isUsingBufferA = true;
        sceDisplaySetFrameBuf(  frameBufferA_p , PSP_SCREEN_WIDTH_TakeMemoryDot, 1,   1 );

    }
    fillVRam( 0 );
}



void PSP_GRAPHICS::fillVRam( unsigned short color)
{
    unsigned short *vram_temp;    
    vram_temp =  getVRamAddr(0,0);  //frameBufferDrawThis_p  ;    

    unsigned long i;
    for ( i=0 ; i< screenFramSize_dot_cg ; i++)
    { 
        vram_temp[i] = color ;     
    }
}   




void PSP_GRAPHICS::flipScreenBuffer( void )
{
    //ONLY  screenModeWriteAndBuffered_cg mode should use flip
    if( screenMode != screenModeWriteAndBuffered_cg )
    {
        return;
    }


    if( isUsingBufferA )
    {
        isUsingBufferA = false;
        sceDisplaySetFrameBuf( frameBufferB_p ,
            PSP_SCREEN_WIDTH_TakeMemoryDot , 1 ,1 );     
    }else
    {   //else: now using buffer B, so change it to A.

        isUsingBufferA  = true;            
        sceDisplaySetFrameBuf( frameBufferA_p ,
            PSP_SCREEN_WIDTH_TakeMemoryDot , 1 ,1 );     
    }

}


/*

void PSP_GRAPHICS:: graphicBitBlt_CheckBorder( const DrawAttribute &drawAtt, 
const ushortBitMap & ushortbitmap )
{     
if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ) 
{
return ;
}	
if ( drawAtt.y > PSP_SCREEN_HEIGH_cg ) 
{
return ;
}
unsigned short *vramp_temp; 
unsigned short xi,yi;
const unsigned short *datap_temp;
datap_temp = ushortbitmap.data_p ;
vramp_temp = getVRamAddr( drawAtt.x , drawAtt.y );
//out of border, do not draw.
if( vramp_temp  == NULL ) return;

for (yi=0; yi< ushortbitmap.heigh; yi++) 
{    
for ( xi=0; xi< ushortbitmap.width ; xi++) 
{            
if( ( (drawAtt.y + yi ) < PSP_SCREEN_HEIGH_cg ) &&
( (drawAtt.x + xi ) < PSP_SCREEN_WIDTH_Visible_cg )  
)  
{
vramp_temp [xi] = *datap_temp ; 
}

// since vramp_temp is unsigned short, ++ is +2
// bytesPerPixel_cg ; 
datap_temp ++;
}
vramp_temp += PSP_SCREEN_WIDTH_TakeMemoryDot;           
}
}
*/


/*
void PSP_GRAPHICS:: graphicBitBlt_CheckBorder( const DrawAttribute &drawAtt, 
const ucharBitMap & ucharbitmap,
unsigned short color   )
{     
if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ) 
{
return ;
}	
if ( drawAtt.y > PSP_SCREEN_HEIGH_cg ) 
{
return ;
}

unsigned short *vramp_temp; 
unsigned short xi,yi;
const unsigned char *datap_temp;
unsigned short datatemp;
datap_temp = ucharbitmap.data_p;
vramp_temp = getVRamAddr( drawAtt.x , drawAtt.y );
//out of border, do not draw.
if( vramp_temp  == NULL ) return;

for (yi=0; yi< ucharbitmap.heigh; yi++) 
{    
for ( xi=0; xi< ucharbitmap.width ; xi++) 
{            
if( ( ( drawAtt.y + yi ) < PSP_SCREEN_HEIGH_cg ) &&
( ( drawAtt.x + xi ) < PSP_SCREEN_WIDTH_Visible_cg )  
)  
{
datatemp = *datap_temp ;
if( datatemp)
{
vramp_temp [xi] =  color ; 
}
}

// since vramp_temp is unsigned short, ++ is +2
// bytesPerPixel_cg ; 
datap_temp ++;
}
vramp_temp += PSP_SCREEN_WIDTH_TakeMemoryDot;           
}
}

*/

void PSP_GRAPHICS::drawRectangle ( const Rectangle &rec,  unsigned short color )

{


    drawHorizonLine ( rec.x , rec.x + rec.width,  rec.y, color );
    drawHorizonLine ( rec.x , rec.x + rec.width,  rec.y + rec.heigh, color );

    drawVerticalLine ( rec.y , rec.y + rec.heigh, rec.x, color );
    drawVerticalLine ( rec.y , rec.y + rec.heigh, rec.x + rec.width, color );


}                                

/*

void PSP_GRAPHICS:: graphicBitBlt_NoCheck ( const DrawAttribute &drawAtt,
const ushortBitMap & ushortbitmap )
{     
unsigned short *vramp_temp; 
unsigned short xi,yi;
const unsigned short *datap_temp;
datap_temp = ushortbitmap.data_p;
vramp_temp = getVRamAddr( drawAtt.x, drawAtt.y);
//out of border, do not draw.
if( vramp_temp  == NULL ) return;

for (yi=0; yi< ushortbitmap.heigh; yi++) 
{    
for ( xi=0; xi< ushortbitmap.width ; xi++) 
{   
vramp_temp [xi] = *datap_temp ;    
// since vramp_temp is unsigned short, ++ is +2
// bytesPerPixel_cg ; 
datap_temp ++;
}
vramp_temp += PSP_SCREEN_WIDTH_TakeMemoryDot;           
}
}

*/

/*
void PSP_GRAPHICS:: graphicBitBlt_NoCheck (  const DrawAttribute &drawAtt,
const ucharBitMap & ucharbitmap,
unsigned short color   )
{     
unsigned short *vramp_temp; 
unsigned short xi,yi;
const unsigned char *datap_temp;
unsigned short tempdata;
datap_temp = ucharbitmap.data_p;
vramp_temp = getVRamAddr( drawAtt.x, drawAtt.y);
//out of border, do not draw.
if( vramp_temp  == NULL ) return;

for (yi=0; yi< ucharbitmap.heigh; yi++) 
{    
for ( xi=0; xi< ucharbitmap.width ; xi++) 
{   tempdata = *datap_temp ;
if ( tempdata )
{ 
vramp_temp [xi] =    color ;
}
// since vramp_temp is unsigned short, ++ is +2
// bytesPerPixel_cg ; 
datap_temp ++;
}
vramp_temp += PSP_SCREEN_WIDTH_TakeMemoryDot;           
}
}

*/



void PSP_GRAPHICS::graphicBitBlt_CheckBorder( const DrawAttribute &drawAtt, 
                                                    const ushortBitMap & ushortbitmap    ) 
{
    if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ) 
    {
        return ;
    }	
    if ( drawAtt.y > PSP_SCREEN_HEIGH_cg ) 
    {
        return ;
    }
    
    unsigned short *vptr0;		//pointer to vram
    unsigned short *vptr;		//pointer to vram
    unsigned long xi, yi, mx, my;
    const unsigned short *dd;
    const unsigned short *data_temp;
    data_temp =  ushortbitmap.data_p;

    vptr0 = getVRamAddr( drawAtt.x, drawAtt.y );

    const Rectangle *drawRec_p   ;

    drawRec_p =  drawAtt.limitedRec_p;
   

    for (yi=0; yi< ushortbitmap.heigh; yi++) 
    {
        vptr=vptr0;
        dd= data_temp;
        for (xi=0; xi< ushortbitmap.width; xi++) 
        {
           /* unsigned short pointX, pointY;
            pointX = drawAtt.x + xi ; 
            pointY = drawAtt.y + yi ;
            */
            *vptr= *dd ;
           /* if ( drawRec_p )
            {   
                if ( drawRec_p ->isInside( pointX, pointY ) )
                {   
                    if ( drawAtt.drawBackGroudZero )
                    {
                        *vptr=*dd;
                    }
                    else
                    {
                        //only draw color point,  black (0) do not draw
                        if(*dd) *vptr=*dd;
                    }
                }      
            }
            else
            {
               if ( drawAtt.drawBackGroudZero )
                {
                    *vptr=*dd;
                }
                else
                {
                    //only draw color point,  black (0) do not draw
                    if(*dd) *vptr=*dd;
                }
            //}
 */

            vptr++;

            dd++;
        }
        vptr0 += PSP_SCREEN_WIDTH_TakeMemoryDot;

        data_temp += ushortbitmap.width;
    }
}                         




void PSP_GRAPHICS::graphicBitBltEnlarge_CheckBorder( const DrawAttribute &drawAtt, 
                                                    const ushortBitMap & ushortbitmap    ) 
{
    if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ) 
    {
        return ;
    }	
    if ( drawAtt.y > PSP_SCREEN_HEIGH_cg ) 
    {
        return ;
    }
    unsigned short enlarge = drawAtt.enlarge ;
    if (enlarge >=4 )
    {
        enlarge =4;
    }else if ( enlarge < 1)
    {
        enlarge = 1;
    }
    unsigned short *vptr0;		//pointer to vram
    unsigned short *vptr;		//pointer to vram
    unsigned long xi, yi, mx, my;
    const unsigned short *dd;
    const unsigned short *data_temp;
    data_temp =  ushortbitmap.data_p;

    vptr0 = getVRamAddr( drawAtt.x, drawAtt.y );

    const Rectangle *drawRec_p   ;

    drawRec_p =  drawAtt.limitedRec_p;
   

    for (yi=0; yi< ushortbitmap.heigh; yi++) 
    {
        for (my=0; my<   enlarge; my++) 
        {
            vptr=vptr0;
            dd= data_temp;
            for (xi=0; xi< ushortbitmap.width; xi++) 
            {
                for (mx=0; mx<   enlarge; mx++) 
                {   
                    unsigned short pointX, pointY;
                    pointX = drawAtt.x + xi*  enlarge+ mx  ; 
                    pointY = drawAtt.y + yi*  enlarge+ my ;

                    //make sure the whole image is inside screen
                    if( (  pointY < (unsigned short)PSP_SCREEN_HEIGH_cg ) &&
                        (  pointX < (unsigned short)PSP_SCREEN_WIDTH_Visible_cg )  

                        ) 
                    
                        if ( drawRec_p )
                        {   
                            if ( drawRec_p ->isInside( pointX, pointY ) )
                            {   
                                if ( drawAtt.drawBackGroudZero )
                                {
                                   *vptr=*dd;
                                }
                                else
                               {
                                  //only draw color point,  black (0) do not draw
                                  if(*dd) *vptr=*dd;
                               }
                            }      
                        }
                        else
                        {
                            if ( drawAtt.drawBackGroudZero )
                            {
                               *vptr=*dd;
                            }
                            else
                           {
                              //only draw color point,  black (0) do not draw
                              if(*dd) *vptr=*dd;
                           }
                        }
                        
                    
                    vptr++;
                }
                dd++;
            }
            vptr0 += PSP_SCREEN_WIDTH_TakeMemoryDot;
        }
        data_temp += ushortbitmap.width;
    }
}                         


void PSP_GRAPHICS::graphicBitBltEnlarge_CheckBorderTurn( const DrawAttribute &drawAtt, 
                                                        const ucharBitMap & ucharbitmap,
                                                        unsigned short color   ) 
{
    if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ) 
    {
        return ;
    }	
    if ( drawAtt.y > PSP_SCREEN_HEIGH_cg ) 
    {
        return ;
    }

    unsigned short enlarge = drawAtt.enlarge ;
    if (enlarge >=4 )
    {
        enlarge = 4;
    }else if ( enlarge < 1)
    {
        enlarge = 1;
    }

    unsigned short *vptr0;		//pointer to vram
    unsigned short *vptr;		//pointer to vram
    unsigned long xi, yi, mx, my;
    const unsigned char *dd;
    const unsigned char *data_temp;
    data_temp =  ucharbitmap.data_p;

    vptr0 = getVRamAddr( drawAtt.x, drawAtt.y );
    dd= data_temp;

    const Rectangle *drawRec_p    ;
    drawRec_p =   drawAtt.limitedRec_p ;
    if( drawRec_p == 0 ) 
    {
        drawRec_p = &defaultRec;
    }

    for (yi=0; yi< ucharbitmap.heigh; yi++) 
    {
        for (my=0; my<   enlarge; my++) 
        {
            vptr=vptr0;

            for (xi=0; xi< ucharbitmap.width; xi++) 
            {
                for (mx=0; mx<  enlarge; mx++) 
                {
                    unsigned short pointX, pointY;
                    pointX = drawAtt.x + xi*  enlarge+ mx  ; 
                    pointY = drawAtt.y + yi*  enlarge+ my ;

                    if(   pointY  < (unsigned short)PSP_SCREEN_HEIGH_cg
                        &&
                        pointX  < (unsigned short)PSP_SCREEN_WIDTH_Visible_cg 
                        ) 
                    {   
                        if( drawRec_p->isInside( pointX, pointY ) )
                        {
                            if (  *(dd + ucharbitmap.width * xi + ucharbitmap .width - yi -1 ))
                            {  *vptr= color ; 
                            }
                        }   

                    }   
                    vptr++;
                }
                //dd++;
            }
            vptr0 += PSP_SCREEN_WIDTH_TakeMemoryDot;
        }
        //data_temp += ucharbitmap.width;
    }
}                         




void PSP_GRAPHICS::graphicBitBltEnlarge_CheckBorder( const DrawAttribute &drawAtt, 
                                                    const ucharBitMap & ucharbitmap,
                                                    unsigned short color   )  
{
    if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ) 
    {
        return ;
    }	
    if ( drawAtt.y > PSP_SCREEN_HEIGH_cg ) 
    {
        return ;
    }
    unsigned short enlarge = drawAtt.enlarge ;
    if (enlarge >=4 )
    {
        enlarge =4;
    }else if ( enlarge <=1)
    {
        enlarge = 1;
    }

    unsigned short *vptr0;		//pointer to vram
    unsigned short *vptr;		//pointer to vram
    unsigned long xi, yi, mx, my;
    const unsigned char *dd;
    const unsigned char *data_temp;
    unsigned short tempdata;
    data_temp = ucharbitmap.data_p;

    vptr0 = getVRamAddr( drawAtt.x, drawAtt.y );

    const Rectangle *drawRec_p;
    drawRec_p =   drawAtt.limitedRec_p;
    if( drawRec_p == 0 ) 
    {
        drawRec_p = &defaultRec;
    }


    for (yi=0; yi< ucharbitmap.heigh; yi++) 
    {
        for (my=0; my< enlarge; my++) 
        {
            vptr=vptr0;
            dd= data_temp;
            for (xi=0; xi< ucharbitmap.width; xi++) 
            {
                for (mx=0; mx< enlarge; mx++) 
                {
                    unsigned short pointX, pointY;
                    pointX = drawAtt.x + xi*  enlarge+ mx  ; 
                    pointY = drawAtt.y + yi*  enlarge+ my ;


                   // if( (  pointY  < (unsigned short)PSP_SCREEN_HEIGH_cg ) &&
                        //(  pointX  < (unsigned short)PSP_SCREEN_WIDTH_Visible_cg )  
                        //) 
                    {    //if inside the draw rec ?
                        if( drawRec_p->isInside( pointX, pointY) )
                        { 
                            tempdata = *dd;
                            if( tempdata )
                            {
                                *vptr =  color ;
                            } 
                        }
                    }   
                    vptr++;
                }
                dd++;
            }
            vptr0 += PSP_SCREEN_WIDTH_TakeMemoryDot;
        }
        data_temp += ucharbitmap.width;
    }
}                         



/*
void PSP_GRAPHICS::graphicBitBltEnlarge_NoCheck( unsigned short x, unsigned short y, 
unsigned short width, 
unsigned short heigh, 
const unsigned short *data,  
unsigned char enlarge              )
{
unsigned short *vptr0;		//pointer to vram
unsigned short *vptr;		//pointer to vram
unsigned long xi, yi, mx, my;
const unsigned short *dd;
unsigned short *data_temp;
data_temp =(unsigned short *) data;

vptr0 = getVRamAddr( x, y );
for (yi=0; yi<heigh; yi++) 
{
for (my=0; my<enlarge; my++) 
{
vptr=vptr0;
dd= data_temp;
for (xi=0; xi<width; xi++) 
{
for (mx=0; mx<enlarge; mx++) 
{   
*vptr = *dd;   
vptr++;
}
dd++;
}
vptr0 += PSP_SCREEN_WIDTH_TakeMemoryDot;
}
data_temp += width;
}
}                         



void PSP_GRAPHICS::graphicBitBltEnlarge_NoCheck( unsigned short x, unsigned short y, 
unsigned short width, 
unsigned short heigh, 
const unsigned char *data,  
unsigned char enlarge , unsigned short color  )
{
unsigned short *vptr0;		//pointer to vram
unsigned short *vptr;		//pointer to vram
unsigned long xi, yi, mx, my;
const unsigned char *dd;
const unsigned char *data_temp;
data_temp = data;
unsigned short tempdata;
vptr0 = getVRamAddr( x, y );
for (yi=0; yi<heigh; yi++) 
{
for (my=0; my<enlarge; my++) 
{
vptr=vptr0;
dd= data_temp;
for (xi=0; xi<width; xi++) 
{
for (mx=0; mx<enlarge; mx++) 
{  
tempdata = *dd;
if( tempdata )
{
*vptr = color;
}   
vptr++;
}
dd++;
}
vptr0 += PSP_SCREEN_WIDTH_TakeMemoryDot;
}
data_temp += width;
}
}                         
*/

unsigned short * PSP_GRAPHICS::getVRamAddr(unsigned  short x, unsigned  short y)
{
    unsigned short * returnAddrp;
    int xx, yy;
    xx = x;
    yy = y;
    if ( xx >= PSP_SCREEN_WIDTH_Visible_cg ) return NULL;
    if ( yy >= PSP_SCREEN_HEIGH_cg ) return NULL;

    // In two buffer /flip mode, return the Inactive buffer addr.
    if( screenMode == screenModeWriteAndBuffered_cg )
    {
        if( isUsingBufferA )
        {
            returnAddrp = frameBufferB_p;
        }else
        {
            returnAddrp = frameBufferA_p;
        }
    }
    else  
    {  //In write and show mode
        returnAddrp = frameBufferA_p;
    }     
    returnAddrp += xx + yy * PSP_SCREEN_WIDTH_TakeMemoryDot ;
    return returnAddrp ;


} 


// always return the front buffer . 
// in case of a swap buffer graphic mode, if you want get a image from the screen
// you must use this getVRamAddrFrontBuffer.
unsigned short * PSP_GRAPHICS::getVRamAddrFrontBuffer (unsigned  short x, unsigned  short y)
{
    unsigned short * returnAddrp;
    int xx, yy;
    xx = x;
    yy = y;
    if ( xx >= PSP_SCREEN_WIDTH_TakeMemoryDot ) return NULL;
    if ( yy >= PSP_SCREEN_HEIGH_cg ) return NULL;

    // In two buffer /flip mode, return the Inactive buffer addr.
   // if( screenMode == screenModeWriteAndBuffered_cg )
   // {
        if( isUsingBufferA )
        {
            returnAddrp = frameBufferA_p;
        }else
        {
            returnAddrp = frameBufferB_p;
        }
    //}
    //else  
    //{  //In write and show mode
        //returnAddrp = frameBufferA_p;
    //}     
    returnAddrp += xx + yy * PSP_SCREEN_WIDTH_TakeMemoryDot ;
    return returnAddrp ;


} 


/*

void PSP_GRAPHICS::drawBitMap(  const DrawAttribute &drawAtt, 
const ushortBitMap &bitmap )

{
if (  (( drawAtt.x + bitmap.width ) < PSP_SCREEN_WIDTH_Visible_cg )
&& 
(( drawAtt.y + bitmap.heigh ) < PSP_SCREEN_HEIGH_cg         )
)
{   // the whole image is inside the screen border

PSP_GRAPHICS::graphicBitBlt_NoCheck(  drawAtt, bitmap );

}else if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ||
drawAtt.y > PSP_SCREEN_HEIGH_cg 
) 
{   
// the whole image is out of screen . 
return ;
}
else 
{   // the image is getting out of the screen border
// some inside, some outside
// must use the check one.
PSP_GRAPHICS ::graphicBitBlt_CheckBorder
(  drawAtt, bitmap ) ;
}                     

}
*/
/*

void PSP_GRAPHICS::drawBitMap(    const DrawAttribute &drawAtt,  unsigned short color,
const ucharBitMap  & bitmap   )

{
if (  (( drawAtt.x + bitmap.width ) < PSP_SCREEN_WIDTH_Visible_cg )
&& 
(( drawAtt.y + bitmap.heigh ) < PSP_SCREEN_HEIGH_cg         )
)
{   // the whole image is inside the screen border

PSP_GRAPHICS::graphicBitBlt_NoCheck 
( drawAtt ,   bitmap, color  );
}
else if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ||
drawAtt.y > PSP_SCREEN_HEIGH_cg 
) 
{   
// the whole image is out of screen . 
return ;
}
else 
{   // the image is getting out of the screen border
// must use the check one.
PSP_GRAPHICS ::graphicBitBlt_CheckBorder ( drawAtt  , bitmap, color  );
}                     

}

*/



void PSP_GRAPHICS::drawBitMap (   const DrawAttribute &drawAtt,   
                               const ushortBitMap & bitmap    )

{    

    // should not check in here
    // because maybe need draw half image.
    /*   if ( ! isInside ( drawAtt.x, drawAtt.y ) )
    {
    return ;
    }  
    */

    if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ||
        drawAtt.y > PSP_SCREEN_HEIGH_cg 
        ) 
    {   
        // the whole image is out of screen . 
        return ;
    }

    // the image is getting out of the screen border
    // must use the check one.

    PSP_GRAPHICS ::graphicBitBltEnlarge_CheckBorder( drawAtt, bitmap );
   //PSP_GRAPHICS ::graphicBitBlt_CheckBorder ( drawAtt, bitmap );

}

void PSP_GRAPHICS::drawBitMap ( const DrawAttribute &drawAtt,  unsigned short color,
                               const ucharBitMap & ucharbitmap  ) 
{   
    if ( drawAtt.x > PSP_SCREEN_WIDTH_Visible_cg ||
        drawAtt.y > PSP_SCREEN_HEIGH_cg 
        ) 
    {   
        // the whole image is out of screen . 
        return ;
    }

    // the image is getting out of the screen border
    // must use the check one.

    PSP_GRAPHICS ::graphicBitBltEnlarge_CheckBorder( drawAtt, ucharbitmap , color );


}       


/*


void PSP_GRAPHICS::graphicPrintChar8_16 ( unsigned short x, unsigned short y,
unsigned short color, unsigned short bgcolor,
char printChar, bool drawfg, bool drawbg, unsigned char mag)
{
unsigned short *vptr0;		//pointer to vram
unsigned short *vptr;		//pointer to vram
const unsigned char *cfont;		//pointer to font
unsigned long cx,cy;
unsigned long b;
char mx,my;

//if (printChar > 255) return;
Dot_Font_Data dotFontData;

//cfont = font + printChar*8;
PSP_ASCFONTS8_16::getFontData( printChar , &dotFontData);
//cfont = good_asc_8_16_font + printChar*16;
cfont = dotFontData.fontData;

vptr0 = getVRamAddr( x , y );
for (cy=0; cy<dotFontData.heigh; cy++) 
{
for (my=0; my<mag; my++) 
{
vptr=vptr0;
b=0x80;
for (cx=0; cx<dotFontData.width; cx++) 
{
for (mx=0; mx<mag; mx++) 
{
if ((*cfont&b)!=0) 
{
if (drawfg) *(unsigned short *)vptr=color;

} else 
{
if (drawbg) *(unsigned short *)vptr=bgcolor;
}
vptr++;
}
b=b>>1;
}
vptr0 += PSP_SCREEN_WIDTH_TakeMemoryDot ;
}
cfont++;
}

}

*/







void PSP_GRAPHICS::fillColorRect ( const Rectangle &rec,  unsigned short color )
{
    unsigned short i , x2, y, x1, heigh;
    //x1 = rec.x +1;
    x1 = rec.x ;
    //x2 = rec.x + rec.width -1;
    x2 = rec.x + rec.width -1;
    
    //y = rec.y + 1;
    y = rec.y ;    
    heigh = rec.heigh  ;
    for ( i =0;i< heigh; i++)
    {
        PSP_GRAPHICS::drawHorizonLine ( x1, x2 , y + i, color);

    }


}


void PSP_GRAPHICS::drawHorizonLine ( unsigned short x1 , unsigned short x2, 
                                    unsigned short y, unsigned short color )
{
    if ( y >=  PSP_SCREEN_HEIGH_cg ) 
    { 
        return ;
    }	 

    if (x1 == x2 ) 
    {

    }

    // swap if the x1 larger than x2 
    if ( x1 > x2 ) 
    {
        unsigned short temp;
        temp = x1;
        x1 = x2;
        x2 = temp;	
    }

    if( x1 >= PSP_SCREEN_WIDTH_Visible_cg )
    {
        return ;
    }
    if( x2 >=  PSP_SCREEN_WIDTH_Visible_cg )
    {
        x2 = PSP_SCREEN_WIDTH_Visible_cg;	
    }


    unsigned short *vram_temp;
    vram_temp = getVRamAddr ( x1, y );
    unsigned short length; 
    length = x2 - x1;    

    int i;
    //   use <=   x1 to x1 should be a pixel
    //    1 to 2  should be two pixels       
    for( i = 0; i<= length ;i++)
    {  
        vram_temp [i]= color;
    }

}



void PSP_GRAPHICS::drawVerticalLine ( unsigned short y1 , unsigned short y2, 
                                     unsigned short x,  unsigned short color )
{
    if ( x >=  PSP_SCREEN_WIDTH_Visible_cg ) 
    { 
        return ;
    }	 

    if (y1 == y2 ) 
    {

    }

    // swap if the x1 larger than x2 
    if ( y1 > y2 ) 
    {
        unsigned short temp;
        temp = y1;
        y1 = y2;
        y2 = temp;	
    }

    if( y1 >= PSP_SCREEN_HEIGH_cg )
    {
        return ;
    }

    if( y2 >=  PSP_SCREEN_HEIGH_cg )
    {
        y2 = PSP_SCREEN_HEIGH_cg;	
    }


    unsigned short *vram_temp;
    vram_temp = getVRamAddr ( x, y1 );
    unsigned short length; 
    length = y2 - y1;    

    int i;
    //   use <=   x1 to x1 should be a pixel
    //    1 to 2  should be two pixels       
    for( i = 0; i<= length ;i++)
    {  
        vram_temp [ i* PSP_SCREEN_WIDTH_TakeMemoryDot ]= color;
    }


}                                   


void PSP_GRAPHICS::drawKitty ( unsigned short x , unsigned short y  )
{
    ushortBitMap bmpKitty;
    bmpKitty.width = 220;
    bmpKitty.heigh = 165;
    bmpKitty.data_p = (unsigned short *)kittycData ;    
    
    //  ***************************************************************************
    //Testing put bitmap at the x border
    //do not draw bmp outside the border.
    DrawAttribute drawAtt;
    drawAtt.x  = x;
    drawAtt.y = y; 
    drawBitMap ( drawAtt ,  bmpKitty ); 
}    
    
    
void PSP_GRAPHICS::selfTestSingleFrame( void )
{
    unsigned short white = 0xffff;
    unsigned short black = 0;
    unsigned short other1 = 0x3333;
    //unsigned short other2 = 0x6666;
    ushortBitMap bmpKitty;
    bmpKitty.width = 220;
    bmpKitty.heigh = 165;
    bmpKitty.data_p = (unsigned short *)kittycData ;
    //   sceCtrlSetSamplingCycle(0);
    //   sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    setScreenFrameMode(  screenModeWriteAndShow_cg );
    int i;    

    //  ***************************************************************************
    //Testing put bitmap at the x border
    //do not draw bmp outside the border.
    DrawAttribute drawAtt;
    drawAtt.y = 0;
    for(i =0; i <30;i++)
    {   drawAtt.x = i +470;
    drawBitMap ( drawAtt ,  bmpKitty ); 
    getch( );
    fillVRam( black );

    }
    // ***************************************************************************
    //Testing put bitmap at the y border
    //do not draw bmp outside the border.
    drawAtt.x = 0;
    for(i =0; i <30;i++)
    {   drawAtt.y = 262+i;
    drawBitMap ( drawAtt , bmpKitty ); 
    getch( );
    fillVRam( black );

    }
    //  ***************************************************************************

    // ***************************************************************************
    //Testing put bitmap at the y border
    //do not draw bmp outside the border.
    drawAtt.y = 0; 
    drawAtt.enlarge = 2;       
    for(i =0; i <20;i++)
    {   drawAtt.x = 470 + i;

    drawBitMap ( drawAtt  , bmpKitty   );
    getch();
    fillVRam( black );
    }

    drawAtt.x = 0; 
    //***************************************************************************
    for(i =0; i <20;i++)
    {   drawAtt.y = 262+i;
    drawBitMap ( drawAtt, bmpKitty );
    getch();
    fillVRam( black );
    }

    //***************************************************************************
    //    getch();

    fillVRam( black );
    drawAtt.x = 300; 
    drawAtt.y = 200; 
    drawAtt.enlarge =2;
    drawBitMap ( drawAtt , bmpKitty );
    getch();

    fillVRam( black );
    drawAtt.enlarge =3;
    drawAtt.x = 400; 
    drawAtt.y = 250; 
    drawBitMap (  drawAtt , bmpKitty );
    getch();

    drawAtt.x= 460;
    drawAtt.y = 270;
    drawAtt.enlarge = 4;
    fillVRam( black );

    drawBitMap ( drawAtt , bmpKitty );
    getch();

    fillVRam( white );

    getch();

    fillVRam( black );
    getch( );

    fillVRam( white );
    getch( ); 
    fillVRam( black );
    getch( );


    drawHorizonLine(0, 100, 1, other1);
    getch( );
    fillVRam( black );
    getch( );
    drawHorizonLine(0, 150, 5, 0xeeee);
    getch( );
    drawHorizonLine(0, 200, 10, 0xaaaa );
    getch( );

    drawHorizonLine(1, 250, 15, 0x3333);
    getch( );

    drawHorizonLine(0, 500, 20, 0x5555);
    getch( );

    drawAtt.x=0;
    drawAtt.y = 0 ;
    drawAtt.enlarge = 1;
    drawBitMap ( drawAtt , bmpKitty );
    getch( );

    fillVRam( 1 );
    getch( );
    fillVRam( 200 );
    getch( );
    fillVRam( 2000 );
    getch( );
    fillVRam( 20000 );
    getch( );
    fillVRam( 0xffff );
    getch( );
    //pspDebugScreenInit();

}





void PSP_GRAPHICS::selfTestDoubleFrame( void ) 
{

    unsigned short black = 0;
    //unsigned short other1 = 0x3333;

    ushortBitMap bmpKitty;
    bmpKitty.width = 220;
    bmpKitty.heigh = 165;
    bmpKitty.data_p = (unsigned short *)kittycData ;
    //   sceCtrlSetSamplingCycle(0);
    //   sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    setScreenFrameMode( screenModeWriteAndBuffered_cg  );
    int i;    

    //  ***************************************************************************
    //Testing put bitmap at the x border
    //do not draw bmp outside the border.

    fillVRam( 200 );
    getch( );
    flipScreenBuffer( );
    getch( );
    fillVRam( 2000 );
    getch( );
    flipScreenBuffer( );    
    getch( );

    fillVRam( 20000 );
    getch( );
    flipScreenBuffer( );    
    getch( );
    fillVRam( 0xffff );
    getch( );
    flipScreenBuffer( );
    getch( );
    fillVRam( 1 );
    flipScreenBuffer( );
    getch( );
    getch( );

    DrawAttribute drawAtt;
    drawAtt.y =0;
    for(i =0; i <20;i++)
    {
        drawAtt.x=i+470;
        drawBitMap ( drawAtt , bmpKitty ); 
        getch( );
        flipScreenBuffer( );
        getch( );
        fillVRam( black );
        getch( );

    }

    drawAtt.x=0;

    for(i =0; i <20;i++)
    { 
        drawAtt.y = 264+i;
        drawBitMap ( drawAtt , bmpKitty ); 
        flipScreenBuffer( );
        getch( );
        fillVRam( black );

    }


}


void PSP_GRAPHICS::selfTestDoubleFrameFlip( void )
{

    setScreenFrameMode( screenModeWriteAndBuffered_cg  );
    SceCtrlData pad; 


    //flipScreenBuffer( );
    //fillVRam( 0xeeee );

    do
    {   fillVRam( 0xffff );
    fillVRam( 0xeeee );
    fillVRam( 0xffff );
    fillVRam( 0xeeee );
    fillVRam( 0xffff );
    fillVRam( 0 );
    flipScreenBuffer( );
    getch();
    sceCtrlReadBufferPositive(&pad, 1); 

    if( pad.Buttons != 0)
    {    
        if (pad.Buttons & PSP_CTRL_CROSS)
        { 
            break;
        }   
    }        
    }while(true);


}



void PSP_GRAPHICS::selfTestDoubleFrameMove( void )
{
    int xi ;
    int vx = 2;
    int y=50;
    xi = 0;
    ushortBitMap  bmpKitty;
    bmpKitty.width = 220;
    bmpKitty.heigh = 165;
    bmpKitty.data_p = (unsigned short *)kittycData ;
    SceCtrlData pad; 
    int moveRight=1;
    DrawAttribute drawAtt ;
    setScreenFrameMode( screenModeWriteAndBuffered_cg  );
    do
    {
        if( moveRight )
        {   xi += vx;
        if( xi >= PSP_SCREEN_WIDTH_TakeMemoryDot - 10 )
        { 
            moveRight = 0;
        }
        }else
        {   xi -= vx;
        if ( xi <=3 )
        {
            moveRight = 1;
        }
        }

        //wait();
        fillVRam( 0xaaaa );        
        drawAtt.x=xi;
        drawAtt.y=y;
        drawBitMap ( drawAtt , bmpKitty );                
        flipScreenBuffer( );        
        sceCtrlReadBufferPositive(&pad, 1); 

        if( pad.Buttons != 0)
        {    
            if (pad.Buttons & PSP_CTRL_CROSS)
            { 
                break;
            }   
        }        
    }while ( true );

    wait( 10 );
    do
    {
        if( moveRight )
        {   xi += vx;
        if( xi >= PSP_SCREEN_WIDTH_TakeMemoryDot - 10 )
        { 
            moveRight = 0;
        }
        }else
        {   xi -= vx;
        if ( xi <=3 )
        {
            moveRight = 1;
        }
        }

        //wait();
        fillVRam( 0xeeee );
        drawAtt.x = xi;
        drawAtt.y=y;        
        drawAtt.enlarge = 2;
        drawBitMap  ( drawAtt , bmpKitty );
        flipScreenBuffer( );        
        wait(2);
        sceCtrlReadBufferPositive(&pad, 1); 

        if( pad.Buttons != 0)
        {    
            if (pad.Buttons & PSP_CTRL_CROSS)
            { 
                break;
            }   
        }        
    }while ( true );

}


/*
void PSP_GRAPHICS::graphicPrint( unsigned short x, unsigned short y,
unsigned short color, const char *str)
{

int i = 0;
char ch = str[i];
while ( ch !=0 ) 
{
//graphicPrintChar( x  , y , color, 0 , ch , true , false, 1);
graphicPrintChar8_16( x  , y , color, 0 , ch , true , false, 1);
i++;
x+= font_size_x ;
if (x >= PSP_SCREEN_WIDTH_Visible_cg ) 
{
x=0;
if( y >= PSP_SCREEN_HEIGH_cg)
{   
// getting out off the Y border.
// don't draw any more
break;
}
y += font_size_y;			
}
ch = str[i];
}


}



void PSP_GRAPHICS::graphicPrint2 ( unsigned short x,  unsigned short y,
unsigned short color, const char *str )
{
int i = 0;
char ch = str[i];
while ( ch !=0 )
{
//graphicPrintChar ( x, y , color, 0 , ch , true, false ,2);
graphicPrintChar8_16( x  , y , color, 0 , ch , true , false, 2);
i++;
x += font_size_x * 2;
if (x >= PSP_SCREEN_WIDTH_Visible_cg ) 
{
x = 0;
if( y >= PSP_SCREEN_HEIGH_cg)
{   
// getting out off the Y border.
// don't draw any more
break;
}
y += font_size_y * 2 ;			
}
ch = str[i];		
}
}





void PSP_GRAPHICS::graphicPrint4 ( unsigned short x,  unsigned short y,
unsigned short color, const char *str )
{
int i = 0;
char ch = str[i];
while ( ch !=0 )
{
//graphicPrintChar ( x, y , color, 0 , ch , true, false , 4 );
graphicPrintChar8_16( x  , y , color, 0 , ch , true , false, 4);
i++;
x += font_size_x * 4;
if (x >= PSP_SCREEN_WIDTH_Visible_cg ) 
{
x = 0;
if( y >= PSP_SCREEN_HEIGH_cg)
{   
// getting out off the Y border.
// don't draw any more
break;
}
y += font_size_y * 4 ;
}
ch = str[i];		
}
}
*/

/*  move to ChineseUtil
void PSP_GRAPHICS::drawAscChar8_16( const DrawAttribute &drawAtt, unsigned short color,  char ascChar )
{
    ucharBitMap  bitmap ;
    ASCFontBitMap *ascbitmap_p;

    ascbitmap_p = PSP_ASCFONTS8_16::getASCBmp( ascChar );     


    bitmap.width = PSP_ASCFONTS8_16::width;
    bitmap.heigh = PSP_ASCFONTS8_16::heigh ;
    bitmap.data_p = ascbitmap_p->data;

#ifdef __PSPSDK__
    drawBitMap (  drawAtt,   color , bitmap );
#else
    PSP_ChineseUtil:: printBitMapInTextMode ( bitmap );
#endif



}                     
*/

void PSP_GRAPHICS::graphicPrintAscString( unsigned short x, unsigned short y,
                                         unsigned short color, const char *str)
{

    int i = 0;
    char ch = str[i];
    DrawAttribute drawAtt;
    drawAtt.x = x;
    drawAtt.y = y;
    drawAtt.enlarge = 1;
    while ( ch !=0 ) 
    {
        //graphicPrintChar( x  , y , color, 0 , ch , true , false, 1);
        PSP_ChineseUtil::drawAscChar8_16( drawAtt , color,   ch);        
        i++;
        drawAtt.x  += DotFont16ASCfont_width ;
        if (x >= PSP_SCREEN_WIDTH_Visible_cg ) 
        {
            x=0;
            if( drawAtt.y >= PSP_SCREEN_HEIGH_cg)
            {   
                // getting out off the Y border.
                // don't draw any more
                break;
            }
            drawAtt.y += DotFont16ASCfont_heigh;			
        }
        ch = str[i];
    }


}


void PSP_GRAPHICS::graphicPrintAscString2( unsigned short x, unsigned short y,
                                          unsigned short color, const char *str)
{
    int i = 0;
    char ch = str[i];

    DrawAttribute drawAtt;
    drawAtt.x = x;
    drawAtt.y = y;
    drawAtt.enlarge =2;
    while ( ch !=0 )
    {
        //graphicPrintChar ( x, y , color, 0 , ch , true, false ,2);
        PSP_ChineseUtil::drawAscChar8_16( drawAtt , color,   ch  );
        i++;
        drawAtt.x += DotFont16ASCfont_width * 2;
        if ( drawAtt.x >= PSP_SCREEN_WIDTH_Visible_cg ) 
        {
            drawAtt.x = 0;
            if( drawAtt.y >= PSP_SCREEN_HEIGH_cg)
            {   
                // getting out off the Y border.
                // don't draw any more
                break;
            }
            drawAtt.y += DotFont16ASCfont_heigh * 2 ;			
        }
        ch = str[i];		
    }

}                                 


void PSP_GRAPHICS::graphicPrintAscString4( unsigned short x, unsigned short y,
                                          unsigned short color, const char *str)
{
    int i = 0;
    char ch = str[i];
    DrawAttribute drawAtt;
    drawAtt.x = x;
    drawAtt.y = y;
    drawAtt.enlarge =4;
    while ( ch !=0 )
    {

        PSP_ChineseUtil::drawAscChar8_16 ( drawAtt , color,   ch  );
        i++;
        drawAtt.x += DotFont16ASCfont_width * 4;
        if ( drawAtt.x >= PSP_SCREEN_WIDTH_Visible_cg ) 
        {
            drawAtt.x = 0;
            if( y >= PSP_SCREEN_HEIGH_cg)
            {   
                // getting out off the Y border.
                // don't draw any more
                break;
            }
            drawAtt.y += DotFont16ASCfont_heigh * 4 ;
        }
        ch = str[i];		
    }

}                                        


/*
void  PSP_GRAPHICS:: drawHanZi( unsigned short x, unsigned short y, char * hanZi2Byte, 
unsigned short color    ) 
{

drawHanZiEnlarge (  x,  y,  hanZi2Byte,  color, 1  );

}                       
*/


void PSP_GRAPHICS::drawBitMapTurn (  const DrawAttribute &drawAtt, 
                                   unsigned short color,                
                                   const ucharBitMap & ucharbitmap   )
{
    PSP_GRAPHICS::graphicBitBltEnlarge_CheckBorderTurn( drawAtt, ucharbitmap,  color );



}        

const ushortBitMap * PSP_GRAPHICS:: getFolderIconBitMap(  void  )
{
    return &folderBitMap;

}


const ushortBitMap * PSP_GRAPHICS::getIconBitMap( const unsigned char * fileExt )
{

    if ( fileExt == NULL )
    {
        return & normalFileBitMap;
    }
    if ( strlen ( ( char *)fileExt ) <= 0 )
    {
        return & normalFileBitMap;
    }

    const ushortBitMap *tempBmp; 
    CommonString fileExtStr ( fileExt );
    fileExtStr.toLowerCase();
    if( fileExtStr == textFileExt ) 
    {
        tempBmp = &textFileBitMap;  

    }else if ( fileExtStr == htmlFileExt )
    {
        tempBmp = & htmlBitMap;

    } else if ( fileExtStr == htmFileExt )
    {
        tempBmp = & htmlBitMap;

    }else if (  fileExtStr == mp3FileExt )
    {
        tempBmp = &mp3BitMap;
    }
    else if ( fileExtStr == winRarFileExt )
    {
        tempBmp = &winRarBitMap;
    }    
    else if ( fileExtStr == winZipFileExt )
    {
        // winZip and winrar file use same icon
        tempBmp = & winZipBitMap;
    }
    else if ( fileExtStr == ttfFileExt || fileExtStr == ttcFileExt )
    {
        // winZip and winrar file use same icon
        tempBmp = & trueTypeFontBitMap;
    }
    
    else 
    {
        tempBmp = & normalFileBitMap;
    }

    return tempBmp ;

}


/**
so far only capture the whole screen.
*/ 
void PSP_GRAPHICS::getScreenImageToBitMap (  ushortBitMap & bitmap   )
{
     unsigned long   yi, xi , width, heigh ;
     unsigned short  *vram_p;
     bitmap.width = PSP_SCREEN_WIDTH_Visible_cg ;
     width = bitmap.width ;
     
     bitmap.heigh = PSP_SCREEN_HEIGH_cg;
     heigh = bitmap.heigh ;
     if ( bitmap.data_p == 0 ) //delete bitmap.data_p ;
     bitmap.data_p = new unsigned short [ width * heigh  ];
     
     
     
            
     for ( yi = 0; yi < heigh; yi ++ )
     {   
         vram_p = PSP_GRAPHICS::getVRamAddrFrontBuffer ( 0, yi );
         for ( xi = 0; xi < width;xi++ )
         {       
     
            bitmap.data_p[ yi*width + xi ] = * vram_p  ;
            vram_p ++;
            // 2* is because they are unsigned short 2 bytes, memcpy use char
            //(  ( void* )& bitmap.data_p[ yi* width ],  ( void* )vram_p ,  2* width );
         }
    // memset (  ( void* ) bitmap.data_p,  0xff0 ,  width * heigh* 2 );
     }
}









void  PSP_GRAPHICS:: copyFrontBufferToBackBuffer( void )
{


}




void PSP_GRAPHICS::selfTestGetImage ( void )
{

   PSP_GRAPHICS::setScreenFrameMode( screenModeWriteAndShow_cg );
   unsigned short width = 220 , heigh = 165;
   ushortBitMap bitmap;
   bitmap.width = width ;
   bitmap.heigh = heigh;   
   bitmap.data_p = new unsigned short [width* heigh];
   
    ushortBitMap bmpKitty;
    bmpKitty.width = 220;
    bmpKitty.heigh = 165;
    bmpKitty.data_p = (unsigned short *)kittycData ;
       
    DrawAttribute drawAtt;
    drawAtt.enlarge = 2;
    drawAtt.x = 1;
    drawAtt.y = 1;
    PSP_GRAPHICS::drawBitMap( drawAtt, bmpKitty );
    
    PSP_GRAPHICS::getScreenImageToBitMap (  bitmap );
   getch();
    PSP_GRAPHICS::fillVRam ( 0 );
   getch(); 
   drawAtt.x = 50;
   drawAtt.y = 100;
   drawAtt.enlarge = 1;
   
    PSP_GRAPHICS::drawBitMap(  drawAtt, bitmap);
   getch();
    delete bitmap.data_p;

}
/*
void PSP_GRAPHICS::selfTestPrintStringABC_8_16 ( void )
{
    pspDebugScreenInit();
    pspDebugScreenPrintf("Graphic testing: selfTestPrintStringABC_8_16( ) ;  \n");   

    setScreenFrameMode(  screenModeWriteAndShow_cg );

    graphicPrintAscString(0,0,0xffff,   "0123456789");
    getch();
    getch();
    graphicPrintAscString(0,20,0xffff,   "~~~~~");
    getch();
    getch();
    const char ch[] = {1,2, 126, 127, 128, 129, 0};
    graphicPrintAscString(0,20,0xffff,   ch);
    getch();
    getch();
    graphicPrintAscString(0,40,0xffff,   "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    getch();  

    graphicPrintAscString(0, 60, 0xffff, "abcdefghijklmnopqrstuvwxyz");
    getch();   

    graphicPrintAscString(0, 80  , 0xffff, "~!@#$%^&*()_+|-=\[]{}\";:?/<>,.\'");
    getch();   
    getch();   

    fillVRam ( 0 );
    graphicPrintAscString2(0, 0 ,0xffff,   "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    getch();   
    graphicPrintAscString2(0,0,0xAAAA,   "0123456789");
    getch();   
    graphicPrintAscString2(0, PSP_ASCFONTS8_16::heigh*2 +1  , 0xcccc, "abcdefghijklmnopqrstuvwxyz");
    getch();   

    graphicPrintAscString2(0,  (PSP_ASCFONTS8_16::heigh+1 )*3  , 0xffff, "~!@#$%^&*()_+|-=\[]{}\";:?/<>,.\'");
    getch();
    getch();

    fillVRam ( 0 );

    graphicPrintAscString4(0,0,0xAAAA,   "0123456789");
    getch();   
    graphicPrintAscString4 (0,0 ,0xffff,   "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    getch();   

    graphicPrintAscString4 (0,   (PSP_ASCFONTS8_16::heigh+1) * 4, 0xffff, "abcdefghijklmnopqrstuvwxyz");
    getch();   

    graphicPrintAscString4 (0 ,  (PSP_ASCFONTS8_16::heigh+1) * 8   , 0xffff, "~!@#$%^&*()_+|-=\[]{}\";:?/<>,.\'");
    getch();
    getch();


}

*/
void PSP_GRAPHICS::selfTestDrawRect ( void )
{
    Rectangle rec;
    rec.x = 70; rec.y=50; rec.width=300; rec.heigh = 120;
    //PSP_Window win( rec );
    //win.setColor( 0xffff, 0x9999);
    //win.show();
    drawRectangle ( rec, 0xaaaa);
    fillColorRect ( rec, 0x5555 );	


}

/*
void PSP_GRAPHICS::selfTestAll ( void )
{
    pspDebugScreenInit();


    selfTestPrintStringABC_8_16();
    getch();
    pspDebugScreenPrintf("Graphic testing: selfTestSingleFrame( );  \n");   
    getch();    
    selfTestSingleFrame( );    

    pspDebugScreenPrintf("Graphic testing: selfTestDoubleFrame(); \n");   
    getch();   
    selfTestDoubleFrame();

    pspDebugScreenPrintf("Graphic testing: selfTestDoubleFrameMove();\n");
    getch();   
    selfTestDoubleFrameMove();

    pspDebugScreenPrintf("Graphic testing: selfTestDoubleFrameFlip();;\n");
    getch();   
    selfTestDoubleFrameFlip();


}



*/











