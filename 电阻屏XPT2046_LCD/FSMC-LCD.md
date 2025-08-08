## 第 5 章 存储器和寄存器

### 5.1 常见存储器

![%E5%AD%98%E5%82%A8%E5%99%A8.png](https://p.sda1.dev/22/94360fbb71926571ba82e585792934c5/存储器.png)

#### ① RAM

RAM，Random-Access Memory，随机访问存储器，分为 SRAM 和 DRAM。

**(1) SRAM**

**名称：** SRAM，Static Random-Access Memory，静态随机存取存储器。

**基本结构：**基本结构就是前面大家学习过的那种触发器结构（比如D触发器），只要保持通电，里面储存的数据就可以恒常保持，这就是**”静态“**一词所形容的特性。

**特点：**读写速度快(纳秒级)、成本高，通常容量较小。

**应用场景**： 寄存器、CPU的L1/L2/L3缓存 、MCU的内存、网络路由器的缓冲区等

![SRAM.png](https://p.sda1.dev/22/7e939e0c98eda018172ad58cf756b593/SRAM.png)

**(2) DRAM**

**名称： ** DRAM， Dynamic Random Access Memory，动态随机存取存储器。

**基本结构：**DRAM的存储单元以电容的电荷来表示数据，有电荷代表1，无电荷代表0。时间一长，代表1的电容会放电，代表0的电容会吸收电荷，因此它需要定期刷新操作，这就是**“动态“**一词所形容的特性。

刷新操作会对电容进行检查，若电量大于满电量的1 / 2，则认为其代表1，并把电容充满电；若电量小于1 / 2，则认为其代表 0，并把电容放电，藉此来保证数据的正确性。

**特点：** 读写速度较SRAM慢(微秒级)、成本较SRAM低，通常容量较SRAM大。

**应用场景：** 计算机内存、GPU显存、手机平板的运行内存。

![DRAM.png](https://p.sda1.dev/22/1a8e72d443ca6331f566729877536a13/DRAM.png)

#### ② ROM

ROM ，意为“Read Only Memory”，只读存储器。 是一种非易失性存储器，断电后数据不丢失 。 

早期ROM（如掩模ROM、PROM）出厂后无法修改数据，仅能读取 ; 后来有了支持写入的存储器(如EEPROM、Flash等)， ROM的核心特性是非易失性，与RAM（易失性）形成根本区别， 仍归类为ROM类型。  

ROM 类型分类如下:

| ROM类型 |                        名称说明                         |       擦写方式       |    典型应用场景    |
| :-----: | :-----------------------------------------------------: | :------------------: | :----------------: |
| 掩模ROM |              掩模只读存储器 <br> Mask ROM               |  出厂固化，不可修改  |    早期BIOS芯片    |
|  PROM   |         可编程只读存储器 <br> Programmable ROM          |      一次性编程      |      淘汰技术      |
|  EPROM  |        可擦除可编程只读存储器 <br> Erasable PROM        |      紫外线擦除      |    工业设备固件    |
| EEPROM  | 电可擦除可编程只读存储器 <br>Electrically Erasable PROM | 电擦除（按字节操作） |  微控制器配置存储  |
|  Flash  |                       闪存，Flash                       |        电擦除        | U盘、SSD、手机存储 |



### 5.2 STM32 的存储器

STM32包含片内 SRAM，它可以以字节、半字（16位）或全字（32位）访问；SRAM的起始地址是**0x2000 0000**；我们当前使用的 STM32F103ZET6 内部 SRAM 为 64 KB。

STM32包含片内 Flash 存储器，它可以以字节、半字（16位）或全字（32位）进行读取操作；Flash的起始地址是 **0x0800 0000**；当前使用的STM32F103ZET6内部Flash容量为 **512 KB**。该Flash支持以半字（16位）为单位进行编程操作，擦除操作需按页或扇区进行，通常用于存储用户程序代码和常量数据。 



### 5.3 STM32 的存储器映射

**存储器映射**是指将物理存储器（如 Flash、RAM、片上外设的寄存器）分配地址的过程，使CPU通过地址访问硬件资源。

地址总线决定可访问空间，STM32为32位地址总线，理论空间4GB。数据字节以**小端格式**（先存低位再存高位）存放在存储器中。

![%E5%A4%A7%E7%AB%AF%E5%92%8C%E5%B0%8F%E7%AB%AF.png](https://p.sda1.dev/22/eea84500445e6d8a584d9a1761207468/大端和小端.png)

**STM32F103ZET6 单片机 存储器映射表：**

![%E5%AD%98%E5%82%A8%E5%99%A8%E6%98%A0%E5%B0%84.png](https://p.sda1.dev/22/dce83c8aa8412ee3b28da4bd7f15a917/存储器映射.png)

ARM 内核把可访问的存储器空间分成8个主要块，每个块为512MB，芯片厂商在每块容量范围内设计各自特色的外设。

#### Block0

 存储代码的Flash区域，地址范围0*x*08000000起，包含用户程序代码和系统存储区（如BootLoader） 

| **地址范围**            | **容量** | **功能介绍**                                                 |
| ----------------------- | -------- | ------------------------------------------------------------ |
| 0x1FFF F810-0x1FFF FFFF | 4KB      | 预留区域，未分配具体功能。                                   |
| 0x1FFF F800-0x1FFF F80F | 16字节   | 可选字节，用于配置读写保护、BOR级别、软件/硬件看门狗及复位模式。 |
| 0x1FFF F000-0x1FFF F7FF | 2KB      | 系统存储器，存放ST出厂时固化的ISP自举程序（BootLoader），用户无法修改。 |
| 0x0808 0000-0x1FFF EFFF | 383MB    | 预留区域，未分配具体功能。                                   |
| 0x0800 0000-0x0807 FFFF | 512KB    | 片内Flash主存储区，存放用户编写的程序代码和常量数据。        |
| 0x0008 0000-0x07FF FFFF | 1MB      | 预留区域，未分配具体功能。                                   |
| 0x0000 0000-0x0007 FFFF | 512KB    | 启动区域，取决于BOOT引脚状态，映射到Flash、系统存储器或SRAM的别名。 |

#### Block1
存储运行时数据的SRAM区域，地址范围0*x*20000000起，用于堆、栈及全局变量。

|        地址范围         |               容量                |                        功能介绍                        |
| :---------------------: | :-------------------------------: | :----------------------------------------------------: |
| 0x2001 0000-0x3FFF FFFF | 536,805,376 字节 (512 MB - 64 KB) |   预留区域，未分配具体功能，用于未来扩展或特殊用途。   |
| 0x2000 0000-0x2000 FFFF |        65,536 字节 (64 KB)        | 主SRAM区域，用于存储运行时数据（全局变量、堆、栈等）。 |

#### Block2
片上外设寄存器区域，地址范围0*x*40000000起，通过AHB/APB总线控制GPIO、UART等外设

|        地址范围         |   容量    |                          功能介绍                          |
| :---------------------: | :-------: | :--------------------------------------------------------: |
| 0x4002 4400-0x5FFF FFFF | 约 511.9M |                 预留区域，未分配具体功能。                 |
| 0x4001 8000-0x4002 33FF |  45.5 KB  | AHB总线外设：连接高速外设（如DMA、SDIO、以太网控制器等）。 |
| 0x4001 4000-0x4001 7FFF |   16 KB   |                 预留区域，未分配具体功能。                 |
| 0x4001 0000-0x4001 3FFF |   16 KB   |   APB2总线外设：连接中低速外设（如GPIO、ADC、TIM1等）。    |
| 0x4000 7800-0x4000 FFFF |   34 KB   |                 预留区域，未分配具体功能。                 |
| 0x4000 0000-0x4000 77FF |  30.5 KB  |   APB1总线外设：连接低速外设（如USART2、I2C1、SPI2等）。   |

#### Block3/4/5

用于扩展外部存储器（如SRAM、NOR/NAND Flash），通过FSMC（Flexible Static Memory Controller）管理。 

### 5.4 STM32 的寄存器映射

在STM32中，寄存器映射通过头文件（如`stm32f10x.h`）实现， 通过这种方式，开发者可以直接使用`GPIOA`来访问GPIOA的寄存器，而不需要记住具体的地址。 

```c
// 外设基址
#define PERIPH_BASE           ((uint32_t)0x40000000) 
// APB2外设的基址 
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000) 
// GPIOA 外设的基址
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
// 做了类型转换，地址仍然是GPIOA 外设的基址
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
```





## 第 6 章 FSMC

### 6.1 FSMC 概述

FSMC（Flexible Static Memory Controller，灵活的静态存储器控制器），STM32 可以通过FSMC 与SRAM、ROM、PSRAM、Nor Flash 和 NandFlash 存储器的引脚相连，从而进行数据的交换。

要注意的是，FSMC 只能扩展静态的内存，不能是动态的内存，比如 SDRAM（ Synchronous  DRAM）  就不能扩展。

FSMC 把 AHB 总线上的操作指令和时序要求，转换成外设（如SRAM、NOR Flash、LCD等）支持的通信协议和时序信号。这样，我们可以直接通过地址访问这些外设，而无需控制复杂的硬件信号。 

### 6.2 FSMC 组成

![FSMC%E5%8A%9F%E8%83%BD%E6%A1%86%E5%9B%BE.png](https://p.sda1.dev/22/ad312b3c43ca195f2a009a0b14c7503f/FSMC功能框图.png)

FSMC主要由4部分组成：AHB总线接口（包括配置寄存器）、NOR-FLASH/SRAM控制器、NAND-FLASH/PC卡控制器、外设接口四个部分构成。                                                     

#### ① AHB 总线接口

1. **协议转换**：作为CPU等主设备访问FSMC的通道，将AHB的8/16/32位事务转换为外设协议（如NOR/SRAM的16位接口）。

2. **位宽适配**

   * 总线位宽＞外设时，拆分事务（如32位拆为2次16位传输）；

   - 总线位宽＜外设时，可能导致数据不一致（需结合外设类型规避）。

3. **配置寄存器**：通过寄存器定义外设类型、协议及时序参数，驱动NOR/SRAM或NAND/PC卡控制器。

#### ② NOR-FLASH 和 SRAM 控制器

1. **同步内存驱动**：读写依赖HCLK分频时钟，仅在操作时激活时钟信号。
2. **异步内存控制**：通过配置寄存器生成时序，支持8/16/32位异步SRAM/ROM及突发模式PSRAM/NOR闪存。
3. **自动时序生成**：根据外设特征（位宽、模式）和时序参数（建立/保持时间）生成对应驱动信号。

#### ③ NAND-FLASH 和 PC卡控制器

NAND/PC卡控制器用于驱动8位或者16位的NAND闪存以及16位的PC卡兼容设备。

> **注：**  FSMC还支持与PC卡的接口，PC卡是一种早期的便携式存储设备，通常用于扩展存储或I/O功能。 

#### ④ 外设接口

用于与要扩展外设联通用的。在接线时必须根据每个外设的特点，来进行合适的接线。

### 6.3 外部设备地址映射

![%E5%A4%96%E8%AE%BE%E5%9C%B0%E5%9D%80%E6%98%A0%E5%B0%84.png](https://p.sda1.dev/22/921ce72d8d6d0ee0088305e58b8a9595/外设地址映射.png)

FSMC 把外部存储器划分为固定大小为256M字节的 4 个 Bank，Bank1/2位于Block3，Bank3/4位于Block4，FSMC寄存器位于Block5。

- **Bank1**：通常用于NOR Flash、PSRAM（伪静态随机存储器）和SRAM，分为4个子存储区（Bank1_1、Bank1_2、Bank1_3、Bank1_4），每个子存储区的大小为64MB。
- **Bank2**：通常用于NAND Flash，大小为256MB。
- **Bank3**：通常用于NAND Flash，大小为256MB。
- **Bank4**：通常用于PC卡，大小为256MB。

 每个 Bank 可以独立配置为不同的存储器类型（如NOR Flash、SRAM、PSRAM、PC卡等）。 



### 6.4 FSMC 控制 NOR-FLASH/PSRAM 的时序

FSMC 外设支持输出多种不同的时序以便于控制不同的存储器，它具有6种模式：1，A，2/B，C，D，复用模式。

| **模式** |    **适用存储器**    |              **关键区别**               |
| :------: | :------------------: | :-------------------------------------: |
|  模式1   |      SRAM/PSRAM      |              共用读写时序               |
|  模式A   |      SRAM/PSRAM      |       `NOE`独立翻转，读写时序独立       |
| 模式2/B  |      NOR Flash       |           `NADV`翻转时序不同            |
|  模式C   |      NOR Flash       |    `NOE`和`NADV`均翻转，读写时序独立    |
|  模式D   | 异步存储器（带扩展） | `NADV`先翻转，`NOE`后翻转，读写时序独立 |
| 复用模式 |     复用接口设备     |  数据/地址线复用，需锁存器或`NADV`控制  |

1. **HCLK的同步作用**  

所有FSMC控制器输出信号（如地址线、片选NE、读写控制NOE/NWE）的变化均由内部HCLK同步，但HCLK本身不输出到外部存储器。这意味着外部存储器无法直接感知HCLK，但FSMC内部通过HCLK协调时序。

2. **数据采样的时序保证**  

   FSMC在片选信号NE失效前完成数据采样，确保满足存储器数据保持时间要求。这一机制通过配置地址建立时间（ADDSET）和数据建立时间（DATAST）实现，与HCLK的上升沿触发信号变化无关。

3. **同步写模式下的CLK下降沿切换**  

   在同步写模式（如PSRAM操作）中，读写数据在存储器时钟CLK的下降沿变化。此时FSMC通过HCLK协调内部时序，使数据在CLK下降沿前稳定，符合外部存储器的时序要求。这与HCLK上升沿触发控制器信号变化是并行且互补的机制。

**模式1 读操作：**

![norflash-sram-%E6%A8%A1%E5%BC%8F1-%E8%AF%BB%E6%93%8D%E4%BD%9C.png](https://p.sda1.dev/22/dee4b8b101806e6bd880bf2a85497115/norflash-sram-模式1-读操作.png)

**模式1 写操作：**

![norflash-sram-%E6%A8%A1%E5%BC%8F1-%E5%86%99%E6%93%8D%E4%BD%9C.png](https://p.sda1.dev/22/9455f20f69521786d3ab835c0deaf7f5/norflash-sram-模式1-写操作.png)



### 6.5 STM32 FSMC 相关寄存器（了解）

| 寄存器名称                      | 描述       | 相关控制位/标志位                                            |
| ------------------------------- | ---------- | ------------------------------------------------------------ |
| BCRx<br>FSMC_Bankx->BTCR[x*2-2] | 控制寄存器 | MBKEN：存储块使能<br>FSMC_BCR3_MTYP: 存储器类型,2个bit<br>FACCEN: 闪存访问使能<br>MWID: 数据总线位宽,2个字节<br>MUXEN: 数据和地址复用总线使能<br>EXTMOD: 读写时序是否独立<br>WREN: 写使能 |
| BTRx<br>FSMC_Bankx->BTCR[x*2-1] | 时序寄存器 | ADDSET: 地址建立时间, 4个bit<br>ADDHLD: 地址保持时间,4个bit<br>DATAST: 数据建立时间,8个bit |

## 第 7 章 LCD显示

### 7.1 液晶显示模块介绍

#### ① LCD 概述

LCD（液晶显示器，Liquid Crystal Display）， 它的基本原理是液晶面板通过调整电流或电压，控制液晶分子排列的角度，从而调节透过光的强度和方向，形成可视图像。 

![LCD%E5%8E%9F%E7%90%86.png](https://p.sda1.dev/22/8be803fb4d5107a1ae0ee31f2ba4b663/LCD原理.png)

#### ② LCD接口类型 

彩色LCD的连接方式主要包括：MCU模式**、**RGB模式、SPI模式、VSYNC模式、MDDI模式、DSI模式等，应用比较多的就是**MCU模式**和**RGB模式**。

**MCU模式**  

- **特点**：基于8080总线标准（I80），内置GRAM，数据需先存入显存再转换显示，控制简单，无需同步信号。  因为主要在单片机的领域使用，因此得名。 
- **适用场景**：中小尺寸静态显示（如嵌入式设备），成本低，但刷新率较低。 

**RGB模式** 

- **特点**：直接传输像素RGB数据，依赖外部控制器生成同步信号（如VSYNC/HSYNC），无内置GRAM，实时写入屏幕。 
- **适用场景**：大尺寸动态显示（如视频/动画），速度快，但需复杂时序控制。 

两者核心差异：**MCU依赖内部显存处理数据，适合静态场景；RGB直接流式传输，适合高刷新率动态场景**。

#### ③ 液晶模块 Z35IT002

Z350IT002 这是一款 TFT LCD（薄膜晶体管液晶显示器）模块，TFT LCD 以其优秀的显示性能和低功耗特性而广泛应用于各种电子设备。

![638c7726ac1ded4ff7f6f54e3e38015d.png](https://i.miji.bid/2025/02/18/638c7726ac1ded4ff7f6f54e3e38015d.png)

**分辨率**

该模块的分辨率为320RGB × 480点阵，屏幕横向有320个像素点，每个像素点由红、绿、蓝三种颜色组成，纵向有480个像素点。

**构造**

该模块由960个源（source）和480个门（gate）构成，源用于横向的像素点驱动，门用于纵向的像素点驱动。

**MCU接口**

Z350IT002 设计了易于 MCU 访问和控制的接口，这使得它非常适合于嵌入式系统或其他需要直接由 MCU 控制显示屏的应用。

**应用领域**

Z350IT002 适用于中小尺寸的显示需求，如便携式设备、工业控制面板、小型嵌入式系统等。

**显示效果**

作为一款TFT LCD，Z350IT002 可以提供良好的颜色对比度和亮度，适合于需要中等分辨率和高色彩质量的应用。 


#### ④ 控制器芯片 ILI9486

Z350IT002 内部使用的控制芯片是 **ILI9486**，该芯片由 [Ilitek 公司](https://www.ilitek.com.tw/)生产。

**ILI9486**芯片具有 320RGBx480 点的分辨率，包括960通道的源驱动器和480通道的门驱动器，以及用 320RGB x 480 点图形数据的 345600字节 GRAM 和电源供应电路。

ILI9486支持多种接口类型：

 ![ILI9486%E6%8E%A5%E5%8F%A3%E7%B1%BB%E5%9E%8B.png](https://p.sda1.dev/22/cf699797f1974bf1673c92d6759ffe7d/ILI9486接口类型.png)

我们使用 16 位并行 8080 接口。

#### ⑤ GRAM

GRAM （Graphics RAM） ，专为图形显示设计的静态显存（SRAM），直接存储像素数据，无需外部刷新 ，GRAM 中每个存储单元都对应着液晶面板的一个像素点，使像素点呈现特定的颜色，多个像素点组合起来就成为一段文字或者一副图。

GRAM总容量为 **345,600字节**，对应分辨率为 **320×480像素**，每个像素采用 **18位色深**（RGB各6位），支持 **262,144种颜色**

需要注意的是，ILI9486 支持 **16位并行接口**（如8080总线），此时输入数据采用 **RGB565格式**（R5G6B5），具体结构如下：

![GRAM-16bit.png](https://p.sda1.dev/22/771d4ef8b27c13f21d3ad7360e9c67f4/GRAM-16bit.png)

尽管输入为16位RGB565，但 ILI9486 的 **GRAM 存储为18位色深（RGB666）**，转换规则如下：

- **红色（R）**：5位输入 → 左移3位扩展为8位 → 取高6位（最低位补0）
- **绿色（G）**：6位输入 → 左移2位扩展为8位 → 取高6位（保留原始6位）
- **蓝色（B）**：5位输入 → 左移3位扩展为8位 → 取高6位（最低位补0）

支持 **65,536种颜色**（2^16^），虽低于18位模式的262,144色（2^18^），但已满足多数图形显示需求 。



### 7.2 案例：FSMC 控制 LCD 显示

#### ① 硬件电路设计

##### (1) LCD 模块相关引脚图

![%E6%A1%88%E4%BE%8B-%E7%A1%AC%E4%BB%B6%E7%94%B5%E8%B7%AF.png](https://p.sda1.dev/22/53216798c508ec4a88140bf221901a55/案例-硬件电路.png)

* D0-D15是16位数据总线接口。分别接FSMC的D0-D15。
* RD是读控制引脚，上升沿时读数据。接FSMC-NOE（PD4）。
* WR是写控制引脚，上升沿时写数据。接FSMC-NWE（PD5）。
* RS是数据或命令选择引脚RS=1写数据，RS=0写命令，接 FSMC-A10（PG0）。
* CS是片选引脚，低电平有效。接FSMC-NE4（PG12）。
* LEDA 是背光电源（3.0V-3.4V）引脚。
* LEDK是背光亮度控制引脚，可以通过给 LCD-BG（PB0） 输出PWM 方波来控制背光的亮度，占空比越大，背光就会越亮。
* RST是LCD复位引脚，低电平复位。接LCD-RST（PG15）。

##### (2) FSMC 设计

使用FSMC接口实现8080时序，我们使用 BANK1 中的第四个区域（片选信号NE4）映射 LCD 模块中的 GRAM，地址范围：0X6C000000 ~ 0X6FFFFFFF。

![%E6%A1%88%E4%BE%8B-FSMC.png](https://p.sda1.dev/22/2705f07ebf5b934b2f56dadceca9ec94/案例-FSMC.png)

注意：16位宽度的外部存储器，AHB总线上的地址会右移1位与FSMC_A 对应，HADDR[25:1]对FSMC_A[24:0]

![%E6%A1%88%E4%BE%8B-FSMC02.png](https://p.sda1.dev/22/d15f35d929b5eeec52d126d41111e82a/案例-FSMC02.png)

当A10=0时，表示**写命令**，所以地址是：`0x6C00 0000`。

当A10=1时，表示**写数据**，所以地址是：`0x6C00 0000 + （1<<11) = 0x6C00 0800`。

#### ③ HAL 库实现方式

##### (1) STM32CubeMX 配置：

FSMC 配置:

![%E6%A1%88%E4%BE%8B-HAL01.png](https://p.sda1.dev/22/ce19afff04c884c752e895835eb1cf92/案例-HAL01.png)

LED-BG引脚和LED-RST引脚：

![%E6%A1%88%E4%BE%8B-HAL02.png](https://p.sda1.dev/22/109d5c61200e4124b795cb4c97bd3e8d/案例-HAL02.png)

> **注意：** HAL库生成的工程默认C语言的优化级别为3,会导致代码执行异常.务必把优化级别设置为0！

##### (2) 代码

1. 移植以下文件到工程中

   ```
   LCD显示屏STM32F103ZET6_并口FSMC例程/SYSTEM/sys.h
   LCD显示屏STM32F103ZET6_并口FSMC例程/SYSTEM/sys.c
   
   LCD显示屏STM32F103ZET6_并口FSMC例程/HANDWARE/LCD/lcd.h
   LCD显示屏STM32F103ZET6_并口FSMC例程/HANDWARE/LCD/lcd.c
   LCD显示屏STM32F103ZET6_并口FSMC例程/HANDWARE/LCD/lcdfont.h
   ```

2. 修改 sys.h 如下

   ![new_demo01.png](https://p.sda1.dev/24/0cf1c44913407b495d0ccca3501d9627/new_demo01.png)

3. 修改 sys.c 如下

   ![new_demo02.png](https://p.sda1.dev/24/b42a51cf0e7db616bcb4627a0c9da43f/new_demo02.png)

4. 修改 lcd.h

   ![new_demo03.png](https://p.sda1.dev/24/8220f5f08ac175fbd911318ee51a5ec4/new_demo03.png)

5. lcd.c 中添加 LCD_Reset() 函数

   ```c
   // 静态函数：LCD复位
   static void LCD_Reset(void)
   {
   	// 拉低复位引脚
   	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
   
   	// 延时
   	HAL_Delay(500);
   
   	// 拉高复位引脚
   	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
   
   	// 延时
   	HAL_Delay(100);
   }
   ```

6. lcd.c 中修改 LCD_Init() 函数
   ```c
   //初始化lcd
   void LCD_Init(void)
   { 					
	// 复位
   	LCD_Reset();
   
   	HAL_Delay(50); 					// delay 50 ms 
   	Set_Dir(DFT_SCAN_DIR);
   
   	LCD_WR_REG(0xE0);
   	LCD_WR_DATA(0x00);
   	LCD_WR_DATA(0x10);
   	LCD_WR_DATA(0x0F);
   	LCD_WR_DATA(0x0E);
   	LCD_WR_DATA(0x11);
   	LCD_WR_DATA(0x0A);
   	LCD_WR_DATA(0x44);
   	LCD_WR_DATA(0x77);
   	LCD_WR_DATA(0x38);
   	LCD_WR_DATA(0x08);
   	LCD_WR_DATA(0x13);
   	LCD_WR_DATA(0x05);
   	LCD_WR_DATA(0x0B);
   	LCD_WR_DATA(0x02);
   	LCD_WR_DATA(0x00);
   
   	LCD_WR_REG(0XE1);
   	LCD_WR_DATA(0x0F);
   	LCD_WR_DATA(0x3B);
   	LCD_WR_DATA(0x39);
   	LCD_WR_DATA(0x0D);
   	LCD_WR_DATA(0x0F);
   	LCD_WR_DATA(0x05);
   	LCD_WR_DATA(0x53);
   	LCD_WR_DATA(0x43);
   	LCD_WR_DATA(0x3D);
   	LCD_WR_DATA(0x05);
   	LCD_WR_DATA(0x0F);
   	LCD_WR_DATA(0x04);
   	LCD_WR_DATA(0x23);
   	LCD_WR_DATA(0x1F);
   	LCD_WR_DATA(0x0F);
   
   	LCD_WR_REG(0x36);
   	LCD_WR_DATA(0x08|DFT_SCAN_DIR); 
   
   	LCD_WR_REG(0x3A);
   	LCD_WR_DATA(0x55); // 8bit,16bit=0x55 18bit=0x66
   
   	LCD_WR_REG(0XB1);    // Frame rate 70Hz b0 11
   	LCD_WR_DATA(0xB0);
   	LCD_WR_DATA(0x11); 
   
   	LCD_WR_REG(0XB4);
   	LCD_WR_DATA(0x02);
   
   	LCD_WR_REG(0XB6);
   	LCD_WR_DATA(0x00);  //MCU:0x02  RGB:0x22
   	LCD_WR_DATA(0x42);  // rotate 180 degree 0x42
   	LCD_WR_DATA(0x3B);
   
   	LCD_WR_REG(0XB7);
   	LCD_WR_DATA(0xC6);
   
   	LCD_WR_REG(0XC0);
   	LCD_WR_DATA(0x05);
   	LCD_WR_DATA(0x05);
   
   	LCD_WR_REG(0XC1);
   	LCD_WR_DATA(0x41);
   	LCD_WR_DATA(0x00);
   
   	LCD_WR_REG(0XC2);
   	LCD_WR_DATA(0x22);
   
   	LCD_WR_REG(0XC5);
   	LCD_WR_DATA(0x00);
   	LCD_WR_DATA(0x31);  // Adjust Flicker
   	LCD_WR_DATA(0x80); 
   		
   	LCD_WR_REG(0x11); //Exit Sleep
   	HAL_Delay(120);
   	LCD_WR_REG(0x29); //display on	
   	LCD_LED=1;
   }
   ```
   
   
   

