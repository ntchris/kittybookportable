 



#include <TextViewPad.h>


#include <PSP_Global.h>
#include <PSP_TrueTypeFont.h>

#include <stdio.h>


#include <PSP_ChineseUtil.h>
//#include <PSP_Window.h>
//#include <PSP_FileUtil.h>
//#include <WinComponent.h>



/*#ifndef __PSPSDK__
#include <conio.h>
#endif
*/

#ifdef __PSPSDK__
#include <pspctrl.h>
#include <PSP_GRAPHICS.h>
#endif


const bool debug_fileView_cg = false;


extern PSP_TrueTypeFont * pspTrueTypeFont_cgp ;


TextViewPadConfig TextViewPad ::config;

unsigned long TextViewPad ::    TextViewPad ::bookMarkLine ;



//================================================================================






LineHeadStore::LineHeadStore()
{
    this->size = 0;   
    lineHead_ap = new unsigned long [ maxSize ];
    if ( !lineHead_ap )
    PSP_Window::showMessage( "new failed: LineHeadStore::LineHeadStore() ");
}

LineHeadStore::~LineHeadStore()
{
    delete lineHead_ap;

}

bool LineHeadStore::add( unsigned long lineHeadPosition )
{
    /**
    before add, make sure the lineHead is NOT added before
    */
    unsigned long lastLineIndex;
    lastLineIndex = this->get();
    if ( size > 0 && lineHeadPosition <= lastLineIndex  )
    {  //already added, now , do not add again
        return true;
    }   


    if ( size >= maxSize )
        return false;
    this->lineHead_ap[ size ] = lineHeadPosition ;   
    size ++;
    return true;
}


void LineHeadStore::clear( void )
{
    for ( unsigned long i=0; i < size ;i++)
    this->lineHead_ap[ i ] = 0 ;   

    size = 0;
}

unsigned long LineHeadStore::get( unsigned long index )
{
    if ( index < size )
        return this->lineHead_ap[ index ]  ;   
    else
        return 0;

}

unsigned long LineHeadStore::get(  void  )
{
    if ( this->size >0 )
        return this->lineHead_ap[ size -1 ]  ;
    else 
        return 0;


}

unsigned long LineHeadStore::getLineNumber( unsigned long position )
{
	unsigned long i = 0, tempposi;
	
	do
	{
	    if( i >= size )break;
	    tempposi = get( i );
	    if( tempposi >= position ) break; 
	    i++;
	}while ( true );
	
	return i;
}


//====================================================================================================
// class  BookMarkHandler::

const char BookMarkHandler :: bookMarkFileName[] = {"KtBookMk.dat"};

BookMarkStruct BookMarkHandler :: currentBookMark , 
               BookMarkHandler :: allBookMark[ BookMarkHandler :: maxBookMarkCount  ];


BookMarkHandler :: BookMarkHandler ( )
{  
	
   

}

BookMarkHandler::~BookMarkHandler()
{
	
}

// sanity checking false:  return false;
// sanity checking passed, return true;
bool BookMarkHandler::sanityCheckingAndCreateNewBookMarkFile ( void  )
{
	bool r = false;
    FILE *fp;
    fp = fopen( BookMarkHandler::bookMarkFileName, "rb+");
    if( !fp )
    {
	    //PSP_Window::showMessage("Creating a new book mark \n 书签文件不存在 新建一个");
	    fp = fopen( BookMarkHandler::bookMarkFileName, "wb");
	    fseek( fp , 0, SEEK_SET);
	    //create a new empty bookmark 
        fwrite (  allBookMark  , 1, maxBookMarkCount * sizeof( BookMarkStruct ), fp );               
	    r = false;
	    
	}
	else
	{
		fseek( fp , 0, SEEK_END);
	    unsigned long bmfilesize ; 
	    bmfilesize = ftell ( fp ); 		
		
	    if ( bmfilesize != sizeof( BookMarkStruct )* maxBookMarkCount )
        {
	      fseek( fp , 0, SEEK_SET);
	      //create a new empty bookmark 
          fwrite (  allBookMark  , 1, maxBookMarkCount * sizeof( BookMarkStruct ), fp );               
          
          r = false;
       }else
       {
	       r = true;
	   }
		
	}  
      
    fclose ( fp );
    return r;
    
}



bool BookMarkHandler::save (  int index    )
{
	 
    sanityCheckingAndCreateNewBookMarkFile();    
	FILE * fp;
    fp = fopen( BookMarkHandler::bookMarkFileName, "rb+");
	if ( !fp ) 
	{
	   return false;
    }
    BookMarkStruct *target;
    target = &allBookMark[index];
        
    
	memcpy ( target, &currentBookMark, sizeof(BookMarkStruct));	         
	
    fseek( fp, 0,  SEEK_SET);    
    fwrite(  allBookMark ,1 , sizeof( BookMarkStruct )* maxBookMarkCount  , fp  );
    
    fclose ( fp );
   // PSP_Window::showMessage( target->filename , target->position );
    return true;
	
}


//for poweron load, only load info and draw the button. do not modify current bookmark;
bool BookMarkHandler::load (   )
{
	bool r = sanityCheckingAndCreateNewBookMarkFile();
	//the file doesn't exist, so the loading should not go on.
	if ( !r ) 
	{
	   return false;
    }	
	
	FILE * fp;	
    fp = fopen( BookMarkHandler::bookMarkFileName, "rb"); 
    fseek( fp , 0 ,  SEEK_SET);
    fread(  BookMarkHandler::allBookMark, 1, 
           maxBookMarkCount*sizeof( BookMarkStruct ), fp  );
    
    fclose ( fp );
        
    return true;
	
	
	
}
bool BookMarkHandler::load ( int index  )
{
	//load (  );
	memcpy ( &currentBookMark, &allBookMark[index], 
	         sizeof(BookMarkStruct) );
    
    return true;
	
}

/*
BookMarkStruct * BookMarkHandler:: getBookMarkInfo ( void )
{
        
    return  &currentBookMark;
	
	
}
*/



//====================================================================================================


