#include "face.h"

void Eyelid::draw(bool sel)
{
  if (visible) {
    int horizontal_pt_x = x + w;
    int horizontal_pt_y = y;
    int vertical_pt_x = dir_right ? horizontal_pt_x : x;
    int vertical_pt_y = y + h;
    spr[sel].fillTriangle(x, y, horizontal_pt_x, horizontal_pt_y, vertical_pt_x, vertical_pt_y, PRIMARY_COLOR);
  }
}

Face::Face() : forehead(43, 0, 156, 69), leftEyelid(42, 20, 50, 50, false), rightEyelid(200, 20, 50, 50, true), leftEye(46, 69, 37, 104, false), rightEye(158, 69, 37, 104, false), faceBottom(18, 178, 204, 63), menu("menu", 3)
{
  // anim_iterator = 0;
  // anim_time = 0;
  faceState = NEUTRAL;

  // forehead = Block(43, 0, 156, 69);
  // leftEyelid = Eyelid(42, 20, 50, 50, false);
  // rightEyelid = Eyelid(200, 20, 50, 50, true);
  // leftEye = Rounded(46, 69, 37, 104, false);
  // rightEye = Rounded(158, 69, 37, 104, false);
  // faceBottom = Block(18, 178, 204, 63);

  // // Menu Structure
  // menu = ScreenRow("menu", 3);

  menu.add(new ScreenCol("listen", 4));
  menu.add(new ScreenCol("record", 2));
  menu.add(new ScreenCol("settings", 1));

  UIElGroup* curr = static_cast<UIElGroup*>(menu["listen"]);
  curr->add(new UIElGroup("main", 2));
  curr->add(new UIElGroup("choose", 6));
  curr->add(new UIElGroup("audio", 8));
  curr->add(new UIElGroup("friends", 8));

  curr = static_cast<UIElGroup*>(menu["record"]);
  curr->add(new UIElGroup("main", 3));
  curr->add(new UIElGroup("record", 3));

  static_cast<UIElGroup*>(menu["settings"])->add(new UIElGroup("main", 2));

  // Page Content
  curr = static_cast<UIElGroup*>(menu["listen"]);
  curr = static_cast<UIElGroup*>((*curr)["main"]);
  curr->add(new Image(75, 75, "icons/headphones.jpg"));
  curr->add(new Text(97, 155, "Listen", true));

  curr = static_cast<UIElGroup*>(menu["listen"]);
  curr = static_cast<UIElGroup*>((*curr)["choose"]);
  curr->add(new Rounded(55, 63, 62, 115, true));
  curr->add(new Rounded(123, 63, 62, 115, true));
  curr->add(new Image(71, 83, "icons/friends.jpg"));
  curr->add(new Image(135, 92, "icons/explore.jpg"));
  curr->add(new Text(69, 149, "Friends", false));
  curr->add(new Text(139, 149, "Explore", false));

  // TODO: menu[listen][audio], menu[listen][friends]

  curr = static_cast<UIElGroup*>(menu["listen"]);
  curr = static_cast<UIElGroup*>((*curr)["audio"]);
  curr->add(new Rounded(55, 66, 132, 112, true));
  curr->add(new Block(62, 113, 0, 2, "progressBar"));
  curr->add(new Text(62, 118, "00:00", false, "audioProgress"));
  curr->add(new Text(161, 118, "00:00", false, "audioLength"));
  curr->add(new Rounded(62, 129, 46, 12, false));
  curr->add(new Rounded(133, 129, 46, 12, false));
  curr->add(new Text(75, 130, "Reply", true));
  curr->add(new Text(138, 130, "Add Friend", true));

  curr = static_cast<UIElGroup*>(menu["record"]);
  curr = static_cast<UIElGroup*>((*curr)["main"]);
  curr->add(new Circle(88, 75, 61, false));
  curr->add(new Circle(95, 82, 48, true));
  curr->add(new Text(139, 149, "Record", true));

  curr = static_cast<UIElGroup*>(menu["record"]);
  curr = static_cast<UIElGroup*>((*curr)["record"]);
  curr->add(new Circle(80, 64, 80, false));
  curr->add(new Circle(90, 73, 62, false, "innerCircle"));
  curr->add(new Text(139, 149, "00:00", true, "timer"));

  curr = static_cast<UIElGroup*>(menu["settings"]);
  curr = static_cast<UIElGroup*>((*curr)["main"]);
  curr->add(new Image(93, 80, "icons/settings.jpg"));
  curr->add(new Text(89, 155, "Settings", true));
}

