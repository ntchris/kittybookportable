


#include <PSP_Window.h>

 
#include <PSP_GRAPHICS.h>
#include <PSP_ChineseUtil.h>
#include <commonstring.h>
#include <PSP_Global.h>
#include <PSP_TrueTypeFont.h>

//Window, to contain information like Text,bmp.
// Contents will not draw outside the window.
 




#ifdef __PSPSDK__
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#endif



PSP_Window::PSP_Window ( const Rectangle &rec ) 
{  
   this->rectangle.x = rec.x; 
   this->rectangle.y = rec.y;
   this->rectangle.width = rec.width;
   this->rectangle.heigh = rec.heigh;
   
   backgroudColor = colorNoColor;
   showWindowBorder = true;
   fontColor = 1;
      
   this->drawAttWin.limitedRec_p   = & this->rectangle;
   ifAllowWrapLine = false;
   spaceSpareLineEnd = 6;
}



PSP_Window::PSP_Window (  ) 
{  
   
   this->rectangle.x = 0; 
   this->rectangle.y = 0;
   this->rectangle.width = PSP_SCREEN_WIDTH_Visible_cg;
   this->rectangle.heigh = PSP_SCREEN_HEIGH_cg;
   
   backgroudColor = colorNoColor;
   showWindowBorder = true;
   fontColor = 1;
   
   this->drawAttWin.limitedRec_p   = & this->rectangle;
   ifAllowWrapLine = false;
   spaceSpareLineEnd = 6;
}



void PSP_Window::setRectangle( unsigned short x, unsigned short y, 
                               unsigned short wid, unsigned short heigh  )
{
    this->rectangle.x = x;
    this->rectangle.y = y;
    this->rectangle.width = wid;
    this->rectangle.heigh = heigh;

}


unsigned short PSP_Window::getWindowHigh( void )
{
    return this->rectangle.heigh;

}


void PSP_Window::getRectangle ( Rectangle *rec ) const
{

   rec->x  = this->rectangle.x ;
   rec->y  = this->rectangle.y ; 
   rec->width  = this->rectangle.width ; 
   rec->heigh  = this->rectangle.heigh ; 
   
   

}


void PSP_Window::setColor ( unsigned short bgColor   , unsigned short _fontColor  )
{
    this->backgroudColor = bgColor ;
    
    if(  _fontColor )
    {  this->fontColor = _fontColor;
    }
    

}

#ifdef __PSPSDK__
void PSP_Window::showWindow( void )
{


    PSP_GRAPHICS::fillColorRect ( rectangle, this->backgroudColor );
    
    if( this->showWindowBorder )
    {
      
        
        PSP_GRAPHICS::drawHorizonLine 
            ( rectangle.x , rectangle.x + rectangle.width,  rectangle.y, this->borderColor1 );
          
        PSP_GRAPHICS::drawHorizonLine 
            ( rectangle.x , rectangle.x + rectangle.width,  rectangle.y + rectangle.heigh, this->borderColor2 );
          

        PSP_GRAPHICS:: drawVerticalLine 
            ( rectangle.y , rectangle.y + rectangle.heigh, rectangle.x, this->borderColor1 );
          
        PSP_GRAPHICS:: drawVerticalLine
            ( rectangle.y , rectangle.y + rectangle.heigh, rectangle.x + rectangle.width, this->borderColor2 );
          

    }else
    {
       
   
    } 
   
  

}

#endif


void PSP_Window::drawBitMap ( unsigned short winX, unsigned short winY, const ushortBitMap & bitmap )
{
     
      //DrawAttribute drawAtt;
      drawAttWin.x = winX + this->rectangle.x + borlderLine;
      drawAttWin.y = winY + this->rectangle.y + borlderLine; 
      //restric the bmp inside the window    
#ifdef __PSPSDK__
      PSP_GRAPHICS::drawBitMap ( drawAttWin, bitmap );
#else
      
#endif      	
     

}