BookMarkWindow ::BookMarkWindow (  void  ) :PSP_Window(  )
{
    this->setRectangle ( 35, 60, 398, 150 );
    this->setColor ( 0xffff , 1 );

    int xStart = 20, xStep = 73,  ySave = 30, yLoad = 100 ;
	int i;
	for( i = 0;i< BookMarkHandler::maxBookMarkCount;i++)
	{
	   this->btn_load_a[i]= new Button ( this,  xStart + xStep*i, yLoad );  
	   this->btn_save_a[i]= new Button ( this,  xStart + xStep*i, ySave );  
	   char btnCaption[20]={""};
	   sprintf( btnCaption, "Load%u",i+1);	   
	   this->btn_load_a[i]->setCaption( btnCaption );
	   this->btn_load_a[i]->setFocused( false );
	   btnCaption[0]=0;
	   sprintf( btnCaption, "Save%u",i+1);	   
	   this->btn_save_a[i]->setCaption( btnCaption );
	   this->btn_save_a[i]->setFocused( false );
	   
	}
	this->btn_save_a[0]->setFocused();
   
	
	
	wholeScreenBMP.width = PSP_SCREEN_WIDTH_Visible_cg;
    wholeScreenBMP.heigh = PSP_SCREEN_HEIGH_cg ;
    unsigned long wholeScreenLen, temp;
    wholeScreenLen = wholeScreenBMP.width ;
    temp = wholeScreenBMP.heigh ;
    wholeScreenLen = wholeScreenLen * temp;
    wholeScreenBMP.data_p = new unsigned short [ wholeScreenLen  ];
    if( !wholeScreenBMP.data_p )
    showMessage( "new failed: BookMarkWindow ::BookMarkWindow (  void  ) :PSP_Window(  )");
    memset (  (unsigned char *)wholeScreenBMP.data_p , 0, wholeScreenLen*2  );
    
  
  
}


BookMarkWindow ::~BookMarkWindow ( )
{
    int i ;
    delete wholeScreenBMP.data_p;
    for ( i=0; i< BookMarkHandler::maxBookMarkCount; i++)
    {
         delete btn_load_a[i];
         delete btn_save_a[i];    
    }
    
    
		
}


void BookMarkWindow ::addComopnents (   void )
{
	 
    
        
    
	
}

bool BookMarkHandler ::getBookMarkValid( int index )
{
   	BookMarkStruct * bm;
   	static bool powerOnLoaded = false;
   	if ( !powerOnLoaded ) 
   	{
	   	load( );
	   	powerOnLoaded = true;
	}
	
    
    if ( index >= maxBookMarkCount ) return false;
   
   	bm = &allBookMark [ index ];
   	return bm->hasData;
}

void BookMarkWindow ::drawAllItems( void )
{
	int i ;
	showWindow();
    
	for ( i = 0;i< BookMarkHandler::maxBookMarkCount ; i++ )
	{
	    bool valid;
	    //for load
	    valid = BookMarkHandler ::getBookMarkValid(i);
	    Button *btn;
	    btn = this->btn_load_a[i];
	    if ( valid )
	    { 
	        btn->setEnabled( true );
	        
	    }else
	    {
	        btn->setEnabled( false );
	    }		    
	    
	    btn->show();    
	    
	    //for save
	    btn = this->btn_save_a[i];
	    if ( BookMarkHandler::currentBookMark.hasData )
	    {
	       btn ->setEnabled ( true );
	    }
	    else
	    {
	       btn ->setEnabled ( false );
	    }
	    btn->show();
	    
	}
	
	
	
	
}

int BookMarkWindow ::show( void )
{
    BookMarkWindow bmWin;
    PSP_GRAPHICS::getScreenImageToBitMap(  bmWin.wholeScreenBMP );
    DrawAttribute drawAttr;
    drawAttr.x = 0;
    drawAttr.y = 0;
    drawAttr.enlarge = 1;
    drawAttr.drawBackGroudZero = true;
    
    
    //draw to back buffer    
    PSP_GRAPHICS::drawBitMap( drawAttr,  bmWin.wholeScreenBMP );          
    int selectedIndex  = 0;
	 
    int result = saveActionDone;
    //=====================================================================================
    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);    
    
   
    bmWin.drawAllItems();
    
    PSP_GRAPHICS::flipScreenBuffer(); 

    skipCurrentKey( );
    do
    {           
           
        sceCtrlReadBufferPositive(&pad, 1); 
 
        if ( !pad.Buttons )
            continue;   

        if ( pad.Buttons & PSP_CTRL_CIRCLE )
        {
            Button *btn;
            bool enabled ;
            if ( result == saveActionDone )
            { 
               btn = bmWin.btn_save_a [ selectedIndex ];
               enabled =  btn->getEnabled ();
               if ( !enabled ) 
               {  //do nothing
                  // ( "No book mark to save\n 目前没有书签可保存");
                  continue;
               }else
               {
                   
	               bool r;
	               r = BookMarkHandler::save( selectedIndex );
	               if ( r )
	               {
		               PSP_Window:: showMessage( "save book mark done");
                   }
	               else
	               {
		               PSP_Window:: showMessage( "save book mark error");
		           }
               }
               break;
               
            
            }else if ( result == loadActionDone )
            {
               btn = bmWin.btn_load_a [ selectedIndex ];
               enabled =  btn->getEnabled ();
               if ( !enabled )
               {  continue;
               }else 
               {
                  bool r =  BookMarkHandler::load( selectedIndex );
                  if( r )
                  {
	                  result = loadActionDone;
                  }else
                  {   // wrong book mark;
	                  result = doneReturn;
	              }
                  
               }
               break;
               
            }
            
        } 
        
        if (pad.Buttons & PSP_CTRL_CROSS)
        {
            result = doneReturn;
	        break;
        } 
 
        if ( pad.Buttons & PSP_CTRL_LEFT )
        {   
            
            if ( selectedIndex > 0 )
            {
               if ( result == saveActionDone ) 
               {
                  bmWin.btn_save_a[ selectedIndex ]->setFocused( false );
               }else
               {
                  bmWin.btn_load_a[ selectedIndex ]->setFocused( false );
               }
               selectedIndex --;
               if ( result == saveActionDone ) 
               {
                  bmWin.btn_save_a[ selectedIndex ]->setFocused( true );
               }else
               {
                  bmWin.btn_load_a[ selectedIndex ]->setFocused( true );
               }
               
            }   


        }
        if (pad.Buttons & PSP_CTRL_RIGHT )
        { 
            if ( selectedIndex + 1 <  BookMarkHandler::maxBookMarkCount )
            {
               if ( result == saveActionDone ) 
               {
                  bmWin.btn_save_a[ selectedIndex ]->setFocused( false );
               }else
               {
                  bmWin.btn_load_a[ selectedIndex ]->setFocused( false );
               }            
                selectedIndex ++;
               if ( result == saveActionDone ) 
               {
                  bmWin.btn_save_a[ selectedIndex ]->setFocused( true );
               }else
               {
                  bmWin.btn_load_a[ selectedIndex ]->setFocused( true );
               }               
            }
        } 
        if (pad.Buttons & PSP_CTRL_UP )
        {
            if ( result == loadActionDone ) 
            {
                 bmWin.btn_load_a[ selectedIndex ]->setFocused( false );
                 bmWin.btn_save_a[ selectedIndex ]->setFocused( true );
                 result = saveActionDone;
            }
               
        }
        if (pad.Buttons & PSP_CTRL_DOWN )
        {
            if ( result == saveActionDone ) 
            {
                 bmWin.btn_load_a[ selectedIndex ]->setFocused( true );
                 bmWin.btn_save_a[ selectedIndex ]->setFocused( false );
                 result = loadActionDone;
            }
        }
        
        

          bmWin.drawAllItems();
  
         
        PSP_GRAPHICS::flipScreenBuffer(); 
        wait(  );     
    }while ( true );
    PSP_GRAPHICS::drawBitMap( drawAttr, bmWin. wholeScreenBMP );
    PSP_GRAPHICS::flipScreenBuffer(); 
    skipCurrentKey();
	return result;
}	
 
