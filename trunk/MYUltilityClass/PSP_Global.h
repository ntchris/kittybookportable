
#define __PSPSDK__
//#define __VC__



#ifndef __PSP_CONSTANTS__
#define __PSP_CONSTANTS__



//#include <PSP_Global.h>


#ifdef __PSPSDK__
#include <pspctrl.h>
#endif 


#include <string.h>

namespace PSP_Constants
{
    //Must use int because SCE functions defined so
    //sceDisplaySetFrameBuf must use this 512
    const int PSP_SCREEN_WIDTH_TakeMemoryDot  = 512 ; 

    //SetMode must use this 480   
    const int PSP_SCREEN_WIDTH_Visible_cg  = 480 ;
    const int PSP_SCREEN_HEIGH_cg = 272 ;
    const int bytesPerPixel_cg = 2; // 16 bit color , 65535 colors.
    const unsigned long uncachedOffsetByte = 0x40000000;
    // 
    const unsigned long screenFramSize_byte_cg = PSP_SCREEN_WIDTH_TakeMemoryDot * PSP_SCREEN_HEIGH_cg * bytesPerPixel_cg ;
    const unsigned long screenFramSize_dot_cg =  PSP_SCREEN_WIDTH_TakeMemoryDot * PSP_SCREEN_HEIGH_cg ;
    unsigned short * const psp_vram_base_cg = (unsigned short *)0x04000000 + uncachedOffsetByte/bytesPerPixel_cg ;

    // if char use 0x40000000   if short use 0x20000000 !!!!!!!!
    // unsigned char *psp_vram_base_cg = (unsigned short *)0x04000000 + 0x40000000;


   static const unsigned short pathMaxLen = 255;

   static const unsigned short returnSymbolUnicode0D = 0x0d;   //  \n\r 
   static const unsigned short returnSymbolUnicode0A = 0x0a;   
   static const unsigned short spaceSymbolUnicode = 0x20;   

  static const unsigned short getchWaitTime = 15;

    enum PSP_SCREEN_MODE
    {       
        screenModeWriteAndShow_cg ,
        screenModeWriteAndBuffered_cg 

    } ;  


    class RGBColor
    {
       public:
        unsigned char  red;         	
        unsigned char  green;
        unsigned char  blue;
        unsigned short color;
        inline RGBColor( unsigned char r, unsigned char g, unsigned char b )
        {
           setColor ( r, g, b );
        }
        
               
        
        inline RGBColor(  )
        {
            red = 0;
            green = 0;
            blue = 0;
        }
        
        inline void setColor ( unsigned char r, unsigned char g, unsigned char b  )
        {
            red = r;
            green = g;
            blue = b; 
            updateColor ();
        }
        
        inline void setColor ( const RGBColor &rgbColor  )
        {
            red = rgbColor.red;
            green = rgbColor.green;
            blue = rgbColor.blue; 
            updateColor ();
        }
        
        inline void updateColor ( void )
        {
            color = ( ( ( ( blue>>3 ) & 0x1F )<< 10 ) +
                   ( ( ( green>>3 ) & 0x1F ) <<5 ) +
                   ( ( ( red>>3 ) & 0x1F ) <<0 ) + 0x8000 ); 
        }
        
        inline unsigned short getColor( void )
        {     	  
            return color;
        }
        
        static unsigned short getShortColor( unsigned char _r, unsigned char _g, unsigned char _b )
        {     	  
            RGBColor temp( _r, _g, _b );
            return temp.getColor();
        }
        
    };



    struct ushortBitMap
    {
        unsigned short width;
        unsigned short heigh;	
        unsigned short *data_p;
    };




    struct ucharBitMap
    {
        unsigned short width;
        unsigned short heigh;	
        unsigned char  *data_p;

    };

    struct BitMapSize
    {
        unsigned short width, heigh;
    };

    class  Rectangle
    {

    public :
        unsigned short x,y, width, heigh;
        Rectangle ( unsigned short _x , unsigned short _y , 
            unsigned short _width,  unsigned short _heigh  )
        {
            x = _x;
            y = _y;
            width = _width;
            heigh = _heigh;
        };

        Rectangle ( )
        {
            // default value is a full screen
            x = 0 ;
            y = 0;
            width = PSP_SCREEN_WIDTH_Visible_cg;
            heigh = PSP_SCREEN_HEIGH_cg;
        };
        inline bool isInside(  unsigned short pointX , unsigned short pointY ) const
        {
            if ( pointX <= x ) return false;
            if ( pointY <= y )  return false;
            if ( pointX >= x + this->width ) return false;
            if ( pointY >= y + this->heigh ) return false;

            return true;
        };

    };



    class DrawAttribute
    {
    public :

