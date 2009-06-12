 
#ifndef __TextViewPad__
#define __TextViewPad__


//#include <stdio.h> 

#include <CommonString.h>
#include <PSP_Window.h>

#include <WinComponent.h>
#include <ConfigWindow.h>
#include <stdio.h>


//Store the lineHead char index for each line ( lines in PSP screen)
class LineHeadStore
{
    const static unsigned long maxSize = 1024*1024*1;
    unsigned long size;
    unsigned long  *lineHead_ap;
    
public:

    bool add( unsigned long lineHeadPosition );
    
    void clear( void );
    //get position from line number
    unsigned long get( unsigned long index);
    //get position of the last line number
    unsigned long get(  void );
    
    unsigned long LineHeadStore::getLineNumber( unsigned long position )    ;
    inline unsigned long getSize( void )
    {
        return size;
    };

    LineHeadStore();
    ~LineHeadStore();
};


//====================================================================================================



class BookMarkStruct
	{
	    
      public:
        bool hasData;
	    unsigned long position;
        //including path
        char filename[ pathMaxLen ];   	
        BookMarkStruct ()
        {
	        hasData = false;
	        memset( filename, 0,  pathMaxLen );	
            position = 0;    
	    }
         
	} ;
	
	

//====================================================================================================
// BookMarkHandler class handler 10 bookmark for different books;
// a bookmark means : a book file name + position.
class BookMarkHandler
{
    
	
 public:	
  static const unsigned short maxBookMarkCount = 5 ;
 private: 	
	static BookMarkStruct  allBookMark[ maxBookMarkCount  ];
	
    static const char bookMarkFileName[];
    
    BookMarkHandler( );
    ~BookMarkHandler( );
    static  bool sanityCheckingAndCreateNewBookMarkFile (  void ) ;
 public:
    static BookMarkStruct currentBookMark;
      // index means the position of the bm file, total is 5
    
    static  bool save (  int index  );
    static  bool load (  int index ); 
    static  bool load (  void ); 
    //static  BookMarkStruct * getBookMarkInfo ( void );
    static  bool getBookMarkValid( int index );
    
};

//====================================================================================================






class BookMarkWindow : public PSP_Window
{
     ushortBitMap wholeScreenBMP;
     Button *btn_save_a [ BookMarkHandler::maxBookMarkCount  ];
	 Button *btn_load_a [ BookMarkHandler::maxBookMarkCount  ];
	
  //   Label * label_save, *label_load ;
     void addComopnents ( void );
      BookMarkWindow (  );
     ~BookMarkWindow ( );
     
     void drawAllItems( void );
     void restoreWindow( void );
	public:
     const static int doneReturn = 0, saveActionDone = 1, loadActionDone = 2;	
     //return the index
	 static int show( void );	
};

//=========================================================================


// to do :  support  unicode text file

class TextViewPad  :public PSP_Window
{

     enum TextFileType
    {
        normalText, unicodeText, unicodeBigEndText  
    } ;
    
    //read 512 KB one time.	

    // 4MB 可以 6MB 可以.
    //这是unicode 的size, 就是说, asc text最大只有一般 即3MB.
    static const unsigned long maxFileLoadLengthOneTime = 1024 * 1024 * 6 ;
    //static const unsigned long maxFileBufferUnicodeLength = maxFileLoadLengthOneTime;
    static unsigned long bookMarkLine;
    unsigned short *unicodeBuffer;//[ maxFileLoadLengthOneTime  ];
    unsigned long loadedBufferSize ;
    unsigned long fileSize;
    unsigned char ascFontSize ;
    unsigned short fontSize ;  
    //  unsigned char onePressLineMoveStep ;
    //bool inited ;
    char filename[ pathMaxLen];
   
    
   
    void showFile ( unsigned long lineNumber = 0 ); 	
    // next page 

     

    bool addLineHeadIndex ( unsigned long headIndex );

    //PSPArray lineHeadIndex_array ;
    LineHeadStore *lineHeadStorep;
    unsigned long totalPagesCount ; 
    void  showAllLineHead ( void );
    void initTextViewPad( void );
    void printOneLineTTFString( const char * );
    bool loadTextFile( const  char *filename , unsigned long fromPosition = 0  );
    void getAllLineHead( void );
    
    static const int quitTextView = 0, loadBookMark = 1;
    int readFileFromBuffer ( void );
    void countTotalPages( void );
   
    TextFileType getTextFileUnicodeType( const  unsigned char * textBuffer );
    
    

    void convertUnicodeToBigEnd ( const unsigned char *unicode, 
        const unsigned char *unicodeBigEnd );


   /* unsigned long  printTTFHanZiUnicodeString
        ( unsigned short winx, unsigned short winy, unsigned short color,
          const unsigned short *unicodeStr, unsigned long size  ) ;
*/
    void loadFontFile( void ); 
    void applyBookMarkLine( void );
  
    unsigned long countOneLineHanZiUnicodeString       (  
         const unsigned short *unicodeStr, unsigned long size  ) ;

    unsigned long  printTTFHanZiUnicodeString
        ( unsigned short winx, unsigned short winy, unsigned short color,
          const unsigned short *unicodeStr, unsigned long size  ) ;
    
    unsigned long  printUnicodeAscHanZi16String
                 ( unsigned short winx, unsigned short winy, 
                   unsigned short color, const unsigned  short *unicodeString ,
                   unsigned long bufferSize ) ;
  
    //const static int cancel = 0;
    int  handleBookMarkWin( void ) ;
         
    ScrollBar *textViewScrollBar;          
    //unsigned long onePageLinesCount;
    void  showInfo( void );
    
    unsigned long countLinesDown( unsigned long currentLineNumber,unsigned long step, 
                                  unsigned long totalLine );
    unsigned long countLinesUp( unsigned long currentLineNumber, unsigned long step   );


	unsigned short countEnglishWordWidth( const unsigned short *uniStr,  
	                                      unsigned short wordAscCount );

    static const unsigned short TextViewPadWinYStart = 0;
    unsigned short maxLineOnePage ; 
    unsigned short stepPage ;
public:

    //How about super big file ??

   static TextViewPadConfig config;
 
   //simplely read one and ONLY one file, not bookmark 
    static void readOneFile ( const   char *fileName );
    static void readMoreFileFromFileMgr ( const   char *fileName );
    static void readMoreFileFromBookMark ( void ) ;

    void loadConfig( void );
    void applyConfig( void );
    
    TextViewPad ();

    TextViewPad (  const Rectangle &rec );

    ~TextViewPad ();

    static void selfTestLineHead( void );
    static void selfTestPrintTextFileTextMode ( void );
    static void selfTestConfig ( void );
    static void selfTestBookMark( void );




};


#endif   //__TextViewPad__



