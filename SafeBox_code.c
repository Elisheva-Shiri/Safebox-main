/* To Libiu Gal: micro procesor lab ; by Elisheva Shiri Decktor & Idan Dayan*/
#include <Servo.h>                    // Include servo librery

/*-----------------------------declare function--------------------------*/
void openServo (void);                // Declare a function to open the servo
void closeServo (void);               // Declare a function to close the servo
void showDigit(int, int);             // Declare a function to desplay the digit on the 4digit display
void showNumber(int);                 // Declare a function accept the digit to the 4digit display
void showNumber_delay( int, int);     // Declare a function to show the digit on the 4digit display
void collectPassword(void);           // Declare a function to marge the key input as a password
void lowLeds (void);                  // Declare a function to close all the leds at ones
char findKey(void);                   // Declare a function to collect keys from the keyboard
int ultraSony(void);                  // Declare a function to operete the Ultra Sony sensor

/*------------------------constance----------------------------------------*/
String display_num ;                  // Varible for display number    
int digit = 0 ;                       // Varible for collecting the keys                     
int count = 0;                        // Varible for counting attempt
int i = 0 ;                           // Varible for loops

/*interupt const*/
static int dis = 1000 ;               // Created to Placement of conditions on the distance
const int actButton = 7 ;             // Need to operate the system
const int openLed = 11 ;              // User interface

/*ultra sony const*/
#define echoPin 49                    // Attach pin for echo
#define trigPin 48                    // Attach pin for triger
long Duration = -1 ;                  // Variable for the duration of sound wave travel
float distance = -1;                  // Variable for the distance measurement


/* keyboard const*/
const int keyPadIn [3] = {42,41,40};                           // Declare pin numbers
const int keyPadOut [3] = {45,44,43};                          // Declare pin numbers
char arr [3][3]= {{'1','2','3'},{'4','5','6'},{'7','8','9'}};  // Without the last row&col
char charOut;                                                  // Declare for key keeper

/*7seg const*/
#define DELAY_TIME 1500
                                                               // bits representing segments A through G for numerals 0-9
const int numeral[10] = {
  //Declare the ABCDEFG
  B11111100,//number 0
  B01100000,//number 1
  B11011010,//number 2
  B11110010,//number 3
  B01100110,//number 4
  B11110010,//number 5
  B10111110,//number 6
  B11100000,//number 7
  B11111110,//number 8
  B11110110,//number 9
  };                                                           // pins for decimal point and each segment dp,G,F,E,D,C,B,A
const int segmentPins[] = {32, 25, 31, 28, 26, 29, 23, 27};    // Complete the pin numbers connected to each segment 
const int nbrDigits = 4;                                       // The number of digits in the LED display
const int digitPins[nbrDigits] = {22, 24, 30, 33};             // Complete the pin numbers connected to each DIGIT (anode)

/* password const*/
String password = "1234" ;                                     // Save Password
const int greenLed = 10 ;                                      // User intarface
const int redLed = 13 ;                                        // User intarface

/*servo const*/
Servo Myservo;                                                 //Declare for the servo
int servoPin = 4;                                              // Servo pin declarationint pos ;                                                      // Varible for servo position


/*-----------------------------------setup-----------------------------------------------*/

void setup() {
  /*user interface*/
  Serial.begin (9600);                     // Setup the serial begin

  pinMode(openLed,OUTPUT);                 // Setup the pin led as an output                    
  pinMode(greenLed, OUTPUT);               // Setup the pin led as an output  
  pinMode(redLed, OUTPUT);                 // Setup the pin led as an output   
  lowLeds();                               // Close all the leds

  /*interupt setup*/
  
  pinMode(actButton, INPUT_PULLUP);        // Setup the pin button as input (inere) pullup 
  
    /*Ultra sony*/
  pinMode(trigPin, OUTPUT);                // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);                 // Sets the echoPin as an INPUT
  
/* keyboard setup*/
  for(int i = 0; i<3; i++)
    pinMode(keyPadIn[i], INPUT_PULLUP);    // Loop to setup the in key pins (inere) pullup
  
  for(int i = 0; i<3; i++)                 // Loop to setup the out key pins as output
    pinMode(keyPadOut[i], OUTPUT);
    
  for (int i = 0; i < 3; i++)              // Loop to setup the out key pins as HIGH
    digitalWrite(keyPadOut[i], HIGH);
       
  /* 7seg setup*/
  for (int i = 0 ; i < 8 ; i++){
    pinMode(segmentPins[i], OUTPUT);       // Set segment pins to output
  }
  for (int i = 0 ; i < nbrDigits ; i++){   // Set DP pins to outpu 
    pinMode(digitPins[i], OUTPUT);
  }

  /*servo*/
  Myservo.attach(servoPin);                // Set pin servo as a PWM
  
}

