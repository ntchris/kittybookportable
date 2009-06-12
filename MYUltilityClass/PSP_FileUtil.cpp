
#include <PSP_Global.h>



#ifdef __PSPSDK__
#include <pspkernel.h>

#include <pspctrl.h>
#endif




#include <stdlib.h> //melloc
//#include <stdio.h>
#include <PSP_FileUtil.h>

#include <CommonString.h>

#include <PSP_GRAPHICS.h>
#include <PSP_ChineseUtil.h>
#include <TextViewPad.h>

//for opendir readdir
#include <dirent.h>






//======================================================================
//   class PSP_FileManager 
//======================================================================


const unsigned char PSP_FileManager::parentDirSymbol[] = {".."};
const unsigned char PSP_FileManager::currentDirSymbol[] = {"."};
const unsigned char PSP_FileManager::msRoot[] = {"ms0:/"};

void ( *PSP_FileManager :: textFileHandler ) ( const char * )  = 0;
void ( *PSP_FileManager :: htmlFileHandler ) ( const char * )  = 0;



// browse need a class var to store the current path
//CommonString  PSP_FileManager ::currentPathString;

void PSP_FileManager ::init( void )
{
    fileCount = 0;
    dirCount = 0;
  
 
    this->setColor( 0xffff   );
    //this->showBorder = false;
    int i;
    for( i = 0 ; i < maxFilesCount; i++ )
    { 
        fileInfoStruc_p[i] = NULL ;
        dirInfoStruc_p[i] = NULL ;
    }
    seletedFileItemIndex = 0;
    //  onScreenItemCount = 0;

    Rectangle tempRec;


    selectBarActive.setColor( 62656 );
    selectBarActive.setRectangle( this->rectangle.x + this->winXStart, 0, 
        this->rectangle.width - 3*borlderLine , dotFontHanZiFontHeigh + PSP_Window::borlderLine );

    /*selectBarNormal.setColor ( this->backgroudColor );
    selectBarNormal.setRectangle ( this->rectangle.x + this->winXStart , 0,
    this->rectangle.width - 3*borlderLine , hanZiFontHeigh + PSP_Window::borlderLine );
    */
    
    
    wholeScreenBMP.width = PSP_SCREEN_WIDTH_Visible_cg;
    wholeScreenBMP.heigh = PSP_SCREEN_HEIGH_cg ;
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
PSP_FileManager ::  PSP_FileManager  (   ) :PSP_Window (    )
{
    init();

}


PSP_FileManager :: PSP_FileManager  (  const Rectangle &rec )  : PSP_Window ( rec  )
{
    init();

}

PSP_FileManager ::~PSP_FileManager ( )
{ 
    clear ();
    delete wholeScreenBMP.data_p ;
}



void PSP_FileManager ::setTitle ( const unsigned char *jisPath )
{
    //unsigned char _title[255];
    //memset( _title, 0 , 255 );
    //PSP_ChineseUtil::jis2cjk (  jisPath ,  _title );
    //PSP_Window::setTitle ( _title );
    PSP_Window::setTitle ( jisPath );
    
}

void PSP_FileManager :: refresh ( void )
{
    /*unsigned char _title[255];
    memset( _title, 0 , 255 );
    PSP_ChineseUtil::jis2cjk (  this->currentPathString.getChar_p_readonly() ,  _title );
    setTitle ( _title );
    */
    restoreBackGround();
    PSP_Window::showWindow();
    showAllItems ( this->showfromFileIndex );
    //drawTitle();
    PSP_GRAPHICS::flipScreenBuffer();
    sceDisplayWaitVblankStart();
}


void PSP_FileManager ::explore (  void )
{
       
    
   
    //reason: nextTime, can go to here
    
    
    
    CommonString pathString;
    pathString = msRoot;
    //this->currentPathString.setString ( msRoot );
    //const unsigned char *p;
    //p = currentPathString.getChar_p_readonly() ;
    /*CommonString pathlocal;
    pathlocal.setString ( p );
    p = pathlocal.getChar_p_readonly();
*/
    //unsigned char buffer[1024];

    PSP_GRAPHICS::getScreenImageToBitMap(  wholeScreenBMP );
   
    do
    {  


        //p = pathlocal.getChar_p_readonly();
        //sprintf( (char *)buffer , "explorePath (p )  p is %s", (char *)p );

      //  this->showMessage ( buffer  );

       // do not care the return value here
       // because it should reEnter forever.
        //explorePath (  this->currentPathString.getChar_p_readonly() );
        
         ExploreResult r;
         r = explorePath (  pathString  );
         if ( r == cancel ) 
         break;
        //pathlocal.setString ( p  );

//        wait(6);

    }while ( true );
    


}


void PSP_FileManager :: restoreBackGround( void )
{
    
     DrawAttribute drawAttr;
     drawAttr.x = 0;
     drawAttr.y = 0;
     drawAttr.enlarge = 1;
     drawAttr.drawBackGroudZero = true;    
     //draw to back buffer    
     
     PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP ); 
     /*PSP_GRAPHICS::flipScreenBuffer();;
     PSP_GRAPHICS::drawBitMap( drawAttr, wholeScreenBMP ); 
     */
        
}

