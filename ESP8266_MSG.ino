/*
 * By: Corey O'Brien
 * 
 * for use with message.cjobrien.com, an online message service designed to send messages from
 * the web to esp8266s and do other cool things!
 * 
 * The code first gets a connection, checks if everything is valid, sends a get request, and then (painfully)
 * converts the string data type that http.getString() uses into a character array for easier data manipulation
 * 
 * the string is then decrypted, copied into another string for comparison purposes, and then split into five strings
 * that will be displayed on the Eink screen. splitting the string allows for words to avoid being split between lines,
 * using a poorly written function that was really hard to figure out
 * 
 * The character array also initializes to all zeros since I couldnt seem to find what delimiter the esp8266 used.
 * 
 * 
 * Created: 4 June 2020
 * 
 * Update:
 *  this code will now implement a simple encryption method for funzies
 *  now it detects if there is a new message!
 *  now im trying on a new network with some other stuff
 *  now when there is a new message itll blink, in a cute way
 *  now it finally reads the right file 
 *  now it displays the message on the eink display ;) Still alot to do
 *  now it seperates the string so that no words get split!
 *            
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>
#include <string.h>

#define BUFFERLENGTH 1000
#define MAXINPUT 100

//Above are the buffer size and the maximum number of characters the eink display can output

//===================================
//-=-        init and dec         -=-

UBYTE* BlackImage;        //I wish I knew what this was or why it was needed

int buttonState;
int LED = 5;                                   //pin for LED
int BTN = 10;
int blinkFlag = 1; 
int newstr;
int i;


const char* ssid = "Hub on Campus State";                     //network name  
const char* password = "password";                       //network password

char currentString[BUFFERLENGTH] = {0};                //the reason for all 0s is explained below


String payload = " ";                           //initialize a string (this took forever to figure out and is the only thing that worked)

//===================================
//-=-            setup            -=-

void setup () {
  
  pinMode(LED, OUTPUT);                         //set up LED pinmode
  pinMode(BTN, INPUT);
  digitalWrite(LED, LOW);                       //and intialize to low
 
  Serial.begin(115200);                         
  WiFi.begin(ssid);                   //start wifi connection with provided info
 
  while (WiFi.status() != WL_CONNECTED) {       //while not connected
    delay(1000);
    Serial.println("connecting to wifi...");    //print this (mainly for debugging)
  }
 
} //end setup

//===================================
//-=-            loop             -=-

void loop() {
  if (WiFi.status() == WL_CONNECTED) {                          //check wifi connection status

   
    HTTPClient http;                                            //declare an object of class HTTPClient
    
    http.begin("http://message.cjobrien.com/datastorage.txt");   //where to get the data
    int httpCode = http.GET();                                            //send the get request
 
    if (httpCode > 0) { //if the request is successful

      payload = http.getString();   //get the request response payload and store in a string variable
      char buf[BUFFERLENGTH] = {0};         //declare a temporay buffer to convert string to array of characters
                                    //also, the esp8266 seeems not to use conventional delimiter characters, as it
                                    //would go crazy if i used '\0' as a delimiter, so just initialize the entire array as zeros
      
      char decrypted[BUFFERLENGTH] = {0};             //allocate an array for the decrypted characters to go to

      
      payload.toCharArray(buf,BUFFERLENGTH);          //put the retrieved data from the http get request into a character array (makes operations easier)
      
      Serial.println("Contents retrieved:");  //denote where the new message begins
      decrypt(buf,decrypted);                 //decrpyt the message from the input buffer to the final character array

      for(i = 0; i <= strlen(decrypted); i++){    //isnt this some nifty code right here ;)
          if(decrypted[i] == '\n'){                   //now im sure theres a better way to do this but oh well, i need to newlines for a secret so its
            decrypted[i] = ' ';                       //easier for me to remove them here and it doesnt really affect much 
          }
          Serial.print(decrypted[i]);
          
        }
        Serial.println(" ");
      
      if(strcmp(decrypted, currentString) != 0){    //current string is the value stored at the beginning of the loop

        currentString[BUFFERLENGTH] = {0};          //reset the current string (this is incase the new decrypted string is too short to overwrite all the characters
        
        for(int k = 0; k < MAXINPUT; k++){          //*very important - I am a fool.
          currentString[k] = decrypted[k];          //I spent about two hours trying to figure out what I did wrong
        }                                           //turns out despite the way most other functions with char arrays work, str1 = str2 just means str1 points to str2 :(
        
        BlackImage = EinkInitialize();              //initialize the screen and ready it to print a string

        char string1[19] = "                  ";
        char string2[19] = "                  ";
        char string3[19] = "                  ";
        char string4[19] = "                  ";
        char string5[19] = "                  ";  
        
        SplitString(decrypted,string1,string2,string3,string4,string5);     //split the string into the five strings above
        
        EinkPuts(0,0,string1,24);                 //put the string at coords 0,0 with font size 24
        EinkPuts(0,24,string2,24);                
        EinkPuts(0,48,string3,24);
        EinkPuts(0,72,string4,24);
        EinkPuts(0,96,string5,24);
        
        EinkFinalize(BlackImage,0);                 //put the display to sleep, the 0 indicates the string should be left on the screen
        
        blinkFlag = 1;                              //turn the blink flag on

      }
      if(blinkFlag){                              //if the button hasnt been pressed   
        blinkLED();                               //do the silly blink code I spent way too long on
      }
      if(digitalRead(BTN) == HIGH){               //if the button is pressed
        blinkFlag = 0;                            //set blinkflag to 0
        digitalWrite(LED, LOW);                   //turn off the led
      }
      

      Serial.print("\nLength of data: ");
      Serial.println(strlen(buf));                 //debugging note just to confirm that the data is being put into the buffer
  
      http.end();   //close the connection
      }
 
    delay(20);    //delay just enough so that the esp8266 doesnt cry
    }
  }

//===================================
//-=-           decrypt           -=-

void decrypt(char inputchar[], char outputchar[]){
  //This function decrypts the input based on a key that is common between the website and this
  //it inputs an array of characters to decrypt
  //and outputs the result
  
  const char* key = "cuties";           //this is the key *must match php key*
  char decryptedChar;                   //the character to store in the decypted array
  int keyLength = strlen(key);          //important for looping through the plaintext
  int keyIndex = 0;                     //initialization
  int asciiOffset;                      //the necessary offset so that for example A would map to 0, B to 1
  int letterValCorrected;               //this is the value of the letter as shown above
  int KeyIndexCorrected;                //same idea as above, but for properly indexing the key
  int keyAsciiVal;                      //change the key into a number so the plaintext can be encrypted
  int keyOffset;                        //same idea                      
  int finalKey;                         //what actually gets used in the encrpytion   
  int output;                           //debugging variable                  
  
  for(int k = 0; k < BUFFERLENGTH; k++){                              //loop through the entire array *this is really bad fix it!
    
    int characterAsciiVal = (int)inputchar[k];                //change the character into a number
    
    if((characterAsciiVal >= 65 && characterAsciiVal <= 90) || (characterAsciiVal >= 97 && characterAsciiVal <= 122)){      
      //only translate characters
  
      if(characterAsciiVal >= 65 && characterAsciiVal <= 90){ 
        asciiOffset = 65;                                          //this is if the character is lowercase
      }
      else{
        asciiOffset = 97;                                         // if the character is lowercase or symbol
      }

      letterValCorrected = characterAsciiVal - asciiOffset;       //this makes the number (0-25)
      KeyIndexCorrected = keyIndex % keyLength;                   //incase the message is longer than the key
      
      keyAsciiVal = (int)key[KeyIndexCorrected];                  //change to integer to use in shifting

      if(keyAsciiVal >= 65 && keyAsciiVal <= 90){
        keyOffset = 65;                                           //if lowercase                  
      }
      else{
        keyOffset = 97;                                           //for all other characters
      }
      finalKey = keyAsciiVal - keyOffset;                         //first, get the actual value that the character in the plaintext will be shifted by

      characterAsciiVal = ((characterAsciiVal - finalKey)- asciiOffset + 26) % 26;    //This ensures that there are no edge cases, like z->a
      characterAsciiVal += asciiOffset;                                               //add that offset back so that its an ascii recognizable character
    
      keyIndex++;                                                                     //go to the next shift value using the key

      outputchar[k] = (char)characterAsciiVal;                                        //store in the decryption array
    }
    else if(characterAsciiVal == 10){                                                 //this section deals with newlines, as it needs to reset the key index
      keyIndex = 0;
      outputchar[k] = inputchar[k];                                                   //this preserves the newline character in the decrypted message.
    }
    else{
      outputchar[k] = inputchar[k];                                                   //this is for punctuation, spaces, ect. maybe in the future I will add some encrytion for that
    }
  }
} //end function decrypt

//===================================
//-=-           blinkLED          -=-

void blinkLED(void){
  long Time=0;              //start the time at 0
  int value;
  int period = 2000;        //period in milliseconds

  Time = millis() % 2000;                           //input the number of milliseconds since the program began. This overflows eventually but it should be fine.
  value = 128+127*cos(2*PI/period*Time);            // add 128 to get to the middle of the pwm and then add/subtract based on how long its been since the program began
  analogWrite(LED, value);
} //end function blinkLED

//===================================
//-=-           EinkPuts          -=-

void EinkPuts(int x, int y, char* str, int fontsize){
  /*
   * int x -        how many pixels to the right the text is placed
   * int y -        how many pixels down the text is placed
   * char* str -    the string to be displayed
   * int fontsize - the fontsize (can only be 8,12,16,20,24)
   */
  switch(fontsize){                                                     //despite my best efforts
    case 8:                                                             //this was the best I could do :'(
            Paint_DrawString_EN(x, y, str, &Font8, WHITE, BLACK);
            break;
    case 12:
            Paint_DrawString_EN(x, y, str, &Font12, WHITE, BLACK);
            break;
    case 16:
            Paint_DrawString_EN(x, y, str, &Font16, WHITE, BLACK);
            break;
    case 20:
            Paint_DrawString_EN(x, y, str, &Font20, WHITE, BLACK);
            break;
    case 24:
            Paint_DrawString_EN(x, y, str, &Font24, WHITE, BLACK);
            break;
    
  }
  
} //End function EinkPuts

