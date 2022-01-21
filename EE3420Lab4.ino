/* Author: Ringtarih Konyu Tamfu
 * 
 * Microprocessor Laboratory 3: Raspberry Pi 4 Code
 * 
 * Spring 2021 - EE 3420 - 001
 * 
 * Instructor: William Stapleton
 * 
 * 
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////
//ALL LIBRARIES HERE
///////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ee3420_mega.h"
#include <LiquidCrystal.h>
#include <Keypad.h>

//Keypad initializtion
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
  };
byte rowPins[ROWS] = {24, 25, 26, 27};
byte colPins[COLS] = {28, 29, 30, 31};

int buzzer = 22;

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2, 34, 35, 36, 37, 38);

int KeyPressed = 33; 

void setup() {
  //initialize the serial port that will communicate to the Raspberry Pi over USB
  Serial.begin(115200, SERIAL_8N1);

  //initializing lcd
  lcd.begin(16, 2);
  //lcd.print("LCD OK");

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, 0);
  
  pinMode(KeyPressed,OUTPUT);
  digitalWrite(KeyPressed, 0);
  
  //wait for the ENQ/ACK handshake to be initialized by the Raspberry Pi
  //function should be called in setup()
  //function defined in ee3420_mega.h
  serial_wait_for_enquire();

  lcd.setCursor(0, 0);

  //initially clear the serial command receive buffer in case there was random data in the variables on startup
  //this should always be done before entering loop()
  clear_serial_receive_buffer();
}

void loop() {
  // put your main code here, to run repeatedly:

  //Keypad Mega Portion (Addition)
  
  
  /* serial_build_command will build a command string one character at a time*/
  /* It will gether characters until a full command line is built */
  /* The function is non-blocking if no characters are available */
  /* Once a complete command is available, the global variable serial_received_instruction_complete is set */
  serial_build_command();

  /*only parse a command and try to execute once a complete command is available */
  if(serial_received_instruction_complete)
  {
    /*starting a new command, make a note that it isn't complete */
    serial_command_execution_complete=0;

    //create a string called lcd_buffer and copy text from serial_receive_buffer
    //parse_command_string destroys serial_receive_buffer contents
    char lcd_buffer[40];
    strcpy(lcd_buffer,&serial_receive_buffer[9]);  //skip "LCD:TEXT:"
  
    /*break command into parameters */
    parse_command_string(serial_receive_buffer, serial_command_parameters, parameter_separators);
    /*for ease of comparision, make parameters uniformly uppercase */
    make_command_parameters_uppercase(serial_command_parameters);

    /* what follows here should be a series of checks to interpret the parsed command and execute */
    /* successfule execution of a command should always set serial_command_execution_complete */

    // beginning of ID:TYPE
    if( !strcmp(&serial_command_parameters[0][0],"ID") &&
        !strcmp(&serial_command_parameters[1][0],"TYPE") 
      )
    {
      if(!strcmp(&serial_command_parameters[2][0],"?"))
      {
        Serial.write(ACK);
        Serial.print("ID:TYPE:");           //output the first part of the response without a newline
        Serial.println(my_device_type);     //complete the response and end with a newline
        serial_command_execution_complete=1;  //indicates completed command
      }
      else
      {
        strcpy(serial_device_type,&serial_command_parameters[2][0]);
        Serial.write(ACK);
        Serial.println("OK");
        serial_command_execution_complete=1;
      }
    }
    // end of ID:TYPE    

    // beginning of ID:NAME
    if( !strcmp(&serial_command_parameters[0][0],"ID") &&
        !strcmp(&serial_command_parameters[1][0],"NAME") 
      )
    {
      if(!strcmp(&serial_command_parameters[2][0],"?"))
      {
        Serial.write(ACK);
        Serial.print("ID:NAME:");
        Serial.println(my_device_name);
        serial_command_execution_complete=1;  //indicates completed command
      }
      else
      {
        strcpy(serial_device_name,&serial_command_parameters[2][0]);
        Serial.write(ACK);
        Serial.println("OK");
        serial_command_execution_complete=1;
      }
    }
    // end of ID:NAME 
    
    // check for ID with unknown parameter
    // must appear after all other options for ID
    if( !strcmp(&serial_command_parameters[0][0],"ID") &&
        (!serial_command_execution_complete) 
      )
    {
        Serial.write(NACK);
        Serial.print("ERROR:");
        Serial.println(ERROR_INVALID_PARAMETER);  //LED can be 0 (off) or 1 (on), any other value is invalid
        serial_command_execution_complete=1;  //indicates completed command
    }
    // end of check for ID with unknown parameter
    

    // beginning of LED:BUILTIN
    if( !strcmp(&serial_command_parameters[0][0],"LED") &&
        !strcmp(&serial_command_parameters[1][0],"BUILTIN") 
      )
    {
      if(!strcmp(&serial_command_parameters[2][0],"?"))  //request LED state
      {
        Serial.write(ACK);
        Serial.print("LED:BUILTIN:");
        Serial.println(digitalRead(LED_BUILTIN));
        serial_command_execution_complete=1;  //indicates completed command
      }
      else if(!strcmp(&serial_command_parameters[2][0],"0")) //turn LED off
      {
        Serial.write(ACK);
        digitalWrite(LED_BUILTIN,0);
        Serial.println("OK");
        serial_command_execution_complete=1;  //indicates completed command
      }
      else if(!strcmp(&serial_command_parameters[2][0],"1"))  //turn LED on
      {
        Serial.write(ACK);
        digitalWrite(LED_BUILTIN,1);
        Serial.println("OK");
        serial_command_execution_complete=1;  //indicates completed command
      }
      else                                    // something else
      {
        Serial.write(NACK);
        Serial.print("ERROR:");
        Serial.println(ERROR_INVALID_DATA);  //LED can be 0 (off) or 1 (on), any other value is invalid
        serial_command_execution_complete=1;  //indicates completed command
      }
    }
    // end of LED:BUILTIN

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // LCD Portion
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // beginning of LCD:TIME
    if( !strcmp(&serial_command_parameters[0][0],"LCD") &&
        !strcmp(&serial_command_parameters[1][0],"TEXT") 
      )
    {
      lcd.print(serial_command_parameters[2]);
      Serial.write(ACK);
      Serial.println("OK");
      serial_command_execution_complete=1;
      //lcd.print(":");
    }
    // ending of LCD:TIME

    char key = myKeypad.getKey();
     
    // beginning of LCD:CURSOR
    if( !strcmp(&serial_command_parameters[0][0],"LCD") &&
        !strcmp(&serial_command_parameters[1][0],"CURSOR") 
      )
    {
      if(!strcmp(&serial_command_parameters[2][0],"NEXT")) 
      {
        lcd.setCursor(0, 1);
        Serial.write(ACK);
        Serial.println("OK");
        serial_command_execution_complete=1;
      }
      else if(!strcmp(&serial_command_parameters[2][0],"BEGIN")) 
      {
        lcd.setCursor(0, 0);
        Serial.write(ACK);
        Serial.println("OK");
        serial_command_execution_complete=1;
      }
      else if(!strcmp(&serial_command_parameters[2][0],"KEY")) 
      {
        lcd.print(key);
        Serial.write(ACK);
        Serial.println("OK");
        serial_command_execution_complete=1;
      }
      else                                    
      {
        Serial.write(NACK);
        Serial.print("ERROR:");
        Serial.println(ERROR_INVALID_DATA);  
        serial_command_execution_complete=1;  
      }
    }

    // beginning of LCD:TIMER
    if( !strcmp(&serial_command_parameters[0][0],"LCD") &&
        !strcmp(&serial_command_parameters[1][0],"TIMER") 
      )
    {
      lcd.print(serial_command_parameters[2]);
      lcd.print(":");
      Serial.write(ACK);
      Serial.println("OK");
      serial_command_execution_complete=1;
    }

    //begining of buzzer
    if( !strcmp(&serial_command_parameters[0][0],"BUZZER") &&
        !strcmp(&serial_command_parameters[1][0],"VALUE") 
      )
    {
      if(!strcmp(&serial_command_parameters[2][0],"?"))  //request LED state
      {
        Serial.write(ACK);
        Serial.print("BUZZER:VALUE:");
        Serial.println(digitalRead(buzzer));
        serial_command_execution_complete=1;  //indicates completed command
      }
      else if(!strcmp(&serial_command_parameters[2][0],"0")) //turn buzzer off
      {
        Serial.write(ACK);
        digitalWrite(buzzer,0);
        Serial.println("OK");
        serial_command_execution_complete=1;  //indicates completed command
      }
      else if(!strcmp(&serial_command_parameters[2][0],"1"))  //turn buzzer on
      {
        Serial.write(ACK);
        digitalWrite(buzzer,1);
        Serial.println("OK");
        serial_command_execution_complete=1;  //indicates completed command
      }
      else                                    // something else
      {
        Serial.write(NACK);
        Serial.print("ERROR:");
        Serial.println(ERROR_INVALID_DATA);  //buzzer can be 0 (off) or 1 (on), any other value is invalid
        serial_command_execution_complete=1;  //indicates completed command
      }
    }
    // end of buzzer
       
    // check for LED with unknown parameter
    // must appear after all other options for LED
    if( !strcmp(&serial_command_parameters[0][0],"LED") &&
        (!serial_command_execution_complete) 
      )
    {
        Serial.write(NACK);
        Serial.print("ERROR:");
        Serial.println(ERROR_INVALID_PARAMETER);  //LED can be 0 (off) or 1 (on), any other value is invalid
        serial_command_execution_complete=1;  //indicates completed command
    }
    // end of check for ID with unknown parameter

/*************************************************************************************************/
/* PUT YOUR FUNCTIONS HERE AND KEEP THE MAIN STRUCTURE INTACT FOR BEST RESULTS                   */
/*************************************************************************************************/
    // Reading the Keypad
    
    if (key == NO_KEY)
    {
      //output keypressed signal to pi
       digitalWrite(KeyPressed, 0);
    }
    if (key != NO_KEY)
    {
      //output keypressed signal to pi
       digitalWrite(KeyPressed, 1);
    }
    if( !strcmp(&serial_command_parameters[0][0],"KEYPAD")) 
      
    {
      if(!strcmp(&serial_command_parameters[1][0],"?"))
      {
        Serial.write(ACK);
        Serial.print("KEYPAD:");
        Serial.println(key);
        delay(50);
        lcd.print(key);
        serial_command_execution_complete=1;  //indicates completed command
      }
      
    }
    // end of Reading the Keypad

    /* If this point is reached without executng a command, return an error message */
    if(!serial_command_execution_complete)
    {
      Serial.write(NACK);
      Serial.print("ERROR:");
      Serial.println(ERROR_UNKNOWN_COMMAND);
      serial_command_execution_complete=1;
    }

    // clear the buffer and clear serial_received_instruction_complete to prepare for next command */
    clear_serial_receive_buffer();
  }  //end of if(serial_received_instruction_complete)

  /*
   * What follows is a template for including periodic actions in the main loop 
   * that does not require blocking "busy" loop.
   * This example program does not require any specific periodic actions but this is
   * a common enough need that thie template code is included here.
   */

   int now;
   int last_action_time;
   int delay_duration=1000;  //this can be whatever amunt of time you want

   now=millis();  //reads the system time in milliseconds
   if( (now-last_action_time) > delay_duration)
   {
    //here is where you do the periodic action
    last_action_time=now;  //be sure to update the last_action_time when you act
   }
  
}  //end of loop()
