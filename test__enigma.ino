#include <hidboot.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <stdio.h>

char alphabet[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 

char rot2[26]    = "AJDKSIRUXBLHWTMCQGZNPYFVOE"; // 기어 2 (1번째 위치)
char inv_rot2[]= "AJPCZWRLFBDKOTYUQGENHXMIVS";               
char in_rot2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char b_in_rot2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char rot1[] =    "EKMFLGDQVZNTOWYHXUSPAIBRCJ"; // 기어 1 (2번째 위치 기어)
char in_rot1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char inv_rot1[] = "UWYGADFPVZBECKMTHXSLRINQOJ";
char b_in_rot1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char rot4[]    = "ESOVPZJAYQUIRHXLNFTGKDCMWB";       // 기어 4 (3번째 위치 기어)
char inv_rot4[] = "HZWVARTNLGUPXQCEJMBSKDYOIF";
char in_rot4[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char b_in_rot4[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char encrypted_msg;

// 반사체 시작. 
char reflector[13][2] = { {"AY"}, 
              {"BR"}, 
              {"CU"}, 
              {"DH"}, 
              {"EQ"}, 
              {"FS"}, 
              {"GL"}, 
              {"IP"},
              {"JX"}, 
              {"KN"}, 
              {"MO"}, 
              {"TZ"}, 
              {"VW"}
};


void init_gear(void) {
    strcpy(in_rot1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    strcpy(in_rot2, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    strcpy(in_rot4, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   
}
char reflect_B(char ch) {
  int i = 0;
  int j = 0;
  int back_f = 0;
  
  for(i = 0; i < 13; i++) {
    for(j = 0; j < 2; j++) {
      if(reflector[i][j] == ch) {
        back_f = (j == 1) ? reflector[i][0] : reflector[i][1];
      }
    }
  }
  
  //printf("reflected : %c\n", back_f);
  return back_f;  
}
// 반사체 끝. 

// inner_alp 밀어주는 함수 시작 
void shift(char * arr) {
  char temp = arr[0];
  
  memmove(arr - 1, arr, sizeof(char) * 26);
  arr[25] = temp;
}
// inner_alp 밀어주는 함수 끝 

// 문자 하나 찾기 
int find_ch(char * arr, char ch) {
  int i = 0;
  
  for(i = 0; i < strlen(arr); i++) {
    if(arr[i] == ch) {
      return i;
    }
  }
}
// 문자 하나 찾기 끝 
char enigma(char ch) {
  char f_ch;
  char m_ch;
  char end_ch;
  char l_ch;
 
    shift(in_rot2); // 한칸 밈.
  
  // 기어 2 시작 
    f_ch = in_rot2[ch - 'A']; 
  //printf("%c\n", f_ch);
  
    m_ch = rot2[f_ch - 'A'];
  //printf("%c\n", m_ch);
  
    end_ch = alphabet[find_ch(in_rot2, m_ch)]; 
  //printf("%c\n", end_ch);
  
  // 기어 2 종료 
  
  if(in_rot2[0] == 'F') {
    shift(in_rot1);
  }
  // 기어 1 시작
    f_ch = in_rot1[end_ch - 'A']; 
  //printf("%c\n", f_ch);
  
    m_ch = rot1[f_ch - 'A'];
  //printf("%c\n", m_ch);
  
    end_ch = alphabet[find_ch(in_rot1, m_ch)]; 
  //printf("%c\n", end_ch);
  // 기어 1 종료
  
  if(in_rot1[0] == 'R') {
    shift(in_rot4);
  }
  // 기어 4 시작
    f_ch = in_rot4[end_ch - 'A']; 
  //printf("%c\n", f_ch);
  
    m_ch = rot4[f_ch - 'A'];
  //printf("%c\n", m_ch);
  
    end_ch = alphabet[find_ch(in_rot4, m_ch)]; 
  //printf("%c\n", end_ch);
  // 기어 4 종료 
  
  // 반사체 시작 
    end_ch = reflect_B(end_ch);
  // 반사체 종료
  // 기어 4 (inverse) 시작 
    f_ch = in_rot4[end_ch - 'A']; 
  //printf("%c\n", f_ch);
  
    m_ch = inv_rot4[f_ch - 'A'];
  //printf("%c\n", m_ch);
  
    end_ch = alphabet[find_ch(in_rot4, m_ch)]; 
  //printf("%c\n", end_ch);
  // 기어 4 (inverse)  끝 
  
  // 기어 1 (inverse) 시작 
    f_ch = in_rot1[end_ch - 'A']; 
  //printf("%c\n", f_ch);
  
    m_ch = inv_rot1[f_ch - 'A'];
  //printf("%c\n", m_ch);
  
    end_ch = alphabet[find_ch(in_rot1, m_ch)]; 
  //printf("%c\n", end_ch);
  // 기어 1 (inverse)  끝 
  
  // 기어 2 (inverse) 시작 
    f_ch = in_rot2[end_ch - 'A']; 
  //printf("%c\n", f_ch);
  
    m_ch = inv_rot2[f_ch - 'A'];
  //printf("%c\n", m_ch);
  
    l_ch = alphabet[find_ch(in_rot2, m_ch)]; 
  //printf("%c\n", l_ch);
  Serial.print("Encrpyed : ");
  Serial.println(l_ch);
  return l_ch;
  // 기어 2 (inverse)  끝 
}

byte arr[] = {0, 1, 2, 3, 4, 5, 6, 7};
byte leds = 0;

int latchPin = 8;      //Pin connected to ST_CP of 74HC595
int clockPin = 7;   //Pin connected to SH_CP of 74HC595
int dataPin = 6;    //Pin connected to DS of 74HC595

int N_Key_Pin = 4;
int M_Key_Pin = 5;

int isUpper(char ch) {
  if('A' <= ch && 'Z' >= ch) return 1;

  else return 0;
}
void updateShiftRegister() {
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

void n_updateShiftRegister() {
   digitalWrite(latchPin, LOW); // 고정
   shiftOut(dataPin, clockPin, MSBFIRST, leds);
   shiftOut(dataPin, clockPin, MSBFIRST, 0); // 고정
   digitalWrite(latchPin, HIGH); // 고정
}

void nn_updateShiftRegister() {
   digitalWrite(latchPin, LOW); // 고정
   shiftOut(dataPin, clockPin, MSBFIRST, leds);
   shiftOut(dataPin, clockPin, MSBFIRST, 0); // 고정
   shiftOut(dataPin, clockPin, MSBFIRST, 0); // 고정
   digitalWrite(latchPin, HIGH); // 고정
}

void nnn_updateShiftRegister() {
   digitalWrite(latchPin, LOW); // 고정
   shiftOut(dataPin, clockPin, MSBFIRST, leds);
   shiftOut(dataPin, clockPin, MSBFIRST, 0); // 고정
   shiftOut(dataPin, clockPin, MSBFIRST, 0); // 고정
   shiftOut(dataPin, clockPin, MSBFIRST, 0); // 고정
   digitalWrite(latchPin, HIGH); // 고정
}

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");

  Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  Serial.print("DN ");
  PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    Serial.println("LeftCtrl changed");
    init_gear();
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    Serial.println("LeftShift changed");
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    Serial.println("RightGUI changed");
  }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  Serial.print("UP ");
  PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  Serial.print("ASCII: ");
  Serial.println((char)key);
  char input_val = (char)key;

  if(isUpper(input_val)) {
    encrypted_msg = enigma(input_val);
    Serial.print("encrypted_msg : ");
    Serial.println(encrypted_msg);
    
    switch(encrypted_msg) {
  
    case 'Q':
        bitSet(leds, 0);
          digitalWrite(latchPin, LOW);
          updateShiftRegister();
          delay(1500);
          break;
      
      case 'W':
          bitSet(leds, 1);
          digitalWrite(latchPin, LOW);
          updateShiftRegister();
          delay(1500);
          break;
    
   
      case 'E':
          bitSet(leds, 2);
          digitalWrite(latchPin, LOW);
          updateShiftRegister();
          delay(1500);
          break;
    
    //Serial.println("R");
      case 'R':
          bitSet(leds, 3);
          digitalWrite(latchPin, LOW);
          updateShiftRegister();
          delay(1500);
          break;
      
     // Serial.println("T");
  case 'T':
        bitSet(leds, 4);
        updateShiftRegister();
        delay(1500);
        break;
      
     // Serial.println("");
    case 'Y':
      bitSet(leds, 5);
        digitalWrite(latchPin, LOW);
        updateShiftRegister();
        delay(1500); break;
      
    
  case 'U':
        bitSet(leds, 6);
        digitalWrite(latchPin, LOW);
        updateShiftRegister();
        delay(1500);
        break;
      
    
  case 'I':
        bitSet(leds, 7);
        digitalWrite(latchPin, LOW);
        updateShiftRegister();
        delay(1500); 
        break;
      
  
   case 'O':
          bitSet(leds, 0);
          digitalWrite(latchPin, LOW);
          n_updateShiftRegister();
          delay(1500); 
          break;
      

    case 'P':
          bitSet(leds, 1);
          digitalWrite(latchPin, LOW);
          n_updateShiftRegister();
          delay(1500); 
          break;
      

    case 'A':
          bitSet(leds, 2);
          digitalWrite(latchPin, LOW);
          n_updateShiftRegister();
          delay(1500); break;
      

    case 'S':
          bitSet(leds, 3);
          digitalWrite(latchPin, LOW);
          n_updateShiftRegister();
          delay(1500); break;
      

    case 'D':
          bitSet(leds, 4);
          digitalWrite(latchPin, LOW);
          n_updateShiftRegister();
          delay(1500); break;
      

    case 'F':
          bitSet(leds, 5);
          digitalWrite(latchPin, LOW);
          n_updateShiftRegister();
          delay(1500); break;
      

      case 'G':
        bitSet(leds, 6);
        digitalWrite(latchPin, LOW);
        n_updateShiftRegister();
        delay(1500); break;
      

      case 'H':
        bitSet(leds, 7);
        digitalWrite(latchPin, LOW);
        n_updateShiftRegister();
        delay(1500); break;
      

      case 'J':
        bitSet(leds, 0);
        digitalWrite(latchPin, LOW);
        nn_updateShiftRegister();
        delay(1500); break;
      

      case 'K':
        bitSet(leds, 1);
        digitalWrite(latchPin, LOW);
        nn_updateShiftRegister();
        delay(1500); break;
      

      case 'L':
        bitSet(leds, 2);
        digitalWrite(latchPin, LOW);
        nn_updateShiftRegister();
        delay(1500); break;
      

      case 'Z':
        bitSet(leds, 3);
        digitalWrite(latchPin, LOW);
        nn_updateShiftRegister();
        delay(1500); break;
      

      case 'X':
        bitSet(leds, 4);
        digitalWrite(latchPin, LOW);
        nn_updateShiftRegister();
        delay(1500); break;
    

      case 'C':
        bitSet(leds, 5);
        digitalWrite(latchPin, LOW);
        nn_updateShiftRegister();
        delay(1500); 
        break;
      

      case 'V':
        bitSet(leds, 6);
        digitalWrite(latchPin, LOW);
        nn_updateShiftRegister();
        delay(1500); break;
      

  
      case 'B':
        bitSet(leds, 7);
        digitalWrite(latchPin, LOW);
        nn_updateShiftRegister();
        delay(1500); 
        break;
      

      case 'N':
        digitalWrite(N_Key_Pin, HIGH);
        delay(1500); 
        digitalWrite(N_Key_Pin, LOW);
        break;
      

      case 'M':
        digitalWrite(M_Key_Pin, HIGH);
        delay(1500); 
        digitalWrite(M_Key_Pin, LOW);
        break;
      
  }
  }
};

USB     Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(N_Key_Pin, OUTPUT);
  pinMode(M_Key_Pin, OUTPUT);
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  HidKeyboard.SetReportParser(0, &Prs);
}

void loop() {
  leds = 0;
  updateShiftRegister();
  Usb.Task();  
}
