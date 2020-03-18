# Arduino 火災警示裝置

## 一、概念
共有2個區域能偵測溫度
若溫度感測器感測到的溫度>30
則會傳送1的訊號到"主"端
主端收到1的訊號，會讓燈進行閃爍
若是收到0的訊號，則能使用開關控制燈

## 二、藍芽設定(HC-05)
### 1. 教學
將以下程式燒錄進Arduino上，方便將Arduino資訊呈現在電腦上，需額外注意藍芽模組的型號與電腦連接埠的速率。
```c=
#include <SoftwareSerial.h>    // 引用程式庫
 
// 定義連接藍牙模組的序列埠
SoftwareSerial BT(8, 9); // 接收腳, 傳送腳
char val;  // 儲存接收資料的變數
 
void setup() {
  Serial.begin(9600);    // 與電腦序列埠連線
  Serial.println("BT is ready!");
 
  // 設定藍牙模組的連線速率
  // 如果是HC-05，改成38400
  // 如果是HC-10，改成9600
  BT.begin(38400);
}
 
void loop() {
  // 若收到「序列埠監控視窗」的資料，則送到藍牙模組
  if (Serial.available()) {
    val = Serial.read();
    BT.print(val);
  }
 
  // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
  if (BT.available()) {
    val = BT.read();
    Serial.print(val);
  }
}
```

進入藍芽模組AT模式，步驟如下
1. 先拔掉 Arduino 的 USB 線，也拔掉 HC-05 VCC 和 Arduino 5V 相連的這條線。
2. 將 Arduino 的 USB 線插入電腦。
3. 先按下HC-05板上的按鈕不放.然行將HC-05 VCC 和 Arduino 5V 相連,約1秒左右放掉HC-05板上的按鈕,此時HC-05板上的燈約2秒鐘閃一次.就代表進入TA COMMAND模式了。

當成功進入AT模式後，打開「序列埠監控視窗」。
在此之前先介紹藍芽模組的AT模式有提供這些指令，可設定模組。
```c=
AT          //連線成功，輸入此指令，模組會回應"ok"
AT+ROLE?    //查詢此模組的角色狀態為主還是從
AT+ADDR?    //查詢此模組的藍芽模組位址
AT+BIND=    //此模組預連線的其他藍芽模組位址
```
### 2. 實作
#### 主(Master, 1)
- 裝置：MEGA_2560
    - PORT = COM8
- 編號3
    - BT_1 (連接：BT_N01)
    - UART = 38400,0,0
    - ADDR = 18,e4,34cf80
- 編號2
    - BT_2 (連接：BT_N02)
    - UART = 9600,0,0
    - ADDR = 98d3,33,813f84

#### 從(Client, 0)
- 裝置 : BT_N02
    - PORT = COM 7
- 藍芽編號4
    - UART = 38400,0,0
    - ADDR = 98d3,71,fd73f3

#### 從(Client, 0)
- 裝置 : BT_N01
    - PORT = COM 3
- 藍芽編號1
    - UART = 9600,0,0
    - ADDR = 98d3,32,71379d

### 3. 參考文獻
1. HC-05與HC-06藍牙模組補充說明（三）：使用Arduino設定AT命令
https://swf.com.tw/?p=712
2. 藍牙模組補充說明（四）：無線連結兩個Arduino控制板
https://swf.com.tw/?p=750

## 三、溫度感測器(DS18B20)
### 1. 教學
需要先安裝兩個程式庫OneWire與DallasTemperature，並將資料夾放入Arduino安裝路徑D:\Arduino\libraries下。
```c=
//匯入程式庫之套件
#include <OneWire.h>
#include <DallasTemperature.h>
```

### 2. 參考文獻
1. Arduino練習:溫度感測器DS18B20
http://yehnan.blogspot.com/2013/01/arduinods18b20.html

## 四、電路圖
### 檯燈裝置
模組：Arduino Mega、Bluetooth HC-05、開關、USB LED燈、蜂鳴器
![](https://i.imgur.com/bpa6tjW.png)

### 感測裝置
模組1：Arduino UNO R3、Bluetooth HC-05、DS18B20(溫度感測器)、MQ-2(甲烷、丁烷、液化石油氣、煙 )
模組2：Arduino UNO R3、Bluetooth HC-05、DS18B20(溫度感測器)、MQ-7(一氧化碳)
![](https://i.imgur.com/5QlNk4E.png)

###### tags: `專題`