void BookMarkWindow :: restoreWindow( void )
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






//====================================================================================================


TextViewPad ::TextViewPad (   )  :PSP_Window( )
{
    //this->inited = false;
    textViewScrollBar = NULL; 
    lineHeadStorep = NULL;
    initTextViewPad();
    
}




TextViewPad ::TextViewPad (  const Rectangle &rec )   :PSP_Window( rec)
{
    //this->inited = false;
    textViewScrollBar = NULL; 
    lineHeadStorep = NULL;
    initTextViewPad();
    
}



void TextViewPad ::initTextViewPad ( void )
{


    this->showWindowBorder = false;
    //======================================================================
    loadedBufferSize = 0;
    unicodeBuffer = 0;
    
    spaceSpareLineEnd = 8;
    
    bookMarkLine = 0;
    fontSize = 0;
    //    inited = false;
    //======================================================================
    //loadConfig();
    
    if( !textViewScrollBar )
    this->textViewScrollBar = new ScrollBar( this );
    
    if( !lineHeadStorep ) 
    lineHeadStorep = new LineHeadStore ;
    

}




void TextViewPad ::loadFontFile( void )
{
 //   char msg[50];
    bool r;   

    r = pspTrueTypeFont_cgp ->loadFontFile( config.trueTypeFontFilePath );    
    

    if( !r )
    {
        //there is NO TTF font file loaded, let's reset the config
        config.setIsUsingTTF( false );        
        return;
    }else
    {
//!!! if font not loaded , should not set size
       r = pspTrueTypeFont_cgp  ->setTTFFontSize ( config.fontSize  );
       //!!!
       if( !r )
       {
           showMessage("set font size error, maybe the size is too large/small.");
           config.fontSize = 14 ;
           r = pspTrueTypeFont_cgp  ->setTTFFontSize ( config.fontSize  );
           
       }
    }
  
    if( debug_fileView_cg )
        showMessage("loadFontFile() font Size set");
     
}

void TextViewPad ::applyConfig( void )
{
    unsigned short lastpart ;
    float percentShowHeighThenConsiderShown = 0.80;
    if ( config.getIsUsingTrueTypeFont() )
    {   
        loadFontFile();
        //当字体小于15的时候 ,asc font width会不正常, 后果是屏幕打印出界
        //====================================
        //ucharBitMap * ucharbitmap = 0;
        //use this 'w' to get the ASC width
        //HanZi hanzitemp; hanzitemp.ushort = 'M';
        //ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap ( hanzitemp    );
         
        //====================================
        //ascFontSize  = //ucharbitmap->width; 
     //   showMessage( "M的长度是 ", ucharbitmap->width );
        //必须要保留, 意义是为了非字母的符号的长度计算.
        ascFontSize  = this->config.fontSize /2  ;// + this->config.fontSize%2;
        
        fontSize = this->config.fontSize ;
        maxLineOnePage = ( this->getWindowHigh() - TextViewPadWinYStart ) / ( this->config.fontSize + this->spaceBetweenLine ) ;
          
         //如果最后一行可以显示, 但是spaceBetweenLine无法显示, 那么可以认为最后一行可以显示, 
         //如果最后一行只是缺一点点,那么也当最后一行可以显示
        
        lastpart  = ( this->getWindowHigh() - TextViewPadWinYStart )  % ( this->config.fontSize + this->spaceBetweenLine ) ;
        if ( lastpart >= config.fontSize * percentShowHeighThenConsiderShown  )
        {  maxLineOnePage ++;
        }
        
        
        
        
        if( debug_fileView_cg )
            showMessage("applyConfig() using TTF font");
    }else
    {
        ascFontSize  = DotFont16ASCfont_width;
        fontSize = dotFontHanZiFontWidth;
        maxLineOnePage = ( this->getWindowHigh() - TextViewPadWinYStart ) / (  dotFontHanZiFontHeigh + this->spaceBetweenLine ) ;
        lastpart  =      ( this->getWindowHigh() - TextViewPadWinYStart ) % (  dotFontHanZiFontHeigh + this->spaceBetweenLine ) ;
        if ( lastpart >= dotFontHanZiFontHeigh *   percentShowHeighThenConsiderShown  )
        {  maxLineOnePage ++;
        }
        
        if( debug_fileView_cg )
            showMessage("applyConfig() NOT using TTF font");
    }
    if( this->config.pageTurnKeepOneLine ) 
    {   stepPage = maxLineOnePage - 1;
    }
    else
    {
       stepPage = maxLineOnePage ;
    }
    this->backgroudColor = this->config.backGroudRGBColor.getColor();
    //this->fontColor  = this->config.fontRGBColor.getColor();

    //======================================================================


}






