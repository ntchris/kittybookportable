



//#define __PSPSDK__
#ifndef __COMMONSTRING__
#define __COMMONSTRING__





#include <string.h> 






 class CommonString
{
   unsigned char *stringData ;    
    //const char * stringDataReadOnly;
    const static unsigned char emptyStr ;
    const static unsigned  char * const emptyStr_p;
    //ending 0 doesn't count
    unsigned int size;

      static const int maxFileExtLength = 10;
  
public: 
    CommonString();
    CommonString( const unsigned char * chars );
    
    ~CommonString();
      void setString( const unsigned  char * charstr);
      void setString( const CommonString *str  );
      
    CommonString & operator = ( const CommonString &str );
    CommonString & operator = (  const    char * charstr );    
    void append( const unsigned  char * charstr );    
    void append( const  CommonString *str );    
    

    void getFileExtention ( const  unsigned char * filename );

    // operator== is non member operator  
    // operator + is non member operator  
  inline  unsigned int  getSize( void ) const  
{
    return this->size;
};
  inline    unsigned char * getChar_p( void );
  inline    const unsigned  char * getChar_p_readonly( void ) const
  {
    if ( this->getSize() == 0 )
    {
        return this->emptyStr_p;
    }else
    { 
        //Promising I will not modify data via the pointer 
        return ( const unsigned  char*)this->stringData;
    }	

}  ;
       static unsigned int getStringSize( const unsigned  char *chData);
       int getIndexFromLast ( const unsigned  char keyword ) ;
       void clear ( void );
       void toUpperCase ( void );
       void toLowerCase ( void );
       
       
       void getUpperDir ( const unsigned char * dirpath );

      static unsigned short getEnglishWordLen( const unsigned char *  str )  ;
      static unsigned short getUnicodeEnglishWordLen ( const unsigned short *  str , unsigned short bufSize )  ;
      static bool isEnglishASC ( unsigned char ch );


	
       
    void print( void );    
    void selfTestC( void );
    void selfTestB( void );
    void selfTestA( void );
    void selfTestD( void );    
    void selfTestE( void );
    void selfTestF( void );
    static void  selfTestFileEXT( const unsigned  char * filename );
    
    void selfTest( void );

};


bool operator == ( const CommonString &str1 , const CommonString &str2 );

bool operator == ( const CommonString &str1 , const unsigned  char *cstr2 ) ;

//const CommonString  &  operator + (const CommonString &str1 , const CommonString &str2 );








class StringList
{   
   
   static const unsigned long maxStringLineCount = 10240;

    // The sizeof ( CommonString )=8( bytes )
   CommonString string_ma[ maxStringLineCount ];
   unsigned long currentSize;
  public:
   StringList( );
   ~StringList( );
   bool loadFromFile ( const char * file );
   bool saveToFile ( const char *file );
   CommonString * getString ( unsigned long index ) const;
   //add to the end of the array
 //  bool addString ( char * charp );
   bool addString ( const CommonString  &str  );
   // this is wrong !!!  bool addString (  CommonString  str  );
   //replace the current string(index)
   bool addString ( unsigned long index, char * charp  );
   void clear ( void );
   void print( void );
   static void selfTest ( void );
    
};


#endif  // __COMMONSTRING__