void PSP_Window::drawBitMap ( unsigned short winX, unsigned short winY, unsigned short color, const ucharBitMap & bitmap )
{
     //if ( this->isInsideWindow( winX + bitmap.width + borlderLine  , winY + bitmap.heigh + borlderLine) )
         
         //DrawAttribute drawAtt;
         drawAttWin.x = winX + this->rectangle.x + borlderLine;
         drawAttWin.y = winY + this->rectangle.y + borlderLine; 
         //restric the bmp inside the window    
#ifdef __PSPSDK__         
         PSP_GRAPHICS::drawBitMap ( drawAttWin, color,   bitmap );
#endif
      

}


#ifdef __PSPSDK__ 

void PSP_Window::drawTTFHanZiWindow ( unsigned short winX, unsigned short winY,  
                                     unsigned short color,  const HanZi &hanzi, BitMapSize *bmpsize ) 
{
    
    //if ( this->isInsideWindow( winX + hanZiFontWidth+ borlderLine , winY + hanZiFontHeigh+ borlderLine ))
     
     //   DrawAttribute drawAtt;
        drawAttWin.x = winX + this->rectangle.x + borlderLine;
        drawAttWin.y = winY + this->rectangle.y + borlderLine;
        drawAttWin.enlarge = 1;
        //drawAtt is physical screen x y
        
        PSP_ChineseUtil::drawTTFHanzi ( drawAttWin, color,  hanzi , bmpsize );

}



#endif



void PSP_Window ::setTitle ( const unsigned char * _title )
{
   this->title.setString( _title);

}

void PSP_Window ::drawTitle ( void )
{
    unsigned winX, winY;
   
    
    winX = 5;
    winY = 1;
    
    PSP_Window::printAscHan16ZiString( winX, winY, 0, (char *) this->title.getChar_p_readonly() );

}
  
  
  
unsigned short  PSP_Window ::getMaxItemCountInWindow ( void )
{
   unsigned short maxItemCount1Window ; 
   
   maxItemCount1Window = ( this->rectangle.heigh - titleHeigh  )/  // space available
                     ( DotFont16ASCfont_heigh + spaceBetweenLine );//one item
   // probably draw one on border ..... faint
   maxItemCount1Window += 1;
   
   return maxItemCount1Window;
}  


 
#ifdef __PSPSDK__

void PSP_Window ::showMessage( const    char * textMessage , int value )
{
	
	char *msg, number [ 10];
	
	int len ;
	len = strlen( textMessage );
	msg = new char [ len + 10 ];
	msg[0]=0;
	number[0]=0;
	
	sprintf ( number, " %d ", value  );
	
	strcat( msg, textMessage );
	strcat( msg, number );
	showMessage( msg );
	delete msg;
	
}

void PSP_Window ::showMessage( const  char * textMessage )
{

    ushortBitMap wholeScreenBMP;
    wholeScreenBMP.width = PSP_SCREEN_WIDTH_Visible_cg ;
    wholeScreenBMP.heigh = PSP_SCREEN_HEIGH_cg ;
    unsigned int wholeScreenLen;
    wholeScreenLen = wholeScreenBMP.width ;
    wholeScreenLen = wholeScreenLen * wholeScreenBMP.heigh ;

    wholeScreenBMP.data_p = new unsigned short [ wholeScreenLen  ];

    if ( wholeScreenBMP.data_p  == NULL )
    {
        pspDebugScreenInit();
        pspDebugScreenPrintf( "Allocate memory failed size: %d\n" ,wholeScreenLen  );

        getch();
        return ;

    }else
    { 
        PSP_GRAPHICS::getScreenImageToBitMap(   wholeScreenBMP );
    }
    
    DrawAttribute drawAttr;
    drawAttr.x = 0; drawAttr.y = 0;
    
    //must set this, otherwise  the backgroup zero can not be restored!!
    drawAttr.drawBackGroudZero = true;
    Rectangle messageWinRec( 90, 61, 300, 150 );
    PSP_Window msgWin( messageWinRec );
    msgWin.ifAllowWrapLine = true;
    msgWin.setColor ( colorLightBlue );

    if ( PSP_GRAPHICS::getScreenMode () == screenModeWriteAndBuffered_cg )
    {  
        //get the whole image A of front screen.
        //draw to back buffer
        //draw the message too.
        //swap()
        //put back the whole image A.
        //PSP_GRAPHICS::fillVRam(0xfff0);
        PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP );
        msgWin.showWindow();
        msgWin.printAscHan16ZiString( 5,5,0 , ( char *) textMessage );
        PSP_GRAPHICS::flipScreenBuffer();
        
        getch();
        
        //PSP_GRAPHICS::drawBitMap( drawAtt, wholeScreenBMP );
        //no need to draw back because the front screen already has the old content.
        
        PSP_GRAPHICS::flipScreenBuffer();               
        PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP );
        
        
        
      

    }else  // write and show mode.
    {
        msgWin.showWindow();
        msgWin.printAscHan16ZiString( 5,5, 0, textMessage );
        getch();
        //restore the old screen
        PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP );

    } 
   
      
    delete  wholeScreenBMP.data_p;
    return ;

}


 void  PSP_Window :: selfTest ( ) 
 {
     
     this->printAscHan16ZiString ( 0 , 0 , 0x23, "A"   );
     getch();
     this->printAscHan16ZiString ( 0, 0 , 0xeeee,
     "测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋\
GCD紫光测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测试的中文句子ABCDEFG测试测试再测试换\
行看行不行火煋GCD紫光AAAAAABBBB测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测试的中文句子\
ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测\
试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测试的中文句子ABCDEFG测试测试再测试换行看行不行火\
煋GCD紫光测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光?"   ); 
  
  getch();
  
  
 }
 

                     
