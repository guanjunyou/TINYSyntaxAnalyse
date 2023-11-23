###                                                            编译原理实验实验  TINY扩充语言的语法树生成

作者：华南师范大学 关竣佑


实验内容：

（一）为Tiny语言扩充的语法有

1.实现改写书写格式的新if语句
2.增加for循环；
3.扩充算术表达式的运算符号：+= 加法赋值运算符号（类似于C语言的+=）、求余%、乘方^， 
4.扩充扩充比较运算符号：==（等于），>(大于)、<=(小于等于)、>=(大于等于)、<>(不等于)等运算符号，
5.增加正则表达式，其支持的运算符号有：  或(|)  、连接(&)、闭包(#)、括号( ) 、可选运算符号（？）和基本正则表达式。 
6.增加位运算表达式，其支持的位运算符号有 and(与)、or（或）、 not(非），如果对位运算不熟悉，可以参考C/C++的位运算。

（二）对应的语法规则分别为：
1. 把TINY语言原有的if语句书写格式  
    if_stmt-->if exp then stmt-sequence end  |  | if exp then stmt-sequence else stmt-sequence end 
    改写为：
    if_stmt-->if(exp) stmt-sequence else stmt-sequence | if(exp) stmt-sequence

2.for语句的语法规则：
（1) for-stmt-->for identifier:=simple-exp  to  simple-exp  do  stmt-sequence enddo    步长递增1
  (2) for-stmt-->for identifier:=simple-exp  downto  simple-exp  do  stmt-sequence enddo    步长递减1
3. += 加法赋值运算符号、求余%、乘方^等运算符号的文法规则请自行组织。
4.==（等于），>(大于)、<=(小于等于)、>=(大于等于)、<>(不等于)等运算符号的文法规则请自行组织。
5.为tiny语言增加一种新的表达式——正则表达式，其支持的运算符号有：  或(|)  、连接(&)、闭包(#)、括号( ) 、可选运算符号（？）和基本正则表达式，对应的文法规则请自行组织。
6.为tiny语言增加一种新的语句，ID:=正则表达式  
7.为tiny语言增加一种新的表达式——位运算表达式，其支持的运算符号有  and(与)  、or (或)、非(not)。
8.为tiny语言增加一种新的语句，ID:=位运算表达式  
9.为了实现以上的扩充或改写功能，还需要注意对原tiny语言的文法规则做一些相应的改造处理。 