PSP_FileManager ::ExploreResult  PSP_FileManager ::explorePath 
                 (  CommonString &explorePathStr ,  CommonString *selectedFilename_p )
{
    this->setTitle ( explorePathStr.getChar_p_readonly()  );
    this->getAllFileNames ( explorePathStr.getChar_p_readonly() );
    this->seletedFileItemIndex = 0;
    this->showfromFileIndex = 0;
    refresh();
    //=====================================================================
    // Get Key input 
    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
  
    ExploreResult exResult = notDone;
    skipCurrentKey( false);
    midSpeedCPU();
    while( true )
    {
        
        sceCtrlReadBufferPositive(&pad, 1); 
        
        if ( pad.Ly < controlPositionUp )
        {
              handleKeyUpPressed();
              
        }
        if ( pad.Ly >  controlPositionDown  )
        {
              handleKeyDownPressed();
        }
         
        if ( pad.Buttons == 0)
        continue;
        midSpeedCPU();            

        if (pad.Buttons & PSP_CTRL_CIRCLE)
        {
            exResult = handleKeyCirclePressed( explorePathStr, selectedFilename_p  );
            
            pad.Buttons = 0;             

        }else if( pad.Buttons & PSP_CTRL_TRIANGLE)
        {
	         //const SceIoDirent *tempDirt = this->getItem( this->seletedFileItemIndex );
	         const dirent *tempDirt = this->getItem( this->seletedFileItemIndex );
             if ( tempDirt-> d_stat.st_attr == FIO_SO_IFDIR )
             {
	            continue;    //Triangle: for dir is useless
	         }
             
	         if( !selectedFilename_p )
	         {  //do not use in fileSelector mode 
                exResult = handleKeyTrianglePressed( explorePathStr  );
             }
                 
	        
	    }
         else if (pad.Buttons & PSP_CTRL_CROSS)
        {
            if ( selectedFilename_p )
            {  
               selectedFilename_p ->clear();
            }
            pad.Buttons = 0;
            exResult = cancel;
            
        } else if (pad.Buttons & PSP_CTRL_UP ) 
        {
            handleKeyUpPressed();


        }else if (pad.Buttons & PSP_CTRL_DOWN )
        {
            handleKeyDownPressed();

        } 
        if ( exResult != notDone ) break;
        
        skipCurrentKey( false );


        
    }
    //this->showMessage (  ( unsigned char *)"quiting explorePath ");
    return exResult;
}


