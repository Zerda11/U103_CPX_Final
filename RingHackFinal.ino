#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>


/*
 Created by Theodore Sandoz
 Creation date 6-12-2023
 Updated 6-13-2023

 Inspired by hacking minigame from GTFO credit: 10Chambers


<Requires ADAFRUIT CIRCUIT PLAYGROUND EXPRESS>


IMPORTANT  ---->  INSTRUCTIONS BELOW
:
: Use both buttons to change the difficulty shown by a ring of lights when you are ready hold down both buttons
:
: A tone will play signaling change and the game will start
:
: Press either of the buttons while the moving light is over the stationary light
:
: If successful a high pitch will play
: If unsuccessful a low pitch will play
:
: Use the same dual button press to go back to the difficulty select and try a new difficulty!
:
*/

//==========

//These are all menu controls and are used to navigate between modes
bool Menu = 1;
bool Play = 0;
bool Countdown = 0;
bool Game = 1;
//==========

bool Direction = 0;

int TargetPixel;
int Difficulty = 9;
int CurrentPixel = 0;
int Landed;
int Score = 0;

const byte LB = 4;
const byte RB = 5;

volatile int LBP = 0;
volatile int RBP = 0;

void LBDown()//What happens any time the left button is pressed
{
  LBP++;
}

void RBDown()//What happens any time the right button is pressed
{
  RBP++;
}




void setup() {

  CircuitPlayground.begin();
  Serial.begin(9600);

//interrupt declrations and interrupts
  pinMode(LB, INPUT_PULLDOWN);
  pinMode(RB, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(LB),LBDown,RISING);
  attachInterrupt(digitalPinToInterrupt(RB),RBDown,RISING);

}

