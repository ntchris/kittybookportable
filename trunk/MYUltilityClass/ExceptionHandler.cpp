

void myExceptionHandler( PspDebugRegBlock *regs )
{
	// Do normal initial dump, setup screen etc 
	pspDebugScreenInit();

	// I always felt BSODs were more interesting that white on black 
	pspDebugScreenSetBackColor(0x00FF0000);
	pspDebugScreenSetTextColor(0xFFFFFFFF);
	pspDebugScreenClear();

	pspDebugScreenPrintf("I regret to inform you your psp has just crashed\n");
	pspDebugScreenPrintf("Please contact Sony technical support for further information\n\n");
	pspDebugScreenPrintf("Exception Details:\n");
 	pspDebugDumpException(regs);
	pspDebugScreenPrintf("\nBlame the 3rd party software, it cannot possibly be our fault!\n");

 
}



//   pspDebugInstallErrorHandler( myExceptionHandler );





void testNULLError( void )
{
	
    char *p = 0;
    p[100]=0;
    	
}





