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
  if (face.getFaceState() == MENU)
  {
    if ((currScreen == face.menu.screen("listen","main")) || (currScreen == face.menu.screen("record","main"))
     || (currScreen == face.menu.screen("settings","main")) || (currScreen == face.menu.screen("emotion","main")))
    {
      if (tilt_left)
      {
        face.menu.navigateTo('l');
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_left = false;
      }
      else if (tilt_right)
      {
        face.menu.navigateTo('r');
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_right = false;
      }
      else if (tilt_up)
      {
        face.menu.navigateTo('d');
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_up = false;
      }
      else if (tilt_down)
      {
        face.changeFaceState(NEUTRAL);
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_down = false;
      }
    }

    else if (currScreen == face.menu.screen("listen", "choose"))
    {
      if (tilt_left)
      {
        face.menu.navigateTo("listen", "friends");
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_left = false;

        // TODO: Load friends
      }
      else if (tilt_right)
      {
        face.menu.navigateTo("listen", "audio");
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_right = false;

        // TODO: Load audio
      }
      else if (tilt_down)
      {
        face.menu.navigateTo("listen", "main");
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_down = false;
      }
    }
    else if (currScreen == face.menu.screen("listen", "audio"))
    {
      if (tilt_left)
      {
        // TODO: Reply

        face.menu.navigateTo("record", "record");
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_left = false;
      }
      else if (tilt_right)
      {
        // TODO: Add friend

        face.menu.navigateTo("listen", "main");
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_right = false;
      }
      else if (tilt_down)
      {
        face.menu.navigateTo("listen", "main");
        //playAudio(const char* path);
        PlayHaptic(27); //PlayHaptic(effect number)
        tilt_down = false;
      }
    }

    else if (currScreen == face.menu.screen("listen", "friends"))
    {

    }

    else if (currScreen == face.menu.screen("record", "record"))
    {
      // TODO: Record

      if (tilt_down)
      {
        face.menu.navigateTo('u');
      }
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