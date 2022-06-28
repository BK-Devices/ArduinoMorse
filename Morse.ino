// Morse code Interpreter & Trainer
// ATMEGA328p, 16x2 lcd display

#include <Arduino.h>
#include <LiquidCrystal.h>

#define Sw1 8
#define Sw2 5
#define Yled 2
#define Buzzer 7

LiquidCrystal lcd(13, 12, 11, 10, 9, 6); //(rs, enable, d4, d5, d6, d7)

const char *Keys[] =
{ 
  // Characters, Numbers & Symbols
  "A", "B", "C", "D", "E", "F",
  "G", "H", "I", "J", "K", "L",
  "M", "N", "O", "P", "Q", "R",
  "S", "T", "U", "V", "W", "X",
  "Y", "Z", "1", "2", "3", "4",
  "5", "6", "7", "8", "9", "0",
  ".", ",", "?", "!", "'", "(",
  ")", "&", ":", ";", "/", "_",
  "=", "+", "-", "$", "@", " "
};

const char *Values[] =
{
  // Morse Codes of Characters, Numbers & Symbols
  ".-", "-...",  "-.-.", "-..",
  ".", "..-.", "--.", "....",
  "..", ".---", "-.-", ".-..",
  "--", "-.", "---", ".--.",
  "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-",
  "-.--", "--..", ".----",
  "..---", "...--", "....-",
  ".....", "-....", "--...",
  "---..", "----.", "-----",
  ".-.-.-", "--..--", "..--..",
  "-.-.--", ".----.","-.--.",
  "-.--.-", ".-...", "---...",
  "-.-.-.", "-..-.", "..--.-",
  "-...-", ".-.-.", "-....-",
  "...-..-", ".--.-.", "|"
};

byte Right[] = 
{
  // Right symbol
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};