TextViewPad ::  ~TextViewPad ()
{

    if ( unicodeBuffer ) delete unicodeBuffer;
    if ( lineHeadStorep ) delete lineHeadStorep ;
    
    delete textViewScrollBar;
    
}




void TextViewPad :: loadConfig( void )
{
    this->config.load();
}




TextViewPad ::TextFileType TextViewPad ::getTextFileUnicodeType( const  unsigned char * textBuffer )
{
    if( textBuffer == 0 ) return normalText;

    if( strlen( (const char *) textBuffer ) < 2 ) return normalText;

    static const unsigned char unicodeFlag1 = 0xff, unicodeFlag2= 0xfe;
    if ( textBuffer [0] == unicodeFlag1 && 
        textBuffer [1] == unicodeFlag2  
        )
    {
        return unicodeText;	
    }

    if ( textBuffer [0] == unicodeFlag2 && 
        textBuffer [1] == unicodeFlag1  
        )
    {
        return unicodeBigEndText;	
    }

    return  normalText;


}

bool TextViewPad :: addLineHeadIndex ( unsigned long headIndex )
{
    //unsigned long *p;
    //p = new unsigned long ;
    //*p = headIndex;
    bool r;
    //r = this->lineHeadIndex_array.add( p );
    r = this->lineHeadStorep->add ( headIndex );
    return r;

}



//========================================================================================








#ifndef __PSPSDK__
#endif









//How about super big file ??
bool  TextViewPad :: loadTextFile( const char *_filename, unsigned long fromPosition   )
{

    
    if ( unicodeBuffer ) delete unicodeBuffer;
    filename [0]=0;
    TextFileType textFileType;
    //this->unicodeBuffer

    FILE *fp = 0;    
    fp = fopen ( (  char *)_filename, "rb" );
    
    if ( !fp )
    { 
        CommonString errorMsg;
        errorMsg = _filename;
        errorMsg.append( ( unsigned char *) ": Open file error" );

#ifdef __PSPSDK__
        this->showMessage(  ( char *)errorMsg.getChar_p_readonly() );
#else
        printf( "%s\n",errorMsg.getChar_p_readonly() );
#endif

        return false;
    }else
    {
    
      // showMessage( "open file done ");

    }

    fseek ( fp , 0, SEEK_END );
    fileSize = ftell ( fp );
    fseek ( fp , 0, SEEK_SET );
    
    //char str [100];
    //sprintf (str, "file size is %u ", fileSize );
    

    if ( fileSize <=0)
    {
        showMessage( "fileSize =0 , This is an empty file\n 这是一个空文件" );
        fclose( fp );
        loadedBufferSize = 0;
        return false ;

    }
    if ( fileSize >= 2 )
    {
        static const int unicodeTagLen = 2;
        unsigned char testUnicode[ unicodeTagLen ];
        //showMessage( "reading file ");
        fread ( testUnicode ,1,  unicodeTagLen , fp );
        //showMessage( "reading file done");
        textFileType = getTextFileUnicodeType(  testUnicode );

    }else
    {   // the file len less than 2 , it must be normal text file.
        textFileType = normalText;

    }

    unsigned long readLen = 0;
    unsigned long readStartPosition;
    if ( fileSize <= maxFileLoadLengthOneTime )
    {

    }else
    {
        fileSize = maxFileLoadLengthOneTime ;
    }

    
    //showMessage( " == normalText");
    if ( textFileType  == normalText ) 
    { 
        //Convert normal Text to unicode format
        unsigned long indexTempBuffer = 0, indexUnicode = 0;
        unsigned char * tempBuffer;
        readStartPosition = 0;
        readLen = fileSize - readStartPosition ;
        if( readLen >= ( maxFileLoadLengthOneTime/2 - 1 )  )
        {
           readLen = maxFileLoadLengthOneTime /2 - 1 ;
        }
        tempBuffer = new unsigned char [ readLen + 1 ];
        if( !tempBuffer )
        {
	        showMessage(" new failed: tempBuffer = new unsigned char [ readLen + 1 ];");
	        return false;
	    }
        memset ( tempBuffer, 0, readLen + 1  );        
        fseek ( fp , readStartPosition, SEEK_SET );
        fread ( tempBuffer ,1,  readLen , fp );
        tempBuffer [ readLen ] = 0;
        fclose( fp );
        //showMessage(" done readfile to tempBuffer");
        
        unicodeBuffer = new unsigned short [ readLen * 2 ];
        
        if( !unicodeBuffer )
        { 
	       showMessage(" new failed:  unicodeBuffer = new unsigned short [ readLen  ];");
	       delete tempBuffer;
	       return false;
        }
        
        memset ( unicodeBuffer, 0, readLen *2  );        
        do
        {
            HanZi  hanzi;
            if ( indexTempBuffer >= readLen )
                break;
            unsigned char ch;
            ch = tempBuffer[indexTempBuffer];
            if( ch < 128 )
            {
               {
                   indexTempBuffer ++;
                   this->unicodeBuffer[ indexUnicode ] = ch ;
                   indexUnicode ++;  
                }   
            }
            else
            {
                hanzi.ch[0]= tempBuffer[indexTempBuffer  ];
                hanzi.ch[1]= tempBuffer[ indexTempBuffer+1];                
                this->unicodeBuffer [ indexUnicode  ] =
                    PSP_ChineseUtil::fullGBKToUnicode ( hanzi.ushort );
               // showMessage( "unicode is " , this->unicodeBuffer [ indexUnicode  ]  );    
                indexTempBuffer+=2;  
                indexUnicode ++;  
            } 


        }while( true );

        loadedBufferSize  = indexUnicode ;        

        delete tempBuffer;
    }	
    else 
    {	
        //the file is unicode , just read.
        readStartPosition = 2;
        readLen = fileSize - readStartPosition ;
        loadedBufferSize = readLen /2 ;	
        this->unicodeBuffer = new unsigned short [ loadedBufferSize  ];
        if( !this->unicodeBuffer )
        {
	        showMessage( "this->unicodeBuffer = new unsigned short [ loadedBufferSize  ];");
	        return false;
	    }
        memset ( (unsigned char *)unicodeBuffer, 0, readLen    ); 
        fseek ( fp , readStartPosition, SEEK_SET );
        fread ( (unsigned char *)unicodeBuffer,1,  readLen  , fp );

    }	

    fclose( fp );


    //unicodeBigEndText  unicodeText 
    if (  textFileType == unicodeBigEndText )
    {   
        PSP_ChineseUtil::convertUnicodeBigEnd  ( unicodeBuffer , loadedBufferSize   ) ;        
    }
    filename[0]=0;
    strcpy( filename, _filename );
    
    return true;


}







