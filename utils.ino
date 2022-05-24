#include "main.h"

void mainControlFlow()
{
  if (face.getFaceState() == MENU)
  {
    bool menu_top = (currScreen == face.menu.screen("listen", "main")) || (currScreen == face.menu.screen("record", "main"))
        || (currScreen == face.menu.screen("emotion", "main")) || (currScreen == face.menu.screen("settings", "main"));
    if (tilt_up)
    {
      tilt_up = false;
      if (menu_top && (currScreen != face.menu.screen("settings", "main")))
      {
        face.menu.navigateTo('d');
      }
    }
    else if (tilt_down)
    {
      tilt_down = false;
      if (menu_top || currScreen == face.menu.screen("emotion", "feedback"))
      {
        face.menu.setVisible(false);
        face.changeFaceState(NEUTRAL);
      }
      else if ((currScreen == face.menu.screen("listen", "friends")) || (currScreen == face.menu.screen("listen", "audio")))
      {
        face.menu.navigateTo("listen", "choose");
      }
      else if (currScreen == face.menu.screen("record", "record"))
      {
        face.menu.navigateTo('u');
      }
    }
    else if (tilt_left)
    {
      tilt_left = false;
      if (menu_top)
      {
        face.menu.navigateTo('l');
      }
      else if (currScreen == face.menu.screen("listen", "choose"))
      {
        face.menu.navigateTo("listen", "friends");
      }
      else if (currScreen == face.menu.screen("listen", "audio"))
      {
        // TODO: Reply
        face.menu.navigateTo("record", "record");
      }
      else if (currScreen == face.menu.screen("emotion", "main"))
      {
        // TODO: Select relaxed
        menuChoice = 1;
        face.menu.navigateTo('d');
      }
    }
    else if (tilt_right)
    {
      tilt_right = false;
      if (menu_top)
      {
        face.menu.navigateTo('r');
      }
      else if (currScreen == face.menu.screen("listen", "choose"))
      {
        // TODO: select random audio
        face.menu.navigateTo("listen", "audio");
      }
      else if (currScreen == face.menu.screen("listen", "audio"))
      {
        // TODO: Add friend
        // TODO: Go to next audio
      }
      else if (currScreen == face.menu.screen("emotion", "main"))
      {
        // TODO: Select stressed
        menuChoice = 0;
        face.menu.navigateTo('d');
      }
    }
    else if (swipe_up)
    {
      swipe_up = false;
      if (currScreen == face.menu.screen("listen", "friends"))
      {
        // TODO: friends screen scroll
      }
      else if (currScreen == face.menu.screen("listen", "audio"))
      {
        // TODO: Go to next audio
      }
    }
    else if (swipe_down)
    {
      swipe_down = false;
      if (currScreen == face.menu.screen("listen", "friends"))
      {
        // TODO: friends screen scroll
      }
      else if (currScreen == face.menu.screen("listen", "audio"))
      {
        // TODO: Go to previous audio
      }
    }
    else if (touch_hold)
    {
      if (currScreen == face.menu.screen("record", "record"))
      {
        // TODO: Start recording
        record("/audio/newest.wav", 20000);
        face.menu.navigateTo('u');
      }
      else if (currScreen == face.menu.screen("emotion", "measure"))
      {
        // TODO: Start recording
        stressCheckUp();
      }
    }
    else if (hold_finish)
    {
      hold_finish = false;
      if (currScreen == face.menu.screen("record", "record"))
      {
        // TODO: Stop recording
        face.menu.navigateTo('u');
      }
      else if (currScreen == face.menu.screen("emotion", "measure"))
      {
        // TODO: Stop recording
        face.menu.navigateTo('d');
      }
    }
    else if (tapped)
    {
      tapped = false;
      if (currScreen == face.menu.screen("listen", "friends"))
      {
        // TODO: Select friend
        face.menu.navigateTo("listen", "audio");
      }
    }
  }
  else
  {
    if (tilt_up)
    {
      face.changeFaceState(MENU);
    }
    else if (swipe_up || swipe_down)
    {
      // TODO: Petting
    }
  }
}