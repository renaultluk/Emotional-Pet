#include "main.h"

void menuNavigation(){
  if (tilt_up){
    face.menu.navigateTo('u'); //(up down left right = u d l r)
    //playAudio(const char* path);
    PlayHaptic(1); //PlayHaptic(effect number)
    tilt_up = false;
  }
  if (tilt_down){
    face.menu.navigateTo('d'); //(up down left right = u d l r)
    //playAudio(const char* path);
    PlayHaptic(4); //PlayHaptic(effect number)
    tilt_down = false;
  }
  if (tilt_left){
    face.menu.navigateTo('l'); //(up down left right = u d l r)
    //playAudio(const char* path);
    PlayHaptic(27); //PlayHaptic(effect number)
    tilt_left = false;
  }
  if (tilt_right){
    face.menu.navigateTo('r'); //(up down left right = u d l r)
    //playAudio(const char* path);
    PlayHaptic(27); //PlayHaptic(effect number)
    tilt_right = false;
  }
}

void mainControlFlow()
{
  if (face.menu.getFaceState() == MENU)
  {
    if ((currScreen == face.menu.screen("listen","main")) || (currScreen == face.menu.screen("record","main"))
     || (currScreen == face.menu.screen("listen","main")))
    {
      
    }
  }
  else
  {
    if (tilt_up)
    {
      face.changeFaceState(MENU);
    }
  }
}