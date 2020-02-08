# 基本介绍  
本项目目标是开发遵循Midi协议的乐器。  
目前已经完成了midi键盘的开发。

## Midi键盘
Midi键盘的硬件继承了该项目：https://github.com/Dimsmary/MagePad  
软件同样使用Arduino开发，文件名为：Keyboard_4x8  
### 工作机制
键盘通过串口(COM)与计算机进行通讯。  
需要搭配：hairless-midiserial和loopMIDI这两个软件将串口转换为音乐软件可识别的MIDI接口。  
程序使用了Timer1进行按键扫描，这意味着Arduino舵机函数将不可用。
### 其他
工程中的键盘扫描函数：void key_scan()过于冗长有待优化。
 
## Midi小提琴
仍在开发