#ifdef __PSPSDK__

 




void TextViewPad ::readMoreFileFromBookMark ( void )
{
	
    TextViewPad  *viewPadp = new TextViewPad ; 
    if( !viewPadp  )
    {
	    showMessage(" new failed: TextViewPad  *viewPadp = new TextViewPad ; "); 
	    return;
	}
    viewPadp ->initTextViewPad();
    viewPadp->loadConfig();
 
    viewPadp->applyConfig();  
    int r;
    do
    {
       
	   normalSpeedCPU();
	   r = viewPadp->loadTextFile( BookMarkHandler ::currentBookMark.filename , 0 );
	   if( !r ) 
	   {
	      //showMessage("Loading file error.");
	      r = quitTextView;
	      break;
		       
       }else
       { 
	       viewPadp->lineHeadStorep->clear();           
           viewPadp->getAllLineHead();
       }
       
       viewPadp->applyBookMarkLine( );
       
       //slowDownCPU();
       r = viewPadp->readFileFromBuffer ( );
       //normalSpeedCPU();
       
       if ( r == quitTextView )
       {
	      break;	   
	   }
	   
	   
   } while( true );
   
   delete viewPadp;
	
		
}

void TextViewPad ::readMoreFileFromFileMgr ( const   char *fileName )
{
	
        
    if( BookMarkHandler ::currentBookMark.filename != fileName  )
    {
	   BookMarkHandler ::currentBookMark.filename[0]=0;
       strcpy( BookMarkHandler ::currentBookMark.filename, fileName );    
	}
    BookMarkHandler ::currentBookMark.hasData = true;    
    BookMarkHandler ::currentBookMark.position = 0;
    readMoreFileFromBookMark();
    	

}











void   TextViewPad :: applyBookMarkLine( void )
{
   
   //unsigned long fromIndex = 0 , charCount = 0;   
   //bookMarkLine must reset 
   this->bookMarkLine = 0;
  
   
   // if current book mark has no data.
   if ( !BookMarkHandler ::currentBookMark.hasData )
   { 
       
	   return;
   }
   
      
      
   if ( BookMarkHandler ::currentBookMark.position >= loadedBufferSize )
   return;

   if ( BookMarkHandler ::currentBookMark.position <=0 ) return;

   this->bookMarkLine   =  lineHeadStorep->getLineNumber ( BookMarkHandler ::currentBookMark.position );
   
}



unsigned long TextViewPad :: countLinesDown( unsigned long currentLineNumber, 
                                             unsigned long step, unsigned long totalLine )
{
    static const unsigned short spareForFileEnd = 2;
    if ( currentLineNumber + step + 1 < totalLine  )
    {   
        currentLineNumber += step;
    }
    else
    {
	    if( totalLine >= spareForFileEnd  ) 
	    {   
		   currentLineNumber = totalLine - spareForFileEnd  ;
	    }else
	    {
		   currentLineNumber = 0;
        }
    }	
    return currentLineNumber ;
}


unsigned long TextViewPad :: countLinesUp( unsigned long currentLineNumber,
                                           unsigned long step   )
{
	 
    if ( currentLineNumber <=0 )
    return currentLineNumber ;
           
    if ( currentLineNumber >= step )
    {
       currentLineNumber -= step;
    }else
    {
       currentLineNumber = 0;
    }   
    return currentLineNumber ;
		
}




