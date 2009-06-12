

1.2 update:

(*) pls. note: songti.ttf has problem displaying english and numbers when using size 11.
    ( install it to Wnidows/fonts, the name is "unisun", you will see the problem in your textedit tool ( like ultraedit)
    But 12 is fine.

(*) english word related bug in 16 dots font fixed.

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











1.2 update:

(*) 一大堆bug fixed了.

(*) 省电措施增强.
     有人抱怨说退出的时候太慢了.
    现在, 只要你退出到[文件浏览器]或者[主窗口]的时候再<HOME>退出, 退出的速度就可以接受了
    而如果在读书的时候按<HOME>退出, 那么速度还是慢的, 要等2秒. 
    总之这是正常的现象, 为了省电, 把cpu速度放慢了.
      
(*) 英数汉字各种模式下的排版增强, 稍微美观了 ......一点.

(*) 注意 songti.ttf 在11大小下面有问题: 英数有多余的点显示. 12没问题.
    但是汉字完全没问题.  所以用它的人注意了.  在UltraEdit下面它也有同样的问题.....白研究了半天.



2005.10.5


====================================================================================

Update 1.1

(a)
超级小字支持
有些朋友说16的字还是太大了, .....
这次更新是放宽了字体选择, 可以从11 - 50选择了.

(b)
翻页按钮的控制增强.
有些朋友说轻轻一按就飞了好几页.....
现在改了下控制:
如果你按牢翻页按钮, 它不翻页. 这可以防止不小心连续翻N页.
如果你快速的按/放/按/放, 则可以快速的翻页.
如果你按牢翻页按钮超过一秒, 那么它也可以自动翻页....
不要分特哦

其实, 左右, 上下, L-R 扳机都是为了慢速翻页而设置的, 
如果你要以飞快的速度的翻页到文章的任何一页, 你应该使用模拟摇杆. 很快的.
扳住向模拟上/下, 记住要扳住, 那么翻页的速度会不断的增加的.




2005.10.3


============================================================
首次发布

==   Kitty Book Portable Ver 1.0    =====================


                               by Chris J.
                               2005.10.1
                               China 
                   KittyBookPSP@hotmail.com

                                        
这是自由软件, 您可以随意使用,拷贝和传播. 但是请不要擅自修改
谢了.

This software is provided as it is. 
This software might have mistake, bug or error, so there is no any guarantee at all.
Use it at your own risk.



问题 1
==  什么是Kitty Book Poortable ? 拿来干什么?  ==============

这是一个PSP (Sony PlayStation Portable)的软件.
在PSP平台上面运行.
这是一个电子书阅读程序. 用来阅读PSP上面的文本文件(text文件).
就是居家旅行的时候看书用的.

文本的内容可以用TrueType Font来显示, 字体会漂亮一点, 而且字体大小可以调整.
字体大小可以从16个点到50个点调整.
如果你不喜欢, 也可以不使用TrueType字体, 本软件内建了16点阵的中英文, 同样可以好用...^_^
 


主要功能一览:
     (*) 图形用户界面, 图标, 按钮, 文件管理器...
     (*) 支持 普通的Asc文件, Unicode文件 and Unicode BE 文件. 
     (*) 支持用TrueType 字体来显示.
     (*) TrueType字体大小可以调整
     (*) TrueType字库相当大, 中文的宋体有10MB.
         所以本软件有内建缓存机制, 当第二次显示某汉字的时候, 就可以高速显示之, 
         可以避免总是读取记忆棒, 也更加的节约电池, 延长使用时间.

     (*) 内建16点阵中日韩点阵字体, 当你不想用TrueType字体的时候, 可以用内建字体.          
     (*) 阅读器窗口里面的字体颜色和背景颜色可以随意用RGB的方法来设置
     (*) 本软件内建节电的机制, 增长了读书的使用时间, 减少电池消耗.
     (*) 五个书签栏位, 记录书名(即文件名)和行数, 随时存取.
     (*) 最大支持读取 3MB的普通文件, 6MB的 Unicode 文件.

不支持的功能: 
   因为太忙了, 暂时没时间搞了. 而且其中某些功能在下觉得意义实在不大... ^_^

     (*) html文件不能用好的格式来显示, 你只能用查看html的源代码的方式来阅读了.
     (*) 不支持打开zip文件. 
     (*) 不能听MP3
     (*) 不能竖着看
     
     
 
     
一般操作说明:
  (*) 确定:   <O>
  (*) 取消或者退出 <X>
  (*) 选择不同项目:  <LEFT><RIGHT><UP><DOWN>  十字键的上下左右键
  (*) 任何时候, 按<HOME>退回PSP系统菜单.  按了<HOME> <Yes>后请等待3秒钟, 会退出系统菜单的. 
      

在文件管理器内:
  (*) 选择不同文件:      <Up> <Down>
  (*) 打开选定的文件     <O>
  (*) 退出               <X>
  (*) 强制打开某文件:    <三角>    
        
        是用于打开 *.h, *.cpp *.dat等等文本文件类型的.
        也可以强制打开任何二进制文件, 但是自然是乱码显示.

       

在配置窗口 ( Config Window) :
    请参考一般操作
    注意: 退出时候请按<Save>按键退出. 否则所作的修改将不被应用和记录.

在书签存取窗口:
    请参考一般操作
    按钮如果为灰色则表示无法在该栏目存取.
    例如当前不是在阅读, 则自然不能保存书签; 而如果某书签栏位为空, 则自然不能读取该书签. 
    

在文本阅读窗口:
  (*)  下翻一行           <Right>  
  (*)  上翻一行           <Left>
  (*)  向下翻页   <Down>
  (*)  向上翻页    <UP>
  (*)  保存书签/读取书签  <Triangle>  三角键
  (*)  快速的向下翻页        <RightTrigger>  右扳机键
  (*)  快速的向上翻页  <LeftTrigger>  左边扳机键 
  (*)  模拟摇杆:  非常快速的上下翻页.
  (*)  退出阅读模式, 按<X>, 出现确认窗口, 然后按左右选择<Yes>按钮, 按<O>
  
数值栏:
  按左右改变数值


== 安装 ==============
更常规程序一样.
如PSP 1.5的, 把RAR包解开, 进入PSP1.5子目录, 
把这两个目录及里面的文件解压/拷贝去记忆棒 G:\PSP\GAME
   \KittyBookPortable\ 和 \KittyBookPortable%\ 






Question 2
== 支持什么文本格式  ? ============
支持以下的文本格式:
 
 (a) 普通文本文件格式, 即ANSI, 这是最常用的格式. 
 (b) Unicode Text file.  
 (c) Unicode BigEndian Text file.
   这三种都直接支持.

 
   Q 到底这三种格式是什么东西??????
   A 打开Windows2000的写字板, <另存为>, 你可以看到这三种选项. 把一个中文文件存来看看. 



Question 3
== 如何使用True Type Font来看书呢 ? ============    

 True Type font的好处是字体漂亮, 然后字体的大小可以随意调节.
点阵字体则很难调节大小.....

本软件并没有自带TrueTypeFont字体, 所以需要您自己提供一个.
您按以下三个简单步骤操作一下就可以使用True Type字体了.


(a) 找一个TrueType字体来.
    例如          arial.TTF ( Arial Font)  
                  cour.TTF  ( Courier New Font )
                  comic.TTF  ( Commic Sans Font )
                  SIMSUN.TTC ( 中文宋体 )
                      ...
    
    在C:\WINNT\Fonts\ 目录里面有非常多TrueType字体文件, 通过鼠标拖拉的方式可以拷贝到任何地方..
     
    
(b) 把你选定的True Type 字体文件拷贝到PSP的记忆棒上.
    拷贝到任何路径都可以.
    本软件一次只用一个TrueType文件, 所以你拷贝一个TrueType文件去就可以了, 
    不需要把你看到的TrueType文件全部都拷贝去哦!
            
       例如
        copy SIMSUN.TTC to g:\  ( 假设 g:盘就是你的记忆棒的驱动器盘符.)
          or 
        copy cour.TTF to g:\
      
       重要: 
        如果要显示汉字字体, 则必须要拷贝汉字TrueType字库. 只拷贝英文字库是无法显示汉字的.
        要显示汉字, 就必须要例如拷贝  simsun.ttc (宋体) 这类汉字字体.  
        事实上, 这个simsun.ttc(宋体) 而且还可以正确的显示日文......我猜韩文也可以, 没试过.
        
 
(c)    在PC端关闭PSP的USB连接, 
       运行Kitty Book,  进入配置窗口[Config Window], 选择打勾框[use TrueTypeFont],
    按<O>打勾, 表示确认使用TrueType字体.
       然后.按 <TTF File...> 按钮, 一个文件选择窗口会出现, 请选择你刚才拷贝的TrueType文件.
       请只选择 *TTF 和 *TTC 文件, 请不要选择其它非TrueType字库的文件.
       完成后, 去保存按钮, 按<O>保存退出.
    

 完成.
      
    

Question 4
== 任何改变阅读器里面True Type字体的大小 ? ========================
   只有当你使用True Type字体的时候 ,才可以改变字体的大小.
内建的点阵字体不能改变大小.
   进入配置窗口[Config Window], 按<上/下键>去[Font Size]数值框, 按左右来改变它的数值.
16 到 50 均可, 这是字体的点数. 
   完成后, 去保存按钮, 按<O>保存退出.




Question 5 
== 如何改变阅读器里的字体颜色和背景颜色 ? =========================
当使用True Type字体和内建点阵字体的时候, 都可以设置不同的颜色.
进入配置窗口, 按[字体颜色]或者[背景颜色]按钮, 会打开一个颜色设定窗口.
用上下键在RGB之间选择, 按左右改变它的值,  0 - 255 .
不同RGB的值确定一个颜色.      例如 RGB 等于 0,0,0是全黑, RGB等于 255,255,255是全白.
设定完成后按<O>确认,  取消颜色的改变则按<X>.
你刚才选定的颜色会出现在颜色框中.
按<保存按钮> 退出.



Question 6
== 如果我不要用True Type字体, 如何设置?  ====================================
在配置窗口 去[use TrueTypeFont], 按 <O>改变, 不要打勾.
保存退出.

这样, Kitty Book会使用内建的16点阵字体.


Question 7.
== 什么版本的PSP可以用Kitty book  ? ==========================
理论上, 任何可以运行非官方软件的PSP均可.
例如1.0和1.5

我的PSP是 1.5, 所以我没试验过1.0的PSP, 但是它肯定也可以运行的.



Question 8.
== 这个软件搞了多久 ?   ========================================
大概3个月. 我从七月份刚买PSP就开始了搞了.  游戏基本没玩过.....分特.

大字体显示对老人比较有用, 不然看不清...


Question 9.
== 开发语言?  工具?   ============================================
 C++, PSPTool chain, PSPSDK.




Question 10.
== 其它   =====================================
(*) 我提供了一些各种编码格式的文本文件供试用...它都能读取.

(*) 如果某些中文文件名不能正确在文件管理器里面显示, 这是正常的, 这是PSP操作系统的局限性,
不是本软件的bug也不是RPWT.

(*) 虽然我已经测试了无数遍, 但是可能还会有一些Bug.
如果你发现操作的时候突然黑屏了, 不要怕, 这很可能是软件里面的空指针操作而已.
请等待10秒钟, 什么按钮都不需要按( 按了也没用), PSP会自动关机的. 
然后再开机就可以了.

(*) 建议使用的字体文件 simsun.ttc,  能用于这些文件: ChineseGB/Big5/JP 
    该字库文件大约10MB.   



======================================================================================

多谢使用!




Thank you very much list:

(*) Thanks SONY to make such a good machine
(*) Thanks authors of PSPSDK, PSP Toolchain, it's a very Great tool/dev. env.
(*) Thanks GNU C++ tools. 
(*) Thanks FreeType Project group for the FreeType ultility.
(*) Thank you for using it !











