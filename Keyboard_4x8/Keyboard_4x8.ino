#include <Keyboard.h>
#include "FastLED.h"
#define LED_PIN     2
#define NUM_LEDS    32

#define col1 3
#define col2 4
#define col3 5
#define col4 6
#define line1 7
#define line2 8
#define line3 9
#define line4 10
#define line5 16
#define line6 14
#define line7 15
#define line8 1

int midi_note[10][12];

CRGB leds[32];
byte arrow;

CRGB led_color  = CRGB(133,0,133);
CRGB led_color2  = CRGB(133,133,0);
CRGB led_white = CRGB(0,0,0);

//按键初始编码：
// |11-|21|31|41-|51|61|71|81|
// |12 |22|32|42 |52|62|72|82|
// |13-|23|33|43-|53|63|73|83|
// |14 |24|34|44 |54|64|74|84|

//钢琴编码：
// |xx|22|42|xx|72|92 |112|xx|
// |12|32|52|62|82|102|122|xx|
// |xx|21|41|xx|71|91 |111|xx|
// |11|31|51|61|81|101|121|xx|

//定义按键按放命令
#define key11_release key_release(0,0);
#define key11_press key_press(0,0);
#define key12_press key_press(12,7);
#define key12_release key_release(12,7);
#define key13_press key_press(0,8);
#define key13_release key_release(0,8);
#define key14_press key_press(11,15);
#define key14_release key_release(11,15);
//2
#define key21_release key_release(22,1);
#define key21_press key_press(22,1);
#define key22_press key_press(32,6);
#define key22_release key_release(32,6);
#define key23_press key_press(21,9);
#define key23_release key_release(21,9);
#define key24_press key_press(31,14);
#define key24_release key_release(31,14);
//3
#define key31_release key_release(42,2);
#define key31_press key_press(42,2);
#define key32_press key_press(52,5);
#define key32_release key_release(52,5);
#define key33_press key_press(41,10);
#define key33_release key_release(41,10);
#define key34_press key_press(51,13);
#define key34_release key_release(51,13);
//4
#define key41_release key_release(0,3);
#define key41_press key_press(0,3);
#define key42_press key_press(62,4);
#define key42_release key_release(62,4);
#define key43_press key_press(0,11);
#define key43_release key_release(0,11);
#define key44_press key_press(61,12);
#define key44_release key_release(61,12);
//5
#define key51_release key_release(72,16);
#define key51_press key_press(72,16);
#define key52_press key_press(82,23);
#define key52_release key_release(82,23);
#define key53_press key_press(71,24);
#define key53_release key_release(71,24);
#define key54_press key_press(81,31);
#define key54_release key_release(81,31);
//6
#define key61_release key_release(92,17);
#define key61_press key_press(92,17);
#define key62_press key_press(102,22);
#define key62_release key_release(102,22);
#define key63_press key_press(91,25);
#define key63_release key_release(91,25);
#define key64_press key_press(101,30);
#define key64_release key_release(101,30);
//7
#define key71_release key_release(112,18);
#define key71_press key_press(112,18);
#define key72_press key_press(122,21);
#define key72_release key_release(122,21);
#define key73_press key_press(111,26);
#define key73_release key_release(111,26);
#define key74_press key_press(121,29);
#define key74_release key_release(121,29);
//8
#define key81_release key_release(0,19);
#define key81_press key_press(0,19);
#define key82_press key_press(0,20);
#define key82_release key_release(0,20);
#define key83_press key_press(0,27);
#define key83_release key_release(0,27);
#define key84_press key_press(0,28);
#define key84_release key_release(0,28);

//定义记录变量
byte key11;
byte key12;
byte key13;
byte key14;
byte key21;
byte key22;
byte key23;
byte key24;
byte key31;
byte key32;
byte key33;
byte key34;
byte key41;
byte key42;
byte key43;
byte key44;
byte key51;
byte key52;
byte key53;
byte key54;
byte key61;
byte key62;
byte key63;
byte key64;
byte key71;
byte key72;
byte key73;
byte key74;
byte key81;
byte key82;
byte key83;
byte key84;

