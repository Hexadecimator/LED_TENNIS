//CONSTANTS

/*******************************************************
Lesson learned: using TX and RX digital IO pins 0 and 1
is a VERY bad idea (if you want to break the ATMEGA chip
off the protoboard into its own board). We're going to 
shift the LED output Pins from 0-7 to 2-9. In order to
keep all the code we've already written valid, we'll
use the "count" variable to index this array myPins[]
which will control the output LEDs now
*******************************************************/
int myPins[] = {8, 9, 2, 3, 4, 5, 6, 7};

const int BTN_L = 10;
const int BTN_R = 11;


const unsigned long interval = 50;

int count = 0;
int LED_STATE = LOW;

int l_btn_prev_state = HIGH;
int r_btn_prev_state = HIGH;
int l_btn_state = HIGH; // LOW = PRESSED
int r_btn_state = HIGH; // LOW = PRESSED

bool r_btn_pressed = false;
bool l_btn_pressed = false;

bool goingRight = true;
bool canVolleyLeft = false;
bool canVolleyRight = false;
bool gameOver = false;
bool nextLED = false;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long l_btn_debounce_millis = 0;
unsigned long r_btn_debounce_millis = 0;
unsigned long debounceDelay = 25;

void setup() 
{
  // set output LEDs to OUTPUT
  for(int i = 0; i <= 7; i++)
  {
    pinMode(myPins[i], OUTPUT);
  }
  
  // set player "paddles" (buttons) to INPUT
  pinMode(BTN_L, INPUT);
  pinMode(BTN_R, INPUT);

  // this is a test because WHY DO THE TX AND RX PINS BEHAVE THIS WAY WHYYYYYYYY
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  
  //enable pullup resistors
  digitalWrite(BTN_L, HIGH); 
  digitalWrite(BTN_R, HIGH); 
 
  // start the LED "bouncing" animation by turning on the first LED in the sequence
  digitalWrite(myPins[0], HIGH);
  
  count = 0;

  previousMillis = millis();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  l_btn_state = digitalRead(BTN_L); // this button is PULLUP, so LOW = PRESSED
  r_btn_state = digitalRead(BTN_R); // this button is PULLUP, so LOW = PRESSED

  if ((r_btn_state != r_btn_prev_state) && r_btn_pressed == false)
  {
    r_btn_pressed = true;
    r_btn_debounce_millis = millis();
  }

  if (r_btn_pressed == true && (millis() - r_btn_debounce_millis > debounceDelay))
  {
    r_btn_pressed = false;
  }

  if ((l_btn_state != l_btn_prev_state) && l_btn_pressed == false)
  {
    l_btn_pressed = true;
    l_btn_debounce_millis = millis();
  }

  if (l_btn_pressed == true && (millis() - l_btn_debounce_millis > debounceDelay))
  {
    l_btn_pressed = false;
  }
  
  currentMillis = millis();
  
  if ((currentMillis - previousMillis >= interval) && !gameOver) // check if "interval" ms has passed or the game has ended
  {
    previousMillis = currentMillis;

    if (nextLED == false)
    {
      if (count >=0 && count <= 7) //make sure we don't out-of-bounds myPins[]
      {
        digitalWrite(myPins[count], HIGH);
      }
      nextLED = true;
    } 
    else if (nextLED == true)
    {  
      if (count >=0 && count <= 7) //make sure we don't out-of-bounds myPins[]
      { 
        digitalWrite(myPins[count], LOW);
      }
      
      if (goingRight == true)
      {   
        if (count >= 7 && count < 8)
        {
          canVolleyRight = true;
        } else if (count >= 8) //right player missed the volley timing, game over
        {
          canVolleyRight = false;
          gameOver = true;
        }
        count++;
      }
      else if (goingRight == false)
      {
        if (count <= 0 && count > -1)
        {
          canVolleyLeft = true;
        } else if (count <= -1) //left player missed the volley timing, game over
        {
          canVolleyLeft = false;
          gameOver = true;
        }
        count--;
      }     
      nextLED = false;
    } 
  }

  if((canVolleyLeft == true) && (l_btn_pressed == true))
  {
    canVolleyLeft = false;
    canVolleyRight = false;
    goingRight = true; //left button successfully pressed, reverse order of LEDs
  } else if ((canVolleyRight == true) && (r_btn_pressed == true))
  {
    canVolleyLeft = false;
    canVolleyRight = false;
    goingRight = false; //right button successfully pressed, reverse order of LEDs
  }

  if (gameOver == true)
  {
    //to indicate game over flash all the LEDs on/off every 750ms forever (until reset button is pressed)
    for(int i = 0; i <= 7; i++)
    {
      digitalWrite(myPins[i], LED_STATE);
    }

    if(LED_STATE == LOW)
    {
      LED_STATE = HIGH;
    } else if (LED_STATE == HIGH)
    {
      LED_STATE = LOW;
    }
    delay(750);
  }
  l_btn_prev_state = l_btn_state;
  r_btn_prev_state = r_btn_state;
}
