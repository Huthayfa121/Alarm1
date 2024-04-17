#include <avr/io.h> // Include AVR register definitions
#include <Keypad.h>  // keypad library internally configures the DDR registers for the row pins as inputs and the column pins as outputs during initialization.
                    // library internally handles the low-level GPIO register
#include <virtuabotixRTC.h> // Within the virtuabotixRTC library, the Wire library functions (Wire.beginTransmission(), 
                            // Wire.write(), Wire.endTransmission(), Wire.requestFrom(), etc.) are used to initiate and manage the I2C communication with the RTC module.
#include <LiquidCrystal_I2C.h>
 
#define I2C_ADDR 0x27 
#define BUZZER_PIN 5 
 
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 (Slave address) for a 16 chars and 2 line display
 
virtuabotixRTC myRTC(2, 3, 4); //Wiring of the RTC (SCL,I/O,RST)


char keymap[4][4]= {
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte rowPins[4] = {12,11,10,9}; //connect to the row pinouts of the keypad 
byte colPins[4]= {8,7,6,5}; //connect to the column pinouts of the keypad

char keypressed,keypressedx;
 
int A_hour=NULL;
int A_minute=NULL;
int B_hour=NULL;
int B_minute=NULL;
int C_hour=NULL;
int C_minute=NULL;
int AlarmIsActive1=NULL;
int AlarmIsActive2=NULL;
int AlarmIsActive3=NULL;

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, 4, 4);
 
 
void setup() {
  DDRB |= (1 << BUZZER_PIN); //set the pin connect with the buzzer in port B to be an output by create a bitmask that corresponds and then 1 is shifted left by 5 positions.
  lcd.init();      
  lcd.backlight();  
                                                   
}
 
void loop() {     
 setDate();
}

int getNum(int x) {
  int num = 0; 
  for (int i = 0; i < x; i++) { 
    char key = myKeypad.waitForKey();
    if (key >= '0' && key <= '9') {
      int digit = key - '0'; 
      num = num * 10 + digit; 
      lcd.setCursor(i, 1);
      lcd.print(digit);
    }
  }
  return num;
}

void setDate(){
    while(keypressed == NO_KEY){ 
      keypressed = myKeypad.getKey();
      lcd.clear();
      myRTC.updateTime();
      
      if(myRTC.hours==A_hour && myRTC.minutes==A_minute && AlarmIsActive1==1){
        while(keypressedx == NO_KEY){
          lcd.clear();
          lcd.print("Medication Time           ."); 
          PORTB |= (1 << BUZZER_PIN); // PORTB register controls the output state == digitalWrite --> Port B = 00000000 | 00100000 = 00100000
          keypressedx = myKeypad.getKey();
        }
      if (keypressedx == 'D'){
        PORTB &= ~(1 << BUZZER_PIN); // Port B = 00000000 & 11011111 = 00000000
        lcd.clear();
        lcd.print("Alarm deactivated");
        AlarmIsActive1=0;
        keypressed=NO_KEY;
        delay(500);
        }
      }

    if(myRTC.hours==B_hour && myRTC.minutes==B_minute && AlarmIsActive2==1){
        while(keypressedx == NO_KEY){
          lcd.clear();
          lcd.print("Medication Time           .");
          PORTB |= (1 << BUZZER_PIN);
          keypressedx = myKeypad.getKey();
        }
        if (keypressedx == 'D'){
          PORTB &= ~(1 << BUZZER_PIN);
          lcd.clear();
          lcd.print("Alarm deactivated");
          AlarmIsActive2=0;
          keypressed=NO_KEY;
          delay(500);
        }
      }

    if(myRTC.hours==C_hour && myRTC.minutes==C_minute && AlarmIsActive3==1){
        while(keypressedx == NO_KEY){
          lcd.clear();
          lcd.print("Medication Time           .");
          PORTB |= (1 << BUZZER_PIN);
          keypressedx = myKeypad.getKey();
        }
        if (keypressedx == 'D'){
          PORTB &= ~(1 << BUZZER_PIN);
          lcd.clear();
          lcd.print("Alarm deactivated");
          AlarmIsActive3=0;
          keypressed=NO_KEY;
          delay(500);
          }
    }

    lcd.clear();
    keypressedx = NO_KEY;
    lcd.setCursor(0,0);
    lcd.print(myRTC.dayofmonth);
    lcd.print("/");
    lcd.print(myRTC.month);
    lcd.print("/");
    lcd.print(myRTC.year);
    lcd.setCursor(0,1);
    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);
    delay(500);
  }

  if (keypressed == '*') {
    lcd.clear();
    lcd.print("     Setup");
    delay(1000);
    lcd.clear();
    lcd.print("Setup Year");
    int N_year=getNum(4);
    delay(1000);
    lcd.clear();
    lcd.print("Setup Month");
    int N_month=getNum(2);
    delay(1000);
    lcd.clear();
    lcd.print("Setup Day");
    int N_day=getNum(2);
    delay(1000);
    lcd.clear();
    lcd.print("Setup Hour");                   
    int N_hour=getNum(2);
    delay(1000);
    lcd.clear();
    lcd.print("Setup Minutes");
    int N_minutes=getNum(2);
    delay(1000);
    lcd.clear();
    myRTC.setDS1302Time(1, N_minutes, N_hour, 1, N_day, N_month, N_year);
    keypressed=NO_KEY;
  }
  /////////////////////////////////////////Alarme setup/////////////////////////////////
  if (keypressed == 'A'){
    lcd.clear();
    lcd.print("Set Alarm A ");
    delay(1000);
    lcd.clear();
    lcd.print("Set Alarm Hour");
    A_hour=getNum(2);
    delay(1000);
    lcd.clear();
    lcd.print("Set Alarm Minutes");
    A_minute=getNum(2);
    delay(1000);
    lcd.clear();
    AlarmIsActive1=1;
    keypressed=NO_KEY;
  }
  if (keypressed == 'B'){
    lcd.clear();
    lcd.print("Set Alarm B ");
    delay(1000);
    lcd.clear();
    lcd.print("Set Alarm Hour");
    B_hour=getNum(2);
    delay(1000);
    lcd.clear();
    lcd.print("Set Alarm Minutes");
    B_minute=getNum(2);
    delay(1000);
    lcd.clear();
    AlarmIsActive2=1;
    keypressed=NO_KEY;
  }
    if (keypressed == 'C'){
    lcd.clear();
    lcd.print("Set Alarm C ");
    delay(1000);
    lcd.clear();
    lcd.print("Set Alarm Hour");
    C_hour=getNum(2);
    delay(1000);
    lcd.clear();
    lcd.print("Set Alarm Minutes");
    C_minute=getNum(2);
    delay(1000);
    lcd.clear();
    AlarmIsActive3=1;
    keypressed=NO_KEY;
  }
  else {
    myRTC.updateTime();
    keypressed=NO_KEY;
  }
}

