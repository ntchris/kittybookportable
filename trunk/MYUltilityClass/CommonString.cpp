

//#define __PSPSDK__



//#include <stdafx.h> 

#include <CommonString.h>

//#define __PSPSDK__

#ifdef __PSPSDK__
#include <PSP_Global.h>
#endif 

#include <string.h> 




#ifdef __PSPSDK__

#else
#include <stdio.h>
#endif



const bool string_debug_cg = false;
// by default this is empty string "" 
const unsigned char CommonString::emptyStr = 0 ;
const unsigned char * const CommonString::emptyStr_p = &CommonString::emptyStr;


void  debugInfo(char * file, int line)
{
    if ( string_debug_cg) 
        printf( "File %s, line %u\n", file, line);

}



void CommonString ::getUpperDir ( const unsigned char * dirpath )
{
    
    
    
    if ( dirpath == NULL ) 
    {
        return  ;   
    }
    
    int len;
    len = strlen ( ( char*)dirpath );
    if ( len <= 0 )
    {
        return ; 
    }

    
    
    //this->setString( dirpath );

   

    const unsigned char *p;
    p = dirpath;  

    
    unsigned i = len -1-1;
    bool found =false;
    int slashIndex = 0;
    do
    {   if ( i == 0 ) break;
        unsigned char c;
        c = * (p +  i) ;
        if ( c == '/')
        {
           found = true;;
           break;
        }   
        i --;
        slashIndex++  ;
    }while (true );
    
    if ( slashIndex <= 0 ) return  ;  
    if ( !found  ) return;
    
    //const unsigned char *temp_p;
    int copyLength;
    copyLength = len  - slashIndex -1;
    
    unsigned char parentDirCh [ 512  ];
    memset ( parentDirCh, 0 , 512 );
    
    memcpy ( parentDirCh , dirpath , copyLength );
    this->setString ( parentDirCh );
    //showMessage ( parentdir.getChar_p_readonly() );
    return ;
    

}


void CommonString::append( const  CommonString *str )
{

     const unsigned char * p;
     p = str->getChar_p_readonly();
     append ( p );

}    

void CommonString::append( const unsigned char * charstr )
{

    if ( charstr == 0 )
    return ;
    

    //NULL and empty string "" is different
    
    size_t part2Size ;
    part2Size = strlen( (const   char *)charstr );
    if (   part2Size <= 0)
    {
        return ;
    }    


    //Both str1 and str2 have something, none of them are "" ""

    debugInfo (   __FILE__, __LINE__); 

   
    unsigned char * temp_p;
    unsigned long size;
    size = this->getSize() + part2Size + 1;
    
    temp_p = new unsigned char [ size ];   
   
    debugInfo (   __FILE__, __LINE__); 
    strcpy ( ( char *)temp_p, (const  char *)this->getChar_p_readonly()  );
    strcat( (char *)temp_p  ,  (const char *)charstr );
    
    this->setString( temp_p);
    delete temp_p;
    
    return    ;


}    
/*
const CommonString & operator + (const CommonString &str1 , const CommonString &str2 )
{

    if( (&str1 == NULL)  &&  (&str2 == NULL)  )
    {
        
        return  CommonString("");    
    }

    if( &str1 != NULL  &&  &str2 == NULL      ) 
    {   
        return  CommonString(  str1.getChar_p_readonly() );
    }

    //debugInfo(   __FILE__, __LINE__);               
    if( &str1 == NULL  &&  &str2 != NULL      ) 
    {
        return  CommonString(str2.getChar_p_readonly());
    }
    /////     

    //NULL and empty string "" is different
    if ( str1.getSize() + str2.getSize() <= 0 )
    {
        debugInfo (   __FILE__, __LINE__); 
        return  CommonString( "" ); 
    }

    if (  str1.getSize() <= 0)
    {

        return CommonString( str2.getChar_p_readonly() );

    }    

    if (  str2.getSize() <= 0)
    {
        return CommonString( str1.getChar_p_readonly() );

    }


    //Both str1 and str2 have something, none of them are "" ""

    debugInfo (   __FILE__, __LINE__); 

    char * temp_p;
    unsigned long size = str1.getSize() + str2.getSize() + 1; 
    temp_p = new char [  size ];
    memset (temp_p,0, size );
    // NOT enough memory 
    if( temp_p == NULL )
    { 
        return CommonString( temp_p ); 
    }

    if( str1.getSize() == 0 || str1.getChar_p_readonly()==NULL )
    {
        //do nothing , don't copy
    }
    else
    {
        strcpy( temp_p, str1.getChar_p_readonly() );

    }

    debugInfo (   __FILE__, __LINE__); 

    if( str2.getSize() == 0 || str2.getChar_p_readonly()==NULL )
    {

    }else
    {
        strcat( temp_p  , str2.getChar_p_readonly() );
    }   
    CommonString tempResult( temp_p);
    delete temp_p;
    return   tempResult;

}
*/


