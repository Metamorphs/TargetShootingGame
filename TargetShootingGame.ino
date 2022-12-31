// Metamorphs
// 2022.12.20
// Automatic shoot stand with many targets. 
// if green led, it's a friendly target, you will loose points. 
// if it's red, it's an ennemy, shoot it !
// you will have different level and target

// library
#include <LiquidCrystal.h>
#include "IRremote.h"

/*-----( Declare objects )-----*/
// pin definition
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Lcd for score. will change with a BLE for a phone app

int ledPin[10][2] = {{22, 23},{24, 25},{26, 27},{28, 29},{30, 31},{32, 33},{34, 35},{36, 37},{38, 39},{40, 41}}; // Pin of led. {target1:{green_led,red_led}, target2:{...}}

int buttonPin[10] = {53,52,51,50,49,48,47,46,45,44}; // pin of button. first pin is for the first targer, (ex: pin 50 is for the target with led pin 22 and 23 ...)

int receiver = 13; // Signal Pin of IR receiver to Arduino Digital Pin 11
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

// value definition
int points; //points
int targetNumberOn[50]; // first target
int ledNumberOn[50]; // led of the first target (green or red)
bool step[50]; // bool to know if you shoot in the target for the first target

bool alreadyUsed;


void setup()
// setup function
{
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  // set all ledPin to be outputs
  for (int target=0; target<10; target++) {
    for (int led=0; led<2; led++) {
      pinMode(ledPin[target][led], OUTPUT);
    }
  }
  // set all buttonPint to be Input_pullup
  for (int target=0; target<10; target++) {
    pinMode(buttonPin[target], INPUT_PULLUP);
  }
  irrecv.enableIRIn(); //start IR receptor
}

void clean_screen()
// clean screen function
{
  lcd.setCursor(0, 0);
  lcd.print("                                         ");
  lcd.setCursor(0, 1);
  lcd.print("                                         ");
}

void loop()
// main menu function
{
  bool level = false; // if level is set
  bool target = false; // if number of target is set
  int numberLevel = 999; // level
  int numberTarget = 0; // number of target
  points = 0; // your points

  //prepare screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Number of target:");

  //wait a command for asking number of target connected to the arduino
  while (target == false){
    if (irrecv.decode(&results)){
      lcd.setCursor(0, 0);
      switch(results.value){
      case 0xFF30CF:
        numberTarget = 1;
        lcd.setCursor(0, 1);
        lcd.print("1");
        target = true;
        break;
      case 0xFF18E7:
        numberTarget = 2;
        lcd.setCursor(0, 1);
        lcd.print("2");
        target = true;
        break;
      case 0xFF7A85:
        numberTarget = 3;
        lcd.setCursor(0, 1);
        lcd.print("3");
        target = true;
        break;
      case 0xFF10EF:
        numberTarget = 4;
        lcd.setCursor(0, 1);
        lcd.print("4");
        target = true;
        break;
      }
      irrecv.resume();
    }
  }
  delay(2000);
  lcd.clear();

  //wait a command for the level
  while (level == false){
    if (irrecv.decode(&results)){
      lcd.setCursor(0, 0);
      lcd.print("level:");
      switch(results.value){
      case 0xFF6897:
        numberLevel = 0;
        lcd.setCursor(0, 1);
        lcd.print("0");
        level = true;
        break;
      case 0xFF30CF:
        numberLevel = 1;
        lcd.setCursor(0, 1);
        lcd.print("1");
        level = true;
        break;
      case 0xFF18E7:
        numberLevel = 2;
        lcd.setCursor(0, 1);
        lcd.print("2");
        level = true;
        break;
      case 0xFF7A85:
        numberLevel = 3;
        lcd.setCursor(0, 1);
        lcd.print("3");
        level = true;
        break;
      case 0xFF10EF:
        numberLevel = 4;
        lcd.setCursor(0, 1);
        lcd.print("4");
        level = true;
        break;
      }
      irrecv.resume();
    }
  }
  delay(2000);
  start(numberLevel, numberTarget);
}


