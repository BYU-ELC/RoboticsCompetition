#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  Serial.begin(9600);
}

void printToRow(int pos, const String& text) {
  static String previousText[2]{"foo", "bar"};

  if (previousText[pos] == text) {
    // They're trying to reprint what's already been printed so don't do anyting!!!
    Serial.print("already printed: ");
    Serial.println(text);
    return;
  }

  if (text.length() < 0 || text.length() > 16) {
    //Serial.println("Text TOO Large!!!");
    previousText[pos] = text;
    return;
  } else {
    //Serial.print("Valid print: ");
    //Serial.println(text);
    lcd.setCursor(0,pos);
    Serial.println(previousText[pos]);
    for (int i = 0; i < text.length(); ++i) {
      if (previousText[pos][i] == text[i]) {
        // They're trying to reprint what's already been printed so don't do anyting!!!
        //Serial.println(String(previousText[pos][i])+String(text[i]));
        continue;
      } else {
        lcd.setCursor(i,pos);
        lcd.print(text[i]);
      }
    }
    for (int i = text.length(); i < 16; ++i) {
      lcd.setCursor(i, pos);
      lcd.print(' ');
    }
  }

  previousText[pos] = text;
}

void printScreen(const String& top, const String& bottom) {
  if (top == "") {}
  else
    astheticPrintToRow(0, top);

  if (bottom == "") {}
  else
    astheticPrintToRow(1, bottom);
}

void loop(){
  /*
  printScreen("HELLO", "WORLD");
  delay(1000);
  printScreen("", "hello");
  delay(1000);
  printScreen("world", "");
  delay(1000);
  printScreen("hello", "world");
  delay(1000);
  printScreen("hello", "Mom!");
  delay(1000);
  printScreen("This text will be too long to print on a single line!", "Dad!");
  delay(1000);
  printScreen("", "Dad!");
  delay(1000);
  */

  /*
  for (long i = 0; i < 30000; ++i) {
    printScreen(String(i) + "  " + String(i+1), "");
    //delay(200);
  }
  */
  
}