bool operator == ( const CommonString &str1 , const CommonString &str2 )
{
    if ( &str1 == &str2 )
    {
        return true;
    }

    if( str1.getSize() != str2.getSize() )
    {
        return false; 
    }

    int result;
    //both is empty string
    if ( str1.getSize() == 0 )
    {
        if( str2.getSize() == 0 )
       	{
            return true;
        }   	
    }

    result = strcmp( ( const char *) str1.getChar_p_readonly () , (const char *) str2.getChar_p_readonly() );

    if( result )   
    {  
        //compare fail
        return false;
    }   
    return true;
}




bool operator == ( const CommonString &str1 , const unsigned char *cstr2 )
{
    if(  str1.getChar_p_readonly() == cstr2 )
    {
        return true;
    }

    if( str1.getSize() == 0)
    {
        if( cstr2[0]==0 )	
        {
            return true;
        }  
    }

    CommonString tempStr( cstr2);
    bool result;
    result = ( str1 == tempStr);

    return result;


}



void CommonString::print( void )
{
    debugInfo (   __FILE__, __LINE__); 

    if( this->getChar_p()!= NULL )
    {    	
        if( this->getSize() <=0)
            printf("<empty string \"\"> " );
        else 
            printf("%s", this->getChar_p() );

    }else
    {
        printf("<NULL>" );

    }


}

void CommonString ::clear ( void )
{
    if( this-> stringData == NULL)
    {   
        //stringDataReadOnly = NULL;
        size = 0;     
    }else
    {
        delete []stringData;
        size = 0;
        stringData = NULL;
        //stringDataReadOnly = NULL;
    } 
    return; 
}


/*

void CommonString::selfTestC( void )
{
    printf(" void CommonString::selfTestC( void )\n");

    CommonString str1;
    str1.print();

    printf("main point\n");
    str1="";
    str1.print();
    str1.append("");
    str1.print();
    CommonString str2(NULL);
    str2.print();


    if(str1==str2)
    {
        printf("same !");
    }   else 
        printf("NOT same !");

}


void CommonString::selfTestB( void )
{

    CommonString str1, str2, str3;
    str1=NULL;


   // str3.append( str1.append( str2) );
    


    str3.print();


    str2="str2";
    str1.append(str2 );
    str3= str1;

    str3.print();

    str3="ABC";
    str3=str3;
    str3.print();
    int i;
    i= (str3 == str3);
    printf("%d",i);
    str3="";
    str3.print();

    if(str3=="")
    {
        printf("same 1\n");
    }else
    {
        printf("not same 1\n");
    }

    str3="AA";
    if( str3 == str3.getChar_p() )
    {
        printf("same 2\n");
    }else
    {
        printf("not same 2\n");
    }

    if( str3 == "AA" )
    {
        printf("same 3\n");
    }else
    {
        printf("not same 3\n");
    }
    if( str3 == NULL)
    {
        printf("same 4\n");
    }else
    {
        printf("not same 4\n");
    }


}

void CommonString::selfTestE( void )
{
    CommonString  str("AAA"), str2,str3;
    str = NULL;
    str.print();
    
    str2.append( str3);
    str=str2;
    str2.append(0);
    str=str2;
    str.print();
    //str =  str2 + NULL;


}

void CommonString::selfTestD( void )
{

    CommonString  str3 , str2;

    str2 = "str2" ;
    //str3 = NULL + str2;


}


void CommonString::selfTest( void )
{
    selfTestF();
    selfTestE();
    selfTestD();
    selfTestA();
    selfTestB();
    selfTestC();
}



void CommonString ::selfTestA( void )
{
    printf ( "void CommonString ::selfTest( void ) \n");
    CommonString string1;
    printf ( "String1 :%s \n", string1.getChar_p() );
    string1="string1";
    printf ( "String1 :%s \n", string1.getChar_p() );
    CommonString string2("string2");
    printf ( "String2 :%s \n", string2.getChar_p() );

    string1=string2;
    printf ( "String1 :%s \n", string1.getChar_p() );

    string1="string1_1";
    printf ( "string1 size is %d\n" , string1.getSize());
    printf ( "string2 size is %d\n" , string2.getSize());

    CommonString string3;
    string1.append(string2);
    string3 = string1 ;
    //printf ( "after + \n");
    printf ( "string3 = string1 + string2; string3= %s\n",
        string3.getChar_p() );


    CommonString string4;
    string4="string1_1string2";
    if(string3==string4)
    {
        printf("compare same %s \n", 
            string3.getChar_p() 
            );
    }else
    {    
        printf("compare not same\n %s : %s \n", 
            string3.getChar_p(), string4.getChar_p() );

    }



    string3="string3";
    string4="string4";
    if(string3==string4)
    {
        printf("compare same %s \n", 
            string3.getChar_p() 
            );
    }else
    {    
        printf("compare not same\n %s : %s \n", 
            string3.getChar_p(), string4.getChar_p() );

    }

    string1.clear();
    printf("%s \n",   string1.getChar_p()    );

    string1 ="ABCDEFG";
    printf("%s \n",   string1.getChar_p()    );
    char *p;
    p=string1.getChar_p();
    p[0]='Z';
    printf("%s \n",   string1.getChar_p()    );
    p[string1.getSize()-1]='Z';
    printf("%s \n",   string1.getChar_p()    );

    CommonString *string5_p;
    string5_p = new CommonString("ABCDEFG");
    printf("string5_p is %s \n",   string5_p->getChar_p()    );
}

*/

