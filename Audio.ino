// fmpeg -i TechBassR120E-02.wav  -acodec pcm_u8 -ar 16000 -ac 1 b.wav
// https://www.google.es/imgres?imgurl=http://www.getmicros.net/wp-content/uploads/2014/01/arduino-and-sdcard_bb.png&imgrefurl=http://www.getmicros.net/arduino-sd-card-example.php&h=903&w=1632&tbnid=nf3iwqTutBduxM:&docid=G_AhGVwTHTt6wM&ei=JC9_VtzMKsP4asuoi8gC&tbm=isch&ved=0ahUKEwjc7ubo4vrJAhVDvBoKHUvUAikQMwg0KAEwAQ


#include <SD.h>                      // need to include the SD library
#define DISABLE_SPEAKER2
#define SD_ChipSelectPin 4 			//using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           		//  also need to include this library...
#include <SPI.h>


TMRpcm tmrpcm;   					// create an object for use in this sketch

unsigned long time = 0;


int bpm = 1024;
unsigned long c = 0;
String filenumber;
char separator[] = ",";

int num_steps = 1;
int* steps = new int[64 /* max steps */];


void setup() {

	steps[0] = 5;
	tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
	//Complimentary Output or Dual Speakers:
	//pinMode(10,OUTPUT); Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45

	Serial.begin(9600);
	pinMode(13, OUTPUT); //LED Connected to analog pin 0
	if (!SD.begin(SD_ChipSelectPin)) { // see if the card is present and can be initialized:
		Serial.println("SD fail...");
		return;   // don't do anything more if not

	} else {
		Serial.println("SD ok");
	}

	Serial.println("PRIND DIR DONE");

	tmrpcm.volume(1);
}

void loop() {
	int counter = 0;
	String serialDataIn;
	while (Serial.available()) {
		int inbyte = Serial.read();
		switch (inbyte) {

		case 'p':
			tmrpcm.pause();
			break;
		case '?':
			if (tmrpcm.isPlaying()) {
				Serial.println("A wav file is being played");
			}
			break;
		case 'S':
			tmrpcm.stopPlayback();
			break;
		case '=':
			tmrpcm.volume(1);
			break;
		case '-':
			tmrpcm.volume(0);
			break;
			//case '0': tmrpcm.quality(0); break;
			//case '1': tmrpcm.quality(1); break;
		default:
			//int inbyte = Serial.read();

			if (isDigit(inbyte)) {
				serialDataIn += (char) inbyte;
			}
			if (inbyte == ',') {  // Handle delimiter
				steps[counter] = serialDataIn.toInt(); //String(serialDataIn);
				serialDataIn = String("");
				counter = counter + 1;
			}
			if (inbyte == '\r' || inbyte == '\n') {  // end of line
				steps[counter] = serialDataIn.toInt(); //String(serialDataIn);
				serialDataIn = String("");
				counter = counter + 1;
				num_steps  = counter;
				c = 0;
			}

			break;
		}

	}



	int reminder = c % num_steps;
	filenumber = String(steps[reminder]);

	bpm = analogRead(A0);


	String str = "/AUDIO/FILE_";
	str += filenumber;
	str += ".wav";
	//9

	Serial.println(str);
	// Length (with one extra character for the null terminator)
	int str_len = str.length() + 1;

	// Prepare the character array (the buffer)
	char char_array[str_len];

	// Copy it over
	str.toCharArray(char_array, str_len);
	tmrpcm.play(char_array);
	delay(bpm);

	c++;

}


void printDirectory(File dir, int numTabs) {
	while (true) {

		File entry = dir.openNextFile();
		if (!entry) {
			// no more files
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++) {
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if (entry.isDirectory()) {
			Serial.println("/");
			//printDirectory(entry, numTabs + 1);
		} else {
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
	}
}