void PSP_FileManager ::launchFile ( const  CommonString & filename )
{
    const char message [50] = { " Applicatoin for this type  was not registerd."};
    CommonString fileExtStr ;    
    fileExtStr.getFileExtention( filename.getChar_p_readonly() ) ; 
    fileExtStr.toLowerCase();
   
    if( fileExtStr == textFileExt ) 
    {
         if ( this->textFileHandler )
         {
             ( *textFileHandler )( (const char*)filename.getChar_p_readonly());
         
         }else
         {
            showMessage ( message );
         
         }

    }else if ( fileExtStr == htmlFileExt )
    {
          if ( this->htmlFileHandler )
         {
             ( *textFileHandler )( (const char*)filename.getChar_p_readonly());
         
         }else
         {
            showMessage ( message );
         
         } 

    } else if ( fileExtStr == htmFileExt )
    {
          if ( this->htmlFileHandler )
         {
             ( *textFileHandler )( (const char*)filename.getChar_p_readonly());
         
         }else
         {
            showMessage ( message );
         
         }

    }else if (  fileExtStr == mp3FileExt )
    {
         showMessage ( message );
    }
    else if ( fileExtStr == winRarFileExt )
    {
         showMessage ( message );
    }
    else if ( fileExtStr == winZipFileExt )
    {
        // winZip and winrar file use same icon
        showMessage ( message );
    }
    else 
    {
        showMessage ( "file type not recongized. However, you can press Triangle to force read it.\n 没有合适的程序来打开此文件类型, 但是仍然可以按三角来强制查看" );
    }


}

//PSP_FileManager ::ExploreResult  
         //PSP_FileManager :: handlePressDir ( CommonString &updateCurrentPath, const SceIoDirent *tempDirt )         
PSP_FileManager ::ExploreResult  
         PSP_FileManager :: handlePressDir ( CommonString &updateCurrentPath, const dirent *tempDirt )         
         
{

    // handle press <  DIR >
    if ( tempDirt-> d_stat.st_attr != FIO_SO_IFDIR )
    {
        return notDone;
    }   


    CommonString testCurrent, testParent;
    testCurrent.setString( currentDirSymbol );
    testParent.setString ( parentDirSymbol );
    if (  testCurrent == ( const unsigned char *)tempDirt->d_name )
    { 
        // press <.> current dir, do nothing
        return notDone;
    }
    else if (  testParent == ( const unsigned char *)tempDirt->d_name )
    {
        // press <..> to go back to upper dir
        CommonString upperDir  ;
        const unsigned char *temp;
        //upperDir.setString ( ( const unsigned char *) tempDirt->d_name ) ) ;
        //this->showMessage( (const unsigned char *)"getting upper dir " ) ;
        temp =  updateCurrentPath.getChar_p_readonly();
        upperDir.setString( temp );
        temp = updateCurrentPath.getChar_p_readonly();
        upperDir.getUpperDir (  temp  );
        //this->showMessage( upperDir.getChar_p_readonly(  ) ) ;
        temp = upperDir.getChar_p_readonly();
        updateCurrentPath.setString (  temp );

        return reEnter;
    }else
    {


        //enter the dir 
        //=========================================================
        //must use append  ms0:/  append  ms_game/ 
        int pathlength;
        const unsigned char *path;     
        pathlength = updateCurrentPath.getSize();
        path = updateCurrentPath.getChar_p_readonly();

        unsigned char slash ='/';
        if ( path[ pathlength -1 ] != slash )
        {  
            updateCurrentPath.append( ( const unsigned char *) "/" );
        }


        updateCurrentPath.append( ( const unsigned char *)tempDirt->d_name );
        
        updateCurrentPath.append( ( const unsigned char *) "/" );

        return reEnter;
    }


}


