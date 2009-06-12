

#include<ColorPad.h>





ColorPad ::  ColorPad (  ) 
{
    static const unsigned short x = 75 ,y = 30, step = 50;
    //this->setRectangle( 10, 10, 250, 200 );
    PSP_Window::setColor ( 0xffff, 1 );
    numbox_Red = new NumberBox( this, x, y );
    numbox_Green = new NumberBox( this, x, y + step );
    numbox_Blue =  new NumberBox( this, x, y + step + step );
    
    numbox_Red ->  setText("Red  ");
    numbox_Green ->setText("Green");
    numbox_Blue -> setText("Blue ");
    
    numbox_Red ->setLabelColor( RGBColor::getShortColor( 255, 0, 0 ) );
    numbox_Green ->setLabelColor( RGBColor::getShortColor( 0, 255, 0 ) );
    numbox_Blue->setLabelColor( RGBColor::getShortColor( 0, 0, 255 ) );
    numbox_Red ->setFocused();
    /*labelR = new Label( this, 10, y  );
    labelR ->setColor( RGBColor::getShortColor( 255, 0, 0 ) );
    
    labelG = new Label( this, 10, y + step );
    labelG ->setColor(RGBColor::getShortColor( 0, 255, 0 ) );
    
    labelB = new Label( this, 10, y + step+step );
    labelB ->setColor(RGBColor::getShortColor( 0, 0, 255 ) );
    */
    labelHelpText = new Label( this, 5, 230 );
    labelHelpText ->setColor( RGBColor::getShortColor( 0, 0, 255 )  );
    labelHelpText ->setText( " O to confirm, X to cancel. Left/Right change value.");
    
    /*labelR ->setText( "Red" );
    labelG ->setText( "Green" );
    labelB ->setText( "Blue" );
    */
    
    
    colorBar.setRectangle( 220, 70, 80, 80 );
    colorBar.setColor( 1 );
    
}





ColorPad ::  ~ColorPad (  ) 
{
    /*delete labelR;
    delete labelG;
    delete labelB;
    */
    delete numbox_Red;
    delete numbox_Green;
    delete numbox_Blue;
   
}



void  ColorPad:: addComponent( void )
{
   
   



}

/*
void ColorPad::setColor( unsigned char r, unsigned char g, unsigned char b )
{
     RGBColor rgbcolor; 
     rgbcolor.setColor( r,g,b);
     
    colorBar.setColor (  rgbcolor.getColor() );
   
   
}
*/

void ColorPad::setColor( RGBColor &rgb )
{
          
    colorBar.setColor (  rgb.getColor() );
   
   
}



void ColorPad::drawAllItems( void )
{
     PSP_Window::showWindow();
     updateAllItems();
   
     //update the back buffer
      if (  PSP_GRAPHICS::getScreenMode () == screenModeWriteAndBuffered_cg )
     {
        
      
        PSP_Window::showWindow();
        updateAllItems();
     }   
   
}

void  ColorPad:: updateAllItems( void )
{
     numbox_Red->show();
     numbox_Green ->show();
     numbox_Blue ->show();
     /*labelR ->show();
     labelG ->show();
     labelB ->show();
     */
     colorBar.showWindow();
     labelHelpText->show();
     PSP_GRAPHICS::flipScreenBuffer();

}
    

void ColorPad:: show( RGBColor &rgbColor )
{
   
     static ColorPad *colorPad = 0;
     if( colorPad == 0 )
      colorPad = new ColorPad ();
      
     //colorPad ->drawAllItems ( );
     
     static const unsigned short componentCount = 3;
     static unsigned short selectedIndex = 0;
     static const unsigned short indexRed = 0, indexGreen = 1, indexBlue =2 ;
 
     RGBColor rgbColorTemp;
     
     NumberBox *numberBox_a[ componentCount ];
     numberBox_a[indexRed] = colorPad ->numbox_Red;
     numberBox_a[indexGreen] = colorPad ->numbox_Green;
     numberBox_a[indexBlue] = colorPad ->numbox_Blue;
     

     SceCtrlData pad;
     sceCtrlSetSamplingCycle(0);
     sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG); 
     
     
     colorPad ->numbox_Red->setValue( rgbColor.red ) ;
     colorPad ->numbox_Green->setValue( rgbColor.green ) ;
     colorPad ->numbox_Blue->setValue( rgbColor.blue ) ;
     colorPad->colorBar.setColor( rgbColor.getColor()); 
     rgbColorTemp.setColor( rgbColor );
     colorPad ->drawAllItems ( ); 
     
     skipCurrentKey();
     do
     {           
        
        sceCtrlReadBufferPositive(&pad, 1);        
        
        if ( !pad.Buttons )
        continue;   
        
        if (pad.Buttons & PSP_CTRL_CIRCLE )
        {
            rgbColor.setColor(   rgbColorTemp  );
            break;//return
        } 
        
        
        if (pad.Buttons & PSP_CTRL_CROSS )
        {
            //do not set rgbcolor, just return
            break;
        } 
        
        if (pad.Buttons & PSP_CTRL_UP)
        {
             
             if ( selectedIndex > 0  )
             {  numberBox_a[ selectedIndex ] ->setFocused ( false );
                selectedIndex --;
                numberBox_a[ selectedIndex ] ->setFocused ( true );
             }   
        } 
        if (pad.Buttons & PSP_CTRL_DOWN)
        {          
             if ( selectedIndex <  componentCount-1  )
             {
                numberBox_a[ selectedIndex ] ->setFocused ( false );
                selectedIndex ++;    
                numberBox_a [ selectedIndex ] ->setFocused ( true );
             }   
        } 
        
        if ( pad.Buttons & PSP_CTRL_LEFT )
        {
            numberBox_a[ selectedIndex ]->decrease(); 
        }             
        if ( pad.Buttons & PSP_CTRL_RIGHT )
        {
            numberBox_a[ selectedIndex ]->increase();         
        
        }
        rgbColorTemp.setColor ( colorPad ->numbox_Red->getValue(),
                                colorPad ->numbox_Green->getValue(), 
                                colorPad ->numbox_Blue->getValue()   
                              );
        colorPad->setColor( rgbColorTemp );
                             
        colorPad ->updateAllItems ( );        
        if ( pad.Buttons & PSP_CTRL_LEFT ||  pad.Buttons & PSP_CTRL_RIGHT )
        {
             wait(1);
             continue;
        
        }
         
        
        wait(  );
         
     }while( true );
     skipCurrentKey();

}











