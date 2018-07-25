```

1. 格式化字符串

字符串格式化使用字符串格式化操作符即百分号%来实现。在%左侧放置一个字符串(格式化字符串)，而右侧则放置希望格式化的值(可以是元组或字典等)。

基本的转换说明符包含以下部分，注意其中的顺序：

%字符：标记转换说明符的开始
转换标识(可选)：-表示左对齐；+表示在转换值之前要加上正负号；""空白字符表示正数之前保留空格；0表示转换值若位数不够则用0填充
最小字段宽度(可选)：转换后的字符串至少应该具有该值指定的宽度。如果是*，则宽度会从值元组中读出
点(.)后跟精度值(可选)：如果转换的是实数，精度值就表示出现在小数点后的位数。如果转换的是字符串，那么该数字就表示最大字段宽度。如果是*，那么精度将会从元组中读出。
转换类型，详见下表：
字符串格式转换类型
格式	描述
%c	字符及其ASCII码
%s	字符串(使用str转换任意Python对象)
%r	字符串(使用repr转换任意Python对象)
%d(%i)	有符号整数(十进制)
%u	无符号整数(十进制)
%o	无符号整数(八进制)
%x	无符号整数(十六进制)
%X	无符号整数(十六进制大写字符)
%e	浮点数字(科学计数法)
%E	浮点数字(科学计数法，用E代替e)
%f(%F)	浮点数字(用小数点符号)
%g	浮点数字(根据值的大小采用%e或%f)
%G	浮点数字(类似于%g)
%p	指针(用十六进制打印值的内存地址)
%n	存储输出字符的数量放进参数列表的下一个变量中
 

1.1 简单转换

简单的转换只需要写出转换类型即可，如下：

复制代码
 1 # --- coding: utf-8 ---
 2 from math import pi
 3 
 4 # 输出：'price of eggs: $42'
 5 print 'price of eggs: $%d' % 42
 6 
 7 # 输出：'Hexadecimal price of eggas: 2a'
 8 print 'Hexadecimal price of eggas: %x' % 42
 9 
10 # 输出：'Pi: 3.141593...'
11 print 'Pi: %f...'% pi
12 
13 # 输出：'Very inexact estimate of pi: 3'
14 print 'Very inexact estimate of pi: %i' % pi
15 
16 # 输出：'using str: 42'
17 print 'using str: %s' % 42L
18 
19 # 输出：'Using repr: 42L'
20 print 'Using repr: %r' %42L
复制代码
 

1.2 字段宽度和精度

转换说明符可以包括字段宽度和精度。字段宽度是转换后的值保留的最小字符个数，精度(对于数字转换来说)则是结果中应该包含的小数位数，或者(对于字符串转换来说)是转换后的值所能包含的最大字符个数。

复制代码
 1 # --- coding: utf-8 ---
 2 from math import pi
 3 
 4 # 输出:  3.141593
 5 print '%10f' % pi # 字段宽：10
 6 
 7 # 输出:      3.14
 8 print '%10.2f' % pi # 字段宽：10，精度：2
 9 
10 # 输出:3.14
11 print '%.2f' % pi #精度2
12 
13 # 输出:Guido
14 print '%.5s' % 'Guido van Rossum' # 精度：5
15 
16 # 输出:Guido(使用元组参数-精度)
17 print '%.*s' % (5,'Guido van Rossum') # 精度：5
18 
19 # 输出:     Guido(使用元组参数-字段宽，精度)
20 print '%*.*s' % (10,5,'Guido van Rossum') # 字段宽：10，精度：5
复制代码
  

1.3 符号、对齐和0填充

在字段宽度和精度值之前可以(可选)放置一个"标表"，该标表的值可以是：

0(零)：表示数字将会用0进行填充
+(加号)：表示不管是正数还是负数都标示出符号(在对齐时很有用)
-(减号)：表示左对齐数值
空格：表示正数前加上空格，这对需要对齐正负数时会很有用
具体使用方法可以参考下面的示例：

复制代码
 1 # --- coding: utf-8 ---
 2 from math import pi
 3 
 4 # 输出:0000003.14
 5 print '%010.2f' % pi
 6 
 7 # 输出:3.14
 8 print '%-10.2f' % pi
 9 
10 # 输出:
11 #    10
12 #   -10
13 print ('% 5d' % 10) + '\n' + ('% 5d' % -10)
14 
15 # 输出:
16 #   +10
17 #   -10
18 print ('%+5d' % 10) + '\n' + ('%+5d' % -10)
复制代码
  

2. 常用字符串函数

Python为字符串创建了很多有用的函数，了解更多请参考Python文档：http://docs.python.org/2/library/string.html，下面例举了一下常用的字符函数。

 

2.1 find

find函数可以在一个较长的字符串中查找子字符串。并返回子串所在位置的最左端索引，如果没有找到则返回：-1，如下所示：

复制代码
 1 # --- coding: utf-8 ---
 2 title = "Monty Python's Flying Circus"
 3 
 4 # 返回:6
 5 print title.find('Python') 
 6 
 7 # 返回:-1
 8 print title.find('ruby')
 9 
10 # 返回:6,提供查找起始点(包含)
11 print title.find('Python',6)
12 
13 # 返回:6,提供查找起始点(包含)和结束点(不包含)
14 print title.find('Python',6,12)
复制代码
  

2.1 join

join函数用来在队列中添加元素，和split函数作用相反。如下：

复制代码
 1 # --- coding: utf-8 ---
 2 seq = ['1','2','3','4','5']
 3 sep = '+'
 4 dirs ='','usr','bin','env'
 5 
 6 # 输出:1+2+3+4+5
 7 print sep.join(seq)
 8 
 9 # 输出:/usr/bin/env
10 print '/'.join(dirs)
11 
12 # 输出:C:\usr\bin\env
13 print 'C:' + '\\'.join(dirs)
复制代码
  

2.2 lower

lower函数返回字符串的小写字母版本。如下：

1 # --- coding: utf-8 ---
2 # 输出：apple
3 print 'APPLE'.lower()
  

2.3 replace

replace函数返回某字符串的所有匹配项均被替换自后得到字符串。如下：

1 # --- coding: utf-8 ---
2 # 输出：Theez eez a test.
3 print 'This is a test.'.replace('is','eez')

2.4 split

join的逆函数，用来将字符串分割成序列，如下：

复制代码
 1 # --- coding: utf-8 ---
 2 seq = '1+2+3+4+5'
 3 unix_dirs ='/usr/bin/env'
 4 windows_dir = r'C:\usr\bin\env'   
 5 
 6 print seq.split('+')
 7 print unix_dirs.split('/')
 8 print windows_dir.split('\\')
 9 # 分别打印输出：
10 # ['1', '2', '3', '4', '5']
11 # ['', 'usr', 'bin', 'env']
12 # ['C:', 'usr', 'bin', 'env']
复制代码
  

2.5 strip

strip函数剔除字符串两侧空格字符串(默认)：

复制代码
1 # --- coding: utf-8 ---
2 fruit = ' apple and orange '
3 print repr(fruit)
4 print repr(fruit[:].strip())
5 print repr(fruit.strip(' app'))
6 # 分别输出：
7 # ' apple and orange '
8 # 'apple and orange'
9 # 'le and orange'
复制代码
  

2.6 translate

translate函数和replace函数一样，用于替换字符串中的某个部分，但是和replace不同，translate只处理单个字符，而且可以同时进行多个替换。在使用translate函数转换之前，需要一张转换表，转换表中是以某个字符替换某个字符的对应关系，这个表中有256个字符，我们可以通过使用string模块中的maketrans函数简化这一操作：

1 # --- coding: utf-8 ---
2 from string import maketrans
3 table = maketrans('cs','kz')
4 # 输出：thiz iz an inkredible tezt
5 print 'this is an incredible test'.translate(table)

```