PSP_FileManager ::ExploreResult
              PSP_FileManager ::handleKeyTrianglePressed ( CommonString &updatePathString  )
{
   //showMessage( "Open the file in Text mode. \n强制按照文本模式打开文件");	
    //const SceIoDirent *tempDirt = this->getItem( this->seletedFileItemIndex );
    const dirent *tempDirt =   this->getItem( this->seletedFileItemIndex );
    
    if ( tempDirt-> d_stat.st_attr == FIO_SO_IFDIR )
    {
        return notDone;        
    }
    CommonString selectedFile;
    selectedFile =  updatePathString  ;
    selectedFile.append( (unsigned char *) tempDirt->d_name );         
        
    if ( this->textFileHandler )
    {
       ( *textFileHandler )( (const char*)selectedFile.getChar_p_readonly() );
    }else
    {
       showMessage ( "this->textFileHandler  is NULL " );
    }         
    refresh();
    return notDone;
}

//return should I Quit this explore( path ) AndReenter using expore( void ) 
// only return true after change current dir.
PSP_FileManager ::ExploreResult
              PSP_FileManager ::handleKeyCirclePressed
                            ( CommonString &updatePathString,  CommonString *selectedFilename )
{

    //const SceIoDirent *tempDirt = this->getItem( this->seletedFileItemIndex );
    const dirent *tempDirt = this->getItem( this->seletedFileItemIndex );
    if ( tempDirt-> d_stat.st_attr == FIO_SO_IFDIR )
    {
        // Handle Dir        
        ExploreResult r;        
        r = handlePressDir ( updatePathString , tempDirt  );
        if ( r  == reEnter )
        {   
           setTitle ( updatePathString.getChar_p_readonly()  ); 
        }
        return r;
        
    }
    else
    {
        CommonString selectedFile;
        selectedFile =  updatePathString  ;
        selectedFile.append( (unsigned char *) tempDirt->d_name );         
           
        if( selectedFilename )
        {
           // Here Working fileSelector mode  , select a file then doneExplore
           // update the return file name string 
           *selectedFilename = selectedFile;
           //showMessage ( (char*)selectedFilename->getChar_p_readonly() );
           return doneExplore;
        }else
        {
            // working in Explorer mode, select file and then laugch
           launchFile ( selectedFile );
           refresh();
           return notDone;
        }
      
        
    
    }
      
    
   // refresh();
  //  this->showMessage( "Quiting  handleKeyCirclePressed() " );
    return notDone;
   

}

void PSP_FileManager ::handleKeyUpPressed( void )
{

   
    //FileMgrDrawItem *tempDrawItem_p;
    //tempDrawItem_p =  this->drawIndexfrom;
    showfromFileIndex = this->drawIndexFrom;

    if ( seletedFileItemIndex <= showfromFileIndex  )
    {
        return;
    }

    if( seletedFileItemIndex > 0  )
    {
        seletedFileItemIndex --;
    }


    if (  showfromFileIndex >0 )
    {   showfromFileIndex --;
    }

    refresh(); 
//    this->showAllItems( showfromFileIndex );

}

void PSP_FileManager ::handleKeyDownPressed( void )
{
  //  unsigned short showfromFileIndex;
    //FileMgrDrawItem *tempDrawItem_p;
    //tempDrawItem_p = &this->fileMgrDrawItem[ 0 ];
    //the prev from Index
    showfromFileIndex = this->drawIndexFrom;


    if  ( seletedFileItemIndex >=  this->fileCount + this->dirCount -1 )
    {
        return;
    }

    this->seletedFileItemIndex ++;      

    // from nw is to decide should I scroll the screen.


    // if the onscreen last one is not the last file in dir        
    //if ( seletedFileItemIndex  >= ( this->fileCount + this->dirCount ) )
    //     return;

    //控制 选择项到了屏幕下方倒数第几个的时候可以向上滚屏
    if( ( this->seletedFileItemIndex + 2 >= this->drawIndexLast )
        && 
        // 当一个屏幕可以装下全部东西(或者浮动2个)的时候, 不滚屏.
        2 + ( this->drawIndexLast - this->drawIndexFrom) >= this->getMaxItemCountInWindow() 
        ) 
    { 
        showfromFileIndex ++;
    }  

    refresh(); 
    //this->showAllItems( showfromFileIndex );
}    




