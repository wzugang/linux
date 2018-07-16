1.       启动GDB的几种方法：

·         GDB 启动时，可以加上一些 GDB 的启动开关，详细的开关可以用 gdb -help 查看

·         gdb <program>  program 也就是你的执行文件，一般在当前目录下

·         gdb <program> core  用 gdb 同时调试一个运行程序和 core 文件，core 是程序非法执行后 core dump 后产生的文件。等同于gdb -c core <program >

·         gdb <program> <PID> 如果你的程序是一个服务程序，那么你可以指定这个服务程序运行时的进程 ID。gdb会自动 attach 上去，并调试他。program 应该在 PATH 环境变量中搜索得到。PID可以通过ps -ef查询

·         gdb -c core  它会启动GNU的调试器，来调试core文件，并且会显示生成此core文件的程序名，中止此程序的信号等等(c是core的简写)

·         gdb attach PID，功能同gdb <program> <PID>，不会拉取新的进程，直接调试当前进程，进入gdb后进程就挂起了，需要使用continue(c)使进行继续运行

·         启动GDB是可以使用-tui参数打开文本界面。使用layout xxx来打开其他窗口。使用ctrl+x A或ctrl+x ctrl+a来切换，在这个模式下，可以使用ctrl+p和ctrl+n来切换命令，可能layout会造成控制台花屏,使用ctrl+L清屏

·         cgdb是GDB的页面增强版，用来替代gdb的 gdb -tui，代码显示部分会有语法高亮。

2.       file 装入想要调试的可执行文件. (也可以gdb 文件名：在进入gdb时自动装入该可执行文件)

3.       kill 终止正在调试的程序.

4.       run(简写r) 执行当前被调试的程序，如果需要应该使用set args 设置运行的参数。例如，smmget命令使用GDB调试时，需要通过set args –d version，然后使用run命令运行。

5.       quit(简写q)  终止gdb

6.       暂停 / 恢复程序运行：

·         break(简写b) 在代码里设置断点,  这将使程序执行到这里时被挂起。设置断点的几种方式，可以使用info命令查看设置过的断点。

Ø        break function：可以进入指定函数时暂停程序；

Ø        break linenum：在指定行号停住；

Ø        break ±offset：当前行号的前面或后面的offset行停住；

Ø        break filename:linenum：在源文件filename的linenum行处停住；

Ø        break filename:function：在源文件filename的function函数的入口处停住；

Ø        break *address：在程序运行的内存地址处停住。

Ø        break：如果break不带任何参数，就在下一个指令设置断点

Ø        break ... if <condition>：如果条件为真时，程序停住，特别是在循环体里面调试的时候非常方便。例如：break if i=100；表示当i值等于100的时候程序暂停运行。

b.       watchpoint(watch) ：设置一个监视点，使你能监视一个变量的值而不管它何时被改变. (需要run之后才能设置)(变量变化前后的值都会打印出来)

观察点一般来观察某个表达式（变量也是一种表达式）的值是否有变化了，如果有变化，马上停住程序。我们有下面的几种方法来设置观察点：

Ø        watch <expr>：表达式（expr）一旦有变化，就暂停程序；

Ø        rwatch <expr>：表达式（expr）被读时，暂停程序；

Ø        awatch <expr>：当表达式（变量）的值被读或被写时，暂停程序；

Ø        info watchpoints：列出所有观察点。

c.       catchpoint(catch)：设置捕捉点，用来补捉程序运行时的一些事件。如：载入共享库（动态链接库）或是C++的异常。设置捕捉点的格式为：catch <event> 当 event 发生时，停住程序。event 可以是下面的内容：

·   1、throw 一个 C++抛出的异常。（throw 为关键字）

·   2、catch 一个 C++捕捉到的异常。（catch 为关键字）

·   3、exec  调用系统调用 exec 时。（exec 为关键字，目前此功能只在 HP-UX 下有用）

