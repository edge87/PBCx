#ifndef VIDEOCONTROLLER_HPP_
#define VIDEOCONTROLLER_HPP_

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_mutex.h"
#include "LogController.hpp"

#include <vlc/vlc.h>

#define WIDTH 640
#define HEIGHT 480

#define VIDEOWIDTH 912
#define VIDEOHEIGHT 513

#define PLAYERSCOREWIDTH 250
#define PLAYERSCOREHEIGHT 75

#define PLAYER1X 100
#define PLAYER1Y 100

#define PLAYER2X 1370
#define PLAYER2Y 100

#define PLAYER3X 100
#define PLAYER3Y 820

#define PLAYER4X 1370
#define PLAYER4Y 820

static SDL_Surface * screen, *background, *scorebackground;

struct players{
	SDL_Surface* surf;
	SDL_Rect rect;
	bool status;
};

static struct players player[4];

class VideoController{
	public:

		// Setups up video and all its stuff.
		static bool init();

		// Destroys Video
		static bool destroy();

		static void Play(std::string filename, int priority);

		//\ brief This will reset our thread static variables and call start afterwards.
		static void Reset();

		static void EnablePlayerScore(int player_number);
		static void DisablePlayerScore(int player_number);

		static void SetScore(int player_number);



	private:
		struct ctx
		{
		    SDL_Surface *surf;
		    SDL_mutex *mutex;
		};

		static void *lock(void *data, void **p_pixels);
		static void unlock(void *data, void *id, void *const *p_pixels);
		static void display(void *data, void *id);

		//Logger has nothing until set
		//LogController* logger;





};


#endif /* VIDEOCONTROLLER_HPP_ */