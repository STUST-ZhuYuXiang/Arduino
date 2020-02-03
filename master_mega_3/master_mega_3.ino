/*
檯燈裝置
負責接收 Client 端的資料
配有藍芽裝置，接收訊息
配有檯燈、蜂鳴器、開關

腳位：
檯燈-5(紅-黃, 黑-綠); 蜂鳴器-6; 開關-7
藍芽2-RX217, TX216; 藍芽3-RX118, TX119

藍芽傳輸規則：
"0"無煙無火; "1"有煙無火; "2"無煙有火; "3"有煙有火
*/
//================================================================
// 接收藍芽訊息
char val_1;
char val_2;

// 元件腳位
int light = 5;
int buzzer = 6;
int sw = 7;

// 檯燈控制
int light_on = 0;
boolean buttonUp = true;

// 發生火災
boolean FireDisaster = false;

//-----------------------------------------------------------------
void setup() {
  FireDisaster = false;
  pinMode(light, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sw, INPUT);
  digitalWrite(light, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  
  Serial.begin(38400);

  //定義藍芽傳送、接收端口
  Serial1.begin(38400);  //編號3，連線至編號1
  Serial2.begin(38400);  //編號2，連線至編號4

  Serial.print("BT1 & BT2 is ready.\n");
  Serial.print("BT_1：\t\tBT_2：\n");
}

//-----------------------------------------------------------------
void loop() {
  mControlLight(sw, light);
  
  if (Serial1.available()) {
    val_1 = Serial1.read();
    Serial.print(val_1);
    mWarnStatus(val_1);
  }

  Serial.print("\t\t");
  
  if (Serial2.available()) {
    val_2 = Serial2.read();
    Serial.print(val_2);
    mWarnStatus(val_2);
  }

  if (FireDisaster) {
    mAlarm(light);
    mBeep(buzzer);
  }
  
  //delay(1000);
  Serial.println();
}

//================================================================
//數值判斷---------------------------------------------------------
//藍芽傳輸規則："0"無煙無火; "1"有煙無火; "2"無煙有火; "3"有煙有火
void mWarnStatus(char val) {
  switch(val) {
    case '1':
      Serial.println("偵測到煙霧");
      break;
    case '2':
      Serial.println("附近溫度過高");
      FireDisaster = true;
      break;
    case '3':
      Serial.println("發生火災!!");
      FireDisaster = true;
      break;
    default:
      Serial.println("狀況安全");
      FireDisaster = false;
      break;
  }
}

//檯燈控制---------------------------------------------------------
void mControlLight(int sw, int light) {
  if (digitalRead(sw) == HIGH && buttonUp == true) {
    light_on = !light_on;
    buttonUp = false;
  }
  else if (digitalRead(sw) == LOW && buttonUp == false) {
    buttonUp = true;
  }
  digitalWrite(light, light_on);
}

//火災警報---------------------------------------------------------
void mAlarm(int light) {
  digitalWrite(light, HIGH);
  delay(10);
  digitalWrite(light, LOW);
  delay(10);
}

//蜂鳴器響---------------------------------------------------------
void  mBeep(int buzzer) {
  tone(buzzer, 1000, 1000);
  delay(1000);
}
