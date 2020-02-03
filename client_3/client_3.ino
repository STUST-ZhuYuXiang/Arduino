/*
感測器裝置
負責傳送資料給 Master 端
配有藍芽裝置，傳送訊息
配有溫度感測器，判斷溫度變化
配有氣體感測器，判斷氣體數值

腳位：
溫度-2; 氣體-3; 氣體-A0

藍芽傳輸規則：
"0"無煙無火; "1"有煙無火; "2"無煙有火; "3"有煙有火
*/
//================================================================
//引用藍芽程式庫
#include <SoftwareSerial.h> 

//引用溫度感測器程式庫
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2      // 接收溫度感測

//引用氣體感測器程式庫
#include <Wire.h>
int gasValue = 0;           //定義氣體感測變數

//建立溫度感測物件
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//定義連接藍牙模組的序列埠 
SoftwareSerial BT(8, 9);      // 接收腳, 傳送腳
char val;                   // 儲存接收資料的變數

//定義調整數值，方便依照會場狀況更改
int FIRE_ADJUST = 27;       // 火災通常有60度
int GAS_ADJUST = 300;       // 數值300，代表有煙

//定義火災與煙霧狀況
boolean fire = false;
boolean gas = false;

//-----------------------------------------------------------------
void setup() {
  fire = false;
  gas = false;
  pinMode(3, INPUT);

  //與電腦序列埠連線
  Serial.begin(9600);
  Serial.println("BT is ready!");
  Serial.println("溫度\t\t\t氣體");
 
  // 藍牙透傳模式的預設連線速率。
  BT.begin(9600);
}

//-----------------------------------------------------------------
void loop() {
  // 要求匯流排上的所有感測器進行溫度轉換
  sensors.requestTemperatures();
  // 取得溫度讀數（攝氏）並輸出
  Serial.print(sensors.getTempCByIndex(0));
  // 對溫度進行判斷，如果 > (溫度調整數值)，表示發生火災
  if (sensors.getTempCByIndex(0) > FIRE_ADJUST) {
    fire = true;
    Serial.print(", 發生火災");
  }
  else {
    fire = false;
  }
  
  Serial.print("\t\t\t");

  // 氣體數值讀取數位接腳0，並輸出
  gasValue = analogRead(0);
  Serial.print(gasValue, DEC);
  // 氣體偵測判斷
  if (gasValue > GAS_ADJUST) {
    Serial.print(", 偵測到!");
    gas = true;
  }
  else {
    Serial.print(", 安全");
    gas = false;
  }

  Serial.println();
  
  //主要傳送程式碼
  //判斷煙霧與溫度狀況，並傳送
  //"0"無煙無火; "1"有煙無火; "2"無煙有火; "3"有煙有火
  if (gas) {
    if (fire) BT.print('3');
    else BT.print('1');
  }
  else {
    if (fire) BT.print('2');
    else BT.print('0');
  }
  
  delay(1000);
}
