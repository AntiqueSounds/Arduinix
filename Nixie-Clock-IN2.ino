// 
// Arduinix 4 tube setup pseudo-clock works with IN-12 or IN-2 header 
// v1.0
// Use physical pin A0 and A1 as hour and minute set switches. Each short to ground increments the hour or minutes setting by one.  
// 
// This code runs a 4 bulb tube board setup and displays a prototype clock setup.
// NOTE: the delay is setup for IN-17 nixie bulbs.
//
// Original code by Jeremy Howa 
// Significant mods by M. Keith Moore for IN-2 and IN-12 4 digit and blinking colon
// www.robotpirate.com
// www.arduinix.com
// www.glowtubeglow.com
// 2009/2016(MKM)
//
// Note: Anod pin 3 and 2 are used for colons
// 
// Anod to number diagram
//
//
//          num array position
//            0   1   2   3
// Anod 0     #           #
// Anod 1         #   #
//
// Anod 2  Array #2 Colon=0
// Anod 3  Array #1 Colon=0

// SN74141 : True Table
//D C B A #
//L,L,L,L 0
//L,L,L,H 1
//L,L,H,L 2
//L,L,H,H 3
//L,H,L,L 4
//L,H,L,H 5
//L,H,H,L 6
//L,H,H,H 7
//H,L,L,L 8
//H,L,L,H 9

// SN74141 (1)
int ledPin_0_a = 2;                
int ledPin_0_b = 3;
int ledPin_0_c = 4;
int ledPin_0_d = 5;
// SN74141 (2)
int ledPin_1_a = 6;                
int ledPin_1_b = 7;
int ledPin_1_c = 8;
int ledPin_1_d = 9;

// anod pins
int ledPin_a_1 = 10;
int ledPin_a_2 = 11;
int ledPin_a_3 = 12;
int ledPin_a_4 = 13;


void setup() 
{
  pinMode(ledPin_0_a, OUTPUT);      
  pinMode(ledPin_0_b, OUTPUT);      
  pinMode(ledPin_0_c, OUTPUT);      
  pinMode(ledPin_0_d, OUTPUT);    
  
  pinMode(ledPin_1_a, OUTPUT);      
  pinMode(ledPin_1_b, OUTPUT);      
  pinMode(ledPin_1_c, OUTPUT);      
  pinMode(ledPin_1_d, OUTPUT);      
  
  pinMode(ledPin_a_1, OUTPUT);      
  pinMode(ledPin_a_2, OUTPUT);      
  pinMode(ledPin_a_3, OUTPUT);
  pinMode(ledPin_a_4, OUTPUT);     
  
  // NOTE:
  // Grounding on pins 14 and 15 will set the Hour and Mins.
  pinMode( 14, INPUT ); // set the virtual pin 14 (pin 0 on the analog inputs ) 
  digitalWrite(14, HIGH); // set pin 14 as a pull up resistor.

  pinMode( 15, INPUT ); // set the virtual pin 15 (pin 1 on the analog inputs ) 
  digitalWrite(15, HIGH); // set pin 15 as a pull up resistor.

  Serial.begin(9600);
  
}

////////////////////////////////////////////////////////////////////////
//
// DisplayNumberSet
// Use: Passing anod number, and number for bulb 1 and bulb 2, this function
//      looks up the truth table and opens the correct outs from the arduino
//      to light the numbers given to this funciton (num1,num2).
//      On a 4 nixie bulb setup.
//
////////////////////////////////////////////////////////////////////////
void DisplayNumberSet( int anod, int num1, int num2 )
{
  int anodPin;
  int a,b,c,d;
  
  // set defaults.
  a=0;b=0;c=0;d=0; // will display a zero.
  anodPin =  ledPin_a_1;     // default on first anod.
  
  // Select what anod to fire.
  switch( anod )
  {
    case 0:    anodPin =  ledPin_a_1;    break;
    case 1:    anodPin =  ledPin_a_2;    break;
    case 2:    anodPin =  ledPin_a_3;    break;
    case 3:    anodPin =  ledPin_a_4;    break;
  }  
  
  // Load the a,b,c,d.. to send to the SN74141 IC (1)
  switch( num1 )
  {
    case 0: a=0;b=0;c=0;d=0;break;
    case 1: a=1;b=0;c=0;d=0;break;
    case 2: a=0;b=1;c=0;d=0;break;
    case 3: a=1;b=1;c=0;d=0;break;
    case 4: a=0;b=0;c=1;d=0;break;
    case 5: a=1;b=0;c=1;d=0;break;
    case 6: a=0;b=1;c=1;d=0;break;
    case 7: a=1;b=1;c=1;d=0;break;
    case 8: a=0;b=0;c=0;d=1;break;
    case 9: a=1;b=0;c=0;d=1;break;
    default: break;  // used to no-op the number in the array
  }  
  
  // Write to output pins.
  digitalWrite(ledPin_0_d, d);
  digitalWrite(ledPin_0_c, c);
  digitalWrite(ledPin_0_b, b);
  digitalWrite(ledPin_0_a, a);

  // Load the a,b,c,d.. to send to the SN74141 IC (2)
  switch( num2 )
  {
    case 0: a=0;b=0;c=0;d=0;break;
    case 1: a=1;b=0;c=0;d=0;break;
    case 2: a=0;b=1;c=0;d=0;break;
    case 3: a=1;b=1;c=0;d=0;break;
    case 4: a=0;b=0;c=1;d=0;break;
    case 5: a=1;b=0;c=1;d=0;break;
    case 6: a=0;b=1;c=1;d=0;break;
    case 7: a=1;b=1;c=1;d=0;break;
    case 8: a=0;b=0;c=0;d=1;break;
    case 9: a=1;b=0;c=0;d=1;break;
    default: break;
  }
  
  // Write to output pins
  digitalWrite(ledPin_1_d, d);
  digitalWrite(ledPin_1_c, c);
  digitalWrite(ledPin_1_b, b);
  digitalWrite(ledPin_1_a, a);

  // Turn on this anod.
  digitalWrite(anodPin, HIGH);   

  // Delay
  // NOTE: With the differnce in Nixie bulbs you may have to change
  //       this delay to set the update speed of the bulbs. If you 
  //       dont wait long enough the bulb will be dim or not light at all
  //       you want to set this delay just right so that you have 
  //       nice bright output yet quick enough so that you can multiplex with
  //       more bulbs.
  delay(3);
  
  // Shut off this anod.
  digitalWrite(anodPin, LOW);
}