extern PSP_TrueTypeFont * pspTrueTypeFont_cgp ;
 void  PSP_Window :: selfTestTTF ( ) 
 {
    


 
    int r = pspTrueTypeFont_cgp ->loadFontFile("ms0:/simsun.ttc");
    //pspTrueTypeFont_cgp  ->setTTFFontSize ( 20  );
    if( !r )
    {
         
         PSP_Window::showMessage("TrueTypeFont字库文件读取失败 请检查");
         PSP_GRAPHICS::flipScreenBuffer( );
         getch();
    }
     this->ifAllowWrapLine = true;
     this->printTTFHanZiString (  0, 0, 0xffff, (const unsigned char *)"测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋\
GCD紫光测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测试的中文句子ABCDEFG测试测试再测试换\
行看行不行火煋GCD紫光AAAAAABBBB测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测试的中文句子\
ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测\
试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光测试的中文句子ABCDEFG测试测试再测试换行看行不行火\
煋GCD紫光测试的中文句子ABCDEFG测试测试再测试换行看行不行火煋GCD紫光?"); 
  
  getch();
  
  
 }
 
#endif 
                          




bool PSP_Window::isInsideWindow( unsigned short winX, unsigned short winY )
{
      if ( winX    >= this->rectangle.width )
      return false;
      
      if ( winY   >= this->rectangle.heigh )
      return false;
      
      return true;

}

bool PSP_Window::isInsideWindow( const Rectangle &rec  )
{
      if ( (rec.x + rec.width)  >= this->rectangle.width )
      return false;
      
            
      
      if ( rec.y + rec.heigh  >= this->rectangle.heigh )
      return false;
      
      return true;
 
}
 
 
bool PSP_Window::wrapLine ( unsigned short &winX , unsigned short &winY, 
                            unsigned short charSize, unsigned short lineHeigh  )
{
    if ( ( winX + charSize + spaceSpareLineEnd )>= this->rectangle.width )
    {
        
        if( !this->ifAllowWrapLine )
        {
            return false;  
        }
        
        winY += lineHeigh + spaceBetweenLine;
        winX =  this->winXStart;
        
    }
    return  isInsideWindow( winX, winY  ) ;
    

}

bool PSP_Window::newLine (  unsigned short &winX , unsigned short &winY, 
                   unsigned short lineHeigh                     )
{
 
   if( !this->ifAllowWrapLine  )
   return false;
   
   winY += lineHeigh + spaceBetweenLine;
   winX = 0;
   return  isInsideWindow( winX, winY + lineHeigh ) ;
  
}




     
 




