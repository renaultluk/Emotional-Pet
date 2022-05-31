#include "main.h"

void mainControlFlow()
{
  // BlueLEDfade();
  if (face.getFaceState() == MENU)
  {
    ScreenCol* temp = static_cast<ScreenCol*>(face.menu[face.menu.getRowIndex()]);
    bool menu_top = (temp->getColIndex() == 0);
    if (tilt_up)
    {
      tilt_up = false;
      if (currScreen == face.menu.screen(3, 0))
      {
        face.menu.navigateTo("listen", "main");
        // stressCheckUp();
      }
      else if (menu_top)
      {
        face.menu.navigateTo('d');
      }
      // else if (currScreen == face.menu.screen(3, 0))
      // {
      //   face.menu.navigateTo('d');
      //   stressCheckUp();
      // }
      else if (currScreen == face.menu.screen(3, 1))
      {
        face.menu.navigateTo('d');
      }
      else if (currScreen == face.menu.screen(3, 2))
      {
        face.menu.navigateTo("emotion", "main");
      }
      else if (currScreen == face.menu.screen(0,3))
      {
        selectFriend();
        face.menu.navigateTo("listen", "audio");
        playAudio(audioTarget.c_str());
      }

      if (currScreen == face.menu.screen(1,1))
      {
        record("/new.wav", 20);
      }
    }
    else if (tilt_down)
    {
      tilt_down = false;
      if (menu_top)
      {
        face.menu.setVisible(false);
        face.changeFaceState(NEUTRAL);
      }
      else if ((currScreen == face.menu.screen(0, 3)) || (currScreen == face.menu.screen(0, 2)))
      {
        face.menu.navigateTo("listen", "choose");
      }
      else if (currScreen == face.menu.screen(0, 1))
      {
        face.menu.navigateTo("listen", "main");
      }
      else if (currScreen == face.menu.screen(1, 1))
      {
        Serial.println(currScreen->name);
        face.menu.navigateTo("record", "main");
      }
    }
    else if (tilt_left)
    {
      tilt_left = false;
      Serial.print("currRow: ");
      Serial.println(face.menu.getRowIndex());
      Serial.print("currScreen: ");
      Serial.print(currScreen->name);
      if (currScreen == face.menu.screen(3, 0))
      {
        Serial.println("Navigate to emotion measurement");
        menuChoice = 1;
        face.menu.navigateTo("emotion", "measure");
        stressCheckUp();
      }
      else if (menu_top)
      {
        face.menu.navigateTo('l');
      }
      else if (currScreen == face.menu.screen(0, 1))
      {
        face.menu.navigateTo("listen", "friends");
        // updateList();
      }
      else if (currScreen == face.menu.screen(0, 2))
      {
        replyTarget = audioTarget;
        face.menu.navigateTo("record", "record");
      }
      // else if (currScreen == face.menu.screen(3, 0))
      // {
      //   Serial.println("Navigate to emotion measurement");
      //   menuChoice = 1;
      //   face.menu.navigateTo("emotion", "measure");
      // }
      else if (currScreen == face.menu.screen(0,3))
      {
        scrollList(0);
      }
    }
    else if (tilt_right)
    {
      tilt_right = false;
      Serial.print("currRow: ");
      Serial.println(face.menu.getRowIndex());
      Serial.print("currScreen: ");
      Serial.println(currScreen->name);
      if (currScreen == face.menu.screen(3, 0))
      {
        Serial.println("Navigate to emotion measurement");
        menuChoice = 0;
        face.menu.navigateTo("emotion", "measure");
        stressCheckUp();
      }
      else if (menu_top)
      {
        Serial.println("Menu top navigate right");
        face.menu.navigateTo('r');
      }
      else if (currScreen == face.menu.screen(0, 1))
      {
        // TODO: select random audio
        face.menu.navigateTo("listen", "audio");
      }
      else if (currScreen == face.menu.screen("listen", "audio"))
      {
        // TODO: Add friend
        // TODO: Go to next audio
      }
      // else if (currScreen == face.menu.screen(3, 0))
      // {
      //   Serial.println("Navigate to emotion measurement");
      //   menuChoice = 0;
      //   face.menu.navigateTo("emotion", "measure");
      // }
      else if (currScreen == face.menu.screen(0,3))
      {
        scrollList(1);
      }
    }
    else if (swipe_up)
    {
      swipe_up = false;
      if (currScreen == face.menu.screen("listen", "friends"))
      {
        scrollList(1);
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
        scrollList(0);
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
        audioTarget = friendsList[currFriend].userID;
        // TODO: Fetch selected friend's audio
        face.menu.navigateTo("listen", "audio");
      }
    }

    // if (currScreen == face.menu.screen(3, 1))
    // {
    //   stressCheckUp();
    // }
  }
  else
  {
    if (tilt_up)
    {
      tilt_up = false;
      face.changeFaceState(MENU);
      // currScreen = face.menu.screen("listen", "main");
    }
    else if (tilt_down)
    {
      tilt_down = false;
      playAudio("/meow.wav");
      face.changeFaceState(HAPPY);
      face.changeFaceState(NEUTRAL);
    }
  }
}