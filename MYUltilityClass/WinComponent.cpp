


#include <PSP_Global.h>


#include <WinComponent.h>
#include <stdio.h>


//===========================================================================

RGBColor Win_Component ::focusBarRGBColor;

Win_Component ::Win_Component (  PSP_Window * win, 
                               unsigned short winx, unsigned short winy )
{
    Rectangle rec;
    win->getRectangle ( &rec );
    x  = rec.x +  winx;
    y  = rec.y + winy;
    
    window_parent = win;
    this->isFocused = false;
    focusBarRGBColor.setColor ( 30, 30, 30 );
    onClick_p = 0;
}

void Win_Component ::setFocused ( bool isFoc)
{
    this->isFocused  = isFoc;

}


bool Win_Component :: getIsFocused( void )
{
    return this->isFocused;

}


void  Win_Component  ::setFocusedBarSize ( unsigned short _width, unsigned short _heigh )
{
    // focus bar inside the btn
    this->recFocused.x = x + 2 ;
    this->recFocused.y = y + 2;
    this->recFocused.width = _width;
    this->recFocused.heigh = _heigh;
}    



void Win_Component ::setAction( void (*action_p)(void) )
{
    this->onClick_p  = action_p;
}

void Win_Component ::onClick ( void )
{
    if( onClick_p ) this->onClick_p();

}



//===========================================================================

Label  ::Label ( PSP_Window * win, unsigned short winx, unsigned short winy ):
Win_Component( win, winx, winy )
{

    color = 0;

}                    



const unsigned char * Label::getText( void ) const
{
     return this->text.getChar_p_readonly();
}


void Label::setColor ( unsigned short _color )
{
   this->color = _color;
   
}


void Label::show( void )
{
    window_parent ->printAscHan16ZiString
        ( x , y, color, (char*)text.getChar_p_readonly() );

}

void Label::setText( const char * textp )
{
    this->text.setString ( (const unsigned char*)textp );

}





//===========================================================================

 
 
 Button ::Button (  PSP_Window * win, unsigned short winx, unsigned short winy ):
                    Win_Component( win, winx, winy )
{
    enabled = true; 
     
    winBotton.setColor ( defaultButtonColor , 0xffff); 
    
     
    
}


Button ::~Button ()
{

      
}
    

bool Button ::getEnabled ( void )
{
   return enabled;
}


void Button ::setEnabled ( bool enable   )
{
    enabled = enable;
    if( enable) 
    {
       this->winBotton.setColor( defaultButtonColor );
    }else
    {
       this->winBotton.setColor( diabledButtonColor );
    
    }   
    

}


void Button ::setCaption( const char * text_p )
{
    unsigned short width, heigh, space = 8 , charLen;
    const unsigned short maxLen = 15;
    this->text = text_p;            
    heigh = 30;
    
    charLen = this->text.getSize();
    if( charLen > maxLen ) charLen = maxLen;
    width = space +  charLen * 10  + space ;
    
    winBotton.setRectangle( this->x , this->y , width , heigh  );
    setFocusedBarSize ( width - 3 , heigh - 3 ) ;

}


void Button :: show( void )
{


    winBotton.showWindow();
    winBotton.printAscHan16ZiString 
        ( 8, 4,0, (char*) this->text.getChar_p_readonly() );

    if( this->isFocused )    
    {
#ifdef  __PSPSDK__

        PSP_GRAPHICS:: drawRectangle( this->recFocused, this->focusBarRGBColor.getColor()  );  
#endif       

        
    }


}
 


//===========================================================================



NumberBox :: NumberBox (  PSP_Window * win,  unsigned short winx, unsigned short winy )
  : Win_Component( win, winx, winy )
{
   
    winBox.setColor( 0xffff, 1 );  
    
    
    this->setAction( showHelpText );
    label = new Label( win, x + headerX, y + 1 );
    label->setText ( "text");
    

}


void NumberBox ::setLabelColor( unsigned short color)
{
     this->label->setColor( color );

}




void NumberBox ::setText( char* text )
{
     static const unsigned short space = 5 , headerY = 3; 
     unsigned short textWidth  ;
     textWidth = (  DotFont16ASCfont_width +spaceBetweenChar )*strlen(text) ;
     
     this->label->setText ( text );
     this->winBox.setRectangle(  x + textWidth + space +headerX,
                                 y + headerY , numboxWidth , numboxHeigh );
    this->setFocusedBarSize( textWidth+space+ numboxWidth + headerX, numboxHeigh+2 );

}

void NumberBox ::showHelpText( void )
{
    PSP_Window::showMessage(" use Left/Right to change value.\n\n 请按[左]和[右]键来改变该值. ^_^");

}


NumberBox ::  ~NumberBox( )
{


}



void NumberBox::setValue ( unsigned char _value )
{
   this->value = _value ;

}

void NumberBox::increase( void )
{
    if ( value < maxValue ) value ++;

}


void NumberBox::decrease( void )
{
    if ( value > 0 ) value --;

}
     