unsigned char * CommonString::getChar_p( void )
{

    /*if( this->getSize() == 0 )
    {
        //return this->emptyStr_p;
    }else
    */
    {
        return this->stringData ;
    }

}
 
 
CommonString & CommonString::operator = (  const  CommonString   &str )
{
    if( &str == NULL)
    {
        this->clear();	
        return *this;
    }

    if( &str == this) 
    {
        return *this;
    }

    if( str.getSize() == 0 )
    {
        this->clear();
    }
    else
    {
        this->setString( str.getChar_p_readonly() );
    }  

    return *this;

}

 

CommonString &  CommonString::operator = (  const    char * charstr )
{  
    if( charstr == NULL )
    {
        this->clear();
        return *this;     
    }
    else if( this->stringData == ( unsigned char *)charstr ) 
    {   
        //the charstr is just my stringData, the same pointer.   
        return *this;
    }else if ( charstr[0] == 0 ) 
    {
        this->clear(); 
        return *this;

    }
    else
    {
        this->setString( ( unsigned char *)charstr );
        return *this;
    }

    return *this;
}




int CommonString::getIndexFromLast ( const unsigned char keyword )
{
    if ( keyword == 0 ) return -1;

    unsigned int thisSize;
    thisSize = this->getSize();
    //keywordSize = getStringSize ( keyword );
    //if ( keywordSize > thisSize ) return -1;

    if (  thisSize <= 0 )
    return -1;
    
    unsigned int index = thisSize -1;
    bool found = false;
    const unsigned char *temp_p;
    temp_p = this->getChar_p_readonly();
    do
    {   
        if ( temp_p [ index ] == keyword )
        {  
            found  = true;
            break;
        }
        if( index <=0 ) 
        {
            break;
        }   
        else
        {
            index --;
        }  

    }while( true );

    if ( found )
    {
        return index;
    }   
    else 
    {
        return -1;
    }   

}



/**
\0  : 0
A   : 1
AB  : 2 
max size limited: 0xffff ffff    unsigned int = uint32
Does not include the ending 0.
*/
unsigned int CommonString::getStringSize(const unsigned  char *chData  )
{
    if ( chData == NULL )
    {
        return 0;
    }   

    unsigned int size_tmp = 0;
    while( chData[size_tmp]!=0 )
    {         
        size_tmp++; 
        // Rare : size gets to 0xffffffff and go back to 0 ??
        if( !size_tmp ) break;
        
        /* this is only to shorten the getsize time,
           sometimes we do not need actual size
        */
        
    }
    return size_tmp;
}


CommonString::CommonString( ) 
{
    debugInfo (   __FILE__, __LINE__); 
    stringData = NULL;    
    size = 0; 

}