// 状态变量 0为显示 1为只弹 2为接收灯光弹
byte note_level = 2;
byte note_vol = 100;
// 常用MIDI命令
int noteON = 144;
int noteOFF = 128;

// 串口灯光控制暂存
int incomingByte = 0;

void setup() {
  int i,j;
  int note = 0;
  for(i = 0;i < 10; i++){
    for(j = 0;j < 12; j++){
      midi_note[i][j] = note;
      note ++;
    }
  }
  Serial.begin(38400);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  //初始化LED
  //扫描输入IO设置
  pinMode(col1, INPUT_PULLUP);
  pinMode(col2, INPUT_PULLUP);
  pinMode(col3, INPUT_PULLUP);
  pinMode(col4, INPUT_PULLUP);
  //扫描输出IO设置
  pinMode(line1, OUTPUT);
  pinMode(line2,OUTPUT);
  pinMode(line3, OUTPUT);
  pinMode(line4,OUTPUT);
  pinMode(line5,OUTPUT);
  pinMode(line6,OUTPUT);
  pinMode(line7,OUTPUT);
  pinMode(line8,OUTPUT);
  Serial.begin(9600);
    // 计时器设置
  noInterrupts();           // 停止所有中断
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;               // 清空寄存器
  OCR1A = 2000;                // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC 模式
  TCCR1B |= (1 << CS12);    // 256 预分频器
  TIMSK1 |= (1 << OCIE1A);  // 启动时钟对比中断
  interrupts();             // 启用中断
  
}
ISR(TIMER1_COMPA_vect)          // Timer1中断处理
{
  TCCR1B &= 0b11111011;         // 停止时钟
  key_scan();
  TCCR1B |= (1 << CS12);        // 启动时钟
}
void loop() {
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 144 || incomingByte == 128){
        unsigned char sta = incomingByte;
        incomingByte = Serial.read();
        incomingByte = incomingByte % 12;
        int led_num;
        switch(incomingByte){
          case 0:
            led_num = 7;
            break;
          case 1:
            led_num = 1;
            break;
          case 2:
            led_num = 6;
            break;
          case 3:
            led_num = 2;
            break;
          case 4:
            led_num = 5;
            break;
          case 5:
            led_num = 4;
            break;
          case 6:
            led_num = 16;
            break;
          case 7:
            led_num = 23;
            break;
          case 8:
            led_num = 17;
            break;
          case 9:
            led_num = 22;
            break;
          case 10:
            led_num = 18;
            break;
          case 11:
            led_num = 21;
            break;
        }
          if(sta == 144){
            leds[led_num] = led_color;
            FastLED.show();
          }
          else{
            leds[led_num] = led_white;
            FastLED.show();
          }
    }
  }
}

// midi输送
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);
  Serial.write(MIDInote);
  Serial.write(MIDIvelocity);
}
// 按键处理函数
void key_press(char id,char led_id){
  leds[led_id] = led_color;
  FastLED.show();
  unsigned char row,col;
  int note;
  col = id / 10;
  row = (id % 10) - 1; // 获取按键信息
  note = midi_note[row + note_level][col - 1];
  MIDImessage(noteON, note, note_vol);
}

void key_release(char id,char led_id){
  leds[led_id] = led_white;
  FastLED.show();
  unsigned char row,col;
  int note;
  col = id / 10;
  row = (id % 10) - 1;
  note = midi_note[row + note_level][col - 1];
  MIDImessage(noteOFF, note, note_vol);
}

