

#ifndef __CONFIGWIN__
#define __CONFIGWIN__


#include <PSP_Global.h>
#include <WinComponent.h>

using namespace PSP_Constants;

//=========================================================================
class TextViewPadConfig
{
    static const char configFileName[  ];

    bool isUsingTrueTypeFont;    
    
public:    
    static bool configFileWasLoaded ;
    bool pageTurnKeepOneLine;
    unsigned char spaceBetweenLine;
    unsigned char fontSize ; 
    unsigned char onePressLineMoveCount ;
    
    RGBColor fontRGBColor;
    RGBColor backGroudRGBColor;  
    char trueTypeFontFilePath[ pathMaxLen ];
    void setIsUsingTTF( bool );
    TextViewPadConfig () ;
    ~TextViewPadConfig () ;
    
    bool getIsUsingTrueTypeFont( void )
    {
        return isUsingTrueTypeFont;
    }
    void save();
    void load();
};
//=========================================================================



//=========================================================================
class TextViewPadConfigWindow :public PSP_Window
{

   static  TextViewPadConfig *config_p;
    
   static  BoolBox *boolBox_usingTTF;
   static  RGBColor tempRGBFontColor, tempRGBBackGroundColor;

    void drawAllItems( void );
    void addComponents  ( void );
    
    //================================================================
    //components
  static  Button *btn_setFontColor , *btn_setBackgroundColor , *btn_setTTFFilePath;
  static  Button *btn_OK;
  static  PSP_Window fontColorBar, backgroundColorBar;
  
  static  Label *label_ttfFilePath ; //*label_fontSize;  
  static  Label  *label_title, *label_helpText1; //, *label_helpText2 ;
  
  static  NumberBox *numbox_fontSize;
    //================================================================
    //void show( void ); 
  static void setFontColor ( void );
  static void setBackGroundColor( void );
  static void setTTFFontFilePath( void );
  static void  setIsUseTTF ( void );
  void   applyConfigToWindow( void );
  void   applyWindowToConfig( void );


  void setComponents( Win_Component *component_a [] );

 public:
   
    TextViewPadConfigWindow (  TextViewPadConfig * _config_p );
    ~TextViewPadConfigWindow (   );     
    void startConfig(  void  );
   

};


#endif