void NumberBox::show( void )
{
    this->winBox.showWindow();
    this->label->show();
    
    char msg[5];
    sprintf( msg, "%u", value);
    this->winBox.printAscHan16ZiString( 5,2, 1, msg );
    
    
    if( this->isFocused )    
    {
#ifdef  __PSPSDK__

        PSP_GRAPHICS:: drawRectangle( this->recFocused, this->focusBarRGBColor.getColor()  );  
#endif              
    }else
    {
        PSP_GRAPHICS:: drawRectangle( this->recFocused, this->window_parent->getBackGroundColor() );
        //this->focusBarRGBColor.getColor()  );  
    }

}

unsigned char NumberBox::getValue( void )
{
    return this->value;
}

//===========================================================================




BoolBox::~BoolBox( )
{


}




BoolBox::BoolBox(  PSP_Window * win, 
                 unsigned short winx, unsigned short winy ):
Win_Component( win, winx, winy )
{

    this->selected = true;
    this->bmpSelected.data_p =  ( unsigned short *)bmpSelData;
    this->bmpSelected.width = 16;
    this->bmpSelected.heigh = 16;

    this->bmpNoSelected.data_p = ( unsigned short *) bmpNoSelData;
    this->bmpNoSelected.width = 16;
    this->bmpNoSelected.heigh = 16;

}


void BoolBox:: updateBoxBMP( void )
{
    if( selected )
    {
        window_parent-> drawBitMap ( x, y, bmpSelected  );
    }
    else
    {   
        window_parent->drawBitMap ( x, y, bmpNoSelected );
    }
}   


void BoolBox::show( void )
{

    updateBoxBMP();    
    window_parent->printAscHan16ZiString
        ( x + this-> bmpSelected.width +3 , this->y, 0, //window->fontColor,      
        (char*)text.getChar_p_readonly() );


    if( this->isFocused )    
    {
#ifdef  __PSPSDK__

        PSP_GRAPHICS:: drawRectangle( this->recFocused, this->focusBarRGBColor.getColor()  );  
#endif       

        
    }

}


void BoolBox:: setLabel( const char * labelText )
{
    text.setString ( (const unsigned char *) labelText );
    setFocusedBarSize ( 20 + strlen( labelText)*10 , 25 );

}







const unsigned short BoolBox:: bmpSelData[]=
{
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 
      0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x2108, 0x2108, 0x2108, 0x2108, 0x2108, 0x2108, 0x2108, 0x2108,
      0x2108, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff,
      0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x2084, 0x1800, 0x2084, 0x6318, 0x7fff, 0x0, 0x0, 0x0,
      0x4210, 0x2108, 0x7fff, 0x2084, 0x7fff, 0x7fff, 0x7fff, 0x2084, 0x1800, 0x1800, 0x2084, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x1800, 0x2084, 0x7fff, 0x2084, 0x1800, 0x1800, 0x1800, 0x2084, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x1800, 0x1800, 0x2084, 0x1800, 0x1800, 0x1800, 0x2084, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x1800, 0x1800, 0x1800, 0x1800, 0x1800, 0x2084, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x2084, 0x1800, 0x1800, 0x1800, 0x2084, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x2084, 0x1800, 0x2084, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 0x2084, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};
 

const unsigned short BoolBox:: bmpNoSelData[]=
{
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210,
    0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x2108, 0x2108, 0x2108, 
    0x2108, 0x2108, 0x2108, 0x2108, 0x2108, 0x2108, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x2108, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x4210, 0x6318, 0x6318, 0x6318, 0x6318,
    0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x6318, 0x7fff, 0x0, 0x0, 0x0, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 
    0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0       
};








ScrollBar::ScrollBar( const PSP_Window * _win )
{
	win = _win;
	Rectangle rec;
	win->getRectangle( &rec );
	
	recBackGnd.x = rec.x + rec.width - ( backGndWidth + 2 )  ;
	recBackGnd.y = rec.y;
	recBackGnd.width = backGndWidth;	
	recBackGnd.heigh = rec.heigh;
	
	recBar.width = backGndWidth;
	recBar.heigh = barHeigh;
	percent = 0;
	
	
}

void ScrollBar::showScrollBar ( unsigned percent_ )
{
	percent = percent_;
	showScrollBar ();
}

void ScrollBar::showScrollBar ( void )
{
    //unsigned short x , y;
    Rectangle rec;
    
	win->getRectangle( &rec );
	
    recBackGnd.x = rec.x + rec.width - ( backGndWidth  )  ;
    
    
    
    recBackGnd.y = rec.y;        
    
    recBackGnd.width = backGndWidth;	
	recBackGnd.heigh = rec.heigh;	
	
	
	
	recBar.x = recBackGnd.x ;//rec.x + rec.width - ( backGndWidth +1 );
	unsigned long temp;
	temp = rec.heigh ;
	
	recBar.y = rec.y +  temp * percent/100;
	
	PSP_GRAPHICS:: drawRectangle ( recBackGnd , backGndColor );
	PSP_GRAPHICS:: drawRectangle ( recBar , barColor  );
	PSP_GRAPHICS:: fillColorRect ( recBar , barColor );
    
	
		
}