//SceIoDirent * PSP_FileManager ::getItem ( unsigned short index )const
dirent * PSP_FileManager ::getItem ( unsigned short index )const
{
    if ( index < this->dirCount )
    {
        return this->dirInfoStruc_p[ index ];
    }
    else if ( index < ( this->dirCount + this->fileCount ) )
    {
        return this ->fileInfoStruc_p[  index - this->dirCount ];
    }else
    {
        return NULL;
    }

}


void PSP_FileManager ::showItem ( unsigned short winX, unsigned short winY, 
                                 unsigned short fileItemIndex )
{
    const static int spaceBMP_Text = 3;
    //SceIoDirent *tempDirt;
    dirent *tempDirt;
    tempDirt = this->getItem ( fileItemIndex );
    drawIndexLast = fileItemIndex; 
    unsigned char hanZiFileName [255];
    memset( hanZiFileName, 0, 255 );
    // since the PSP only return JIS code, need convert to CJK ( Chinese )
    // maybe use ttf unicode to show ......someday


    //PSP_ChineseUtil::jis2cjk (  (unsigned char *) tempDirt->d_name , (unsigned char *) hanZiFileName );


    if ( tempDirt == NULL)
    {
        return;
    }   

    unsigned short color;
    const ushortBitMap *tempBitMap;
    if ( tempDirt-> d_stat.st_attr == FIO_SO_IFDIR )
    {             
        tempBitMap = PSP_GRAPHICS::getFolderIconBitMap( );
    }else
    {   
        tempBitMap = this->getFileIconBitMap( (unsigned char *) tempDirt->d_name );
    }

    if(  fileItemIndex == this->seletedFileItemIndex )
    {
        this->selectBarActive.setPositionY(  winY + this->rectangle.y   );
        this->selectBarActive.showWindow();
        color = 0xffff;
        selectBarActive.drawBitMap (  winXStart ,  0 , *tempBitMap );
        selectBarActive.printAscHan16ZiString( this->winXStart + tempBitMap->width + spaceBMP_Text ,
            0, color, 
            //(char*)hanZiFileName  );
           tempDirt->d_name  );

    }else
    {  
        color = fontColor;
        //this->selectBarNormal.setPositionY ( winY );
        //this->selectBarNormal.show();    
        //selectBarNormal.drawBitMap ( 0  , 0, *tempBitMap );     
        //selectBarNormal.printAscHan16ZiString ( tempBitMap->width + 2 , 0, color , tempDirt->d_name );
        // in case  the file name is too long
        PSP_Window::drawBitMap ( this->winXStart ,   
        //+this->rectangle.x 
         winY , //+ this->rectangle.y, 
                      *tempBitMap );       
        PSP_Window::printAscHan16ZiString( this->winXStart
        //+ this->rectangle.x
         + tempBitMap->width + spaceBMP_Text , 
                                           winY, //+ this->rectangle.y, 
                                            color,
                                              tempDirt->d_name  );  
                                              // (char*)hanZiFileName   ); 
                                            
    }

}

