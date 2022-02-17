程序分成四个模块

主应用
app/server

数据库
lib/db

网络
lib/net

ui部分
lib/ui


其中ui依赖db和net
server依赖ui


要求db net ui都要编译成库，server链接这些库


