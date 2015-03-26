//
// Morse Encoder and Decoder
// 
// Forked from Matthias Esterl (madc) on Github
//
// This version by Neel Shearer
//

//
// This version designed to be interactive with the serial port
//
// Type content on the serial port to have it encoded by the onboard LED on pin13
//

//Define the LED Pin
#define PIN_OUT        13
//Define unit length in ms
#define UNIT_LENGTH    200
 
//Build a struct with the morse code mapping
static const struct {const char letter, *code;} MorseMap[] =
{
	{ 'A', ".-" },
	{ 'B', "-..." },
	{ 'C', "-.-." },
	{ 'D', "-.." },
	{ 'E', "." },
	{ 'F', "..-." },
	{ 'G', "--." },
	{ 'H', "...." },
	{ 'I', ".." },
	{ 'J', ".---" },
	{ 'K', ".-.-" },
	{ 'L', ".-.." },
	{ 'M', "--" },
	{ 'N', "-." },
	{ 'O', "---" },
	{ 'P', ".--." },
	{ 'Q', "--.-" },
	{ 'R', ".-." },
	{ 'S', "..." },
	{ 'T', "-" },
	{ 'U', "..-" },
	{ 'V', "...-" },
	{ 'W', ".--" },
	{ 'X', "-..-" },
	{ 'Y', "-.--" },
	{ 'Z', "--.." },
	{ ' ', "     " }, //Gap between word, seven units 
		
	{ '1', ".----" },
	{ '2', "..---" },
	{ '3', "...--" },
	{ '4', "....-" },
	{ '5', "....." },
	{ '6', "-...." },
	{ '7', "--..." },
	{ '8', "---.." },
	{ '9', "----." },
	{ '0', "-----" },
		
	{ '.', "·–·–·–" },
	{ ',', "--..--" },
	{ '?', "..--.." },
	{ '!', "-.-.--" },
	{ ':', "---..." },
	{ ';', "-.-.-." },
	{ '(', "-.--." },
	{ ')', "-.--.-" },
	{ '"', ".-..-." },
	{ '@', ".--.-." },
	{ '&', ".-..." },
};

void setup()
{
  pinMode( PIN_OUT, OUTPUT );
  digitalWrite( PIN_OUT, LOW );
  Serial.begin(9600);
  Serial.print("Ready");
}
 
void loop()
{
  
String morseWord = "";
String asciiWord = "";

    if (Serial.available() > 0) {
    // read incoming serial data:
    char inChar = Serial.read();
    // Type the next ASCII value from what you received:
    asciiWord += inChar;
   };
   morseWord = encode(asciiWord);

  Serial.print(morseWord);  
  for(int i=0; i<=morseWord.length(); i++)
  {
    switch( morseWord[i] )
    {
      case '.': //dit
        digitalWrite( PIN_OUT, HIGH );
        delay( UNIT_LENGTH );
        digitalWrite( PIN_OUT, LOW );
        delay( UNIT_LENGTH );
          
        break;
 
      case '-': //dah
        digitalWrite( PIN_OUT, HIGH );
        delay( UNIT_LENGTH*3 );
        digitalWrite( PIN_OUT, LOW );
        delay( UNIT_LENGTH );
          
        break;
 
      case ' ': //gap
        delay( UNIT_LENGTH );
    }
  }
}
 
String encode(String string)
{
  size_t i, j;
  String morseWord = "";
  
  for( i = 0; string[i]; ++i )
  {
    for( j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j )
    {
      if( toupper(string[i]) == MorseMap[j].letter )
      {
        morseWord += MorseMap[j].code;
        break;
      }
    }
    morseWord += " "; //Add tailing space to seperate the chars
  }
 
  return morseWord;  
}
 
String decode(String morse)
{
  String msg = "";
  
  int lastPos = 0;
  int pos = morse.indexOf(' ');
  while( lastPos <= morse.lastIndexOf(' ') )
  {    
    for( int i = 0; i < sizeof MorseMap / sizeof *MorseMap; ++i )
    {
      if( morse.substring(lastPos, pos) == MorseMap[i].code )
      {
        msg += MorseMap[i].letter;
      }
    }
 
    lastPos = pos+1;
    pos = morse.indexOf(' ', lastPos);
    
    // Handle white-spaces between words (7 spaces)
    while( morse[lastPos] == ' ' && morse[pos+1] == ' ' )
    {
      pos ++;
    }
  }
 
  return msg;
}