CommonString::CommonString( const  unsigned char * chars )
{
    stringData = NULL;    
    size = 0; 
    if( chars == NULL )
    {
        return;
    }

    if( chars[0]==0 )
    {
        return;	
    }
    this->setString( chars );
}




CommonString::~CommonString( )
{   
    if ( this->stringData != 0 )
    {
       delete  []this->stringData;  
       this->stringData = 0 ;
    }   
}



void CommonString   :: getFileExtention ( const  unsigned char * filename )
{
  //  CommonString fileExtStr ;
    
    if ( filename == NULL ) 
    {
        return ;   
    }
    int len;
    len = strlen (  (const char *)filename );
    if ( len   <= 0 )
    {
        return ; 
    }

    //CommonString fileName( filename );
    CommonString temp;
    temp.setString( filename );
    
    int dotIndex = temp.getIndexFromLast( '.');
    if ( dotIndex < 0 ) return ;  


    unsigned char fileExt [ maxFileExtLength  ];
 
    int extLength;
    int copyLength;
    extLength = len  - dotIndex ;
    if ( maxFileExtLength < extLength  )
    {
        copyLength = maxFileExtLength;

    }else
    {
        copyLength = extLength;
        //strcpy ( fileExt, temp_p + dotIndex );
    }
    
    memcpy (  fileExt , filename + dotIndex, copyLength );
    fileExt [ copyLength ] = 0;
    
    this->setString( fileExt );
    return ;

}



void CommonString::setString( const CommonString *str  )
{

     const unsigned char * p;
     p = str->getChar_p_readonly();
     this->setString(  p );


}


void CommonString::setString( const unsigned char * charstr )
{
    debugInfo (   __FILE__, __LINE__); 


    //set me to be a NULL  ?
    if( charstr == NULL)
    {
        this->clear();
        return;
    }

    //use my char pointer to set to myself ??
    if( this->stringData ==  charstr ) 
    {
        return ;
    }

    debugInfo (   __FILE__, __LINE__); 

    //set me to a empty string "" 
    if( charstr[0]==0 )
    {  
        if(string_debug_cg)
        {
            printf(" hit ! \n");
        }
        this->clear();
        return;
    }

    //If this already has string , erase it.
    if ( stringData != NULL )
    {  
        this->clear();
    }
    debugInfo (   __FILE__, __LINE__);   

    //copy 
    this->size = getStringSize( charstr ) ;
    if( string_debug_cg)
    {
        printf("size = %u \n", this->size ) ;
    } 

    stringData = new unsigned char [ this->size+1 ];    
    //Not enough memory
    if( stringData  == NULL )
    {   

        this->size = 0 ;
        return;
    }    
    strcpy( (char *)this->stringData,  ( char *) charstr ) ;    
    return ;

}


void CommonString::toUpperCase ( void )
{
   
   if ( this->getSize() <= 0 )
   {
      return;
   }
   const int delta = 'A'-'a';
   unsigned char *p;
   unsigned int index = 0;
   p = this->getChar_p();
   do
   {    
        char tempc;
        tempc = p[ index ];
        if ( tempc == 0 ) break;
        
        if ( tempc >='a' && tempc <='z' )
        {
            p[index] = p[index] + delta;
        }
        index++;
        if ( index >= this->getSize())
        break;
        
   }while( true );
   
   return;
   
}


void CommonString::toLowerCase ( void )
{
   
   if ( this->getSize() <= 0 )
   {
      return;
   }
   const int delta = 'a'-'A';
   unsigned char *p;
   unsigned int index = 0;
   p = this->getChar_p();
   do
   {    
        char tempc;
        tempc = p[ index ];
        if ( tempc == 0 ) break;
        if ( tempc >='A' && tempc <='Z' )
        {
            p[index] = p[index] + delta;
        }
        index++;
        if ( index >= this->getSize())
        break;
        
   }while( true );
   
   return;
   
}




/*
void CommonString::selfTestF( void )
{
    CommonString str1("AAAAA");
    const char * const p = str1.getChar_p();
    printf("%s\n", p);

}


void  CommonString::selfTestFileEXT( const char * filename )
{  
    CommonString fileName( filename );

    int dotIndex = fileName.getIndexFromLast( '.');
    
   const int maxFileExtLength  = 10;

    char fileExt [ maxFileExtLength + 1 ];
    const char *temp_p =  fileName.getChar_p_readonly();

    
    int extLength;
    int copyLength;
    extLength = fileName.getSize() - dotIndex ;
    if ( maxFileExtLength < extLength  )
    {
        copyLength = maxFileExtLength;
        
    }else
    {
        copyLength = extLength;
        //strcpy ( fileExt, temp_p + dotIndex );
    }
    memcpy ( fileExt, temp_p + dotIndex, copyLength );
    fileExt [ copyLength ] = 0;
    
    CommonString fileExtStr ( fileExt );
    fileExtStr.print();

}

*/


