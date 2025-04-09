#include <LiquidCrystal.h>

const int reflPin = A0;
const int transPin = A1;
const int reflThreshold = 25; // Temporary, adjust after increasing readings
const int transThreshold = 19;  // Temporary, adjust after increasing readings

int decimalDigits = 0;
int binaryDigits = 0;
int numberCount = 0;
String binaryNumber = "";
int generatedNumbers = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("QRNG Starting...");

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Ready...");
  lcd.setCursor(0, 1);
  lcd.print("Enter digits");

  Serial.println("Enter number of decimal digits (e.g., 3 for 0-999):");
  while (Serial.available() == 0) {}
  decimalDigits = Serial.parseInt();
  Serial.print("Decimal digits set to: ");
  Serial.println(decimalDigits);

  int maxDecimal = pow(10, decimalDigits) - 1;
  binaryDigits = ceil(log(maxDecimal + 1) / log(2));
  Serial.print("Binary digits required: ");
  Serial.println(binaryDigits);

  while (Serial.available() > 0) {
    Serial.read();
  }

  Serial.println("Enter how many random numbers you want (e.g., 3):");
  while (Serial.available() == 0) {}
  numberCount = Serial.parseInt();
  Serial.print("Numbers set to: ");
  Serial.println(numberCount);

  while (Serial.available() > 0) {
    Serial.read();
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Generating...");
  Serial.println("Generating random numbers...");
}

void loop() {
  if (generatedNumbers >= numberCount) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Done!");
    lcd.setCursor(0, 1);
    lcd.print("Restart to run");
    Serial.println("Done! Restart Arduino to generate more.");
    while (true) {}
  }

  int reflValue = analogRead(reflPin);
  int transValue = analogRead(transPin);

  Serial.print("Refl: ");
  Serial.print(reflValue);
  Serial.print(" Trans: ");
  Serial.print(transValue);
  Serial.print(" Bit: ");
  if (reflValue > reflThreshold && transValue < transThreshold) {
    binaryNumber += "1";
    Serial.println("1 (Refl > ReflTh, Trans < TransTh)");
    delay(100);
  } 
  else if (transValue > transThreshold && reflValue < reflThreshold) {
    binaryNumber += "0";
    Serial.println("0 (Trans > TransTh, Refl < ReflTh)");
    delay(100);
  } else {
    Serial.print("None - ");
    if (reflValue > reflThreshold && transValue > transThreshold) {
      Serial.println("Both above their thresholds");
    } else if (reflValue < reflThreshold && transValue < transThreshold) {
      Serial.println("Both below their thresholds");
    } else {
      Serial.println("Other mismatch");
    }
  }

  if (binaryNumber.length() >= binaryDigits) {
    long decimalNumber = strtol(binaryNumber.c_str(), NULL, 2);
    int maxDecimal = pow(10, decimalDigits) - 1;
    if (decimalNumber > maxDecimal) {
      decimalNumber = decimalNumber % (maxDecimal + 1);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bin: ");
    lcd.print(binaryNumber);
    lcd.setCursor(0, 1);
    lcd.print("Dec: ");
    lcd.print(decimalNumber);
    Serial.print("Binary: ");
    Serial.print(binaryNumber);
    Serial.print(" | Decimal: ");
    Serial.println(decimalNumber);
    generatedNumbers++;
    binaryNumber = "";
    delay(2000);
  }
}