/*
void PSP_FileManager ::addOnScreenItem( unsigned short winX, unsigned short winY,
unsigned short fileItemIndex )
{
if ( this->onScreenItemCount >= maxOnScreenItem )
{
return;
}    

FileMgrDrawItem *p;
p = & this->fileMgrDrawItem[ this->onScreenItemCount];
p->winX = winX;
p->winY = winY;
p->fileItemIndex = fileItemIndex;
onScreenItemCount++;

}

void PSP_FileManager ::showItem ( unsigned short  onScreenDrawItemindex  )
{
FileMgrDrawItem *tempDrawItem;
tempDrawItem = & fileMgrDrawItem[ onScreenDrawItemindex ];
//this->showItem( tempDrawItem ->winX, tempDrawItem ->winY, tempDrawItem ->index);
unsigned short winX, winY, fileItemIndex;
winX = tempDrawItem->winX;
winY = tempDrawItem->winY;
fileItemIndex = tempDrawItem->fileItemIndex  ;

drawIndexLast = fileItemIndex; 

SceIoDirent *tempDirt;
tempDirt = this->getItem ( fileItemIndex );
if ( tempDirt == NULL)
{
return;
}   

unsigned short color = fontColor;

if(  fileItemIndex == this->seletedFileItemIndex )
{
this->selectBarActive.setPositionY(  this->rectangle.y + winY + borlderLine );
this->selectBarActive.show();


}else
{
this->selectBarNormal.setPositionY ( this->rectangle.y + winY + borlderLine );
//  this->selectBarNormal.show();    
}

const ushortBitMap *tempBitMap;

if ( tempDirt-> d_stat.st_attr == FIO_SO_IFDIR )
{             
tempBitMap = PSP_GRAPHICS::getIconBitMap ( IconsFolder );
PSP_Window::drawBitMap ( winX  , winY, *tempBitMap );       
PSP_Window::printAscHan16ZiString( winX + tempBitMap->width + 2 , winY, color, tempDirt->d_name );

}else
{  

EnumIcons iconType;
iconType = getFileType ( tempDirt->d_name );
tempBitMap = PSP_GRAPHICS::getIconBitMap ( iconType );
PSP_Window::drawBitMap ( winX  , winY, *tempBitMap );
PSP_Window::printAscHan16ZiString( winX + tempBitMap->width+ 2 , winY, color, tempDirt->d_name );

}
}


*/

/*
void PSP_FileManager ::clearOnScreenItem( void )
{
this->onScreenItemCount = 0;


}

*/



void   PSP_FileManager ::showAllItems ( unsigned short fromFileIndex ) 
{
    unsigned short winX = winXStart;
    unsigned short winY = winTitleHeighSpare ;
    unsigned short maxItemCount1Window = getMaxItemCountInWindow ();
    unsigned short showedItemCount = 0;
    unsigned short fileIndex = fromFileIndex;
    //clearOnScreenItem();
    // clear window
    this->showWindow();
    //
    this->drawTitle();
    PSP_GRAPHICS::drawHorizonLine ( rectangle.x + 2 , rectangle.x + rectangle.width - 2 ,
                                    winTitleHeighSpare + rectangle.y -2 ,  RGBColor::getShortColor( 160, 160, 160) 
                                  );
    drawIndexFrom= fromFileIndex; 
    do
    {         
        if ( fileIndex >= this->fileCount + this->dirCount)
        {
            break;
        }
        if ( showedItemCount >= maxItemCount1Window )
        {   break;
        }
        showItem( winX, winY, fileIndex );         
        winY = winY+ DotFont16ASCfont_heigh + this->spaceBetweenLine;         
        showedItemCount ++;
        fileIndex ++;

    }while( true );
   
}



const ushortBitMap * PSP_FileManager :: getFileIconBitMap ( const unsigned  char * filename )
{

    CommonString fileExtStr;
    //fileExtStr.setString( filename );
    fileExtStr.getFileExtention ( filename );
    return PSP_GRAPHICS::getIconBitMap( fileExtStr.getChar_p_readonly() );

}



void PSP_FileManager ::clear ( void )
{


    int i;
    //SceIoDirent *tempDirt;
    dirent *tempDirt;
    
    for( i=0 ; i<fileCount; i++ )
    { 
        tempDirt = fileInfoStruc_p[i];
        if ( tempDirt) 
        {
            delete tempDirt ;
        }

    }
    for( i=0 ; i<dirCount; i++ )
    { 
        tempDirt = dirInfoStruc_p[i];
        if ( tempDirt) 
        {
            delete tempDirt ;
        }

    }
    fileCount = 0;
    dirCount = 0;
    

}



