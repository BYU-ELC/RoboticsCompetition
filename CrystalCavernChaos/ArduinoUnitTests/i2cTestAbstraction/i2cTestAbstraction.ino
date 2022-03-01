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
  static String previousText = "foo";

  if (previousText == text) {
    // They're trying to reprint what's already been printed so don't do anyting!!!
    Serial.print("already printed: ");
    Serial.println(text);
    return;
  }
  
  if (text.length() < 0 || text.length() > 16) {
    Serial.println("Text TOO Large!!!");
    previousText = text;
    return;
  }
  else {
    Serial.print("Valid print: ");
    Serial.println(text);
    lcd.setCursor(0,pos);
    lcd.print("                ");
    lcd.setCursor(0,pos);
    lcd.print(text);
  }

  previousText = text;
}

void printScreen(const String& top, const String& bottom) {
  if (top == "") {}
  else
    printToRow(0, top);

  if (bottom == "") {}
  else
    printToRow(1, bottom);
}

void loop(){
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
}