        unsigned short x;
        unsigned short y;
        //	bool drawBackground;
        unsigned char enlarge;
        Rectangle *limitedRec_p;
        bool drawBackGroudZero ;
        const static unsigned short backGroudColor = 0x8888;
        inline DrawAttribute ( )
        { 
            enlarge = 1;    
            x = 0;
            y = 0;
            limitedRec_p  = 0 ;
            drawBackGroudZero = false;
        };

    };




    //For the ASC Font LIB
    const unsigned char oneAscCharUseByte_LIB = 16;
    static const unsigned short DotFont16ASCfont_width = 8;
    static const unsigned short DotFont16ASCfont_heigh = 16;
    const static unsigned short spaceBetweenChar = 1 ;
    const unsigned char oneAscCharUseByte_BMP = DotFont16ASCfont_width * DotFont16ASCfont_heigh ;
    struct ASCFontBitMap
    {   
        unsigned char  data [ oneAscCharUseByte_BMP  ];
        const static unsigned short size = oneAscCharUseByte_BMP ;
    };
    //The Dot count




    static const unsigned short dotFontHanZiFontWidth = 16;
    //Font heigh  : 16 dots
    static const unsigned short dotFontHanZiFontHeigh = 16;
    //One HanZi uses 32 bytes
    static const unsigned char oneHanZiUseByte_LIB = 32;
    static const unsigned short oneHanZiUseByte_BMP = dotFontHanZiFontWidth * dotFontHanZiFontHeigh;
    struct HanZi16FontBitMap
    {
        unsigned char  data [ oneHanZiUseByte_BMP  ];
        const static unsigned short size = oneHanZiUseByte_BMP ;        
    };





    class HanZi
    {
    public:

        union 
        {   
            unsigned char ch[2];
            unsigned short ushort;
        }; 
        bool isUnicode;

        HanZi( const char * hanzic)
        {
            ushort = 0;
            //Null String
            if ( hanzic == 0 )
            {
                return;     	
            }
            else if ( strlen ( hanzic ) <  2 )
            {
                // strlen()=1  it's a ASC
                ch[0] =  hanzic[0];

            }else  
            {
                ch[0] =  hanzic[0];
                ch[1] =  hanzic[1];              
            }   
            isUnicode = false;

       	};

        HanZi( )
        {  
            ushort = 0; 
            isUnicode = false;
        };

    };   



    const static unsigned short colorBlue = 62656;
    const static unsigned short colorLightBlue =   65270 ;
    const static unsigned short colorNoColor = 0;
    const static unsigned short colorBlack = 1;
    const static unsigned short colorWhite = 0xffff;
    const static unsigned short colorGray = 41122;
    const static unsigned short colorWhite2 = 59193;
    const static RGBColor rgbBlack ( 1, 1, 1 );
    const static RGBColor rgbWhite ( 255, 255, 255 );
    

    const unsigned char textFileExt[]= { ".txt"};
    const unsigned char mp3FileExt[] = { ".mp3"};
    const unsigned char htmlFileExt[]= { ".html"};
    const unsigned char htmFileExt[]= { ".htm"};
    const unsigned char winRarFileExt[]= { ".rar"};
    const unsigned char winZipFileExt[]= { ".zip"};
    const unsigned char ttfFileExt[]= { ".ttf"};
    const unsigned char ttcFileExt[]= { ".ttc"};


};




#ifndef __VC__


void* operator new( unsigned int size ) ;


void* operator new[]( unsigned int size ); 


void operator delete( void *p ) ;

void operator delete[]( void *p ) ;
#endif


#ifdef __PSPSDK__

void   wait( unsigned long count = PSP_Constants::getchWaitTime );

SceCtrlData  getch( void )   ;

void skipCurrentKey( bool powerSave = true  );

#endif 

// Analog control is from 0 to 255;  up = 0 , down = 255;
const unsigned char controlPositionMid = 128;
const unsigned char controlPositionDelta = 20;
const unsigned char controlPositionUp = controlPositionMid - 
controlPositionDelta;
const unsigned char controlPositionDown = controlPositionMid + 
controlPositionDelta;

//the meaning of Mid Speed is for quitting, don't keep users waiting for too long.
static const int cpuNormalSpeed = 222, cpuSlowSpeed = 10 , cpuMidSpeed = 80,
                 busNormalSpeed = 111, busSlowSpeed = 5 , busMidSpeed = 40 ;
                 
                 
void slowDownCPU( void );

void normalSpeedCPU( void );
//the meaning of Mid Speed is for quitting, don't keep users waiting for too long.
void midSpeedCPU( void );

int getBufFreq( void );

int getCPUFreq( void );

//minTTFFontSize = 10 cause error drawing fonts
static const int minTTFFontSize = 11, maxTTFFontSize = 50;


//extern bool begin ;

#endif // __PSP_CONSTANTS__



