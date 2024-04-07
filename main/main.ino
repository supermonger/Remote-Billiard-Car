#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>

PS2X ps2x; // create PS2 Controller Class
Servo MyServo;

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;
const int I1 = 2;
const int I2 = 4;
const int I3 = 6;
const int I4 = 7;
const int ENA = 3;
const int ENB = 5;
int Lx,Ly,Rx,Ry;
int P = 180;
int t=1;
double G = 0.8;
double B = 0.77;
boolean run = false;

void setup(){
Serial.begin(57600);
MyServo.attach(9);
  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);
  pinMode(I3, OUTPUT);
  pinMode(I4, OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
//CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

if(error == 0)
{
  Serial.println("Found Controller, configured successful");
  Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to [url]www.billporter.info[/url] for updates and to report bugs.");
}
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit [url]www.billporter.info[/url] for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit [url]www.billporter.info[/url] for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   //Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType();
     switch(type) 
     {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
        Serial.println("GuitarHero Controller Found");
       break;
     }
  
}

void loop(){
   /* You must Read Gamepad to get new values
   Read GamePad and set vibration values
   ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
   if you don't enable the rumble, use ps2x.read_gamepad(); with no values
   
   you should call this at least once a second
   */
   
   if(error == 1) //skip loop if no controller found
   return;
   
   Lx = ps2x.Analog(PSS_LX);
   Ly = ps2x.Analog(PSS_LY);
   Lx = map(Lx,0,255,-160,160);
   Ly = map(Ly,0,255,160,-160);
   Rx = ps2x.Analog(PSS_RX);
   Ry = ps2x.Analog(PSS_RY);
   Rx = map(Rx,0,255,-105,105);
   Ry = map(Ry,0,255,105,-105);
   if(ps2x.Button(PSB_L2))
   {
     t=2;
   }
   else
   {
     t=1;
   }
    if(t==1)
    {
      if(Ly>=40)
   {
     if(Lx>=40)
     {
        Lmove(Ly-Lx);
        Rmove(G*(Ly+Lx)/2);
     }
     else if(Lx<=-40)
     {
        Lmove((Ly-Lx)/2);
        Rmove(G*(Ly+Lx));
     }
     else
     {
        Lmove(Ly);
        Rmove(G*Ly);
     }     
   }
   else if(Ly<=-40)
   {
     if(Lx>=40)
     {
        Lmove((Ly+Lx)/2);
        Rmove(B*(Ly-Lx));
     }
     else if(Lx<=-40)
     {   
        Lmove(Ly+Lx);
        Rmove(B*(Ly-Lx)/2);
     }
     else
     {
        Lmove(Ly);
        Rmove(B*Ly);
     }
   }
   else
   {
     if(Lx>=40)
     {
        Lmove(-Lx);
        Rmove(G*Lx);
     }
     else if(Lx<=-40)
     {
        Lmove(-Lx);
        Rmove(G*Lx);
     }
     else
     {
        mstop();
     }
   }
    }
    else
    {
        if(Ry>=10)
     {
     if(Rx>=10)
     {
        Lmove(Ry-Rx);
        Rmove(G*(Ry+Rx)/2);
     }
     else if(Rx<=-10)
     {
        Lmove((Ry-Rx)/2);
        Rmove(G*(Ry+Rx));
     }
     else
     {
        Lmove(Ry);
        Rmove(G*Ry);
     }     
   }
   else if(Ry<=-10)
   {
     if(Rx>=10)
     {
        Lmove((Ry-Rx)/2);
        Rmove(B*(Ry+Rx));
     }
     else if(Rx<=-10)
     {
        Lmove(Ry-Rx);
        Rmove(B*(Ry+Rx)/2);
     }
     else
     {
        Lmove(Ry);
        Rmove(B*Ry);
     }
   }
   else
   {
     if(Rx>=10)
     {
        Lmove(-Rx);
        Rmove(G*Rx);
     }
     else if(Rx<=-10)
     {
        Lmove(-Rx);
        Rmove(G*Rx);
     }
     else
     {
        mstop();
     }
   }
    }
    
   
      
   
   
   
   if(ps2x.Button(PSB_GREEN))
   {
     P =0;
   }
   else if(ps2x.Button(PSB_RED))
   {
    P = 360;
   }
   else
   {
    P = 90;
   }
   MyServo.write(P);
   
  if(ps2x.Button(PSB_BLUE))
  {
    mstop();
  }
  /*if(ps2x.ButtonPressed(PSB_RED))
  {
    mfront();
  }
  if(ps2x.ButtonPressed(PSB_PINK))
  {
    mback();
  }*/
  
  if(type == 2){ //Guitar Hero Controller
   
   ps2x.read_gamepad();          //read controller
   
   if(ps2x.ButtonPressed(GREEN_FRET))
     Serial.println("Green Fret Pressed");
   if(ps2x.ButtonPressed(RED_FRET))
     Serial.println("Red Fret Pressed");
   if(ps2x.ButtonPressed(YELLOW_FRET))
     Serial.println("Yellow Fret Pressed");
   if(ps2x.ButtonPressed(BLUE_FRET))
     Serial.println("Blue Fret Pressed");
   if(ps2x.ButtonPressed(ORANGE_FRET))
     Serial.println("Orange Fret Pressed");
     

    if(ps2x.ButtonPressed(STAR_POWER))
     Serial.println("Star Power Command");
   
    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
     Serial.println("Up Strum");
    if(ps2x.Button(DOWN_STRUM))
     Serial.println("DOWN Strum");
  

    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");

   
    if(ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE
    {
        Serial.print("Wammy Bar Position:");
        Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
    }
}

else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
   
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");
         
         
     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       Serial.print("Up held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      }
      if(ps2x.Button(PSB_PAD_RIGHT)){
       Serial.print("Right held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
       Serial.print("LEFT held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
       Serial.print("DOWN held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      }   
  
   
      vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on
                                              //how hard you press the blue (X) button   
   
    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {
     
      
         
        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2))
         Serial.println("R2 pressed");
        if(ps2x.Button(PSB_GREEN))
         Serial.println("Triangle pressed");
         
    }   
         
   
    if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
         Serial.println("Circle just pressed");
         
    if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
         Serial.println("Square just released");     
   
    if(ps2x.NewButtonState(PSB_BLUE))
    {            //will be TRUE if button was JUST pressed OR released
         Serial.println("X just changed");
         
    }
   
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC);
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC);
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
   
   
}


delay(50);
     
}

void mstop()
{
  digitalWrite(I1, LOW);
  digitalWrite(I2, LOW);
  digitalWrite(I3, LOW);
  digitalWrite(I4, LOW);
  run = false;
}
void Rmove(int input)
{
  if(input>10)
  {
    analogWrite(ENA,input);
    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
  }
  else if(input<-10)
  {
    analogWrite(ENA,-input);
    digitalWrite(I1, LOW);
    digitalWrite(I2, HIGH);
  }
  else
  {
    digitalWrite(I1, LOW);
    digitalWrite(I2, LOW);
  }
}
void Lmove(int input)
{
  if(input>10)
  {
    analogWrite(ENB,input);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);
  }
  else if(input<-10)
  {
    analogWrite(ENB,-input);
    digitalWrite(I3, LOW);
    digitalWrite(I4, HIGH);
  }
  else
  {
    digitalWrite(I3, LOW);
    digitalWrite(I4, LOW);
  }
}
/*void Rmfront(int input)
{
  analogWrite(ENA,input);
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  run = true;
}
void Rmback(int input)
{
  analogWrite(ENA,-input);
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  run = true;
}
void Lmfront(int input)
{
  analogWrite(ENB,input);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);
  run = true;
}
void Lmback(int input)
{
  analogWrite(ENB,-input);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);
  run = true;
}*/