////////////////////////////////////////////////////////////////////////
//
// DisplayNumberString
// Use: passing an array that is 8 elements long will display numbers
//      on a 6 nixie bulb setup.
//
////////////////////////////////////////////////////////////////////////
void DisplayNumberString( int* array )
{
  // bank 1 (bulb 0,3)
  DisplayNumberSet(0,array[0],array[3]);   
  // bank 2 (bulb 1,2)
  DisplayNumberSet(1,array[1],array[2]);
  if (array[4]<1) DisplayNumberSet(3,array[4],11);  // 11 is a dummy value to ignore in the code (fall-thru case/switch value)
  if (array[5]<1) DisplayNumberSet(2,11,array[5]); // this colon is turned on if 0  

}

// Defines
long MINS = 60;         // 60 Seconds in a Min.
long HOURS = 60 * MINS; // 60 Mins in an hour.
long DAYS = 24 * HOURS; // 24 Hours in a day. > Note: change the 24 to a 12 for non military time.

long runTime = 0;       // Time from when we started.
long previous = 0;
long interval = 1000;
int colon = 1;

// default time sets. clock will start at 12:59:00
// NOTE: We start seconds at 0 so we dont need a clock set
//       The values you see here would be what you change 
//       if you added a set clock inputs to the board.
long clockHourSet = 12;
long clockMinSet  = 59;

int HourButtonPressed = false;
int MinButtonPressed = false;

////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void loop()     
{
   // Get milliseconds.
  runTime = millis();

  int hourInput = digitalRead(14);  
  int minInput  = digitalRead(15);


  if( hourInput == 0 )
    HourButtonPressed = true;
    Serial.println("button down");
  if( minInput == 0 )
    MinButtonPressed = true;
  
  if( HourButtonPressed == true && hourInput == 1 )
  {
    clockHourSet++;
    HourButtonPressed = false;
  }
  
  if( MinButtonPressed == true && minInput == 1 )
  {
    clockMinSet++;
    MinButtonPressed = false;
  }

  // Get time in seconds.
  long time = (runTime) / 1000;
  
  // Set time based on offset..
  long hbump = 60*60*clockHourSet;
  long mbump = 60*clockMinSet;
  time += mbump + hbump;

  // Convert time to days,hours,mins,seconds
  long days  = time / DAYS;    time -= days  * DAYS; 
  long hours = time / HOURS;   time -= hours * HOURS; 
  long minutes  = time / MINS;    time -= minutes  * MINS; 
  long seconds  = time; 

  //*****************************
  unsigned long current = millis();
  if( (current - previous) > interval)
  {
    previous = current;
    if (colon == 0) 
      {
        colon = 1;
 //       Serial.println("Colon off");
      } 
    else 
      {
        colon = 0;
 //       Serial.println("Colon on");
      }
  }
  if( hours > 12 )
      time-= 60*60*12;

  // Get the high and low order values for hours,min,seconds. 
  int lowerHours = hours % 10;
  int upperHours = hours - lowerHours;
  int lowerMins = minutes % 10;
  int upperMins = minutes - lowerMins;
  int lowerSeconds = seconds % 10;
  int upperSeconds = seconds - lowerSeconds;
  if( upperSeconds >= 10 )   upperSeconds = upperSeconds / 10;
  if( upperMins >= 10 )      upperMins = upperMins / 10;
  if( upperHours >= 10 )     upperHours = upperHours / 10;

  // Fill in the Number array used to display on the tubes.
  int NumberArray[6]={0,0,0,0,0,0};
  NumberArray[0] = upperHours;
  NumberArray[1] = lowerHours;
  NumberArray[2] = upperMins;
  NumberArray[3] = lowerMins;
  NumberArray[4] = colon;
  NumberArray[5] = colon;

  // Display.
  DisplayNumberString( NumberArray );
}


