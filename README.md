# 班级积分管理系统

在班级的一体机上装上这个软件，利用积分和学习小组来激励同学上进。

目前都是测试版本。0.1.0以前的版本没有记录。因为在那之前我一直是直接修改代码、覆盖编译的。

学习小组的计分代码专门做成了单独的文件，是为了方便各个班级根据自己学校、班级的特点修改代码。

代码调用了一些Windows特有的函数，所以不能在其它操作系统上使用。毕竟学校电脑也没人会用MacOS吧。

希望使用此项目的人遵守GPL协议。

代码不全是我写的，但另一位（他是我同学）没有Github账号。部分代码由AI编写。

## 注意

本软件仅支持单机使用。由于作者后续要将这个控制台应用改为服务器端并开发客户端，作者会在2025年8月1日停止支持这个项目，转而基于它开发[ClassPointsManagementSystem](https://github.com/ShiMingXuanSimon/ClassPointsManagementSystem) （顺便修改一下名称的翻译错误）。

## 更新日志

### 0.2.0
 - 更人性的输出，使用户更能理解该做什么
 - 将密码默认值改为12345，而不是之前的一长串
 - 打破了只能添加49个学生的限制
 - 每星期自动归零积分变化和排名变化，取代原来的手动归零
 - 添加设置，可以设置密码和“变化归零操作”频率

*********
*********

# ClassScoreManageSystem

Install this APP on the computer in the class, and use score and study groups to make students progress.

Currently, they are all test versions. There are no records for versions before 0.1.0. Because before that, I always directly modified the code and overwrote the compilation.

The scoring code for study groups is specially made into a separate file to facilitate each class to modify the code according to the characteristics of their own school and class.

The code uses some Windows commands, so it cannot be used on other OS. I bet that no school computers will use MacOS or Linux. 

I hope that those who use this project abide by the GPL license. 

The code is not all written by me, but the other person (he is my classmate) doesn't have a Github account. Some part of the code is written by AI. Some of the words in this file are translated by AI. 

## Attention

This software only supports stand-alone use. Due to the author's subsequent moving it to sever-side and devenloping client-side app, the author will stop supporting the repository on August 1, 2025 and develop [ClassPointsManagementSystem](https://github.com/ShiMingXuanSimon/ClassPointsManagementSystem) based on it instead. (By the way, I fixed the translation error of the name)

## Changelog  

### 0.2.0  
- More user-friendly output to help users understand what to do better
- Changed the default password to "12345" instead of the previous long string  
- Removed the 49-student limit  
- Automatically reset point changes and rank changes weekly, replacing the previous manual reset  
- Added settings to change the password and the frequency of "reset point changes and rank changes"
