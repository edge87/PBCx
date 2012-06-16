#include "WikiMode.hpp"
#include "scon.hpp"


WikiMode::WikiMode(SwitchHandler *handler)
{
  handler->registerObserver(16, this);
  resetMode();
}

void WikiMode::resetMode()
{
  progress = 0;
  set_light(0, 8, 5);
  set_light(1, 8, 5);
  set_light(2, 8, 5);
  set_light(3, 8, 5);
  reset = false;
}

void WikiMode::notify(int SwitchNumber)
{
  if(progress <= 4 && reset != true)
  {
    set_light(progress, 8, 5);
    progress++;
  }  
  if(progress == 4)
  {
    //mode complete
    reset = true;
    clock_gettime(CLOCK_MONOTONIC, &endTimer);	
    endTimer.tv_sec += 5;
  }
}

void WikiMode::run()
{
  timespec tempTime;
  clock_gettime(CLOCK_MONOTONIC, &tempTime);	
  if(reset == true)
  {
    if(tempTime.tv_sec > endTimer.tv_sec)
    {
      resetMode();
    }
  }  
}
