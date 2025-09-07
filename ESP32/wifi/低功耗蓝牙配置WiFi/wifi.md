# 使用低功耗蓝牙配置WiFi

# 主控芯片

主控芯片我们选择的是**<font style="color:red;">ESP32-S3FN8</font>**, 相比于门锁项目中用到的ESP32-C3芯片, ESP32-S3芯片性能更强, 外设更丰富.

+ Xtensa® 32 位 LX7 **<font style="color:red;">双核</font>**处理器, 时钟频率：最高 240 MHz
+ 384 KB ROM：用于程序启动和内核功能调用. 

<font style="color:red;">512 KB 片上 SRAM</font>：用于数据和指令存储，时钟频率可配置，最大 240 MH

<font style="color:#ee0000;">8MB</font><font style="color:#ee0000;">片内</font><font style="color:#ee0000;">Flash(</font><font style="color:#ee0000;">用于烧写程序</font><font style="color:#ee0000;">)</font>

+ 45 个可编程 GPIO
+ <font style="color:red;">2 个 SPI 接口用于连接FLASH和 RAM</font>
+ 2 个通用 SPI 接口
+ LCD 接口
+ <font style="color:red;">DVP 8 位 + 16 位摄像头接口</font>
+ 3 个 UART 接口
+ 2 个 I2C 接口
+ <font style="color:red;">2 个 I2S 接口</font>
+ RMT (TX/RX)
+ 4个54 位通用定时器, 1个52位系统定时器和 3个看门狗定时器
+ …

## SDK配置

| 作用                  | 配置名                   | 配置值        |
| --------------------- | ------------------------ | ------------- |
| 滴答频率配置为1000Hz  | FREERTOS_HZ              | 1000          |
| Flash类型             | ESPTOOLPY_FLASHMODE      | DOUT          |
| Flash大小             | ESPTOOLPY_FLASHSIZE      | 8MB           |
| Flash速度             | ESPTOOLPY_FLASHFREQ      | 80MHz         |
| SPI_FLASH的高性能模式 | SPI_FLASH_HPM            | Enable        |
| 使能外置PSRAM         | SPIRAM                   | ESP PSRAM打勾 |
| 外置RAM的速度         | SPIRAM_SPEED             | 80MHz         |
| Cpu主频               | ESP_DEFAULT_CPU_FREQ_MHZ | 240MHz        |

### 勾选Bluetooth 

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F09%2F07%2F18-06-10-8961b850be1fdd58b1bd745e2aabcd46-Snipaste_2025-09-07_18-05-56-edcd28.png)

### 更改空间大小

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F09%2F07%2F18-08-32-ce1b29637c6a2575c19988b55f6aaaf4-Snipaste_2025-09-07_18-07-34-7d51ca.png)

添加 partitions.csv 文件到根目录

```c
partitions.csv
```

```c
# Name,   Type, SubType, Offset,  Size, Flags
# Note: if you have increased the bootloader size, make sure to update the offsets to avoid overlap

nvs,      data, nvs,     ,        0x4000,
otadata,  data, ota,     ,        0x2000,
phy_init, data, phy,     ,        0x1000,
# Original App Section
ota_0,    app,  ota_0,   ,        3000K,
# New App Section
ota_1,    app,  ota_1,   ,        3000K,
```

在 SDK配置中勾选 Custom partition table CSV

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F09%2F07%2F18-11-58-6fe4748f305468d364afb00004903691-Snipaste_2025-09-07_18-11-34-c70cc9.png)

## 二维码生成组件

下载地址: [ESP官方组件库](https://components.espressif.com/) 

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F09%2F07%2F18-17-56-fa44fd915d2faeecf6997e39c16ba877-Snipaste_2025-09-07_18-17-41-03072f.png)

复制: idf.py add-dependency "espressif/qrcode^0.1.0~2" 添加到 idf终端

```c
idf.py add-dependency "espressif/qrcode^0.1.0~2"
```

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F09%2F07%2F18-22-03-83c2a9fb3f5756768e6e85ba27d449ed-Snipaste_2025-09-07_18-21-50-c68b33.png)

## 下载esp专用的手机配网app

下载地址: 

Android:

https://github.com/espressif/esp-idf-provisioning-android/releases/download/Provisioning_App_Release_2.2.0/ESP_BLE_Prov_2_2_0.apk

IOS: 

https://apps.apple.com/in/app/esp-ble-provisioning/id1473590141

### 软件设置

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F09%2F07%2F18-31-43-a5035b392f2dc448a0706978df840aaa-Snipaste_2025-09-07_18-31-34-795078.png)