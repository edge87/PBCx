#include "VideoController.hpp"

bool VideoController::init(){
	//setup small video - location
    smallvideo.rect.x = 504;
    smallvideo.rect.y = 283;
    smallvideo.status = false;

    smallvideo.surf = SDL_CreateRGBSurface(SDL_SWSURFACE, VIDEOWIDTH, VIDEOHEIGHT,
    	                                    16, 0x001f, 0x07e0, 0xf800, 0);

    smallvideo.mutex = SDL_CreateMutex();

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	};

	if((screen = SDL_SetVideoMode(1920, 1080, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
			return false;
	};

	SDL_Flip(screen);

	if((background = SDL_SetVideoMode(1920, 1080, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
				return false;
	};

	//Load Images

	//Set Player Score board locations
	player[0].rect.x = PLAYER1X;
	player[0].rect.y = PLAYER1Y;

	player[1].rect.x = PLAYER2X;
	player[1].rect.y = PLAYER2Y;

	player[2].rect.x = PLAYER3X;
	player[2].rect.y = PLAYER3Y;

	player[3].rect.x = PLAYER4X;
	player[3].rect.y = PLAYER4Y;


	for(int i=0; i<4; i++){
		player[i].surf = SDL_DisplayFormat(SDL_LoadBMP("player.bmp"));
		player[i].status = false;

		SDL_BlitSurface(scorebackground, NULL, player[i].surf, NULL);
		SDL_BlitSurface(player[i].surf, NULL, screen, &player[i].rect);
	}

	 SDL_Flip(screen);

	 //kick off display thread and go back
	 if(pthread_create(&videorefresht, NULL, RefreshDisplay, NULL)){
		 //Thread didn't launch
	 }

	 return 0;
};

void VideoController::EnablePlayerScore(int player_number){
	player[player_number - 1].status = true;
}

void VideoController::DisablePlayerScore(int player_number){
	player[player_number - 1].status = false;
}

void *VideoController::lock(void *data, void **p_pixels){
    struct ctx *ctx = static_cast<struct ctx*>(data);

    SDL_LockMutex(ctx->mutex);
    SDL_LockSurface(ctx->surf);
    *p_pixels = ctx->surf->pixels;
    return NULL; /* picture identifier, not needed here */
}

void VideoController::unlock(void *data, void *id, void *const *p_pixels){
    struct ctx *ctx = static_cast<struct ctx*>(data);

    /* VLC just rendered the video, but we can also render stuff */
    uint16_t *pixels = static_cast<uint16_t*>(*p_pixels);
    int x, y;

    for(y = 10; y < 40; y++)
        for(x = 10; x < 40; x++)
            if(x < 13 || y < 13 || x > 36 || y > 36)
                pixels[y * VIDEOWIDTH + x] = 0xffff;
            else
                pixels[y * VIDEOWIDTH + x] = 0x0;

    SDL_UnlockSurface(ctx->surf);
    SDL_UnlockMutex(ctx->mutex);

    assert(id == NULL); /* picture identifier, not needed here */
}

void VideoController::display(void *data, void *id){
    /* VLC wants to display the video */
    (void) data;
    assert(id == NULL);
}

void VideoController::Play(std::string filename, int priority){
	// TODO If we're already playing and a lower priority wants to play. No.

	libvlc_instance_t *libvlc;
	libvlc_media_t *m;
	libvlc_media_player_t *mp;

	char const *vlc_argv[] =
	    {
	        "--no-xlib", /* tell VLC to not use Xlib */
	        "--no-video-title",
	    };

	    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

	    SDL_Surface *empty;

	    empty = SDL_CreateRGBSurface(SDL_SWSURFACE, VIDEOWIDTH, VIDEOHEIGHT,
	                                 32, 0, 0, 0, 0);


	    int options = SDL_ANYFORMAT | SDL_HWSURFACE | SDL_DOUBLEBUF;


	    //Initialise libVLC

	    libvlc = libvlc_new(vlc_argc, vlc_argv);
	    m = libvlc_media_new_path(libvlc, filename.c_str());
	    mp = libvlc_media_player_new_from_media(m);
	    libvlc_media_release(m);

	    libvlc_video_set_callbacks(mp, VideoController::lock, VideoController::unlock, VideoController::display, &smallvideo);
	    libvlc_video_set_format(mp, "RV16", VIDEOWIDTH, VIDEOHEIGHT, VIDEOWIDTH*2);
	    libvlc_media_player_play(mp);

		//Main loop

	    smallvideo.status = true;
	    while(smallvideo.status){

	    }

	    /*
	     * Stop stream and clean up libVLC
	     */
	    libvlc_media_player_stop(mp);
	    libvlc_media_player_release(mp);
	    libvlc_release(libvlc);

	    return;
}

void VideoController::Reset(){

}

void *VideoController::RefreshDisplay(void* args){
	//Init
	SDL_Event event;
	int action = 0;


//Main Looping
	while(true){
		//Blit Our background

		for(int i=0; i<4; i++){
			if(player[i].status){
			//update this players score
			//blit background for scorebox
			//add score text
			//lock screen
				//blit scorebox to screen
			//unlock screen
			}
		}

		//Do our FPS calculations and display them IF we're debugging. Which we are.

		while( SDL_PollEvent( &event ) ){
			switch(event.type){
				case SDL_QUIT:
					programRunning = false;
			        break;
				case SDL_KEYDOWN:
					action = event.key.keysym.sym;

			        if(action == SDLK_ESCAPE){
			        programRunning = false;
			        }
			        break;
			}
		}

		//if our small video is enabled
		 SDL_LockMutex(smallvideo.mutex);
		 SDL_BlitSurface(smallvideo.surf, NULL, screen, &smallvideo.rect);
		 SDL_UnlockMutex(smallvideo.mutex);

		 SDL_Flip(screen);
		 SDL_Delay(10);
		 //redraw our background if not in full screen mode
	}
	return NULL;//Shuts the editor up. "oh you didn't return anything, you must be an ahole"
}
