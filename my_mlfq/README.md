# Usage

在my_mlfq目录下:
- make all: 编译
- make run: 执行
- make clean: 清理

config文件格式：
~~~
1 20 0
2 2 4
3 15 4
4 3 10
5 10 11
~~~
每一行代表一个process，第一个数字为pid，第二个数字为总执行时间，第三个数字为开始等待调度的时间。

通过修改config文件配置，然后编译执行即可进行测试。