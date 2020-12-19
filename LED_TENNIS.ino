




//CONSTANTS
const int LED_0 = 0;
const int LED_1 = 1;
const int LED_2 = 2;
const int LED_3 = 3;
const int LED_4 = 4;
const int LED_5 = 5;
const int LED_6 = 6;
const int LED_7 = 7;

const int BTN_L = 10;
const int BTN_R = 11;

// may not need both of these timing variables

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
  // put your setup code here, to run once:
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
  pinMode(LED_7, OUTPUT);

  pinMode(BTN_L, INPUT);
  pinMode(BTN_R, INPUT);
  digitalWrite(BTN_L, HIGH); //enable pullup resistor
  digitalWrite(BTN_R, HIGH); //enable pullup resistor

  digitalWrite(LED_0, HIGH);
  
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
  
  if ((currentMillis - previousMillis >= interval) && !gameOver) // check if 250ms has passed or the game has ended
  {
    previousMillis = currentMillis;

    if (nextLED == false)
    {
      digitalWrite(count, HIGH);
      nextLED = true;
    } 
    else if (nextLED == true)
    {  
      digitalWrite(count, LOW);
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
    goingRight = true;
  } else if ((canVolleyRight == true) && (r_btn_pressed == true))
  {
    canVolleyLeft = false;
    canVolleyRight = false;
    goingRight = false;
  }

  if (gameOver == true)
  {
    //to indicate game over flash all the LEDs on/off every half second forever
    digitalWrite(LED_0, LED_STATE);
    digitalWrite(LED_1, LED_STATE);
    digitalWrite(LED_2, LED_STATE);
    digitalWrite(LED_3, LED_STATE);
    digitalWrite(LED_4, LED_STATE);
    digitalWrite(LED_5, LED_STATE);
    digitalWrite(LED_6, LED_STATE);
    digitalWrite(LED_7, LED_STATE);

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