void loop() {
//===================================================================================
//Here is where the program starts and the place where you can change difficulty
  if(Menu == 1)
  {
    CircuitPlayground.clearPixels();
    for(int i=0; i<Difficulty; i++)//displays the difficulty as an increasing ring of lights and colors
    {
      CircuitPlayground.setPixelColor(i,(28.3*i),0,(28.3*(9-i)));
    }

    if(LBP == 1)//if the Left button interrupt is enabled it reduces difficulyu by 1
    {
      if(Difficulty > 0)
      {
        Difficulty--; //reducing difficulty
        Serial.println(Difficulty); //printing to serial for visual confirmation       
        delay(200); //debounce  (This could be reduced for more functional boards my buttons are incredibly bouncy)
        LBP = 0; //reset the left button press function
      }
    }
        if(RBP == 1)
    {
      if(Difficulty < 10)
      {
        Difficulty++; //increasing difficulty
        Serial.println(Difficulty); //printing to serial for visual confirmation   
        delay(200); //debounce  (This could be reduced for more functional boards my buttons are incredibly bouncy)
        RBP = 0; //reset the right button press function
      }
    }

    if(CircuitPlayground.leftButton() == 1 && CircuitPlayground.rightButton() == 1) //because we are not concerned about accurate timing of the button press here
    //This statement uses the CircuitPlayground button functions because this is where you press and hold the buttons to change the mode from difficulty select to playing
    {
      Serial.println("Mamasita"); //Visual confirmation
      delay(500); //delay to show intent to change
      if(CircuitPlayground.leftButton() == 1 && CircuitPlayground.rightButton() == 1) //if both buttons still presed after half a second then change modes
      {
        CircuitPlayground.clearPixels();
        Menu = 0; //stop menu
        Countdown = 1; //countdown into game
        Play = 1;
      }
    }
    LBP = 0;
    RBP = 0;
    delay(10);
  
  }

//===================================================================================

//Here in lies the game itself in all its simplicity

  if(Play == 1)
  {
    
    if(Countdown == 1)
    {

      //=======================================================
      //countdown sequence when you initially switch to play the game.
      //when this is called it will play a little jingle before continuing to play the game
      CircuitPlayground.clearPixels();
      TargetPixel = ((random(0,9)+random(0,9))/2);
      CircuitPlayground.setPixelColor(TargetPixel,255,255,255);
      delay(1000);
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(TargetPixel,255,50,50);
      CircuitPlayground.playTone(200,200);
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(TargetPixel,255,50,50);
      CircuitPlayground.playTone(200,200);
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(TargetPixel,255,50,50);
      CircuitPlayground.playTone(200,200);
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(TargetPixel,50,255,50);
      CircuitPlayground.playTone(400,400);
      Play = 1; //starts the game
      Countdown = 0; //stops the countdown from repeating
    }


    if(CircuitPlayground.leftButton() == 1 && CircuitPlayground.rightButton() == 1)//here is the menu change similar to that of the lines above
    {
      Serial.println("Mamasita"); //Visual confirmation
      delay(500); //delay to show intent to change
      if(CircuitPlayground.leftButton() == 1 && CircuitPlayground.rightButton() == 1) //if both buttons still presed after half a second then change modes
      {
        CircuitPlayground.clearPixels();
        Play = 0; //stop the game
        Menu = 1; //start menu
      }
    }
//===================================================================================

//Here is where the actual game takes place
    if(Game == 1)
    {

        if(RBP == 1 or LBP == 1)
        {
          Landed = abs(CurrentPixel - TargetPixel);//This calculates if the player landed on the highlighted pixel or not
          delay(200);
          if(Landed == 0)
          {
            CircuitPlayground.playTone(400,50);//if landed on correct pixel play a nice high tone
          }

          if(Landed > 0)
          {
            CircuitPlayground.playTone(100,50);//if landed on an incorrect pixel play a low tone
          }
          CircuitPlayground.clearPixels();//reset pixels
          TargetPixel = ((random(0,9)+random(0,9))/2);//Reset the target pixel to a psudeo random pixel
          CircuitPlayground.setPixelColor(TargetPixel,255,255,255);//light up the new pixel in white light
          delay(500);//wait then continue the game
          RBP = 0;
          LBP = 0;      
        }


      if(Direction == 0 && CurrentPixel < 10)//spins the pixels around in 1 direction
      {
        if(CurrentPixel == TargetPixel)//this makes sure that the tracer pixel does not cover up the target pixel if it passes by
        {
          CircuitPlayground.setPixelColor(CurrentPixel,255,255,255);
          CurrentPixel++;
          delay(200-(Difficulty*15));//This changes the delay between when the pixels move depending on the difficulty
        }
        else
        {
         CircuitPlayground.setPixelColor(CurrentPixel,(25.5*(Difficulty)),0,(25.5*(10-Difficulty))); //This lights up the current pixel with the difficulty selected in the menu
         CurrentPixel++;
         delay(200-(Difficulty*15));//This changes the delay between when the pixels move depending on the difficulty
        }
        
      }

      if(Direction == 0 && CurrentPixel == 10) //if the program is lighting up the last pixel in a given direction the program will reverse direction here
      {
        Direction = 1;
      }


      if(Direction == 1 && CurrentPixel > 0)//spins the pixels around in the other direction
      {
        if(CurrentPixel == TargetPixel)//this makes sure that the tracer pixel does not cover up the target pixel if it passes by
        {
          CircuitPlayground.setPixelColor(CurrentPixel,255,255,255);
          CurrentPixel--;
          delay(200-(Difficulty*15));//This changes the delay between when the pixels move depending on the difficulty
        }
        else
        {
         CircuitPlayground.setPixelColor(CurrentPixel,(28.3*(Difficulty)),0,(28.3*(9-Difficulty))); //This lights up the current pixel with the difficulty selected in the menu
         CurrentPixel--;
         delay(200-(Difficulty*15));//This changes the delay between when the pixels move depending on the difficulty
        }
        
      }

      if(Direction == 1 && CurrentPixel == 0)//if the program is lighting up the last pixel in a given direction the program will reverse direction here
      {
        Direction = 0;
      }


    }
  
  
  
    CircuitPlayground.clearPixels();//every cycle the pixels reset so that the ones that would remain on get kicked off
    CircuitPlayground.setPixelColor(TargetPixel,255,255,255);//re  hilights the target pixel as a backup
  }


}
