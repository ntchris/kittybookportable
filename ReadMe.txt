

1.2 update:

 
(*) english word related bug in 16 dots font fixed.
    tons of bugs fixed

(*) Some Chinese display bugs fixed.

(*) Page turning logic enhanced again.

(*) Power saving logic enhanced.
      Some people complained that when he wanna quit the app and press <HOME>, it's just too slow.
      This time I have opened a window for users want to quit quickly:
         Please exit to Main Window or to File Explorer  Before you Press <HOME>. ( quick)
         Otherwise, if you press <HOME> when reading books( in the reading window)
         it takes about 2 seconds to quit. ( slow )
      
(*) Draw English and HanZi better.



2005.10.5




========================================================================================
1.1  update:

(a) Some people say they like really small fonts...
    So, in this version, font size can be changed from 11 to 50.

(b)  Enhanced the page turning control:
     Even if you keep holding the <Down><R-Trig>, it doesn't turn pages.
     But if you keep holding the <Down><R-Trig> for more than 1 seconds, it turns...
     And if you tap the key quickly, it turns the pages quickly.....
     
     Left, Right, Up,Down, L-R Trigger is dedicated for turning pages slowly.
     If you want turnning the pages like REAL FAST, please use Analog stick:
     KEEP HOLDING "Analog - down/up ". ( keep holding increases the step ).




2005.10.3

=================================================================================
First release.


Kitty Book Portable Ver 1.0


                     by Chris J.
                       2005.10.1
                          China 
                 KittyBookPSP@hotmail.com

                                        
                                         
This is a freeware, you can keep, use, copy, distribute it as you like.
But please don't modify it.

This software is provided as it is. 
This software might have mistake, bug or error, so there is no any guarantee at all.
Use it at your own risk.


Question 1
==  What is Kitty Book Poortable ? ==============

 This is a PSP software, your can use it to read text file with your PSP.
 It's useful when we travel, and when we're in bed, maybe.
 It can open different types of text format file ( normal/unicode/unicod BE). 
 The content can be shown in True Type Font display format.
 You can change the font 's size, from small to big.
 If you don't like True Type font, this Kitty Book has a built-in font library,
it works too.



Features:
     (*) Graphic user interface, icons, buttons, GUI file managers.
     (*) Asc, Unicode and Unicode BE text supported
     (*) True Type font display supported
     (*) TTF Font display size adjustable.
     (*) Memory cache built in. 
         It draw the font quicker and reduces the memory stick access time, 
         saves battery life.....
     (*) Built-in 16*16 and 8X16 font library, working without TTF provided.  
     (*) Font color and back ground color adjustable.
     (*) PSP Power saving mechanism used. You can read book for long time.
     (*) 5 bookmark slots to remember 5 files and progress.
     (*) Can load Normal text file up to 3MB.  Unicode file up to 6MB.

NOT supported feature:
     (*) Display and read html file with format: you can only read the source code of html.
     (*) open zip file.
     (*) listen mp3 file.
     
 

== Install ==============
PSP 1.5:  open the Winrar package, enter \PSP1.5\
       extract dir \KittyBookPortable\ and \KittyBookPortable%\ to
       your memory stick G:\PSP\GAME

PSP 1.0:
         open the Winrar package, enter \PSP1.0\
       extract dir \KittyBookPortable\ to your memory stick G:\PSP\GAME

    
 
     
General Contrls:
  (*) To Confirm   <O>
  (*) To cancel, quit <X>
  (*) To navigate between items:  <LEFT><RIGHT><UP><DOWN>
  (*) To quit to PSP system menu: <HOME> and <Yes>
      please wait 3 seconds while PSP quiting to system menu.

