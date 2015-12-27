#include <SD.h>                      // need to include the SD library
#define DISABLE_SPEAKER2
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

unsigned long time = 0;

void setup(){

	// fmpeg -i TechBassR120E-02.wav  -acodec pcm_u8 -ar 16000 -ac 1 b.wav
	// https://www.google.es/imgres?imgurl=http://www.getmicros.net/wp-content/uploads/2014/01/arduino-and-sdcard_bb.png&imgrefurl=http://www.getmicros.net/arduino-sd-card-example.php&h=903&w=1632&tbnid=nf3iwqTutBduxM:&docid=G_AhGVwTHTt6wM&ei=JC9_VtzMKsP4asuoi8gC&tbm=isch&ved=0ahUKEwjc7ubo4vrJAhVDvBoKHUvUAikQMwg0KAEwAQ

  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  //Complimentary Output or Dual Speakers:
  //pinMode(10,OUTPUT); Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45

  Serial.begin(9600);
  pinMode(13,OUTPUT); //LED Connected to analog pin 0
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail...");
    return;   // don't do anything more if not

  }
  else{
    Serial.println("SD ok");
  }


 // File root = SD.open("/");

 // printDirectory(root, 0);



 // tmrpcm.play("music"); //the sound file "music" will play each time the arduino powers up, or is reset
}



void loop(){

  //blink the LED manually to demonstrate music playback is independant of main loop
  if(tmrpcm.isPlaying() && millis() - time > 50 ) {
      digitalWrite(13,!digitalRead(13));
      time = millis();
  }else
  if(millis() - time > 500){
    digitalWrite(13,!digitalRead(13));
    time = millis();
  }



  if(Serial.available()){
    switch(Serial.read()){
    case 'd': tmrpcm.play("a.wav"); break;
    case 'P': tmrpcm.play("b.wav"); break;
    case 'p': tmrpcm.pause(); break;
    case '?': if(tmrpcm.isPlaying()){ Serial.println("A wav file is being played");} break;
    case 'S': tmrpcm.stopPlayback(); break;
    case '=': tmrpcm.volume(1); break;
    case '-': tmrpcm.volume(0); break;
    case '0': tmrpcm.quality(0); break;
    case '1': tmrpcm.quality(1); break;
    default: break;
    }
  }


}



void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