·   4、fork 调用系统调用 fork 时。（fork 为关键字，目前此功能只在 HP-UX 下有用）

·   5、vfork  调用系统调用 vfork 时。（vfork 为关键字，目前此功能只在 HP-UX 下有用）

·   6、load 或 load <libname> 载入共享库（动态链接库）时。（load 为关键字，目前此功能只在 HP-UX 下有用）

·   7、unload  或  unload  <libname>  卸载共享库（动态链接库）时。（unload 为关键字，目前此功能只在 HP-UX 下有用）

·   8、tcatch <event> 只设置一次捕捉点，当程序停住以后，应点被自动删除。

d.       维护停止点：clear、delete、disable、enable (停止点就是指上面的三类)

·   clear：清除所有的已定义的停止点

·   clear <function>、clear <filename:function>清除所有设置在函数上的停止点。

·   clear <linenum>、clear <filename:linenum>清除所有设置在指定行上的停止点。

·   delete [breakpoints] [range...]删除指定的断点，breakpoints 为断点号。如果不指定断点号，则表示删除所有的断点。range 表示断点号的范围（如：3-7）。其简写命令为 d。

·   disable [breakpoints] [range...]disable 所指定的停止点，breakpoints 为停止点号。如果什么都不指定，表示 disable所有的停止点。简写命令是 dis.

·   enable [breakpoints] [range...]enable 所指定的停止点，breakpoints 为停止点号。

·   enable [breakpoints] once range...enable 所指定的停止点一次，当程序停止后，该停止点马上被 GDB 自动 disable。

·   enable [breakpoints] delete range...enable 所指定的停止点一次，当程序停止后，该停止点马上被 GDB 自动删除。

e.       为停止点设定运行命令：

我们可以使用 GDB 提供的 command 命令来设置停止点的运行命令。也就是说，当运行的程序在被停止住时，我们可以让其自动运行一些别的命令，格式如下：

commands [bnum]

... command-list ...

end

例如：

break foo if x>0

commands

printf "x is %d\n",x

continue

end

断点设置在函数 foo 中，断点条件是 x>0，如果程序被断住后，也就是，一旦 x 的值在 foo 函数中大于 0，GDB 会自动打印出 x 的值，并继续运行程序。

如果你要清除断点上的命令序列，那么只要简单的执行一下 commands 命令，并直接在打个 end 就行了。

f.        断点菜单：

在 C++中，可能会重复出现同一个名字的函数若干次（函数重载），在这种情况下，break <function>不能告诉 GDB 要停在哪个函数的入口。当然，你可以使用 break <function(type)>也就是把函数的参数类型告诉 GDB，以指定一个函数。否则的话，GDB会给你列出一个断点菜单供你选择你所需要的断点。你只要输入你菜单列表中的编号就可以了

g.       恢复程序运行：continue [ignore-count](简写c或fg)。当程序停住运行时，可以使用continue命令继续运行到结束或者到下一个断点。ignore-count 表示忽略其后的断点次数

h.       单步调试：

a.  next <count>(简写n)  执行一行源代码但不进入函数内部. count表示执行后面指令的条数。

b.  step <count>(简写s)  执行一行源代码而且进入函数内部. count表示执行后面指令的条数。

c.  finish：退出当前函数并打印当前函数的返回值

d.  set step-mode

set step-mode on：打开 step-mode 模式，于是，在进行单步跟踪时，程序不会因为没有 debug 信息而不停住。这个参数有很利于查看机器码。

set step-mod off：关闭 step-mode 模式。

e.  until 或 u：当你厌倦了在一个循环体内单步跟踪时，这个命令可以运行程序直到退出循环体。

f.  stepi 或 si、nexti 或 ni：

单步跟踪一条机器指令！一条程序代码有可能由数条机器指令完成，stepi 和 nexti 可以单步执行机器指令。与之一样有相同功能的命令是“display/i $pc” ，当运行完这个命令后，单步跟踪会在打出程序代码的同时打出机器指令（也就是汇编代码）