In file explorer ( file manager , file selector > :
  (*) To select different file :      <Up> <Down>
  (*) To launch the file              <O>
  (*) To quit the explore :           <X>
  (*) To force text viewer open the file     :  <Triangle>
      You can use it to read a html or a bin file 's content.

In Config Window  
    refer to general contrls

In book mark save/load window
    refer to general contrls

In text viewer window
  (*)  to Turn page down   <Down>
  (*)  to Turn page up     <UP>
  (*)  to save and load book mark :  press <Triangle>

  (*)  to Turn page down fast <RightTrigger>
  (*)  to Turn page up fast <LeftTrigger>
  (*)  to Turn down one line <Right>
  (*)  to Turn up one line <Left>
  (*)  to quit viewing the text, press <X> and confirm.
  (*)  To Turn page up and down VERY FAST :   Analog stick Up/Down  
  

How to start:

  (*)  Install the program to PSP
  (*)  Launch it.
  (*)  Press Browse Button
  (*)  In file manager, press <O> to select a text file to read.



Question 2
== What text file format does it support ? ==================
The following text format are supported:
 
 (a) Normal Text file (ANSI format ). 
 (b) Unicode Text file.
 (c) Unicode BigEndian Text file.
 Kitty Book can read all of them.

 
 For your infomation but not related to the topic:
   You can use Windows2000 NotePad to <Save As> between these encoding options.



Question 3
== How to read books using True Type Font ? ============    

 True type font(TTF) is beautiful, and the size is easy to change without making the
font picture getting ugly.

However this utility does not has a built-in TTF library.
You must provide TTF like *.TTC or  *.TTF file by yourself.
To use TTF, you must do the following:

(a) Find a True Type Font file by yourself.
    for example:  arial.TTF ( Arial Font)  
                  cour.TTF  ( Courier New Font )
                  comic.TTF  ( Commic Sans Font )
                  SIMSUN.TTC ( Chinese Song Ti Font )
                      ...
    
    FYI: There are a lot in C:\WINNT\Fonts.
    And you can download from some Linux sites ...maybe.
 
    Anyway, find it by yourself.

(b) Copy a TrueTypeFont file to your PSP Memory Stick, to any path is fine.
      Kitty Book only uses ONE(1) TTF at one time, so just copy any one is fine.
      You don NOT have to copy all of them.
       for example:
        copy SIMSUN.TTC to g:\  ( assume g: is your memory stick 's drive)
          or 
        copy cour.TTF to g:\
      
       To display Chinese GB/Chinese Big5 / Japanese/ Korea fonts, please copy
       the proper font file.
     
       It DOES NOT work if you only using the English Font file ( like cour.ttf).
     
       You MUST use the following font to display Asian fonts:
         Chinese GB:   simsun.ttc  ( Song Ti Font)
                       simfang.ttf ( Fang Song Font )
                       .....

         Chinese Big5:  Refer to Chinese GB, it works, I already tested.

         Japanese:  Refer to Chinese , it works, I already tested.

         KOREA   :  I don't know ... maybe it works too.  ^_^
 
(c) Launch Kitty Book, enter [Config] Menu, select the [use TrueTypeFont] box.
    Press the <TTF File...> button, a file selector menu will pop up, please browse your Font file.  
    Only *TTF and *TTC are allow to select.

    After it's done,  press <Down> to [Save and Done] button, press it ( button <O> ) 
    to save and quit the Config Window.

 Done !
      
    

Question 4
== How to change TTF font's size ? ========================
When your are using TTF, ( refer to Question 3 How to read books using True Type Font ? )
you can change the font size.
Enter config window, press UP/Down button to go to [Font Size] box, and press Left/Right 
to change the font size value. 16 to 50 is fine, it's the pixel size. 
After it's done, save and quit config window.




Question 5 
== How to change the font and background's color =============================
When you are using or not using TTF, you can change font and background 's color.
Enter Config window,  press the color setting button, it opens a color setting menu,
use Left/Right to change the RGB value to define the color.
To confirm, press <O>,  to cancel the change, press <X>.
after it's done, save and quit config window.



Question 6
== If I don't like TTF, how to set ? ==========================================
Go to config window, use UP/Down to focus [use TrueTypeFont], press <O> to select or clear the box.
Save and quit.

If you clear the select box, the Kitty Book will use the built-in  8 X 16 dots font for ASC chars
and  16 X 16 Dot font for Chinese GB/Chinese Big5 /Jap...




Question 7.
== What version of PSP can it run on ? ========================================
It can run on PSPs which can run unofficial programs.
So far as I know, PSP with firmware 1.5 and 1.0.
Mine is 1.5, so I don't have a PSP1.0 to test, it will work fine hopefully.



Question 8.
== How long did it take you  ?  ===============================================
Total about 3 months.  I just bought my PSP in July 2005, then I start to work on it.

In my family , some elder people can't read small fonts, so I think I'd better make a tool
to display big fonts......



Question 9.
== What languge /Tools do you use to make it ? ================================
Language is C++, Development env is PSPTool chain, PSPSDK.




Question 10.
== Anything else ?   =====================================
(*) I also provide some sample text for your test

(*) In some cases you will find the file explorer can not show the correct Chinese file name,
it's because of the PSP OS' limitation... it's neither a bug of Kitty Book nor you got RPWT.

(*) Although I have tested it for many times, it might still have bugs.
If after some actions your PSP goes to black screen and stop to responds,
it's likely the software just performed a NULL pointer access.
Don't worry.
Your don't need to press any button then, just wait 10 seconds, the PSP will turn off by itself, 
You can turn on it after it's off.


(*) Font file simsun.ttc is recommended to display ChineseGB/Big5/JP.
    The file size is about 10MB.   



===============================================================================

Thank you very much list:

(*) Thanks SONY to make such a good machine
(*) Thanks authors of PSPSDK, PSP Toolchain, it's a very Great tool/dev. env.
(*) Thanks GNU C++ tools. 
(*) Thanks FreeType Project group for the FreeType ultility.
(*) Thank you for using it !










