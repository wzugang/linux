有时候，我们需要在 Bash 中重复执行先前的命令。你当然可以使用上方向键来查看之前曾经运行过的命令。但这里有一些更好的方式：

1) !!：重复执行上一条指令

2) !a：重复执行上一条以a为首的指令

3) !number：重复执行上一条在history表中记录号码为number的指令

4) !-number：重复执行前第number条指令

5) !$：表示获得上一条命令中的最后一项内容

6) 用Ctrl + r 组合键来进入历史搜索模式在history表中查询某条过往指令，找到需要重复执行的命令后，按回车键即可重复命令参数(即上一点中的第5条)

先来看一个例子： mkdir /exampledircd !$

本例中，第一行命令将创建一个目录，而第二行的命令则转到刚创建的目录。

这里，“!$”的作用就是重复前一个命令的参数。

事实上，不仅是命令的参数可以重复，命令的选项同样可以。

另外，Esc + . 快捷键可以切换这些命令参数或选项。

用于编辑的快捷键Ctrl + a：

将光标定位到命令的开头Ctrl + e：

与上一个快捷键相反，将光标定位到命令的结尾Ctrl + u：

剪切光标之前的内容Ctrl + k：

与上一个快捷键相反，剪切光标之后的内容Ctrl + y：

粘贴Ctrl + u和Ctrl + k所剪切的内容Ctrl + t：

交换光标之前两个字符的顺序Ctrl + w：

删除光标左边的参数(选项)或内容Ctrl + l：

清屏Ctrl + d：输入已结束。

在shell下相当于一个exitCtrl + c：键盘中断请求。

Ctrl + s &Ctrl + q：

暂停/恢复屏幕输出Ctrl + n(↓)：

显示下一条命令Ctrl + p(↑)：

显示上一条命令Ctrl + b：

向回移动Ctrl + f：

向前移动Ctrl + shift +↓：

终端向下滚动Ctrl + shift +↑：

终端向上滚动Shift+pgup/pgdown：

终端上下翻页滚动处理作业

首先，使用 Ctrl + z 快捷键可以让正在执行的命令挂起。如果要让该进程在后台执行，那么可以执行 bg 命令。而 fg 命令则可以让该进程重新回到前台来。

使用 jobs 命令能够查看到哪些进程在后台执行。

你也可以在 fg 或 bg 命令中使用作业 id，

如： fg %3

又如： bg %7使用置换命令置换

先看例子： du -h -a -c $(find . -name *.conf 2>&-)

注意 $() 中的部分，这将告诉 Bash 运行 find 命令，然后把返回的结果作为 du 的参数。

进程置换

仍然先看例子： diff <(ps axo comm) <(sshuser@host ps axo comm)

该命令将比较本地系统和远程系统中正在运行的进程。

请注意 <() 中的部分。

xargs 看例：

find . -name *.conf -print0 | xargs -0 grep -l -Z mem_limit | xargs -0 -i cp {}

{}.bak 该命令将备份当前目录中的所有 .conf 文件。

使用管道

下面是一个简单的使用管道的例子：

ps aux | grep init 这里，

“|”操作符将 ps aux 的输出重定向给 grep init。

下面还有两个稍微复杂点的例子：

ps aux | tee filename | grep init 及： ps aux | tee -a filename | grep init将标准输出保存为文件 你可以将命令的标准输出内容保存到一个文件中，

举例如下： ps aux >filename 注意其中的“>”符号。

你也可以将这些输出内容追加到一个已存在的文件中： ps aux 》 filename

你还可以分割一个较长的行： command1 | command2 | … | commandN >tempfile1 cat tempfile1 | command1 | command2 | … | commandN >

重定向与组合

重定向流的例子： ps aux 2>&1 | grep init

这里的数字代表：0：stdin1：stdout2：sterr 上面的命令中，“grep init”不仅搜索“ps aux”的标准输出，而且搜索 sterr 输出。

你也可以将这些输出内容追加到一个已存在的文件中： ps aux 》 filename

你还可以分割一个较长的行： command1 | command2 | … | commandN >tempfile1 cat tempfile1 | command1 | command2 | … | commandN >

Ubuntu中的一些常用的快捷键：

窗口管理Win+E 显示所有桌面，方便的左右选择。

Ctrl + Alt + → / ← 在不同工作台间切换

Ctrl + Alt + Shift + → / ← 移动当前窗口到不同工作台

Alt + Shift+ ↑ 以很酷的方式显示当前窗口

Ctrl + Alt + D 显示桌面/恢复当前窗口

Alt+ Tab 切换窗口

Alt + F4 关闭窗口

Alt + F5 取消最大化窗口 (恢复窗口原来的大小)

Alt + F7 移动窗口 (注： 在窗口最大化的状态下无效)

Alt + F8 改变窗口大小 (注： 在窗口最大化的状态下无效)

Alt + F9 最小化窗口

Alt + F10 最大化窗口

Alt + Space 打开窗口的控制菜单 (点击窗口左上角图标出现的菜单)

Ctrl+Alt+F7 返回桌面

文件浏览器NautilusShift+Ctrl+N 新建文件夹， 很有用

Alt + Enter 查看选择文件/文件夹的属性，代替单击右键选择属性

Ctrl + 1/2 改变文件夹视图查看方式，图标视图/列表视图

Ctrl + W 关闭当前Nautilus标签

Ctrl + Shift + W 关闭所有Nautilus标签

Ctrl+T 在Nautilus中新建一个标签

Ctrl+Page Up 上一个标签

Ctrl+Page Down 下一个标签

Alt + ↑/↓ 移动到父文件夹/选择的文件夹

Alt + → / ← 后退/前进

Alt + Home 直接移动到主文件夹

Alt+N 切换到第N个标签(N为数字)

F9 开关显示隐藏Nautilus侧边栏

Ctrl + H 开关显示隐藏文件夹

应用程序中的常用快捷键Ctrl+N 新建窗口

Ctrl+X 剪切

Ctrl+C 复制

Ctrl+V 粘贴

Ctrl+Z 撤销上一步操作

Ctrl+Shift+Z 重做刚撤销的一步操作

Ctrl+S 保存

其他Ctrl + Alt + L 锁屏

Alt + F1 打开应用程序菜单

Alt + F2 打开运行应用程序对话框

Win + 鼠标滚轮 放大/缩小屏幕

PrintScreen 全屏截图

Alt + PrintScreen 当前窗口截图

Ctrl+Alt+Shift+Fn 终端N或模拟终端N(n和N为数字1-6)