i.         handle <signal> <keywords...>：信号处理，keywords是指可以连续跟多个，如：handle SIG34 nostop noprint

在 GDB 中定义一个信号处理。信号<signal>可以以 SIG 开头或不以 SIG 开头，可以用定义一个要处理信号的范围（如：SIGIO-SIGKILL，表示处理从 SIGIO 信号到 SIGKILL的信号，其中包括 SIGIO，SIGIOT，SIGKILL 三个信号），也可以使用关键字 all 来标明要处理所有的信号。一旦被调试的程序接收到信号，运行程序马上会被 GDB 停住，以供调试。其<keywords>可以是以下几种关键字的一个或多个。：

nostop：当被调试的程序收到信号时，GDB 不会停住程序的运行，但会打出消息告诉你收到这种信号。

stop：当被调试的程序收到信号时，GDB 会停住你的程序。

print：当被调试的程序收到信号时，GDB 会显示出一条信息。

noprint：当被调试的程序收到信号时，GDB 不会告诉你收到信号的信息。

pass、noignore：当被调试的程序收到信号时，GDB 不处理信号。这表示，GDB 会把这个信号交给被调试程序会处理。

nopass、ignore：当被调试的程序收到信号时，GDB 不会让被调试程序来处理这个信号。

info signals、info handle：查看有哪些信号在被 GDB 检测中。

j.         线程（Thread Stops）：

如果你程序是多线程的话，你可以定义你的断点是否在所有的线程上，或是在某个特定的线程。GDB 很容易帮你完成这一工作。

break <linespec> thread <threadno>

break <linespec> thread <threadno> if ...

这个threadno是 GDB 分配的，你可以通过“info threads”命令来查看正在运行程序中的线程信息。

当你的程序被 GDB 停住时，所有的运行线程都会被停住。这方便你你查看运行程序的总体情况。而在你恢复程序运行时，所有的线程也会被恢复运行。那怕是主进程在被单步调试时。

thread x：将当前gdb线程设置成x

7.       查看栈信息：

backtrace(简写bt)，比较常用的是在分析core文件时，使用命令bt full显示调用栈的全部信息，如果编译中使用-g选项，GDB会自动关联当前执行指令对应的函数，以及调用者传入的形参值及局部变量的所有信息

·         backtrace <n>、bt <n> ：n 是一个正整数，表示只打印栈顶上 n 层的栈信息。

·         backtrace <-n> 、bt <-n> ：-n 表一个负整数，表示只打印栈底下 n 层的栈信息。

·         frame <n> 、f <n> ：n 是一个从 0 开始的整数，是栈中的层编号。比如：frame 0，表示栈顶，frame 1，表示栈的第二层，不加参数时表示打印当前栈信息。例如，f 3就跳转到frame 3的堆栈空间，就好像当前程序就停留在当前这条指令，其实，用了这命令后，GDB根据堆栈的信息恢复了现场，包括寄存器的值，通过这个功能可以了解底层CPU处理指令的过程以及函数调用的过程。不想打印栈信息时用：select-frame <n>

·         up <n>：表示向栈的上面移动 n 层，可以不打 n，表示向上移动一层。不想打印栈信息时用：up-silently <n>

·         down <n>：表示向栈的下面移动 n 层，可以不打 n，表示向下移动一层。不想打印栈信息时用：down-silently <n>

8.       list(简写l)   line1[,line2]列出产生执行文件的源代码的一部分，前提是源代码的位置在相应的目录中.

a.  list <linenum>：显示程序第 linenum 行的周围的源程序。

b.  list <function> ：显示函数名为 function 的函数的源程序。

c.  list ：显示当前行后面的源程序。

d.  list -：显示当前行前面的源程序。

list <first>, <last>：显示从 first 行到 last 行之间的源代码。

list , <last> ：显示从当前行到 last 行之间的源代码。