//===================================
//-=-           Einkinit          -=-

UBYTE* EinkInitialize(){
    /*
     * Id sincerely love to understand this
     * but alas
     */
    DEV_Module_Init();                                  //do whatever this is, it came from the library :0
    EPD_2IN9_Init(EPD_2IN9_FULL);                       //configure the screen size

    UBYTE *BlackImage;                                  //declare this thing
    UWORD Imagesize = ((EPD_2IN9_WIDTH % 8 == 0)? (EPD_2IN9_WIDTH / 8 ): (EPD_2IN9_WIDTH / 8 + 1)) * EPD_2IN9_HEIGHT;       //ooo spicy, dont really know what this does
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        while(1);                                                   //i wish i knew why this was here, if i remove it the code dont work
    }
    
    Paint_NewImage(BlackImage, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT, 270, WHITE);      //refresh the screen
    Paint_Clear(WHITE);                                                           //refresh the refresh? :0

    return(BlackImage);                                                           //this is needed in the other function
  
} //end function EinkInitialize

//===================================
//-=-          Einkfinal          -=-

void EinkFinalize(UBYTE* BlackImage, int toClear){
    //Run this function after displaying something
    //int toClear - 0 if you want the string to persist
    
    EPD_2IN9_Display(BlackImage);       


    EPD_2IN9_Init(EPD_2IN9_FULL);
    if(toClear){
      EPD_2IN9_Clear();
    }

    EPD_2IN9_Sleep();
    free(BlackImage);
    BlackImage = NULL;
  
} //end function EinkFinalize