/*

//=========================================================================
//  StringList methods
//=========================================================================
StringList::StringList()
{
    currentSize = 0;
}


StringList:: ~StringList( )
{
    // do nothing
}


bool StringList::loadFromFile ( const char * file )
{
    return true;

}


bool StringList::saveToFile ( const char *file )
{
    return true;
}

CommonString * StringList::getString ( unsigned long index ) const
{
    return NULL;
}

 

bool StringList::addString ( const CommonString & str  )
{
    if ( this->currentSize >= maxStringLineCount )
    {
        return false;
    }   
    //use current size as index

    CommonString *string_p;
    string_p = & ( this->string_ma[ this->currentSize ] );
     string_p->setString(   str.getChar_p_readonly()) ;
    this->currentSize ++ ;
    return true;

}


//replace the current string(index)
bool StringList::addString ( unsigned long index , char * charp )
{

    return true;
}

void  StringList::clear ( void )
{
    unsigned long i;
    CommonString *string_p;

    for ( i =0;i< currentSize; i++)
    {
        string_p =& this->string_ma[ i ];
        if( string_p != NULL )
        {
            string_p->clear();
        }   
    }

}

void StringList::print( void )
{
    unsigned long i;
    CommonString *string_p;
    printf("\n");
    for ( i =0;i< currentSize; i++)
    {
        string_p =& this->string_ma[ i ];
        if( string_p != NULL )
        {
            string_p->print();
            printf("\n");
        }   
    }

}

*/



unsigned short CommonString::getUnicodeEnglishWordLen 
      ( const unsigned short *  str , unsigned short bufSize )  
{
     if ( str == 0 ) return 0;
     unsigned short wordLen = 0;
     unsigned short maxWordLen = 12;
     if ( maxWordLen > bufSize ) 
     maxWordLen  = bufSize;
     //unsigned char  testch = 0;
     unsigned short unicodeCh = 0;
         
     do
     {
         
         unicodeCh = str[ wordLen ];
         //larger than 127 , it must not be abc.
         if ( unicodeCh > 127  )
         break;
         
        // testch = unicodeCh % 0x100 ;
         if ( isEnglishASC ( unicodeCh  ))//testch  ))
         {
             wordLen ++;
         }else
         {
             break;
         }    
         if ( wordLen > maxWordLen ) break;
      
     }while( true );
     
     return wordLen;

} 


unsigned short CommonString ::getEnglishWordLen( const unsigned  char *  str )
{
     if ( str == 0 ) return 0;
     unsigned short wordLen = 0;
     const unsigned short maxWordLen = 12;
  
     do
     {
         
         char ch;
         ch = str[ wordLen ];
         if ( ch == 0 ) break;
         if ( isEnglishASC ( ch ) )
         {
             wordLen ++;
         }else
         {
             break;
         }    
         if ( wordLen > maxWordLen ) break;
      
     }while( true );
     
     return wordLen;
 
}
 
 
 
bool CommonString:: isEnglishASC ( unsigned char ch )
{
    if ( ch >='a' && ch<='z' ) return true;
    if ( ch >='A' && ch<='Z' ) return true;
   // if ( ch == '_' ) return true;
    return false;
    
   
}



/*


void StringList::selfTest ( void )
{
    StringList strList;
    strList.addString("AAA");
    strList.addString("BBB");
    strList.addString("CCCCCCC");
    
    strList.print();


    CommonString basePathStr  ;
    //add / at the end
    char path[]={ "ms0:" }, filename[] = {"ABCDE.TXT"};
    int pathlength;
    pathlength = CommonString ::getStringSize( path );
    basePathStr  = path;
    if ( path[ pathlength -1 ] != '/' )
    {  
        basePathStr.append( "/" );
    }
    basePathStr .print();

    StringList dirList;
    
    basePathStr.append(  filename );
    CommonString str2 =basePathStr;
    dirList.addString ( str2   );
    dirList.print ();
    printf("testing done\n");     

}



*/