list +：往后显示源代码。

一般来说在 list 后面可以跟以下这们的参数：

<linenum>  行号。

<+offset>  当前行号的正偏移量。

<-offset>  当前行号的负偏移量。

<filename:linenum>  哪个文件的哪一行。

<function>  函数名。

<filename:function> 哪个文件中的哪个函数。

<*address>  程序运行时的语句在内存中的地址。

9.       搜索源代码

forward-search <regexp>

search <regexp>

向前面搜索。

reverse-search <regexp>

全部搜索。

其中，<regexp>就是正则表达式，也主一个字符串的匹配模式

10.   指定源文件的路径：

directory <dirname ... >、dir <dirname ... > ：加一个源文件路径到当前路径的前面。如果你要指定多个路径，UNIX 下你可以使用“:”，Windows 下你可以使用“;”。

directory ：清除所有的自定义的源文件搜索路径信息。

show directories ：显示定义了的源文件搜索路径。

11.   make 使你能不退出gdb 就可以重新产生可执行文件. 前提是有makefile文件

12.   shell 使你能不离开gdb 就执行UNIX shell 命令. 比如shell ls -al

13.   info：info命令可以显示诸如当前断点，观察点，寄存器等的信息。

·         info registers：查看寄存器的情况。（除了浮点寄存器）；

·         info registers <regname ...>：查看所指定的寄存器的情况；

·         info locals：打印出当前函数中所有局部变量及其值。

·         info args：打印出当前函数的参数名及其值。

·         info frame：这个命令会打印出更为详细的当前栈层的信息，比如：函数地址，调用函数的地址，被调用函数的地址，目前的函数是由什么样的程序语言写成的、函数参数地址及值、局部变量的地址等等。

·         info break [n]，n为可选，查看断点信息；

·         info watchpoints：列出当前所设置了的所有观察点。

·         info sysmol 0xXXXXX：显示对应内存对应的符号，编译时需要使用-g选项。

·         info threads：可以列出所有的线程，缺省设为当前的线程前面有一个*号

·         info func：显示所有函数名称

·         info files：显示被调试文件的详细信息

·         info program 来查看程序的是否在运行，进程号，被暂停的原因

·         info catch：打印出当前的函数中的异常处理信息，现在已经没有这个命令了

14.   gcore，可以立即产生一个coredump

15.   回车自动执行上一个命令(对大部分命令有效)

16.   set：设置程序入参或变量的值。设置程序输入参数：set args 参1，参2……；set i=2;可以用show args来查看设置的输入参数

17.   whatis、ptype 变量名：检测变量类型，其中whatis只打印变量类型，ptype会打印出变量结构(结构体变量会打印出结构体的定义)

18.   disassemble 函数名：打印指定函数的汇编代码，也可以使用 info line 命令来查看源代码在内存中的地址。info line 后面可以跟“行号”，“函数名”，“文件名:行号”，“文件名:函数名”，这个命令会打印出所指定的源码在运行时的内存地址；disass function 反汇编一个函数；disass 0xXXXXX 反汇编一个地址

19.   查看运行时的数据：

·         print(简写p)  显示表达式的值 (inspect是print的同义函数)

简写为p，用于查看运行时的数据。命令格式：print /<f> <expr>，<expr>是表达式，是你所调试的程序的语言的表达式，<f>是输出的格式，如下：

    x  按十六进制格式显示变量。

    d  按十进制格式显示变量。

    u  按十六进制格式显示无符号整型。

    o  按八进制格式显示变量。

    t  按二进制格式显示变量。

    a  按十六进制格式显示变量。

    c  按字符格式显示变量。

    f  按浮点数格式显示变量。

p {<type>} <addr>：表示一个指向内存地址<addr>的类型为 type 的一个对象。

可以一行打印多个变量的值，如：p {i,j,k}

