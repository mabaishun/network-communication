网络数据报文的格式定义

1. 报文有两个部分，包头和包体，是网络消息的基本单元
2. 包头：描述本次消息包的大小，描述数据的作用
3. 包体：数据

# 单线程网络测试（linux）
## 测试10个链接
带宽在11000Mbps上下浮动    
time:[ 1.0000050 ]  socket:[    5 ]  recv count:[   809897 ]  client:[   10 ]    
time:[ 1.0000000 ]  socket:[    4 ]  recv count:[   795916 ]  client:[   10 ]    
time:[ 1.0000040 ]  socket:[    9 ]  recv count:[   761532 ]  client:[   10 ]    
time:[ 1.0000080 ]  socket:[   12 ]  recv count:[   837104 ]  client:[   10 ]    
time:[ 1.0000000 ]  socket:[    7 ]  recv count:[   806804 ]  client:[   10 ]    
time:[ 1.0000010 ]  socket:[   13 ]  recv count:[   828718 ]  client:[   10 ]    
time:[ 1.0000110 ]  socket:[    9 ]  recv count:[   742850 ]  client:[   10 ]    
time:[ 1.0001270 ]  socket:[    5 ]  recv count:[   795472 ]  client:[   10 ]    
time:[ 1.0000010 ]  socket:[    5 ]  recv count:[   805470 ]  client:[   10 ]    
time:[ 1.0000050 ]  socket:[    4 ]  recv count:[   820281 ]  client:[   10 ]    
time:[ 1.0000000 ]  socket:[    9 ]  recv count:[   815028 ]  client:[   10 ]    
time:[ 1.0000200 ]  socket:[   13 ]  recv count:[   780808 ]  client:[   10 ]    
time:[ 1.0000020 ]  socket:[    4 ]  recv count:[   749769 ]  client:[   10 ]    
time:[ 1.0000030 ]  socket:[    7 ]  recv count:[   835772 ]  client:[   10 ]    
time:[ 1.0000000 ]  socket:[    4 ]  recv count:[   828529 ]  client:[   10 ]    
time:[ 1.0000040 ]  socket:[    7 ]  recv count:[   797259 ]  client:[   10 ]    


## 测试100个链接
带宽在10000Mbps上下浮动    
time:[ 1.0000160 ]  socket:[    4 ]  recv count:[   739430 ]  client:[  100 ]    
time:[ 1.0000020 ]  socket:[   77 ]  recv count:[   716749 ]  client:[  100 ]    
time:[ 1.0000180 ]  socket:[   37 ]  recv count:[   706075 ]  client:[  100 ]    
time:[ 1.0000010 ]  socket:[   58 ]  recv count:[   747944 ]  client:[  100 ]    
time:[ 1.0005160 ]  socket:[   16 ]  recv count:[   726879 ]  client:[  100 ]    
time:[ 1.0005590 ]  socket:[   72 ]  recv count:[   709459 ]  client:[  100 ]    
time:[ 1.0000390 ]  socket:[   77 ]  recv count:[   686517 ]  client:[  100 ]    
time:[ 1.0000010 ]  socket:[   48 ]  recv count:[   656171 ]  client:[  100 ]    
time:[ 1.0002340 ]  socket:[   85 ]  recv count:[   721257 ]  client:[  100 ]    
time:[ 1.0000050 ]  socket:[   49 ]  recv count:[   719544 ]  client:[  100 ]    
time:[ 1.0000000 ]  socket:[   18 ]  recv count:[   731786 ]  client:[  100 ]    
time:[ 1.0000120 ]  socket:[    4 ]  recv count:[   650220 ]  client:[  100 ]    
time:[ 1.0000040 ]  socket:[   44 ]  recv count:[   692673 ]  client:[  100 ]    
time:[ 1.0000100 ]  socket:[   26 ]  recv count:[   734258 ]  client:[  100 ]    
time:[ 1.0000680 ]  socket:[   55 ]  recv count:[   724741 ]  client:[  100 ]    
time:[ 1.0000050 ]  socket:[   86 ]  recv count:[   726676 ]  client:[  100 ]    
time:[ 1.0000110 ]  socket:[   86 ]  recv count:[   648468 ]  client:[  100 ]    
time:[ 1.0000010 ]  socket:[   65 ]  recv count:[   721082 ]  client:[  100 ]    
time:[ 1.0000000 ]  socket:[   56 ]  recv count:[   671591 ]  client:[  100 ]    
    