int   TextViewPad :: readFileFromBuffer ( void )
{
    
    if ( ! unicodeBuffer )
    {
        this->printTTFHanZiString( 0, 0, this->config.fontRGBColor.getColor() , 
              ( unsigned char *)"an empty file ...\n <这是一个空文件>" );
        PSP_GRAPHICS::flipScreenBuffer();
        getch();          
    }
    
    //
    static const int turnPageWait = 25;
    unsigned long lineNumber = 0, totalLine = 0 ;

    /** !!!
    totalLine = this->lineHeadStore.getSize();
    */

    //=====================================================================
    // Get Key input 
    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);    

    
    
    

    showFile ( this->bookMarkLine  ); 
    lineNumber = this->bookMarkLine ;
    
    totalLine = this->lineHeadStorep->getSize();
    unsigned long nextLineNumber;
    unsigned short analongStkUpStep = stepPage, analongStkDownStep = stepPage;
    skipCurrentKey();
    slowDownCPU();    
    do
    {           
        
	    sceCtrlReadBufferPositive(&pad, 1); 


        if ( pad.Ly < controlPositionUp )
        {
            normalSpeedCPU();
            analongStkDownStep = stepPage;
            analongStkUpStep +=2;
            nextLineNumber = countLinesUp( lineNumber, analongStkUpStep  );
            if( nextLineNumber != lineNumber ) 
	        {
		       lineNumber = nextLineNumber ;
	           showFile ( lineNumber  ); 
	           
            }
            slowDownCPU();    
            continue;
            
        }else if ( pad.Ly >  controlPositionDown  )
        {
            normalSpeedCPU();
            analongStkUpStep = stepPage;
            
            analongStkDownStep += 2;
            nextLineNumber = countLinesDown( lineNumber, analongStkDownStep , totalLine );
            if( nextLineNumber != lineNumber ) 
	        {
		       lineNumber = nextLineNumber ;
	           showFile ( lineNumber  ); 
            }            
            slowDownCPU();
            continue;            
        }
        else 
        {   // analog stick is not moved, so reset the step
	        analongStkUpStep = stepPage;
	        analongStkDownStep = stepPage;
	    }

        if ( !pad.Buttons )
        {    
	        continue;   
        }
        
        normalSpeedCPU();
        
          
        if (pad.Buttons & PSP_CTRL_LTRIGGER )
        {	        
	        nextLineNumber = countLinesUp ( lineNumber, stepPage );
	        if( nextLineNumber != lineNumber ) 
	        {
		       lineNumber = nextLineNumber ;
	           showFile ( lineNumber  ); 
	           //wait( turnPageWait );
	           skipCurrentKey();
            }
            
               	    
        }else if( pad.Buttons & PSP_CTRL_UP)
        {            
	        nextLineNumber = countLinesUp ( lineNumber, stepPage );
	        if( nextLineNumber != lineNumber ) 
	        {
	            lineNumber = nextLineNumber ;
	            showFile ( lineNumber  ); 
	            //wait( turnPageWait );
	            skipCurrentKey();
	            
            }                  
            
        
        } else if (pad.Buttons & PSP_CTRL_LEFT)
        {
 	        unsigned long nextLineNumber;
	        nextLineNumber = countLinesUp ( lineNumber, 1 );
	        if( nextLineNumber != lineNumber ) 
	        {
	            lineNumber = nextLineNumber ;
	            showFile ( lineNumber  ); 
	            wait( turnPageWait );
	            //skipCurrentKey();
            }
            
        }       
        else if (pad.Buttons & PSP_CTRL_RTRIGGER )
        {   
	        nextLineNumber = countLinesDown( lineNumber, stepPage, totalLine );
            if( nextLineNumber != lineNumber ) 
	        {
	            lineNumber = nextLineNumber ;
	            showFile ( lineNumber  ); 
	            //wait( turnPageWait );
	            skipCurrentKey();
            }
                        
	    }else if ( pad.Buttons & PSP_CTRL_DOWN )
        {   
            
	        nextLineNumber = countLinesDown( lineNumber, stepPage, totalLine );
            if( nextLineNumber != lineNumber ) 
	        {
	            lineNumber = nextLineNumber ;
	            showFile ( lineNumber  ); 
	            //wait( turnPageWait );
	            skipCurrentKey();
            }           
            
            
        }else if (  pad.Buttons & PSP_CTRL_RIGHT )  
        {           
            nextLineNumber = countLinesDown( lineNumber, 1, totalLine );
            if( nextLineNumber != lineNumber ) 
	        {
	            lineNumber = nextLineNumber ;
	            showFile ( lineNumber  ); 
	            wait( turnPageWait );
	            //skipCurrentKey();
            }            
            
            
        }else if (pad.Buttons & PSP_CTRL_CROSS)
        {
            bool r ;
            ConfirmWindow *confWinp;
            confWinp = new ConfirmWindow;
            if( !confWinp )
            {   // new memory failed
	           return quitTextView;
            }   
            
            r = confWinp->show( "Quit reading the file ?" );
            delete confWinp;
            if ( r ) 
            {
	            BookMarkHandler::currentBookMark.hasData = false;
	            BookMarkHandler::currentBookMark.position = 0;
	            midSpeedCPU();
	            return quitTextView;
            }    

        }else if ( pad.Buttons & PSP_CTRL_CIRCLE)
        {
            //handleKeyCirclePressed();

        } 
        else if ( pad.Buttons & PSP_CTRL_TRIANGLE )
        {
	        int r;
            	        
	        r = handleBookMarkWin(); 	       
	        if ( r == loadBookMark )
	        {
		       return loadBookMark ;
            }	        
	        wait(  );
	    }
	    else if ( pad.Buttons & PSP_CTRL_SQUARE )
	    {
		    //showMessage ("bookMarkLine is ", lineNumber  );
		     
		}else if (  pad.Buttons & PSP_CTRL_START )
		{
			
			
		}
        else
        {
	          slowDownCPU();
	          continue;
	    } 
	    slowDownCPU();
        
       
    }while( true );
    
    return quitTextView;

}




#endif  //__PSPSDK__


void TextViewPad :: showFile ( unsigned long lineNumber )
{

#ifdef __PSPSDK__    
    PSP_Window::showWindow();    
#endif

    unsigned long fromIndex = 0, maxLine ;//, unicodeBufferSize = 0 ;    
    unsigned short showLineCount = 0;    
    unsigned short winY = TextViewPadWinYStart ;    
    
    if(  this->lineHeadStorep )
    fromIndex = this->lineHeadStorep->get ( lineNumber );
        
    BookMarkHandler ::currentBookMark.position = fromIndex ; 
    //showMessage( "percent is ", fromIndex / loadedBufferSize );
    if( textViewScrollBar )
    {  textViewScrollBar->setPercent ( fromIndex , loadedBufferSize );
    }else
    {
          showMessage("NULL : textViewScrollBar->setPercent ( fromIndex , loadedBufferSize );");
          return ;
     }

    do
    {
   
        maxLine = this->lineHeadStorep->getSize();
        if ( lineNumber > maxLine )
        {
            break;
        }
        if ( showLineCount > maxLineOnePage ) 
        { 
            break;
        }       

        fromIndex = this->lineHeadStorep->get ( lineNumber );
        unsigned long showLength = 0;;
        showLength =  this->lineHeadStorep->get ( lineNumber + 1 );
        if ( showLength == 0 )
        {
           //已经是 最后行了
           showLength = loadedBufferSize - fromIndex;
        }else
        {
           showLength = showLength - fromIndex ;
        }
        
        
        if ( fromIndex == 0 && lineNumber > 0 )
        {
            //fromIndex = 0, means all line is done
            break;
        } 	
        
        
        if( this->config.getIsUsingTrueTypeFont() )
        {
            //Using True Type Font
            this->printTTFHanZiUnicodeString
                ( 0, winY, this->config.fontRGBColor.getColor(), 
                  unicodeBuffer + fromIndex , showLength );//this->loadedBufferSize - fromIndex   );

      //      this->lineHeadStorep->add( fromIndex + ascCount   );
            winY += ( this->config.fontSize + this->spaceBetweenLine );

        }else //using DOT Font Matrix HZK16
        { 
            printUnicodeAscHanZi16String 
                    ( 0, winY, this->config.fontRGBColor.getColor(), 
                      unicodeBuffer + fromIndex , showLength );//this->loadedBufferSize - fromIndex );
        //    this->lineHeadStorep->add( fromIndex + ascCount   );
            winY += ( DotFont16ASCfont_heigh + this->spaceBetweenLine );
        }    

        lineNumber ++; 
        showLineCount ++;
        
    }while( true );   
    
     
    textViewScrollBar->showScrollBar ( );
#ifdef __PSPSDK__
    PSP_GRAPHICS::flipScreenBuffer();
    sceDisplayWaitVblankStart();    	
#endif

    
    return ;
}