b.       显示变量：可以随时查看以下三种变量（全局变量、静态全局变量和局部变量）的值，打印的时候跟的变量名和在写C语言的变量名是一样，例如，查看g_sysApp全局量的值时，可以使用命令：p *g_sysApp，其实，程序中g_sysApp是个全局指针，将指针所指向的地址以Csystem类的形式打印出来。

p file::variable、p function::variable：可以通过这种形式指定你所想查看的变量，是哪个文件中的或是哪个函数中的

最好使用“-gstabs”选项来关闭GUN的优化开关

c.       显示数组：p *<addr>@len，如p *0xXXXX@10，可以显示对应地址len大小的连续内存空间。

d.       打印函数的返回值：p sum(3)；sum是函数名，括号里的是sum的参数

e.       打印结构体各成员的值：p *io；io是指向某个结构体的指针

f.        打印之前打印的历史值：p &9

g.       打印构造数组的值：p *io@2；从io指向的内存开始，按照io的结构体，连续打印两组数据(后面一组数据可能非法)

h.       显示内存：examine (简写x) 查看内存中的信息，x/<n/f/u> <addr>，其中n、f、u 是可选的参数：

n  是一个正整数，表示显示内存的长度，也就是说从当前地址向后显示几个地址的内容。

f 表示显示的格式，参见上面。如果地址所指的是字符串，那么格式可以是 s，如果地址是指令地址，那么格式可以是 i。

u 表示从当前地址往后请求的字节数，如果不指定的话，GDB 默认是 4 个 bytes。u 参数可以用下面的字符来代替，b 表示单字节，h 表示双字节，w 表示四字节，g 表示八字节。当我们指定了字节长度后，GDB 会从指内存      定的内存地址开始，读写指定字节，并把其当作一个值取出来。例如：

a.  x/80x 0x10508c58显示以地址（0x10508c58）开始的80*4个字节内存空间

b.  x/10i 0x38070464将以地址（0x38070464）开始机器码，GDB会自动将其转换成汇编指令，便于分析整个程序的调用过程

c.  x/3uh 0x54320 表示，从内存地址 0x54320 读取内容，h 表示以双字节为一个单位，3 表示三个单位，u 表示按十六进制显示。

i.         自动显示：display

display <expr>

display/<fmt> <expr>

display/<fmt> <addr>

expr 是一个表达式，fmt 表示显示的格式(格式 i[输入汇编格式] 和 s 同样被 display 支持)，addr 表示内存地址

display/i $pc：$pc 是 GDB 的环境变量，表示着指令的地址，/i 则表示输出格式为机器指令码，也就是汇编。于是当程序停下后，就会出现源代码和机器指令码相对应的情形

undisplay <dnums...> 、delete display <dnums...>：删除自动显示，dnums 意为所设置好了的自动显式的编号。如果要同时删除几个，编号可以用空格分隔，如果要删除一个范围内的编号，可以用减号表示（如：2-5）

disable display <dnums...>、enable display <dnums...>：disable 和 enalbe 不删除自动显示的设置，而只是让其失效和恢复。

info display：查看 display 设置的自动显示的信息

可以一次设置多个变量观测点：display {i,j,k}

j.         设置显示选项：

a. set print address：设置地址显示开关 set print address on/off；show print address 查看当前地址显示选项是否打开；默认为on

b. set print array：设置数组显示开关 set print array on/off；show print array，默认为off

c. set print elements <number-of-elements>这个选项主要是设置数组的，如果你的数组太大了，那么就可以指定一个<number-of-elements>来指定数据显示的最大长度，当到达这个长度时，GDB 就不再往下显示了。如果设置为 0，则表示不限制。show print elements ：查看 print elements 的选项信息。

d. set print null-stop <on/off>如果打开了这个选项，那么当显示字符串时，遇到结束符则停止显示。这个选项默认为 off

e. set print pretty on/off ：如果打开 printf pretty 这个选项，那么当 GDB 显示结构体时会比较漂亮；show print pretty ，默认为off