unsigned long PSP_Window::printTTFHanZiString
( unsigned short winx, unsigned short winy, 
 unsigned short color, const unsigned char *str ,
 unsigned long bufferSize ) 
{

    ucharBitMap * ucharbitmap = 0;


    //BitMapSize bmpsize;

    unsigned short winX, winY; 
    if( str == NULL) 
    {  
        return 0;
    }
    //The whole string( to show) size.
    unsigned long size = bufferSize;//CommonString::getStringSize( str ); 

    //use this to proceed str[ index ];
    unsigned long index = 0 ,  charCount = 0;;

    if (size <=0 ) return 0;
    bool r;
    HanZi tempHZ;
    winY = winy ;
    winX = winx;
    unsigned char  ch;
    while(true)
    {         

        //eg. if size =5, then index can be 4 at largest
        if( ( index + 1 ) > size  ) break;
        ch = str[index];
        if  ( index + 2 <= size )
        {         
            bool isHanZi;
            isHanZi = PSP_ChineseUtil::isThisAHanZi ( ch  ); 
            if (  isHanZi )    
            {   
                tempHZ.ch[0]=str[index];
                tempHZ.ch[1]=str[index+1];            
                ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap ( tempHZ   );

                r = this->wrapLine ( winX, winY, ucharbitmap->width , ucharbitmap->heigh );
                if ( !r )
                {                
                    break;
                }
#ifdef __PSPSDK__                
                this->drawBitMap( winX, winY, color,   *ucharbitmap );         
#else
                PSP_ChineseUtil:: printBitMapInTextMode ( *ucharbitmap ); 
#endif                

                winX += ucharbitmap->width + spaceBetweenChar;
                charCount = index;  
                index += 2; 
                continue;                
            }
        }
        //This is only a ASC char.   
        if ( ch =='\n' || ch =='\r' )
        {  
            index ++;
            charCount = index;
            if ( !this->ifAllowWrapLine )
            {
                break;
            }
            r = newLine ( winX, winY, DotFont16ASCfont_heigh );
            if( !r )
            {
                break;
            }
            
            

        }
        else if ( ch == ' ' )
        {
            unsigned short fontSize;
            fontSize = PSP_ChineseUtil::getTTFSize();
            r = wrapLine ( winX, winY,  fontSize, fontSize  );
            if ( !r )
            {  //window full
                break;
            } 
            charCount = index;
            index ++;           
            winX += fontSize   ;
        }
        else 
        {
            unsigned short englishWordLen ;
            englishWordLen =  CommonString::getEnglishWordLen ( (unsigned char*) str + index  );
            if ( englishWordLen == 0 )
            {
                englishWordLen  = 1;
            }
            ch  = str [ index ];
            HanZi hanzi ;
            hanzi.ushort = ch;
                
            ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap (  hanzi   );
            r = wrapLine ( winX, winY, ( ucharbitmap->width + spaceBetweenChar )* englishWordLen,
                 ucharbitmap->heigh );
            if ( !r )
            {  //window full
                break;
            }    
            unsigned short wordCounter = 0;
            do
            {              
                hanzi.ushort = str [ index ];

                //this->drawAscChar8_16Window( winX, winY, color, ch  );
                ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap (  hanzi   );
#ifdef __PSPSDK__           
                this->drawBitMap( winX, winY, color,   *ucharbitmap );         
#else
                PSP_ChineseUtil:: printBitMapInTextMode ( *ucharbitmap ); 
#endif           

                index ++;//= englishWordLen;                 
                winX += ( ucharbitmap->width + spaceBetweenChar ); 
                wordCounter ++;              
                if ( wordCounter >= englishWordLen ) break;

            }while ( true );
            charCount = index; 


        }   
    } 

    return charCount ;



}




//Print Chinese or ASC chars.
//==========================================================================
 //  print Asc (pure english/1234 ? #*$) and Chinese HanZi in graphic mode.
 //  return the how many char has been processed.
 //  this method should be moved to Windwos class.
 //========================================================================== 