void start(int level, int target) 
//start game function
{
  bool setOff = false;

  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Points:");// Print a message to the LCD.

  //infinite loop while button off of the ir receiver is not pushed
  while (setOff == false){
    // for level 0 only red led can be on and after it pushed, another target will spawn
    if (level == 0)
    {
      int targetAloneOn = random(target);
      bool stepAlone = false;
      
      digitalWrite(ledPin[targetAloneOn][1], HIGH);

      //check if you shoot on target
      while (stepAlone == false || digitalRead(buttonPin[targetAloneOn]) == LOW){
        // if button of this target is pushed, attribuate points
        if (digitalRead(buttonPin[targetAloneOn]) == LOW && stepAlone == false){
          stepAlone = true;
          points = points + 10;
          digitalWrite(ledPin[targetAloneOn][1], LOW);
        }
        // if another button is pushed, you loosing points
        for (int enum_target=0; enum_target<target; enum_target++){
          alreadyUsed = false;
          if (enum_target == targetAloneOn){
            alreadyUsed = true;
          }
          if (alreadyUsed == false) {
            if (digitalRead(buttonPin[enum_target]) == LOW){
              while (digitalRead(buttonPin[enum_target]) == LOW){
              }
              points = points - 5;
            }
          }
        }
      }
      // and set off lights
      digitalWrite(ledPin[targetAloneOn][1], LOW);
    }

    //that part is for target with green and red light with on or more target at same time
    else
    {
      //déclare "target" nombre aléatoire de variable non similaire pour définir quel cible va s'allumer ou pas
      int myH1[target];
      for (int i = 0; i<target; i++){
        myH1[i] = i;
      }
      int count = sizeof myH1 / sizeof myH1[0];
      for (int i = 0; i < level; i++) {
        int index = random(count);   // Random integer from 0 to count-1
        targetNumberOn[i] = myH1[index];
        myH1[index] = myH1[count-1];
        count--;
        step[i] = false;
        ledNumberOn[i] = random(2);        
      }

      long delta = 4000-points*2; // more you have points, more speedrate will be
      unsigned long t0  = millis(); // actual time
      
      //light 1 led for 2 différents target
      for (int val=0; val<level; val++){
        digitalWrite(ledPin[targetNumberOn[val]][ledNumberOn[val]], HIGH);
      }

      //check button during this delta. after that, points will be attribuate and light will be off, then start again...
      while (millis() - t0 < delta){
        //boucle for avec x et y
        // if button of this target is pushed, attribuate points
        for (int val=0; val<level; val++){
          if (digitalRead(buttonPin[targetNumberOn[val]]) == LOW && step[val] == false){
            step[val] = true;
            // if it was green... sorry
            if (ledNumberOn[val] == 0){
              points = points - 15;
            }
            else{
              points = points + 10;
            }
            digitalWrite(ledPin[targetNumberOn[val]][ledNumberOn[val]], LOW);
          }
        }
  
        // if another button is pushed, you loosing points
        for (int enum_target=0; enum_target<4; enum_target++){
          alreadyUsed = false;
          for (int enum_on=0; enum_on<level; enum_on++){
            if (enum_target == targetNumberOn[enum_on]){
              alreadyUsed = true;
            }
          }
          if (alreadyUsed == false) {
            if (digitalRead(buttonPin[enum_target]) == LOW){
              while (digitalRead(buttonPin[enum_target]) == LOW){
              }
              points = points - 5;
            }
          }
        }
      }

      // then distribuate points if not touch
      for (int enum_on=0; enum_on<level; enum_on++){
        if (ledNumberOn[enum_on] == 1 && step[enum_on] == false){
          points = points - 5;
        }
      }

      // and set off lights
      for (int enum_on=0; enum_on<level; enum_on++){
        digitalWrite(ledPin[targetNumberOn[enum_on]][ledNumberOn[enum_on]], LOW);
      }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("points:");
    lcd.setCursor(0, 1);
    lcd.print(points);
    delay(100);
    
    if (irrecv.decode(&results)){ //rajouter le mode pause pour voir les points
      if (results.value == 0xFFA25D){
        setOff = true;
      }
      irrecv.resume();
    }
  }
}