/**
Count how much char(unsigned short count) can print in one line
*/

//注意 AAbasdfasdfweasdfsadferwefasfsa这种情况, 
//aasdfsdafasdfsdafasdfasdf_______|
//asdfasdfsadfsdaf
//换行前会空一大段, 
//是因为省空间给英文单词.
//然而它超过12个最大单词的长度 所以转弯换行了. 是正确的结果.
unsigned long TextViewPad :: countOneLineHanZiUnicodeString
( const unsigned short *unicodeStr, unsigned long size  ) 
{

    unsigned short  winX = 0 , winY = 0;
    //ucharBitMap * ucharbitmap = 0;
        
    if( unicodeStr == NULL) 
    {  
        return 0;
    }
    
    
    unsigned long index = 0, charCount = 0;

    if (size <=0 ) return 0;
    bool r;
    HanZi tempHZ;
     
    tempHZ.isUnicode = true;
  //  bool showmsg = false ;      
    while(true)
    {   
       
        //eg. if size =5, then index can be 4 at largest
        if( ( index + 1 ) > size  ) break;

        tempHZ.ushort = unicodeStr[ index ];        
        
        bool isHanZi;
        isHanZi = PSP_ChineseUtil::isThisAHanZi ( tempHZ.ushort  ); 
         
        if (  isHanZi )    
        {
            
            r = this->wrapLine ( winX, winY, fontSize , fontSize );
            if ( !r )
            {   
                break;
            }
            //一般来说, 汉字并不会都会满满的宽fontSize.
            //无论16dot还是ttf 都是这样算
            winX += (fontSize );// + spaceBetweenChar;
            index ++;             
            continue;                

        }else if ( tempHZ.ushort == returnSymbolUnicode0D  )
        { 
            
            index ++;            
        }
        else if ( tempHZ.ushort == returnSymbolUnicode0A )
        {            
               
            index ++;
            break;
        }
        else if ( tempHZ.ushort ==  spaceSymbolUnicode )
        {           
           
            r = wrapLine ( winX, winY,  ascFontSize, fontSize  );
            if ( !r )
            {  
               //index ++ here means consumes a space.
               index ++; 
               //window full
               break;
            }else
           {
               index ++; 
               winX += ascFontSize ;
           } 
            
        }
        else 
       {            
            unsigned short englishWordLen ;
            
            englishWordLen =  CommonString::getUnicodeEnglishWordLen 
                       ( unicodeStr + index ,  size - index  );
            //this is NOT a english word, so It just takes ONE ASC.
            if ( englishWordLen <= 0 )
            {    
                 unsigned short tempascFontSize  ;
                 if(  this->config.getIsUsingTrueTypeFont() )
                 {   //字符系的长度跟是否ttf有关的, 
                     ucharBitMap * ucharbitmap = 0; 
                     HanZi hanzitemp; 
                     hanzitemp.ushort =   unicodeStr [ index ] ;
                     ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap ( hanzitemp  );                 
                     tempascFontSize = ucharbitmap->width ;                
                 }else
                 {
                    tempascFontSize  = DotFont16ASCfont_width;// + spaceBetweenChar;
                 }                  
                 r = wrapLine ( winX, winY, tempascFontSize  , fontSize );
                 if ( !r )
                 {  //window full
                    break;
                 }else
                 {
                    index ++ ;
                    winX += tempascFontSize  + spaceBetweenChar  ;
                 }                             
            }
            else
            {   //英语单词字符串
                unsigned short   engWordWidth ;//, needX = 0; 
          
                engWordWidth  = countEnglishWordWidth( unicodeStr + index , englishWordLen );    
               
                r = wrapLine ( winX, winY, engWordWidth, fontSize );

                if ( !r )
                {   //window full
                    break;
                }else
                {
                    index += englishWordLen;
                    //winX += needX;
                    winX += engWordWidth;
                }                
            }
        }//normal ASC
    }// While () 
    charCount = index;
    return charCount ;
}  


unsigned short TextViewPad :: countEnglishWordWidth( const unsigned short *uniStr,  unsigned short wordAscCount )
{
    ucharBitMap * ucharbitmap = 0; 
    HanZi hanzitemp; 
    unsigned short  i, englishWordWidth = 0 ;
    //use this 'w' to get the ASC width
    
    if( this->config.getIsUsingTrueTypeFont() )
    {   //true type font , get length from bitmap width
        for( i = 0 ; i < wordAscCount ; i++ )        
        {
           hanzitemp.ushort = uniStr[i];
           //showMessage("countEnglishWordWidth () " , hanzitemp.ushort );
           ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap ( hanzitemp    );
           //对TTF的英文数字来说, 本身图就很小, 最好要加上间距值
           englishWordWidth += ucharbitmap->width + spaceBetweenChar;
        }         
    }else
    {
        //englishWordWidth +=  (DotFont16ASCfont_width + spaceBetweenChar  ) * wordAscCount ;
        englishWordWidth +=  (DotFont16ASCfont_width   ) * wordAscCount ;
    }
        
    return englishWordWidth ;
}

