

#ifndef __PSP_FILEUTIL__
#define __PSP_FILEUTIL__


#ifdef __PSPSDK__
#include <pspkernel.h>
#endif


#include <CommonString.h>
#include <PSP_Window.h>
#include <dirent.h>
  
 /*
struct FileMgrDrawItem
{
    unsigned short winX, winY;
    unsigned short fileItemIndex;

};
*/

/*
class FileExtHandler
{
    

};

class FileHandlerMap
{
   




};

*/

class PSP_FileManager :public PSP_Window
{

    enum ExploreResult
    {
        notDone, reEnter, doneExplore, cancel 
    };
    static const unsigned short maxFilesCount = 2048;
  
    static const unsigned char parentDirSymbol[];
    static const unsigned char msRoot[]; 
    static const unsigned char currentDirSymbol[]   ;

    const static unsigned short winTitleHeighSpare =  22    ;
    ushortBitMap wholeScreenBMP;
    
    
  //  static CommonString  currentPathString;

#ifdef __PSPSDK__   
    //SceIoDirent *fileInfoStruc_p[ maxFilesCount ];
    //SceIoDirent *dirInfoStruc_p[ maxFilesCount ];
    
#endif
    
    dirent *fileInfoStruc_p[ maxFilesCount ];
    dirent *dirInfoStruc_p[ maxFilesCount ];

    

    unsigned short fileCount;
    unsigned short dirCount;
    void getAllFileNames ( const unsigned char *path  );
   // bool addItem( const SceIoDirent * );    
    
    
    bool addItem( const dirent *   );
    
    void showItem( unsigned short winX, unsigned short winY, unsigned short fromIndex );   
   // void showItem( unsigned short onScreenItem  );   
    void showAllItems ( unsigned short fromIndex ) ;

    //EnumIcons getFileType ( const char * filename );
    //SceIoDirent * getItem ( unsigned short index ) const; 
    dirent * getItem ( unsigned short index ) const; 
    
  //  CommonString getUpperDir ( const unsigned char * dirpath );
    
    //================================================================
   // static const unsigned short maxOnScreenItem = 20;
//     FileMgrDrawItem fileMgrDrawItem[ maxOnScreenItem ];
   //  unsigned short onScreenDrawItemIndex;
     unsigned short seletedFileItemIndex;
   //  void addOnScreenItem ( unsigned short winX, unsigned short winY, unsigned short  index );
//     void clearOnScreenItem( void );
     //unsigned short onScreenItemCount;
     
     unsigned short drawIndexFrom , drawIndexLast;
     //===============================================================
    
    void handleKeyUpPressed( void );
    void handleKeyDownPressed( void );
    ExploreResult handleKeyCirclePressed(  CommonString &updateCurrentPath ,  CommonString * selectedFilename = 0   );
    ExploreResult handleKeyTrianglePressed(  CommonString &updateCurrentPath  );
     PSP_Window selectBarActive;
   //  PSP_Window selectBarNormal;
     
     
    const ushortBitMap * getFileIconBitMap ( const unsigned char * filename );
    //static CommonString getFileExtention ( const unsigned char * filename );
    void launchFile ( const  CommonString & filename  );
    void init ( void );
    ExploreResult explorePath (    CommonString & explorePath ,  CommonString *selectedFilename = 0 );
    
    //ExploreResult  handlePressDir (  CommonString &updateCurrentPath , const SceIoDirent *tempDirt );
    ExploreResult  handlePressDir (  CommonString &updateCurrentPath , const dirent *tempDirt );
    unsigned short showfromFileIndex;
    void refresh ( void );   
    // before set title, tranlate JIS to CHS  
    void setTitle ( const  unsigned char * );
    
    
    //===========================================================================================  
    static void (  *textFileHandler ) ( const   char * )  ;
    static void (  *htmlFileHandler ) ( const char * )  ;

    //===========================================================================================  
    
    void restoreBackGround( void );
  public:
    //PSP_FileManager  ( const Rectangle &rec );
    PSP_FileManager  (  const Rectangle &rec ); 
    
    
    /**
     void ( *onClick_p )( void );    
   
   
    */  
    
    
   static void registerFileHandler( const unsigned char* fileExt, void( *handler_p)( const   char * )  );
    
    
    
    PSP_FileManager  (   ); 
    
    
    
    
   ~PSP_FileManager ( );
    
    void explore ( void  );
    
    static bool fileSelector( CommonString &file );
   // void show ( void );
    void clear ( void );    
    void print ( void );
    // max draw Item on screen
   
};


#endif