unsigned long PSP_Window::printAscHan16ZiString( unsigned short winx, unsigned short winy ,unsigned short color , 
                                            const char *str,  unsigned long bufferSize    )
{   
    unsigned short winX, winY ;
    if( str == NULL) 
    {  
        return 0;
    }
    
    if ( color == 0 )
    {
       color = this->fontColor;
    }
    	
    //The whole string( to show) size.
    unsigned long size ;
    if ( bufferSize == 0 ) 
    {
       size = CommonString::getStringSize( (unsigned char*) str ); 
    }else 
    {
       size = bufferSize;
    } 
   
    //use this to proceed str[ index ];
    unsigned long index = 0 , charCount = 0;
  
    if (size <=0 ) return 0;
    bool r;
    HanZi tempHZ;
     winY = winy ;
     winX = winx;
     char  ch;
    while(true)
    {         
        //winX  = ( index * ASCfont_width ) * ( spaceBetweenChar + PSP_ASCFONTS8_16::width ) ;       
        // drawAtt.x = winX + this->rectangle.x ;
        //drawAtt.y = winY + this->rectangle.y ;
        //eg. if size =5, then index can be 4 at largest
        if( ( index + 1 ) > size  ) break;
        ch = str[index];
        if  ( index + 2 <= size )
        {         
            bool isHanZi;
            isHanZi = PSP_ChineseUtil::isThisAHanZi ( ch  ); 
            if (  isHanZi )    
            {   
                tempHZ.ch[0]=str[index];
                tempHZ.ch[1]=str[index+1];            
                r = this->wrapLine ( winX, winY, dotFontHanZiFontWidth , DotFont16ASCfont_heigh );
                if ( !r )
                {                
                    break;
                }
                this->drawHanZi16Window ( winX, winY ,color, tempHZ  );
                winX += dotFontHanZiFontWidth + spaceBetweenChar;
                charCount = index;  
                index += 2; 
                continue;                
            }
        }
        //This is only a ASC char.   
        if ( ch =='\n' || ch =='\r' )
        {             
           index ++;
           charCount = index;
           r = newLine ( winX, winY, DotFont16ASCfont_heigh );
           if( !r )
           {
              break;
           }
           
        }else   
        {
          unsigned short englishWordLen ;
          englishWordLen =  CommonString::getEnglishWordLen ( (unsigned char*) str + index  );
          //this is NOT a english word, so It just takes ONE ASC.
          if ( englishWordLen == 0 )
          {
              englishWordLen  = 1;
          }
 
          r = wrapLine ( winX, winY, DotFont16ASCfont_width *  englishWordLen ,
                         DotFont16ASCfont_heigh );
           if ( !r )
           {  //window full
              break;
           }    
           //draw the whole word  !!
           unsigned short wordCounter = 0;
           do
           {              
               ch  = str [ index ];
               this->drawAscChar8_16Window( winX, winY, color, ch  );
               index ++;//= englishWordLen;                 
               winX += ( DotFont16ASCfont_width + spaceBetweenChar ); 
               wordCounter ++;              
               if ( wordCounter >= englishWordLen ) break;
               
           }while ( true );
           charCount = index;
        }   
    }//  while(true)

    return charCount ;
}
 











void PSP_Window::drawAscChar8_16Window 
( unsigned short winX, unsigned short winY,  
  unsigned short color, char ch ) 
{
        drawAttWin.x = winX + this->rectangle.x + borlderLine;
        drawAttWin.y = winY + this->rectangle.y + borlderLine;
        drawAttWin.enlarge = 1;
        //drawAtt is physical screen x y
        //PSP_GRAPHICS::drawAscChar8_16 (drawAttWin , color, ch ); 
        PSP_ChineseUtil::drawAscChar8_16 (drawAttWin , color, ch ); 
   
}

void PSP_Window::drawHanZi16Window ( unsigned short winX, unsigned short winY,  
                                     unsigned short color, const HanZi &hanzi) 
{
    
    //if ( this->isInsideWindow( winX + hanZiFontWidth+ borlderLine , winY + hanZiFontHeigh+ borlderLine ))
     
     //   DrawAttribute drawAtt;
        drawAttWin.x = winX + this->rectangle.x + borlderLine;
        drawAttWin.y = winY + this->rectangle.y + borlderLine;
        drawAttWin.enlarge = 1;
        //drawAtt is physical screen x y
        PSP_ChineseUtil::drawHanZi16 (drawAttWin , color, hanzi ); 
    

}



unsigned short PSP_Window::getBackGroundColor( void )
{
     return this->backgroudColor;
}