## 测试500个链接
带宽在8000Mbps上下浮动    
time:[ 1.0000220 ]  socket:[  256 ]  recv count:[   628724 ]  client:[  500 ]  
time:[ 1.0000080 ]  socket:[  113 ]  recv count:[   630494 ]  client:[  500 ]    
time:[ 1.0000040 ]  socket:[  219 ]  recv count:[   622267 ]  client:[  500 ]    
time:[ 1.0000010 ]  socket:[  289 ]  recv count:[   600216 ]  client:[  500 ]    
time:[ 1.0000040 ]  socket:[  386 ]  recv count:[   589392 ]  client:[  500 ]    
time:[ 1.0000100 ]  socket:[  123 ]  recv count:[   619855 ]  client:[  500 ]    
time:[ 1.0000000 ]  socket:[  190 ]  recv count:[   646124 ]  client:[  500 ]    
time:[ 1.0000000 ]  socket:[  206 ]  recv count:[   625344 ]  client:[  500 ]    
time:[ 1.0000000 ]  socket:[    8 ]  recv count:[   556901 ]  client:[  500 ]    
time:[ 1.0000030 ]  socket:[  194 ]  recv count:[   614308 ]  client:[  500 ]    
time:[ 1.0000040 ]  socket:[  127 ]  recv count:[   641987 ]  client:[  500 ]    
time:[ 1.0000180 ]  socket:[   41 ]  recv count:[   595731 ]  client:[  500 ]    
time:[ 1.0000110 ]  socket:[  464 ]  recv count:[   623077 ]  client:[  500 ]    
time:[ 1.0000010 ]  socket:[  276 ]  recv count:[   560370 ]  client:[  500 ]    
time:[ 1.0000000 ]  socket:[   26 ]  recv count:[   523256 ]  client:[  500 ]    
time:[ 1.0000010 ]  socket:[  297 ]  recv count:[   595758 ]  client:[  500 ]    
    
## 测试1000个链接
带宽在8000Mbps上下浮动    
time:[ 1.0000010 ]  socket:[   11 ]  recv count:[   523016 ]  client:[ 1000 ]    
time:[ 1.0000030 ]  socket:[  981 ]  recv count:[   565662 ]  client:[ 1000 ]    
time:[ 1.0000030 ]  socket:[  781 ]  recv count:[   558123 ]  client:[ 1000 ]    
time:[ 1.0000030 ]  socket:[  874 ]  recv count:[   558978 ]  client:[ 1000 ]   
time:[ 1.0000010 ]  socket:[  286 ]  recv count:[   500126 ]  client:[ 1000 ]    
time:[ 1.0000020 ]  socket:[  648 ]  recv count:[   529928 ]  client:[ 1000 ]    
time:[ 1.0000010 ]  socket:[   62 ]  recv count:[   554787 ]  client:[ 1000 ]    
time:[ 1.0000560 ]  socket:[    4 ]  recv count:[   556272 ]  client:[ 1000 ]    
time:[ 1.0000630 ]  socket:[  348 ]  recv count:[   551631 ]  client:[ 1000 ]    
time:[ 1.0000080 ]  socket:[  777 ]  recv count:[   515103 ]  client:[ 1000 ]    
time:[ 1.0000010 ]  socket:[  370 ]  recv count:[   525061 ]  client:[ 1000 ]    
time:[ 1.0000020 ]  socket:[  838 ]  recv count:[   555805 ]  client:[ 1000 ]    
time:[ 1.0000170 ]  socket:[  279 ]  recv count:[   561638 ]  client:[ 1000 ]    
    
## 总结：
链接数尽量再10 - 100 中间    