bool PSP_FileManager ::addItem( const dirent * newItem)
{
    if( newItem == NULL )
    {
        return true;
    }   

  //  SceIoDirent *tempDirt_p;  
    dirent * tempDirt_p;
    if (  newItem->d_stat.st_attr == FIO_SO_IFDIR )
    {
        //this is a new Dir to add.
        if ( this->dirCount>= this->maxFilesCount )
        {  //full 
            return false;
        }        
        //tempDirt_p = ( SceIoDirent * )malloc ( sizeof( SceIoDirent )  );  
        tempDirt_p = ( dirent * )malloc ( sizeof( dirent )  );  
        
        if( tempDirt_p == NULL )
        {
            return false;
        }        
        //this is wrong  :  faint !!!  
        //  memcpy( tempDirt_p ,  newItem, sizeof(SceIoDirent) ); !!!!!!!!!!!
        memcpy( (void*)tempDirt_p ,  (void*)newItem, sizeof(dirent) );
        this->dirInfoStruc_p[ this->dirCount ] =  tempDirt_p;        
        this->dirCount++;
    }
    else
    {    
        //this is a new file to add.
        if ( this->fileCount>= this->maxFilesCount )
        {  //full 
            return false;
        }        
    //    tempDirt_p = ( SceIoDirent * )malloc ( sizeof( SceIoDirent )  );  
        tempDirt_p = ( dirent * )malloc ( sizeof( dirent )  );  
        if( tempDirt_p == NULL )
        {
            return false;
        }        
        //this is wrong  :  faint !!!  
        //  memcpy( tempDirt_p ,  newItem, sizeof(SceIoDirent) ); !!!!!!!!!!!
        memcpy( (void*)tempDirt_p ,  (void*)newItem, sizeof(dirent) );
        this->fileInfoStruc_p[ this->fileCount ] =  tempDirt_p;        
        this->fileCount++;    
    }

    return true;
	

	
}

/*bool PSP_FileManager ::addItem( const dirent * newItem)
//bool PSP_FileManager ::addItem( const SceIoDirent * newItem)
{

    if( newItem == NULL )
    {
        return true;
    }   

    SceIoDirent *tempDirt_p;  
    if (  newItem->d_stat.st_attr == FIO_SO_IFDIR )
    {
        //this is a new Dir to add.
        if ( this->dirCount>= this->maxFilesCount )
        {  //full 
            return false;
        }        
        tempDirt_p = ( SceIoDirent * )malloc ( sizeof( SceIoDirent )  );  
        if( tempDirt_p == NULL )
        {
            return false;
        }        
        //this is wrong  :  faint !!!  
        //  memcpy( tempDirt_p ,  newItem, sizeof(SceIoDirent) ); !!!!!!!!!!!
        memcpy( (void*)tempDirt_p ,  (void*)newItem, sizeof(SceIoDirent) );
        this->dirInfoStruc_p[ this->dirCount ] =  tempDirt_p;        
        this->dirCount++;
    }
    else
    {    
        //this is a new file to add.
        if ( this->fileCount>= this->maxFilesCount )
        {  //full 
            return false;
        }        
        tempDirt_p = ( SceIoDirent * )malloc ( sizeof( SceIoDirent )  );  
        if( tempDirt_p == NULL )
        {
            return false;
        }        
        //this is wrong  :  faint !!!  
        //  memcpy( tempDirt_p ,  newItem, sizeof(SceIoDirent) ); !!!!!!!!!!!
        memcpy( (void*)tempDirt_p ,  (void*)newItem, sizeof(SceIoDirent) );
        this->fileInfoStruc_p[ this->fileCount ] =  tempDirt_p;        
        this->fileCount++;    
    }

    return true;
}*/