void Face::changeFaceState(faceState_t newFaceState)
{
  Serial.print("Switched to ");
  Serial.println(newFaceState);
  switch (newFaceState)
  {
    case NEUTRAL: {
      forehead.addKeyframe(43, 0, 156, 69, 2);
      leftEyelid.addKeyframe(42, 20, 50, 50, 2, false);
      rightEyelid.addKeyframe(200, 20, 50, 50, 2, false);
      leftEye.addKeyframe(46, 69, 37, 104, 2);
      rightEye.addKeyframe(158, 69, 37, 104, 2);
      faceBottom.addKeyframe(18, 178, 204, 63, 2);
      velocityFunc = easeOut;
      anim_time = 2 * FRAME_RATE;
      break;
    }

    case HAPPY: {
      forehead.addKeyframe(23, 0, 170, 35, 3);
      leftEyelid.addKeyframe(25, -4, 50, 50, 3, false);
      rightEyelid.addKeyframe(157, -4, 50, 50, 3, false);
      leftEye.addKeyframe(26, 35, 37, 104, 3);
      rightEye.addKeyframe(117, 35, 37, 104, 3);
      faceBottom.addKeyframe(18, 100, 204, 63, 3);
      velocityFunc = easeInOut;
      anim_time = 5 * FRAME_RATE;
      break;
    }

    case SAD: {
      forehead.addKeyframe(60, 38, 156, 45, 2);
      leftEyelid.addKeyframe(83, 83, 50, 50, 2);
      rightEyelid.addKeyframe(162, 83, 50, 50, 2);
      leftEye.addKeyframe(83, 94, 37, 104, 2);
      rightEye.addKeyframe(175, 94, 37, 104, 2);
      faceBottom.addKeyframe(36, 203, 204, 63, 2);
      velocityFunc = easeInOut;
      anim_time = 2 * FRAME_RATE;
      break;
    }
    
    case BLINK: {
      keyframe snapshot = forehead.getCurrentKeyFrame();
      forehead.addKeyframe(snapshot.x, snapshot.y, snapshot.w, snapshot.h + 120, 0.5);
      forehead.addKeyframe(snapshot.x, snapshot.y, snapshot.w, snapshot.h, 0.5);
      velocityFunc = linear;
      anim_time = 0.5 * FRAME_RATE;
      break;
    }

    default: {
      forehead.addKeyframe(43, 0, 156, 69, 2);
      leftEyelid.addKeyframe(42, 20, 50, 50, 2, false);
      rightEyelid.addKeyframe(200, 20, 50, 50, 2, false);
      leftEye.addKeyframe(46, 69, 37, 104, 2);
      rightEye.addKeyframe(158, 69, 37, 104, 2);
      faceBottom.addKeyframe(18, 178, 204, 63, 2);
      velocityFunc = easeOut;
      anim_time = 2 * FRAME_RATE;
      break;
    }
  }
}

void Face::update()
{
  forehead.update(velocityFunc);
  leftEyelid.update(velocityFunc);
  rightEyelid.update(velocityFunc);
  leftEye.update(velocityFunc);
  rightEye.update(velocityFunc);
  faceBottom.update(velocityFunc);
  menu.update(velocityFunc);

  anim_iterator++;
  Serial.print("Global iterating: ");
  Serial.print(anim_iterator);
  Serial.print("/");
  Serial.println(anim_time);
}

void Face::draw(bool sel)
{
  spr[sel].fillScreen(PRIMARY_COLOR);
  leftEye.draw(sel);
  rightEye.draw(sel);
  leftEyelid.draw(sel);
  rightEyelid.draw(sel);
  faceBottom.draw(sel);
  forehead.draw(sel);

  menu.draw(sel);

  tft.pushImageDMA(0, sel * TFT_HEIGHT / 2, TFT_WIDTH, TFT_HEIGHT / 2, sprPtr[sel]);
  if (sel) update();
}

