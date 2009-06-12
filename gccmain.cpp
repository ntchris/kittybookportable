

#include<stdio.h>
#include<stdlib.h>

#include <PSP_ChineseUtil.h>
#include <TextViewPad.h>

int main()
{
     
    printf("PSPString test main.cpp\n");
    //CommonString  str;
    //str.selfTest();
    //printf("PSP_CHINESEHANDLER::maxIndex %u \n ", PSP_CNFONTS16_16::maxIndex);
    

//    PSP_GRAPHICS::setScreenFrameMode( screenModeWriteAndShow_cg );
    
    //PSP_CHINESEHANDLER::selfTestPSPGraphicMode ( );    
    
    //PSP_CHINESEHANDLER::selfTestPSPGraphicMode ( );    
    //PSP_ChineseUtil::selfTestTextModeDrawAll (   );
    // PSP_ChineseUtil::selfTestTextModeDrawAllHanzi(  );
    PSP_ChineseUtil::selfTestTTFTextMode (); //printBitMapInTextMode
    //  TextViewPad::selfTestBookMark ();
    return 0; 
}


