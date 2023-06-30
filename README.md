# 基于STM32的便携多媒体查看器
本项目以STM32F103RCT6开发板为主体，基于HAL开发，结合SD卡、W25Q128芯片、ST7735 LCD-TFT显示屏、VS1053模块、矩阵键盘等外部设备完成可以查看TXT格式文本文件，BMP、JPG/JPEG、GIF格式图片文件，MP3格式音乐格式的便携多媒体查看器。

## 使用说明

### 前提条件

#### 硬件准备

1. STMF103RCT6     魔女开发板（包含SD卡座，W25Q128 Flash芯片）
2. ST7735     LCD-TFT 1.8寸160*128 彩色显示屏
3. 正点原子 VS1053 音乐播放模块
4. SD卡（容量不超过16GB）
5. 3.5mm耳机设备或者音响设备用于播放音乐

#### 软件准备

1. Windows     10 64bit
2. STM32CubeMX
3. Keil     uVision5

#### 模块连接

1. VS1053与STM32F103RCT6开发板连接

| VS1053引脚 | 对应的STM32开发板上的引脚 |
| ---------- | ------------------------- |
| XCS        | PA8                       |
| XDCS       | PA4                       |
| SCK        | PA5                       |
| SI         | PA7                       |
| SO         | PA6                       |
| DREQ       | PA12                      |
| RST        | PA11                      |

2. 矩阵键盘的连接

| 矩阵键盘行列 | 对应的STM32开发板上的引脚 |
| ------------ | ------------------------- |
| 行1          | PB7                       |
| 列1          | PC0                       |
| 列2          | PC1                       |
| 列3          | PC2                       |
| 列4          | PC3                       |

矩阵键盘行列排布如下：

![image-20230628193838844](https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/image-20230628193838844.png)

#### SD卡文件准备
| 文件夹 | 存储的文件 |
| ------------ | ------------------------- |
| txt          | 文本文件                  |
| pic          | 图片文件                  |
| music        | 音乐文件                  |

### 使用过程

#### 1. 克隆仓库

```shell
git clone https://github.com/guofuwei/Course-Exercise-of-Software-Design.git
```

#### 2. 打开Keil uVision5进行配置

1. 打开`Project/MDK-ARM`目录下的`Hardware-Course-Design.uvprojx`文件

2. 注意安装 STMF103开发包

2. 配置魔术棒设置

在Device页面选择STMF103RC开发板

<img src="https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/image-20230630111003244.png"/>

在Target页面勾选`Use MicroLIB`，调整IROM1和IRAM1大小

<img src="https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/image-20230630110828759.png" alt="image-20230630110828759"/>

在Output页面勾选`Create HEX File`

<img src="https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/image-20230630111053777.png" alt="image-20230630111053777"/>

在Debug页面选择`CMSIS-DAP Debugger`，点击`Settings`按钮

<img src="https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/image-20230630111151118.png" alt="image-20230630111151118"  />

在Settings页面中Reset设置选择`SYSRESETREQ`

![image-20230630111359398](https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/image-20230630111359398.png)

在Flash Download勾选`Reset and Run`

![image-20230630111437378](https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/image-20230630111437378.png)

#### 3.编译和烧录

点击`编译`和`加载`按钮即可将程序烧录到板子上

![image-20230630111600630](https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/image-20230630111600630.png)

#### 4.【可选】重新设计项目

使用`STM32CubeMX`打开`Project/Hardware-Course-Design.ioc`文件

## 整体框架

整体硬件设计图

![img](https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/clip_image002.png)

系统运行流程图

![img](https://hanshansite-1307452666.cos.ap-shanghai.myqcloud.com/site-img/clip_image002.png)

## 功能说明

1. 支持TXT格式文本查看

   * 支持前后翻页

2. 支持BMP、JPG、GIF格式图片查看

3. 支持MP3格式音乐播放

   * 支持播放暂停功能

   * 支持音量加减

   * 支持播放快进

## 参考文献

[TJpgDec - Tiny JPEG Decompressor (elm-chan.org)](http://elm-chan.org/fsw/tjpgd/00index.html)

[STM32项目设计：基于STM32F4的电子阅读器制作教程_stm32电子书_化作尘的博客-CSDN博客](https://blog.csdn.net/mbs520/article/details/110817173)
