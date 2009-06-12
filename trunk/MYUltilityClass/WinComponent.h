



#include <PSP_Global.h>
#include <PSP_Window.h>
#include <PSP_GRAPHICS.h>

/**
Define some components in the window
like  label, counter, fileSelector, boolBox.



*/

#ifndef  __WIN_COMPONENT__
#define  __WIN_COMPONENT__



/**
base class

*/
class Win_Component
{
    //Parent window
   
protected :
    //static const FocusedBarColor = 0xffff;
    //static const NotFocuedBarColor = 0xffff;
    
    PSP_Window *window_parent;
    unsigned short x, y;
    bool isFocused ;
    Rectangle recFocused;
    static RGBColor focusBarRGBColor;
    
    //This is pointer
    void ( *onClick_p )( void );    
    
    
    void setFocusedBarSize ( unsigned short width, unsigned short heigh );
    
    virtual void show( void ){};
    
    
    
public:
    void setAction( void (*action_p)(void) );
    void setFocused ( bool foc = true );
    bool getIsFocused( void );  
    void onClick ( void );
    Win_Component(  PSP_Window * win, unsigned short winx, unsigned short winy );	


};



//=========================================================================

class Label :public Win_Component
{
    CommonString text;
    unsigned short color;
public:
    void setText ( const   char * );
    void setColor ( unsigned short _color );
    
    void show( void );
    const unsigned char * getText( void ) const;
    Label  (  PSP_Window * win, 
        unsigned short winx, unsigned short winy );




};


//=========================================================================



class Button :public Win_Component
{
    CommonString text;
    bool enabled;
    static const  unsigned short defaultButtonColor =  65108;
    static const  unsigned short diabledButtonColor =  60250;
    
    PSP_Window winBotton;
    
    

    
   // RGBColor btnColor;//( 200, 200, 255 );
    
    
    //void setFocusedBarSize ( unsigned short width, unsigned short heigh );
  public:
    Button ( PSP_Window * win , unsigned short winx, unsigned short winy );
    void setCaption( const char * text_p);
    void setEnabled ( bool enable = true );
    bool getEnabled ( void );
    void show( void );
    ~Button ();
  
    //void (*function_pointer)(float);
    
    
};


//=========================================================================

/**
select or not.

*/

class BoolBox : public Win_Component
{

    bool selected;
    const static unsigned short bmpSelData[], bmpNoSelData[];

    CommonString text;
    ushortBitMap bmpSelected, bmpNoSelected;
    void updateBoxBMP( void );
public:

    inline void click( void )
    {
        selected = !selected ;

    }

    inline void setValue( bool select )
    {
        this->selected = select;
    }

    inline bool getValue( void )
    {  
        return selected;
    }

    void setLabel( const char * labelText );
     

    void show( void );
    BoolBox( PSP_Window * win, 
        unsigned short winX, unsigned short winY );


    ~BoolBox( );

};





class NumberBox : public Win_Component
{
     const static unsigned char maxValue = 255;
     const static unsigned short numboxWidth = 55 , numboxHeigh = 25;
     static const unsigned short headerX = 5  ;
     
     unsigned char value;
     PSP_Window winBox;
     Label *label;
     static void showHelpText( void );
     
   public:
     NumberBox ( PSP_Window * win, 
        unsigned short winX, unsigned short winY );
     ~NumberBox ( );
     void  setValue( unsigned char value );
     void  increase( void );
     void  decrease( void );
     void   show( void ); 
     void setLabelColor( unsigned short color);
     void setText( char* text );
     unsigned char getValue( void );


};



class ScrollBar 
{
	
	unsigned percent;
	
	static const unsigned short backGndWidth = 3;
	static const unsigned short barHeigh = 8;
	
	
	Rectangle recBar;
	Rectangle recBackGnd;
	const PSP_Window * win;
	static const unsigned short backGndColor = colorGray, barColor = colorBlue;
 public:
    ScrollBar 	( const PSP_Window * win );
    ~ScrollBar 	( );
    
   // void setPercent ( unsigned percent_ );
    void setPercent ( unsigned long a, unsigned long b );
   
	
//	void setPercent ( unsigned a, unsigned b );
	unsigned short getPercent ( void );
    void showScrollBar ( void );
    void showScrollBar ( unsigned percent_ );
	
	
};









class ConfirmWindow : PSP_Window
{
     void addComopnents (   void );
     Button *btn_Yes , *btn_No;
     bool inited;
     ushortBitMap wholeScreenBMP;
   public:
     bool show( const char * message );
     void restoreScreen( void );
     ConfirmWindow ( );
     ~ConfirmWindow ( );

};




class PopUpWindow
{};





#endif   //__WIN_COMPONENT__




