#include <SoftwareSerial.h>

#define LED_PIN 13

#define RED_BUT 9 
#define YEL_BUT 8
#define GRN_BUT 7

// comment this line, if you want functionality of having the LED on during a button press. 
#define FALLINGREAD

//having an array of 3 values helps with debouncing
//'memory aspect'
bool rbutt[3] = {false, false, false};
bool ybutt[3] = {false, false, false}; 
bool gbutt[3] = {false, false, false};

// maximum limit for buffers. 
#define BUFFER_SIZE 64

//buf is the buffer that we build on the arduino side
char buf[BUFFER_SIZE];

//correct is the buffer that we compare it to
char correct[BUFFER_SIZE];

SoftwareSerial mySerial(3,2); // RX, TX. 
//channel 3 is receiving
//channel 2 is transferring

//this is the current number of buttons saved on the arduino
uint8_t cur_buf_len = 0;

//this is the current turn
//turn + 1 is going to be the wanted length of the buffer
uint8_t turn = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);

  pinMode(RED_BUT, INPUT); 
  pinMode(YEL_BUT, INPUT);
  pinMode(GRN_BUT, INPUT);

  Serial.begin(9600);
  //serial.begin is for Tx & Rx
  //useless in terms of transmission, 
  //but allows the values to be seen on screen. 
  mySerial.begin(9600);
  //mySerial.begin begins the serial for channel 3 and channel 2
  //Serial1.begin(9600);
  delay(1000);
  digitalWrite(13, HIGH); // to make sure it's working. 
}

void updatevalues(int button, bool arr[3],  char letter){
  //constantly keeps the last three read button values
  arr[0] = arr[1];
  arr[1] = arr[2];
  arr[2] = (bool) digitalRead(button);
  
  //basically if it detects that it was low, and then becomes high
  //transmission is successfull
  if ( !arr[0] && arr[1] && arr[2] ) {
    buf[cur_buf_len] = letter; //adds the letter to the array
    cur_buf_len++; //re-sets end of buffer. 
    Serial.write(letter);
    mySerial.write(letter);
  }
  
#ifdef FALLINGREAD //used this for checkpoint 2. 
                   // allows for buttons to be turned on 
                   // during button press.  
  if (arr[0] && !arr[1] && !arr[2]){
    Serial.write(letter);
    mySerial.write(letter);
  }
#endif
  
}

// returns 0 if the buttons pressed differ from the received buf.
// else returns 1. 
int compare()
{
  for (int i=0; i <= turn; i++)
  {
    if (correct[i] != buf[i])
      return 0;
  }
  return 1;
}

void loop() {
  // update the button arrays with the current button status.
  updatevalues(RED_BUT, rbutt, 'R');
  updatevalues(YEL_BUT, ybutt, 'Y');
  updatevalues(GRN_BUT, gbutt, 'G');

  if (turn + 1 <= cur_buf_len && mySerial.available()>0)
  {
    //basically, on 0th turn, when buffer reaches a size of 1
    //and when there is a buffer available to compare
    //begin comparison
    mySerial.readBytes(correct, BUFFER_SIZE);
    //reads bytes to the correct buffer
    int compresult=compare();
    //compares the correct buffer with the inputter buffer [buf]
    mySerial.write(compresult); //send back the result of comparison
    cur_buf_len=0; //reset the buffer length 
    //technically resets it, since we deem current values as 
    // "garbage" to be overriden. 
    if (compresult) // we correclty input
      turn++; // move to next turn. 
    else // we failed to input. 
      turn=0; //start the game over
  }
  delay(25); // "clock period" for button presses. 
}