/*---------------------------------loop--------------------------------*/
void loop() {                                            // Main function , the loop
  lowLeds();                                             // Close all teh leds
  Serial.println ("Hello, good Morning!");               // User interface
  digitalWrite(openLed , HIGH);                          // Open the with led
  
  while (dis > 10){
    dis = ultraSony() ;                                   // Call the US function
    Serial.println ("The distance is:");                  // User interface
    Serial.println (dis);                                 // User interface
    delay(1000);                                          // Creat a delay between calls
  }
  
  Serial.println("Someone is here");                       // User interface
    while(digitalRead(actButton) == HIGH);                 // Wait for pushbutton
    Serial.println("Please enter your password:");         // User interface
    delay (100);                                           // Dalay for user interface
    Serial.print("Your enter is:");                        // User interface
    collectPassword();                                     // Call the functiom to receive the keyboard input   
    showNumber_delay(display_num.toInt(), DELAY_TIME);     // Function that define the ammount of time the desplay is on
      if ( display_num == password ){                      // Wright password senario
        digitalWrite(greenLed , HIGH);                     // Show a green light to notificate on a sucsses
        Serial.println("Your password is correct");        // User interface
        openServo();                                       // Open the door to the safe box
      }
      else {                                            // Wrong password senario
        digitalWrite(redLed , HIGH);                    // Show a red light to notificate on an error
        Serial.println("Your password is wrong");       // User interface
        display_num = "";                               // offset the display_num as an empty string
      }
   
  delay (10000);            // Create a breack for the owner to use the safebox
  closeServo ();            //Close the door
  lowLeds();                //Close the led
  exit(0);                  //Exit the loop

  }



/*---------------------------functions-----------------------------------------*/
/*keyboard*/
char findKey(){                                          //function given name 
 for( int r = 0;r < 3 ;r++){                             // pass all the rows
  for(int j = 0 ;j <3; j++)                              //pass all the coloumbs
     digitalWrite(keyPadOut[j],HIGH);                    // define all the columb as HIGH (voltage) ==  1
  digitalWrite(keyPadOut[r],LOW);                        //define only the choosen raw as LOW (voltage) == 0
  for(int c = 0; c < 3; c++)                             //pass all the coloumbs
    if(digitalRead(keyPadIn[c]) == 0)
      return arr[r][c]; 
 }
 return '$';                                             // If a key didn't detacted return a control signle
}


/*7seg*/
void showNumber_delay( int number, int millis_delay){    //function given name
  int start_time, end_time;                              // Varibles for time messared
  start_time = millis();                                 // Setup the time start
  end_time = millis();                                   // Setup the time end
  while (end_time - start_time < millis_delay) {         // Loop to Chack if we are still in the display piriote
    showNumber(number);                                  // Show the display
    end_time = millis();                                 // Check the time that pass
  }
}
//display a 4 digit number
void showNumber( int number){                            //function given name
  if (number == 0)                                       // Chack if we in the initialized
    showDigit( 0, nbrDigits - 1) ;                       // display 0 in the rightmost digit
  else
  {
    // display the value corresponding to each digit
    // leftmost digit is 0, rightmost is one less than the number of places
    for ( int digit = nbrDigits - 1; digit >= 0; digit--)
    {
      if (number > 0)                                   // Control conditions
      {
        showDigit( number % 10, digit) ;                // Call function to displays the number
        number = number / 10;                           // Devided by ten to show the next digit
      }
    }
  }
}

// Displays given digit on a 7-segment display at the given digit location
void showDigit(int digit, int location){                    // Function given name
  digitalWrite( digitPins[location], HIGH);                 // Set all pins to high
  for (int segment = 0; segment < 8; segment++)             // Loop to pass all the segment
  {
    boolean isBitSet = bitRead(numeral[digit], segment);    // Varible that reads a bit of a number.
    isBitSet = ! isBitSet;                                  // Common anode
    digitalWrite(segmentPins[segment], isBitSet);           // Output the bits as order 
  }
  delay(100);                                               //Displays digit for 100 milliseconds.
  digitalWrite( digitPins[location], LOW );                 // Set all pins as low, turn off the desplay   
}

/* collect password*/
void collectPassword(){
  while (count < 4 ){                       // limit the running, as the number of the display digit
    charOut = findKey();                    // Call the function to detact keys
    if (charOut != '$'){                    // Condition to check if we got a key
      delay(300);                           // Delay for a writh input
      display_num += charOut ;              // Add the numbers togther
      display_num.trim()  ;                 // Remove ecxes spaces
      count++;                              //count our runer interaction
     }
     } 
  Serial.println(display_num);              // User interface
}

/*low leds*/
void lowLeds (){                            // Function that close all the leds
  digitalWrite(openLed , LOW);              // Close the white led
  digitalWrite(greenLed , LOW);             // Close the green led
  digitalWrite(redLed , LOW);               // Close the red led  
}

/*servo */
void openServo (){                          // Function to open the servo 'gate'
  Myservo.writeMicroseconds(700);           // servo motor in position 0°
  delay(1000);                              // delay for the servo ection to end
}

void closeServo (){                         // Function to close the servo 'gate'
  Myservo.writeMicroseconds(2300);          // servo motor in position 180°
  delay(1000);                              // delay for the servo ection to end
}

/* ultra sony*/
int ultraSony() {                           // Function to operate the Ultra Sony sensor
  digitalWrite(trigPin, LOW);               // Clears the trigPin condition
  delayMicroseconds(2);                     //delay for the pulse to recive

  digitalWrite(trigPin, HIGH);              // Sets the trigPin HIGH
  delayMicroseconds(10);                    //Active for 10 microseconds
  digitalWrite(trigPin, LOW);               // Sets the trigPin off
  
  Duration = pulseIn(echoPin, HIGH);        // Reads the echoPin, returns the sound wave travel time in microseconds
  
  // Calculating the distance
  distance =(float) Duration /58;           // Speed of sound wave
  return distance;                          // Return the measure distance 
}