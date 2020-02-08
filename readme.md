# 基本介绍  
本项目目标是开发遵循Midi协议的乐器。  
目前已经完成了midi键盘的开发。

## Midi键盘
Midi键盘的硬件继承了该项目：https://github.com/Dimsmary/MagePad  
软件同样使用Arduino开发，文件名为：Keyboard_4x8  
### 工作机制
键盘通过串口(COM)与计算机进行通讯。  
需要搭配：hairless-midiserial和loopMIDI这两个软件将串口转换为音乐软件可识别的MIDI接口。  

## midi小提琴
仍在开发