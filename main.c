/*
 * Electromagnetic Feeder Program
 */
#define ZVC 2
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// Set the LCD I2C address
#include <HX711_ADC.h>

// HX711 constructor (dout pin, sck pin):
HX711_ADC LoadCell(A0, A1);
long t;
float k;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
    {'1', '2', '3', '7'},
    {'4', '5', '6', 'B'},
    {'t', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {10, 9, 8, 7};                                  // Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = {6, 5, 4, 3};                                   // Connect keypad COL0, COL1 and COL2 to these Arduino pins.
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); //  Create the Keypad
int i = 0;
int j = 0;
float Num1, Num2;
float Number;
int wait = 500;
float zeroCross;
float liveWeight;

float percentage;
int Mode = 1;
char key, action;
boolean enter = false;
int Start = 2;

void setup()
{
    pinMode(ZVC, INPUT_PULLUP);
    // pinMode(A2,INPUT);
    pinMode(A3, OUTPUT);
    lcd.begin(16, 2);   // We are using a 16*2 LCD display
    lcd.print("RCOEM"); // Display a intro message
    delay(1000);
    lcd.setCursor(0, 1);          // set the cursor to column 0, line 1
    lcd.print("Electromagnetic"); // Display a intro message
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Feeder");
    delay(2000); // Wait for display to show info
    lcd.clear(); // Then clean it
    lcd.setCursor(0, 0);
    lcd.print("Please Set Your");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Desired Weight");
    delay(1000);
    lcd.clear();
    Serial.begin(9600);
    delay(10);
    LoadCell.begin();
    long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
    LoadCell.start(stabilisingtime);
    LoadCell.setCalFactor(659.36); // user set calibration value (float)
}

void (*resetFunc)(void) = 0;

void loop()
{

    key = kpd.getKey(); // storing pressed key value in a char
    if (key != NO_KEY)
        DetectButtons();

    if (Mode == 1)
    {
        if (enter == true)
        {
            Weight();
            Mode = 2;
            lcd.clear();
        }
    }
    if (Mode == 2)
    {
        // update() should be called at least as often as HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS
        // longer delay in sketch will reduce effective sample rate (be carefull with delay() in the loop)
        LoadCell.update();
        // get smoothed value from the data set
        if (millis() > t + 250)
        {
            k = LoadCell.getData();
            lcd.setCursor(0, 0);
            lcd.print("Live Weight:");
            lcd.setCursor(0, 1);
            lcd.print(k, 3);
            lcd.print("g");
            t = millis();
        }
        percentage = (k / Number) * 100;
        lcd.setCursor(10, 1);
        lcd.print(percentage);
        lcd.print("%");
        if (percentage >= 0 && percentage <= 50)
        {
            wait = 7000;
        }
        else if (percentage >= 51 && percentage <= 95)
        {
            wait = 7000;
        }
        else if (percentage > 95 && percentage < 100)
        {
            wait = 7000;
        }
        else if (percentage >= 100)
        {
            /* wait= ;
               Start=0; */
            resetFunc();
        }

        attachInterrupt(0, Fire, FALLING);

        /* zeroCross=digitalRead(A2);

        if(zeroCross==HIGH && wait>0)
       {
         delay(wait);
         digitalWrite(A3,HIGH);
        }
         */
    }
}

void Fire()
{
    // if(percentage<100 && Start==1){

    // Serial.println("REad");

    delayMicroseconds(wait); // read AD0
    digitalWrite(A3, HIGH);

    // Serial.println(digitalRead(triacPulse));

    delayMicroseconds(50); // delay 50 uSec on output pulse to turn on triac
    digitalWrite(A3, LOW);

    // Serial.println(digitalRead(triacPulse));
    //}
}

void DetectButtons()
{

    if (key == '*') /*If cancel Button is pressed*/
    {
        Num1 = Number;
        Number = 0;
        lcd.print('.');
    }

    if (key == '1') // If Button 1 is pressed
    {
        lcd.setCursor(i, j);
        lcd.print("1");
        if (Number == 0)
            Number = 1;
        else
            Number = (Number * 10) + 1; // Pressed twice
    }

    if (key == '4') // If Button 4 is pressed
    {
        lcd.setCursor(i, j);
        lcd.print("4");
        if (Number == 0)
            Number = 4;
        else
            Number = (Number * 10) + 4; // Pressed twice
    }

    if (key == '7') // If Button 7 is pressed
    {
        lcd.setCursor(i, j);
        lcd.print("7");
        if (Number == 0)
            Number = 7;
        else
            Number = (Number * 10) + 7; // Pressed twice
    }

    if (key == '0')
    {
        lcd.setCursor(i, j);
        lcd.print("0"); // Button 0 is Pressed
        if (Number == 0)
            Number = 0;
        else
            Number = (Number * 10) + 0; // Pressed twice
    }

    if (key == '2') // Button 2 is Pressed
    {
        lcd.setCursor(i, j);
        lcd.print("2");
        if (Number == 0)
            Number = 2;
        else
            Number = (Number * 10) + 2; // Pressed twice
    }

    if (key == '5')
    {
        lcd.setCursor(i, j);
        lcd.print("5");
        if (Number == 0)
            Number = 5;
        else
            Number = (Number * 10) + 5; // Pressed twice
    }

    if (key == '8')
    {
        lcd.setCursor(i, j);
        lcd.print("8");
        if (Number == 0)
            Number = 8;
        else
            Number = (Number * 10) + 8; // Pressed twice
    }

    if (key == '#')
    {
        lcd.setCursor(0, 1);
        lcd.print("Enter");
        delay(1000);
        Num2 = Number;
        Number = 0;
        enter = true;
        float n = 10;
        for (int D = 1; D > 0; n = n * 10)
        {
            D = Num2 / n;
        }
        float diviser = n / 10;
        Number = Num1 + (Num2 / diviser);
    }

    if (key == '3')
    {
        lcd.setCursor(i, j);
        lcd.print("3");
        if (Number == 0)
            Number = 3;
        else
            Number = (Number * 10) + 3; // Pressed twice
    }

    if (key == '6')
    {
        lcd.setCursor(i, j);
        lcd.print("6");
        if (Number == 0)
            Number = 6;
        else
            Number = (Number * 10) + 6; // Pressed twice
    }

    if (key == '9')
    {
        lcd.setCursor(i, j);
        lcd.print("9");
        if (Number == 0)
            Number = 9;
        else
            Number = (Number * 10) + 9; // Pressed twice
    }
    if (key == 't' || key == 'B' || key == 'C' || key == 'D') // Detecting Buttons on Column 4
    {

        if (key == 't')
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("t");
            delay(1000);
            lcd.clear();
            LoadCell.begin();
            long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
            LoadCell.start(stabilisingtime);
            LoadCell.setCalFactor(323.03); // user set calibration value (float)
            lcd.setCursor(0, 1);
            lcd.print("Tare is complete");
            delay(1000);
            lcd.clear();
        }
        if (key == 'B')
        {
            lcd.print("Decrease");
        }
        if (key == 'C')
        {
            lcd.print("Yet to be decided");
        }
        if (key == 'D')
        {
            lcd.println("Decimal");
        }
        delay(100);
    }
    i++;
}

void Weight()
{
    if (enter == true)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Set Weight is");
        lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
        lcd.print(Number, 3); // Display the result
        lcd.print("g");
        delay(1000);
    }
}