void  PSP_FileManager ::getAllFileNames ( const unsigned char *path  )
{

    //int fileDesc; 
    int r;

    this->clear();

    //add / at the end
    int pathlength;
    pathlength = strlen( ( char *)path );
    
    /*fileDesc = sceIoDopen( ( char *) path );
    if ( fileDesc<0 )
    {
        //something is  wrong
        return;
    }
    */
    
    DIR *dp;
    dp = opendir( (char*)path );
    if( dp == 0 )
    {
	   return ;
    }
    

    //SceIoDirent tempDirt;
    dirent *tempDirt;
    do
    {
        //MUST RESET the info struct before call sceIoDread
        //otherwise , ......!!!!!!
        //this is a sceIoDirent( ) Bug !!        
        //tempDirt = fileInfoStruc_p[ this->fileCount ];
           
       ////////////////////////////////////////////////////////////////////
        //    r = sceIoDread( fileDesc, &tempDirt  ) ;
        
        tempDirt = readdir(  dp ) ;
        
        if( tempDirt == 0 )break;
        
        
        ///////////////////////////////////////////////////////       

        // since the PSP only return JIS code, need convert to CJK ( Chinese )
        // maybe use ttf unicode to show ......someday
        /*   PSP_ChineseUtil::jis2cjk ( (unsigned char *)tempDirt.d_name , 
        (unsigned char *) tempDirt.d_name  );*/
        //PSP_Window::showMessage(  tempDirt-> d_name);
        r= this->addItem ( tempDirt );

        //r= this->addItem ( &tempDirt );
        if ( !r )
        {   
            //full  
            break;
        }       

    }while ( true );
    closedir( dp);
    /*buffer [0]=0;
    sprintf ( (char * )buffer ,"in getAllFileNames 5  %s ", (char *)path );
    showMessage ( buffer );
    */
}


void PSP_FileManager ::print ( void )
{
    int i;
    //SceIoDirent *tempDirt_p;   
    dirent *tempDirt_p;   

    printf ("Dir list: \n");
    for( i=0;i<this->dirCount;i++)
    {
        tempDirt_p = this->dirInfoStruc_p[i];
        printf ( tempDirt_p->d_name );
        printf ("\n");

    }
    printf ("\nFile list: \n");
    for( i=0;i<this->fileCount;i++)
    {
        tempDirt_p = this->fileInfoStruc_p[i];
        printf ( tempDirt_p->d_name );
        printf ("\n");

    }


}


bool PSP_FileManager ::fileSelector( CommonString &fileNameString )
{ 
     
     CommonString pathString;
     //pathString.setString ( msRoot );
     pathString.setString( msRoot );
   //  const unsigned char *p;
   //  p =  currentPathString.getChar_p_readonly() ;
     
     Rectangle recfMgr ( 20, 20, 390, 220 );
     PSP_FileManager  fileMgr ( recfMgr);
     ExploreResult result ;
     
     //PSP_GRAPHICS::getScreenImageToBitMap(  wholeScreenBMP );
     PSP_GRAPHICS::getScreenImageToBitMap(  fileMgr.wholeScreenBMP );
   
     do 
     {
     
        result = fileMgr.explorePath 
               ( pathString , &fileNameString  ); 
        if ( result != reEnter ) break;
        
     }while ( true );   

     if ( result == cancel ) return false;
     if ( result == doneExplore )
     {
         if ( fileNameString.getSize() > 0 ) 
        {
           return  true;
        }   
         else
        {
           return false;
        }   
     }
     return false;

}


void  PSP_FileManager::registerFileHandler
              ( const unsigned char* fileExt, void( *handler_p)( const   char * )  )
{
      CommonString testExt;
      testExt = fileExt;
      testExt .toLowerCase();
      if ( testExt == textFileExt )
      {
          textFileHandler = handler_p;
      }
      else if ( testExt == htmFileExt )
      {
          htmlFileHandler = handler_p;
      }else if ( testExt == htmFileExt )
      {
          htmlFileHandler = handler_p;
      }
      
      



}
    
  
  