byte Block[] = 
{
  // Block symbol
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

const int U_Delay = 75;
const int B_Delay = 250;
int Bt1, Bt2;
int Mode = 0;
int Marks = 0;
int Test = 0;
int ch = 0;
int cursor = 0;
int Index = -1;
String Key, Value, Word, Word1;
boolean Pressed1 = false;
boolean Pressed2 = false;
boolean Not_Pressed = true;
boolean Sh_Press = false;
boolean Result = false;
unsigned long Pr_Str1, Rl_Str1, Pr_Str2;
unsigned long Pr_End1, Rl_End1, Pr_End2;
unsigned long Release, Press1, Press2;

void Decode_Key()
{
  // Convert morse code to characters
  int Length = sizeof(Values)/ sizeof(*Values);
  for (int i = 0; i < Length; i++)
  {
    if (String (Values[i]) == Value)
    {
      Index = i;
      break;
    }
    else Index = -1;
  }
  if (Index < 0) Key = "?";
  else Key = Keys[Index];
}

void Decode_Value()
{
  // Converts Characters to Morse code
  int Length = sizeof(Keys)/ sizeof(*Keys);
  for (int i = 0; i < Length; i++)
  {
    if (String (Keys[i]) == Key)
    {
      Index = i;
      break;
    }
    else Index = -1;
  }
  if (Index < 0) Value = "?";
  else Value = Values[Index];
}

void Trainer_Press()
{
  Release = 0;
  digitalWrite(Buzzer, HIGH);
  digitalWrite(Yled, HIGH);
  if (Not_Pressed == false)
  {
    Rl_End1 = millis();
    Release = round(Rl_End1 - Rl_Str1);
  }
  if (Pressed1 == false)
  {
    Pr_Str1 = millis();
    if (Release >= 3 * U_Delay)
    {
      Decode_Key();
      lcd.setCursor(cursor,1);
      lcd.print(Key);
      Word += Key;
      if (Release >= 18 * U_Delay)
      {
        cursor++;
        lcd.setCursor(cursor,1);
        lcd.print(" ");
        Word += " ";
      }
      Value = "";
      cursor++;
      if(cursor >= 16)
      {
        Pressed1 = false;
        Not_Pressed = true;
        Word1 = Word;
        Word = "";
        Value = "";
        lcd.clear();
        cursor = 0;
      }
    }
    Pressed1 = true;
  }
}

void Trainer_Release()
{
  digitalWrite(Buzzer, LOW);
  digitalWrite(Yled, LOW);
  if (Pressed1 == true)
  {
    Pr_End1 = millis();
    Rl_Str1 = millis();
    Press1 = round(Pr_End1 - Pr_Str1);
    if (Press1 >= U_Delay)
    {
      if (Press1 >= 3 * U_Delay)
      {
        Value += "-";
        lcd.setCursor(cursor,1);
        lcd.print("-");
      }
      else
      {
        Value += ".";
        lcd.setCursor(cursor,1);
        lcd.print(".");
      }
    }
    Pressed1 = false;
    Not_Pressed = false;
  }
}

void Tester_Press()
{
  Release = 0;
  digitalWrite(Buzzer, HIGH);
  digitalWrite(Yled, HIGH);
  if (Not_Pressed == false)
  {
    Rl_End1 = millis();
    Release = round(Rl_End1 - Rl_Str1);
  }
  if (Pressed1 == false)
  {
    Pr_Str1 = millis();
    if (Release >= 3 * U_Delay)
    {
      int Length = sizeof(Values)/ sizeof(*Values);
      for (int i = 0; i < Length; i++)
      {
        if (String (Values[i]) == Value)
        {
          Index = i;
          break;
        }
        else Index = -1;
      }
      if (Index < 0)
      {
        lcd.setCursor(cursor,1);
        lcd.write(byte(0));
      }
      else
      {
        Key = Keys[Index];
        if (Key == Word1)
        {
          lcd.setCursor(cursor,1);
          lcd.write(byte(1));
          Marks++;
        }
        else
        {
          lcd.setCursor(cursor,1);
          lcd.write(byte(0));
        }
      }
      ch++;
      cursor++;
      Value = "";
      if (cursor >= 16)
      {
        Result = true;
        Not_Pressed = true;
        cursor = 0;
        lcd.setCursor(0,1);
        lcd.print("                ");
        digitalWrite(Buzzer, LOW);
        digitalWrite(Yled, LOW);
      }
    }
    Pressed1 = true;
  }
}

void Tester_Release()
{
  digitalWrite(Buzzer, LOW);
  digitalWrite(Yled, LOW);
  if (Pressed1 == true)
  {
    Pr_End1 = millis();
    Rl_Str1 = millis();
    Press1 = round(Pr_End1 - Pr_Str1);
    if (Press1 >= U_Delay)
    {
      if (Press1 >= 3 * U_Delay) Value += "-";
      else Value += ".";
    }
    Pressed1 = false;
    Not_Pressed = false;
  }
}

void Trainer()
{
  if (Sh_Press == true)
  {
    cursor = 0;
    lcd.setCursor(cursor, 1);
    lcd.clear();
    Pressed1 = false;
    Not_Pressed = true;
    Value = "";
    Word = "";
    Word1 = "";
    Sh_Press = false;
  }
  if (Word1 == "")
  {
    lcd.setCursor(0,0);
    lcd.print("  Trainer Mode  ");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print(Word1);
  }
  if (Bt1 == LOW) Trainer_Press();
  if (Bt1 == HIGH) Trainer_Release();
}

void Character_Morse()
{
  lcd.setCursor(0,0);
  lcd.print("Character  Morse");
  while (Serial.available()) 
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    Word = Serial.readString();
    Word.remove(int (Word.length() - 1));
    Serial.print(Word);
    Serial.print(" = ");
    for (int i = 0; i < int (Word.length()); i++)
    {
      Key = Word[i];
      Key.toUpperCase();
      Decode_Value();
      for (int j = 0; j < int (Value.length()); j++)
      {
        if (String (Value[j]) == ".")
        {
          digitalWrite(Buzzer, HIGH);
          digitalWrite(Yled, HIGH);
          delay(U_Delay);
          digitalWrite(Buzzer, LOW);
          digitalWrite(Yled, LOW);
          Serial.print(".");
        }
        else if (String (Value[j]) == "-")
        {
          digitalWrite(Buzzer, HIGH);
          digitalWrite(Yled, HIGH);
          delay(U_Delay * 3);
          digitalWrite(Buzzer, LOW);
          digitalWrite(Yled, LOW);
          Serial.print("-");
        }
        else if (String (Value[j]) == "|")
        {
          delay(U_Delay * 18);
          Serial.print("|");
        }
        else
        {
          digitalWrite(Buzzer, HIGH);
          digitalWrite(Yled, HIGH);
          delay(U_Delay * 18);
          digitalWrite(Buzzer, LOW);
          digitalWrite(Yled, LOW);
          Serial.print(String (Value[j]));
        }
        delay(U_Delay);
      }
      delay(U_Delay * 3);
      Serial.print(" ");
      lcd.print(String (Word[i]));
    }
    Serial.println("");
    delay(2000);
  }
}

void Morse_Character()
{
  lcd.setCursor(0,0);
  lcd.print("Morse  Character");
  while (Serial.available()) 
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    Value = "";
    Word = Serial.readString();
    Word.remove(int (Word.length() - 1));
    Serial.print(Word);
    Serial.print(" = ");
    for (int i = 0; i < int (Word.length()); i++)
    {
      if (String (Word[i]) != " ") Value += Word[i];
      else
      {
        Decode_Key();
        Value = "";
        digitalWrite(Buzzer, HIGH);
        digitalWrite(Yled, HIGH);
        delay(U_Delay * 3);
        digitalWrite(Buzzer, LOW);
        digitalWrite(Yled, LOW);
        Serial.print(Key);
        lcd.print(Key);
        delay(U_Delay * 3);
      }
    }
    Decode_Key();
    Value = "";
    digitalWrite(Buzzer, HIGH);
    digitalWrite(Yled, HIGH);
    delay(U_Delay * 3);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Yled, LOW);
    Serial.print(Key);
    lcd.print(Key);
    delay(U_Delay * 3);
    Serial.println("");
    delay(2000);
  }
}