//===================================
//-=-         SplitString         -=-

void SplitString(char *str, char string1[], char string2[], char string3[], char string4[], char string5[]){
  int posTotal;         //Keeps track of how many total chars were written
  int posArray;
  int Available;        //18 - how many spaces were used, this is how many spaces are left
  int arrayCount;       //Which array is being used
  int Length;           //the length of the current token
  int i;

  char *token;

  arrayCount = 1;
  posArray = 0;

  token = strtok(str," ;");
   
   while (token != NULL){
       Length = strlen(token);        //length of token
       Available = 17 - posArray;   //find how many spaces are available

       if(Length > Available){      //if theres not enough space for the word
           arrayCount++;            //go to the next array
           posArray = 0;            //reset the array counter
       }

       switch(arrayCount){
           case(1): for(i = 0; token[i] != '\0'; i++){
                        string1[i + posArray] = token[i];
                    }
                    string1[i + posArray] = ' ';
                    posArray += i + 1;
                    break;
            case(2): for(i = 0; token[i] != '\0'; i++){
                        string2[i + posArray] = token[i];
                    }
                    string2[i + posArray] = ' ';
                    posArray += i + 1;
                    break;
            case(3): for(i = 0; token[i] != '\0'; i++){
                        string3[i + posArray] = token[i];
                    }
                    string3[i + posArray] = ' ';
                    posArray += i + 1;
                    break;
            case(4): for(i = 0; token[i] != '\0'; i++){
                        string4[i + posArray] = token[i];
                    }
                    string4[i + posArray] = ' ';
                    posArray += i + 1;
                    break;
            case(5): for(i = 0; token[i] != '\0'; i++){
                        string5[i + posArray] = token[i];
                    }
                    string5[i + posArray] = ' ';
                    posArray += i + 1;
                    break;
       }
       token = strtok (NULL, " ;");
   }

} //end function SplitString