void ScrollBar::setPercent ( unsigned long a, unsigned long b )
{
	percent = a*100 / b ;

		
}

unsigned short ScrollBar::getPercent ( void )
{
	return percent;
}

ScrollBar::~ScrollBar()
{
	
}




//===========================================================================================

/*
bool ConfirmWindow::inited = false;
Button * ConfirmWindow::btn_Yes , *ConfirmWindow::btn_No;
*/

ConfirmWindow:: ConfirmWindow ( )
{
    
    const static unsigned short int btn_winY = 95;
    this->setRectangle ( 80, 50, 300, 150 );
    this->setColor ( colorLightBlue );
    btn_Yes = new Button(  this , 40, btn_winY );
    btn_No = new Button( this , 200, btn_winY );    
    addComopnents();
    wholeScreenBMP.width = PSP_SCREEN_WIDTH_Visible_cg;
    wholeScreenBMP.heigh = PSP_SCREEN_HEIGH_cg ;
    this->ifAllowWrapLine = true; 
   
    // ============================================================
    // allocate space for screen backup
    unsigned long wholeScreenLen, temp;
    wholeScreenLen = wholeScreenBMP.width ;
    temp = wholeScreenBMP.heigh ;
    wholeScreenLen = wholeScreenLen * temp;
    wholeScreenBMP.data_p = new unsigned short [ wholeScreenLen  ];
    memset (  (unsigned char *)wholeScreenBMP.data_p , 0, wholeScreenLen*2  );
    
    // ============================================================
    
}


ConfirmWindow::   ~ConfirmWindow ( )
{
    delete btn_Yes;
    delete btn_No;
    delete wholeScreenBMP.data_p;

}


void ConfirmWindow::addComopnents (  void  )
{
    btn_Yes ->setCaption( " Yes " );  
    btn_Yes ->setFocused ( false );
    
    btn_No ->setCaption(  " No  " );
    btn_No ->setFocused ( true );

}



void ConfirmWindow::restoreScreen( void )
{
    DrawAttribute drawAttr;
    drawAttr.x = 0;
    drawAttr.y = 0;
    
#ifdef __PSPSDK__    

    if ( PSP_GRAPHICS::getScreenMode () == screenModeWriteAndBuffered_cg )
    {  
        //get the whole image A of front screen.
        //draw to back buffer
        //draw the message too.
        //swap()
        //put back the whole image A.
        //PSP_GRAPHICS::fillVRam(0xfff0);

        PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP );       
        PSP_GRAPHICS::flipScreenBuffer();

        
                
    }else  // write and show mode.
    {
        PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP );        


    } 
#endif        
    
}

bool ConfirmWindow::show( const char * message )
{
    bool result = false;


    PSP_GRAPHICS::getScreenImageToBitMap(  wholeScreenBMP );
    DrawAttribute drawAttr;
    drawAttr.x = 0;
    drawAttr.y = 0;
    drawAttr.enlarge = 1;
    drawAttr.drawBackGroudZero = true;
    
    
    //draw to back buffer    
    PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP );          
    
    
    //=====================================================================================
    
    
    //=====================================================================================
    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);    
    
    
    PSP_Window::showWindow();
    btn_Yes->show();
    btn_No->show(); 
    if ( message ) 
    {
        printAscHan16ZiString ( 10, 10, 0 , ( char *)message );
    }    
    else
    {
        printAscHan16ZiString ( 10, 10, 0 , (  char *) "Confirm  ? " );
    }
    PSP_GRAPHICS::flipScreenBuffer(); 

    skipCurrentKey();
    do
    {           
           
        sceCtrlReadBufferPositive(&pad, 1); 
 
        if ( !pad.Buttons )
            continue;   

        if ( pad.Buttons & PSP_CTRL_CIRCLE )
        {
            if ( btn_Yes->getIsFocused () )
            {
                result =  true;
            }
            else
            {
                result =  false;
            }
            break;
        } 
        
        if (pad.Buttons & PSP_CTRL_CROSS)
        {
            result = false;
            break;
        } 
 
        if (pad.Buttons & PSP_CTRL_LEFT)
        {   
            this->btn_Yes->setFocused( true );
            this->btn_No->setFocused( false );


        }
        if (pad.Buttons & PSP_CTRL_RIGHT )
        { 
            this->btn_No->setFocused( true);
            this->btn_Yes->setFocused( false ); 

        } 

        PSP_Window::showWindow();
        btn_Yes->show();
        btn_No->show();   
   
        if ( message ) 
        {
           printAscHan16ZiString ( 10, 10, 0 , ( char *)message );
        }    
        else
        {
           printAscHan16ZiString ( 10, 10, 0 ,(  char *) "Confirm  ? " );
        }
        PSP_GRAPHICS::flipScreenBuffer(); 
        wait(  );     
    }while ( true );
    PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP );
    PSP_GRAPHICS::flipScreenBuffer(); 
    skipCurrentKey();


return result;
   

}   





//===========================================================================================