// 按键扫描函数
void key_scan(){
  ///////////////////////////////// 第一列扫描
  digitalWrite(line1,LOW);
  //第一行第一列扫描
  if(digitalRead(col1)){
    if(key11)
      {
      key11 = 0;
      key11_release
      }
  }
   else
   {
    if(!key11){
      key11 = 1;
      key11_press
    }
   }
   //第二行第一列扫描
  if(digitalRead(col2)){
    if(key12)
      {
      key12 = 0;
      key12_release
      }
  }
   else
   {
    if(!key12){
      key12 = 1;
      key12_press
    }
   }
      //第三行第一列扫描
  if(digitalRead(col3)){
    if(key13)
      {
      key13 = 0;
      key13_release
      }
  }
   else
   {
    if(!key13){
      key13 = 1;
      key13_press
    }
   }
      //第四行第一列扫描
  if(digitalRead(col4)){
    if(key14)
      {
      key14 = 0;
      key14_release
      }
  }
   else
   {
    if(!key14){
      key14 = 1;
      key14_press
    }
   }
  digitalWrite(line1,HIGH);
  ///////////////////////////////// 第二列扫描
  digitalWrite(line2,LOW);
  //第一行第二列扫描
  if(digitalRead(col1)){
    if(key21)
      {
      key21 = 0;
      key21_release
      }
  }
   else
   {
    if(!key21){
      key21 = 1;
      key21_press
    }
   }
   //第二行第二列扫描
  if(digitalRead(col2)){
    if(key22)
      {
      key22 = 0;
      key22_release
      }
  }
   else
   {
    if(!key22){
      key22 = 1;
      key22_press
    }
   }
      //第三行第二列扫描
  if(digitalRead(col3)){
    if(key23)
      {
      key23 = 0;
      key23_release
      }
  }
   else
   {
    if(!key23){
      key23 = 1;
      key23_press
    }
   }
      //第四行第二列扫描
  if(digitalRead(col4)){
    if(key24)
      {
      key24 = 0;
      key24_release
      }
  }
   else
   {
    if(!key24){
      key24 = 1;
      key24_press
    }
   }
  digitalWrite(line2,HIGH);
///////////////////////////////// 第三列扫描
  digitalWrite(line3,LOW);
  //第一行第三列扫描
  if(digitalRead(col1)){
    if(key31)
      {
      key31 = 0;
      key31_release
      }
  }
   else
   {
    if(!key31){
      key31 = 1;
      key31_press
    }
   }
   //第二行第三列扫描
  if(digitalRead(col2)){
    if(key32)
      {
      key32 = 0;
      key32_release
      }
  }
   else
   {
    if(!key32){
      key32 = 1;
      key32_press
    }
   }
      //第三行第三列扫描
  if(digitalRead(col3)){
    if(key33)
      {
      key33 = 0;
      key33_release
      }
  }
   else
   {
    if(!key33){
      key33 = 1;
      key33_press
    }
   }
      //第四行第三列扫描
  if(digitalRead(col4)){
    if(key34)
      {
      key34 = 0;
      key34_release
      }
  }
   else
   {
    if(!key34){
      key34 = 1;
      key34_press
    }
   }
  digitalWrite(line3,HIGH);
///////////////////////////////// 第四列扫描
  digitalWrite(line4,LOW);
  //第一行第四列扫描
  if(digitalRead(col1)){
    if(key41)
      {
      key41 = 0;
      key41_release
      }
  }
   else
   {
    if(!key41){
      key41 = 1;
      key41_press
    }
   }
   //第二行第四列扫描
  if(digitalRead(col2)){
    if(key42)
      {
      key42 = 0;
      key42_release
      }
  }
   else
   {
    if(!key42){
      key42 = 1;
      key42_press
    }
   }
      //第三行第四列扫描
  if(digitalRead(col3)){
    if(key43)
      {
      key43 = 0;
      key43_release
      }
  }
   else
   {
    if(!key43){
      key43 = 1;
      key43_press
    }
   }
      //第四行第四列扫描
  if(digitalRead(col4)){
    if(key44)
      {
      key44 = 0;
      key44_release
      }
  }
   else
   {
    if(!key44){
      key44 = 1;
      key44_press
    }
   }
  digitalWrite(line4,HIGH);
///////////////////////////////// 第五列扫描
  digitalWrite(line5,LOW);
  //第一行第五列扫描
  if(digitalRead(col1)){
    if(key51)
      {
      key51 = 0;
      key51_release
      }
  }
   else
   {
    if(!key51){
      key51 = 1;
      key51_press
    }
   }
   //第二行第五列扫描
  if(digitalRead(col2)){
    if(key52)
      {
      key52 = 0;
      key52_release
      }
  }
   else
   {
    if(!key52){
      key52 = 1;
      key52_press
    }
   }
      //第三行第五列扫描
  if(digitalRead(col3)){
    if(key53)
      {
      key53 = 0;
      key53_release
      }
  }
   else
   {
    if(!key53){
      key53 = 1;
      key53_press
    }
   }
      //第四行第五列扫描
  if(digitalRead(col4)){
    if(key54)
      {
      key54 = 0;
      key54_release
      }
  }
   else
   {
    if(!key54){
      key54 = 1;
      key54_press
    }
   }
  digitalWrite(line5,HIGH);
///////////////////////////////// 第六列扫描
  digitalWrite(line6,LOW);
  //第一行第六列扫描
  if(digitalRead(col1)){
    if(key61)
      {
      key61 = 0;
      key61_release
      }
  }
   else
   {
    if(!key61){
      key61 = 1;
      key61_press
    }
   }
   //第二行第六列扫描
  if(digitalRead(col2)){
    if(key62)
      {
      key62 = 0;
      key62_release
      }
  }
   else
   {
    if(!key62){
      key62 = 1;
      key62_press
    }
   }
      //第三行第六列扫描
  if(digitalRead(col3)){
    if(key63)
      {
      key63 = 0;
      key63_release
      }
  }
   else
   {
    if(!key63){
      key63 = 1;
      key63_press
    }
   }
      //第四行第六列扫描
  if(digitalRead(col4)){
    if(key64)
      {
      key64 = 0;
      key64_release
      }
  }
   else
   {
    if(!key64){
      key64 = 1;
      key64_press
    }
   }
  digitalWrite(line6,HIGH);
///////////////////////////////// 第七列扫描
  digitalWrite(line7,LOW);
  //第一行第七列扫描
  if(digitalRead(col1)){
    if(key71)
      {
      key71 = 0;
      key71_release
      }
  }
   else
   {
    if(!key71){
      key71 = 1;
      key71_press
    }
   }
   //第二行第七列扫描
  if(digitalRead(col2)){
    if(key72)
      {
      key72 = 0;
      key72_release
      }
  }
   else
   {
    if(!key72){
      key72 = 1;
      key72_press
    }
   }
      //第三行第七列扫描
  if(digitalRead(col3)){
    if(key73)
      {
      key73 = 0;
      key73_release
      }
  }
   else
   {
    if(!key73){
      key73 = 1;
      key73_press
    }
   }
      //第四行第七列扫描
  if(digitalRead(col4)){
    if(key74)
      {
      key74 = 0;
      key74_release
      }
  }
   else
   {
    if(!key74){
      key74 = 1;
      key74_press
    }
   }
  digitalWrite(line7,HIGH);
  ///////////////////////////////// 第八列扫描
  digitalWrite(line8,LOW);
  //第一行第八列扫描
  if(digitalRead(col1)){
    if(key81)
      {
      key81 = 0;
      key81_release
      }
  }
   else
   {
    if(!key81){
      key81 = 1;
      key81_press
    }
   }
   //第二行第八列扫描
  if(digitalRead(col2)){
    if(key82)
      {
      key82 = 0;
      key82_release
      }
  }
   else
   {
    if(!key82){
      key82 = 1;
      key82_press
    }
   }
      //第三行第八列扫描
  if(digitalRead(col3)){
    if(key83)
      {
      key83 = 0;
      key83_release
      }
  }
   else
   {
    if(!key83){
      key83 = 1;
      key83_press
    }
   }
      //第四行第八列扫描
  if(digitalRead(col4)){
    if(key84)
      {
      key84 = 0;
      key84_release
      }
  }
   else
   {
    if(!key84){
      key84 = 1;
      key84_press
    }
   }
  digitalWrite(line8,HIGH);
}