void Tester()
{
  lcd.setCursor(0,0);
  lcd.print("  Tester  Mode  ");
  if (Sh_Press == true)
  {
    cursor = 0;
    lcd.setCursor(cursor, 1);
    lcd.clear();
    Pressed1 = false;
    Not_Pressed = true;
    Value = "";
    Test++;
    Marks = 0;
    ch = 0;
    Result = false;
    Sh_Press = false;
  }
  if (Test >= 10) Test = 0;
  if (Test == 9) Word = "4CS7$%SH&!KJB9HX";
  if (Test == 8) Word = "IHD5$@#HKJDFBV.J";
  if (Test == 7) Word = "ISD6$W.,543@VJ%B";
  if (Test == 6) Word = "XYT#B6%8&GD.-TS2";
  if (Test == 5) Word = "C7Z3Q8W4F7G3JE3Y";
  if (Test == 4) Word = "BV3C5SAI8G35CH95";
  if (Test == 3) Word = "ZBE3W7DFG6VSKYVK";
  if (Test == 2) Word = "OGBSWSBCLUDFVXCV";
  if (Test == 1) Word = "HTDESGDTFSFIGUKL";
  if (Test == 0) Word = "HELLOWORLDHOWARE";
  if (Result == true)
  {
    lcd.setCursor(0, 1);
    int per = round(Marks * 100 / 16);
    lcd.print("Marks ");
    lcd.print(Marks);
    lcd.print("/16 ");
    lcd.print(per);
    lcd.print("%");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print(Word);
    Word1 = Word[ch];
    if (Bt1 == LOW) Tester_Press();
    if (Bt1 == HIGH) Tester_Release();
    if (Not_Pressed == false)
    {
      for (ch; ch < int (Word.length()); ch)
      {
        Word1 = Word[ch];
        Bt1 = digitalRead(Sw1);
        if (Bt1 == LOW) Tester_Press();
        if (Bt1 == HIGH) Tester_Release();
      }
    }
  }
}

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  
  lcd.setCursor(0,0);
  lcd.print("   Created by   ");
  lcd.setCursor(0,1);
  lcd.print("  BHAVESH KALE  ");   
  delay(2500);
  lcd.clear();
       
  lcd.setCursor(0,0);
  lcd.print("Welcome to morse");
  lcd.setCursor(0,1);
  lcd.print("code interpreter");   
  delay(2500);
  lcd.clear();

  Serial.println("INSTRUCTIONS\n");
  Serial.println("Type massage only upto 16 characters");
  Serial.println("Then press send button and wait\n");
  Serial.println("For Morse to Character");
  Serial.println("1) Type your message in '.' and '-'");
  Serial.println("2) For spacing word use '|'");
  Serial.println("3) Every letter code is saperated by ' '\n");
  Serial.println("For Character to Morse");
  Serial.println("1) Type your message in characters\n");
   
  pinMode(Sw1, INPUT_PULLUP);
  pinMode(Sw2, INPUT_PULLUP);
  pinMode(Buzzer, OUTPUT);
  pinMode(Yled, OUTPUT);
    
  lcd.createChar(0, Block);
  lcd.createChar(1, Right);
}

void loop()
{
  Bt1 = digitalRead(Sw1);
  Bt2 = digitalRead(Sw2);
  if (Bt2 == LOW && Pressed2 == false)
  {
    Pr_Str2 = millis();
    Pressed2 = true;
  }
  if (Bt2 == HIGH && Pressed2 == true)
  {
    Pr_End2 = millis();
    Press2 = round(Pr_End2 - Pr_Str2);
    if (50 <= Press2 <= B_Delay) Sh_Press = true;
    if (Press2 >= 2 * B_Delay)
    {
      cursor = 0;
      lcd.setCursor(cursor, 1);
      lcd.clear();
      Pressed1 = false;
      Not_Pressed = true;
      Result = false;
      Value = "";
      Word = "";
      Word1 = "";
      Marks = 0;
      Sh_Press = false;
      Mode++;
    }
    Pressed2 = false;
  }
  if (Mode >= 4) Mode = 0;
  if (Mode == 3) Tester();
  else if (Mode == 2) Morse_Character();
  else if (Mode == 1) Character_Morse();
  else Trainer();
}