//print one line
unsigned long TextViewPad :: printTTFHanZiUnicodeString
       ( unsigned short winx,unsigned short winy,unsigned short color,
         const unsigned short *unicodeStr, unsigned long size  ) 
{
     //PSP_Window::showMessage("TextViewPad :: printTTFHanZiUnicodeString () 1");
     if( unicodeStr == NULL) 
     {  
        return 0;
     }
     HanZi tempHZ;
     //unsigned short _fontSize = config.fontSize;
     unsigned long totalCharToPrint, index = 0 ;
     totalCharToPrint = size;//countOneLineHanZiUnicodeString( unicodeStr, size );     
     
     tempHZ.isUnicode = true;
     ucharBitMap * ucharbitmap = 0;

     do
     {
         if( index >= totalCharToPrint ) break;
         tempHZ.ushort = unicodeStr[ index ];
         
         if ( tempHZ.ushort == spaceSymbolUnicode )
         {
              winx += ascFontSize  ; 
              index++;
              
         }else if ( tempHZ.ushort == returnSymbolUnicode0A ||
                    tempHZ.ushort == returnSymbolUnicode0D 
                  )            
         {
              break;
         }else
         {         
             
            ucharbitmap = PSP_ChineseUtil::getHanZiTTFBitMap ( tempHZ    );
               
#ifdef __PSPSDK__
            this->drawBitMap( winx, winy,  color,  *ucharbitmap );         
#else
            PSP_ChineseUtil:: printBitMapInTextMode ( *ucharbitmap  );  
#endif     
            //有时候, 某个怪字的图形是空格, 然后count的时候却当成fontSize, 
            //而画的时候却不占空格, 导致行末出现空余.
            if( ucharbitmap->width <=0 )
            {
               unsigned short stepX ;
               if( PSP_ChineseUtil::isThisAHanZi ( tempHZ.ushort ) )
               {
                   stepX = fontSize;
               }else
               {   stepX = ascFontSize ;
               }
               winx  = winx + stepX ;
            }
            else
            {
                if( PSP_ChineseUtil::isThisAHanZi ( tempHZ.ushort ) )
                {
                   //汉字统一当fontSize 但是不搞间距了.
                   winx  = winx + fontSize;
                }else
                {
                   //英文和数字 TTF, 要加间距, 否则太紧
                   winx  = winx + (ucharbitmap->width + spaceBetweenChar ) ;
                }
            }
            
            index++;
         }
      
     }while( true );

     return totalCharToPrint;

}    



 



//Print Chinese or ASC chars.
//==========================================================================
 //  print Asc (pure english/1234 ? #*$) and Chinese HanZi in graphic mode.
 //  return the how many char has been processed.
 //  this method should be moved to Windwos class.
 //========================================================================== 

unsigned long TextViewPad:: printUnicodeAscHanZi16String
   ( unsigned short winx, unsigned short winy, 
     unsigned short color, const unsigned  short *unicodeStr ,
     unsigned long size ) 

{

     if( unicodeStr == NULL) 
     {  
        return 0;
     }

     unsigned long totalCharToPrint, index = 0 ;
     totalCharToPrint = size;///countOneLineHanZiUnicodeString( unicodeStr, size );     
     HanZi tempHZ;
     tempHZ.isUnicode = true;
     do
     {
         if( index >= totalCharToPrint ) break;
         tempHZ.ushort = unicodeStr[ index ];
         
         if ( tempHZ.ushort == spaceSymbolUnicode )
         {
              winx += ascFontSize  ; 
              index++;              
         }else if ( tempHZ.ushort == returnSymbolUnicode0A ||
                    tempHZ.ushort == returnSymbolUnicode0D 
                  )            
         {
              break;
         }else
         {                        
            //PSP_ChineseUtil:: printBitMapInTextMode ( *ucharbitmap  );  
            if( tempHZ.ushort< 127 )
            {
                this-> drawAscChar8_16Window( winx, winy ,color, tempHZ.ushort  );                
                winx += DotFont16ASCfont_width ;//+ spaceBetweenChar ;
            }
            else 
            {
                this->drawHanZi16Window ( winx, winy ,color, tempHZ  );
                winx += dotFontHanZiFontWidth;  // + spaceBetweenChar ;
            }   
            index++;
         }
      
     }while( true );
     return totalCharToPrint;

}




int TextViewPad:: handleBookMarkWin( void )
{
    int action = 0  ;
    action = BookMarkWindow::show(   );   
    
    if ( action == BookMarkWindow::doneReturn )
    {
        return BookMarkWindow::doneReturn;
    }else if ( action == BookMarkWindow::loadActionDone )
    {
               
        if ( BookMarkHandler::currentBookMark .hasData )
        {   
	        
            return loadBookMark;
            //FireStarReader::viewPad.readMoreFile( bm->filename );           
        }
        else
        {
	        
	    }
  	}else if(  action == BookMarkWindow::saveActionDone )
  	{	  		  	
	  	return BookMarkWindow::doneReturn ;
	}
	return BookMarkWindow::doneReturn ;
   
}



void TextViewPad ::getAllLineHead( void )
{
   unsigned long fromIndex = 0 , charCount = 0;   
   //bookMarkLine must reset 
   /*onePageLinesCount =  this->getWindowHigh()/ 
        ( this->config.fontSize + this->spaceBetweenLine );
        */
   lineHeadStorep->add( 0 );
   do
   {  
     
	 charCount = countOneLineHanZiUnicodeString
              ( this->unicodeBuffer + fromIndex , this->loadedBufferSize - fromIndex );      
     this->lineHeadStorep->add(  fromIndex + charCount   );
     fromIndex += charCount;
     //this->bookMarkLine  ++;
     
     if ( fromIndex >=  this->loadedBufferSize  )
     {   
        break;
     }
     
     if ( charCount <=0 ) break;
     
   
   }while( true );

   //showMessage( "get all line head done");	
	
	
}

void TextViewPad :: showInfo( void )
{
    //TextViewPad ::readMoreFileFromFileMgr ( "ReadmeC_E.txt" );
	
	//char msg[]={""};
}