f.  set print sevenbit-strings <on/off>：设置字符显示，是否按“\nnn”的格式显示，如果打开，则字符串或字符数据按\nnn 显示，如“\065”。show print sevenbit-strings：查看字符显示开关是否打开，默认为off

g. set print union <on/off>设置显示结构体时，是否显式其内的联合体数据；show print union：查看联合体数据的显示方式，默认为on

h. set print object <on/off>在 C++中，如果一个对象指针指向其派生类，如果打开这个选项，GDB 会自动按照虚方法调用的规则显示输出，如果关闭这个选项的话，GDB 就不管虚函数表了。这个选项默认是 off。

show print object：查看对象选项的设置。

i.         set print static-members <on/off>这个选项表示，当显示一个 C++对象中的内容是，是否显示其中的静态数据成员。默认是 on。show print static-members：查看静态数据成员选项设置

j.         set print vtbl <on/off>当此选项打开时，GDB 将用比较规整的格式来显示虚函数表示。其默认是关闭的。show print vtbl查看虚函数显示格式的选项。

k.       GDB环境变量：set $foo = *object_ptr；show convenience ：该命令查看当前所设置的所有的环境变量，如：

set $i = 0

print bar[$i++]->contents

于是，当你就不必，print bar[0]->contents, print bar[1]->contents 地输入命令了。输入这样的命令后，只用敲回车，重复执行上一条语句，环境变量会自动累加，从而完成逐个输出的功能

l.         查看寄存器：

info registers ：查看寄存器的情况。（除了浮点寄存器）

info all-registers：查看所有寄存器的情况。（包括浮点寄存器）

info registers <regname ...>：查看所指定的寄存器的情况。

用print命令也可以查看寄存器情况，只需要在寄存器名字前加一个$符号就可以了。如：p $eip

20.   改变程序的执行：

a.       修改变量值：print，比如print i=4，把变量 x 的值修改为 4；set var i=7，把变量 i 的值修改为 7

b.       跳转执行：jump <linespec>指定下一条语句的运行点。<linespce>可以是文件的行号，可以是 file:line 格式，可以是+num 这种偏移量格式。表式着下一条运行语句从哪里开始；jump <address>：这里的<address>是代码行的内存地址。

注意，jump 命令不会改变当前的程序栈中的内容，所以，当你从一个函数跳到另一个函数时，当函数运行完返回时进行弹栈操作时必然会发生错误，可能结果还是非常奇怪的，甚至于产生程序 Core Dump。所以最好是同一个函数中进行跳转。

可以使用“set $pc”来更改跳转执行的地址。如：set $pc = 0x485

c.       产生信号量：signal <singal>， <singal>为信号编号，如SIG34

d.       强制函数返回：return  、return <expression>使用 return 命令取消当前函数的执行，并立即返回，如果指定了<expression>，那么该表达式的值会被认作函数的返回值

e.       强制调用函数：call function：调用该函数

21.   GDB进程的初始化文件.gdbinit：

默认情况下该文件应放在用户根目录下才能在gdb启动的时候被加载，其他目录下的.gdbinit在加载时会被禁止。这时可以在根目录下的.gdbinit文件中加入set auto-load safe-path /("/"也可以换成指定的目录)，这样其他目录下的.gdbinit也可以被加载。两个.gdbinit可以同时存在，一般可以将公共性的放在根目录下的.gdbinit文件中，独有的如断点设置放在其他目录下的.gdbinit中。如根目录下的.gdbinit：

set auto-load safe-path /

define cls

shell clear

end

document cls

Clears the screen with a simple command.

end

此定义的上半部分（在 define ... end 动词所界定的范围内）构成了在调用该命令时所执行的代码。

此定义的下半部分（在 document ... end 所界定的范围内）由 GDB 命令解释器使用，用于在您键入help cls 时显示，也可以使用help user来查看所有的自定义命令

调试目录下的.gdbinit：

handle SIG34 nostop noprint

b func
