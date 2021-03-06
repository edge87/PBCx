//Flag Definations
#define flag0	0x01	// 0000 0001
#define flag1	0x02	// 0000 0010
#define flag2	0x04	// 0000 0100
#define flag3	0x08	// 0000 1000
#define flag4	0x10	// 0001 0000
#define flag5	0x20	// 0010 0000
#define flag6	0x40	// 0100 0000
#define flag7	0x80	// 1000 0000

#define bit0(x) (x & flag0)
#define bit1(x)	(x & flag1)
#define bit2(x) (x & flag2)
#define bit3(x) (x & flag3)
#define bit4(x) (x & flag4)
#define bit5(x) (x & flag5)
#define bit6(x) (x & flag6)
#define bit7(x) (x & flag7)

//Standard Libraries
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>


#include </usr/include/SDL/SDL_mixer.h>


//My includes
#include "AudioController.hpp"
#include "LogController.hpp"
#include "WikiMode.hpp"
#include "SwitchHandler.hpp"
#include "Game.hpp"
#include "SerialController.hpp"
#include "LogController.hpp"
#include "AudioController.hpp"
#include "BallDrain.hpp"
#include "PlayerMonitor.hpp"

//Wait for Marcus's Fix
//#include "VideoController.hpp"

extern pthread_mutex_t switch_lock;

//function prototypes
void* read_switches_thread(void* );
void * SerialOutThread(void *args);

//Global Threads
pthread_t readSwitchesThread;

//SerialSend() Thread
pthread_t sdthread;

//global variables
unsigned char switches[8] = {(char)0};
unsigned char cabinet[2] = {(char)0};

//Video Stuff
bool programRunning = true;

SDL_Event event;

//Load Serial
//NEW Serial Object
SerialController *Serial = new SerialController();
	
void initSounds()
{
	AudioController::instance()->addMusic("main", "Music/metallica.wav");
	AudioController::instance()->addSound("scream", "Sounds/2scream.wav");
	AudioController::instance()->addSound("gunShot", "Sounds/rifle.wav");
	AudioController::instance()->addSound("explode", "Sounds/lowexplo.wav");
	AudioController::instance()->addSound("bottle", "Sounds/bottlenobreak.wav");
}
	
int main (){
	VideoController::init();
	initSounds();
	
	VideoController::PlayVideo("video_0.mpg",1);
	AudioController::instance()->playMusic("main");

	LogController::instance()->info("Log Object Created");

	if(pthread_create(&sdthread, NULL, SerialOutThread, (void *)NULL)){
		LogController::instance()->error("SerialSend Thread failed to spawn. Fatal Error.");
	}
	else{
		LogController::instance()->info("SerialSend Thread was Successfully Created");
	}
	
	//Start our serial reading thread
	if(pthread_create( &readSwitchesThread, NULL, read_switches_thread, NULL)){
		LogController::instance()->error("readSwitchesThread Thread failed to spawn. Fatal Error.");
	}
	else{
		LogController::instance()->info("readSwitchesThread Thread was Successfully Created");
	}
  
  SwitchHandler switchHandler;
  WikiMode wikiMode(&switchHandler);
  BallDrain ballDrain(&switchHandler);

  PlayerMonitor::instance()->resetGame(2, 3);
  
  while(programRunning)
  { //game loop

    for(int i = 0; i < 8; i++)
    {      
      switchHandler.giveSwitchData(i, switches[i]);
    }

    wikiMode.run();
    ballDrain.run();
  }

  //Shutdown Serial
  //Shutdown Logger
  //And die
  return 0;
};

void* read_switches_thread(void *){

	while(true){
		//old read
		//read_switches();

		//TODO Need a sleep function here
		Serial->RecieveData();
	}
};

void * SerialOutThread(void *args){
	while (true){
		Serial->SendData();
	}
	return NULL;
};
