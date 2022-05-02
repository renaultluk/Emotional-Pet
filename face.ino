#include "face.h"

void Eyelid::draw()
{
  if (visible) {
    int horizontal_pt_x = x + w;
    int horizontal_pt_y = y;
    int vertical_pt_x = dir_right ? horizontal_pt_x : x;
    int vertical_pt_y = y + h;
    tft.fillTriangle(x, y, horizontal_pt_x, horizontal_pt_y, vertical_pt_x, vertical_pt_y, PRIMARY_COLOR);
  }
}

Face::Face()
{
  anim_iterator = 0;
  anim_time = 0;
  faceState = NEUTRAL;

  forehead = Block(43, 0, 156, 69);
  leftEyelid = Eyelid(42, 20, 50, 50, false);
  rightEyelid = Eyelid(200, 20, 50, 50, true);
  leftEye = Rounded(46, 69, 37, 104, false);
  rightEye = Rounded(158, 69, 37, 104, false);
  faceBottom = Block(18, 178, 204, 63);

  menu = new ScreenRow("menu", 3);

  menu.add(new ScreenCol("listen", 4));
  menu.add(new ScreenCol("record", 2));
  menu.add(new ScreenCol("settings", 1));

  menu["listen"]->add(new UIElGroup("main", 5));
  menu["listen"]->add(new UIElGroup("choose", 6));
  menu["listen"]->add(new UIElGroup("audio", 3));
  menu["listen"]->add(new UIElGroup("friends", 8));

  menu["record"]->add(new UIElGroup("main", 3));
  menu["record"]->add(new UIElGroup("record", 3));

  menu["settings"]->add(new UIElGroup("main", 11));
}

void Face::changeFaceState(faceState_t newFaceState)
{
  switch (newFaceState)
  {
    case NEUTRAL:
      forehead.addKeyframe(43, 0, 156, 69, 2);
      leftEyelid.addKeyframe(42, 20, 50, 50, 2);
      rightEyelid.addKeyframe(200, 20, 50, 50, 2);
      leftEye.addKeyframe(46, 69, 37, 104, 2);
      rightEye.addKeyframe(158, 69, 37, 104, 2);
      faceBottom.addKeyframe(18, 178, 204, 63, 2);
      velocityFunc = easeOut;
      anim_time = 2 * FRAME_RATE;
      break;

    case HAPPY:
      forehead.addKeyframe(23, 0, 156, 35, 2);
      leftEyelid.addKeyframe(25, -4, 50, 50, 2, false);
      rightEyelid.addKeyframe(157, -4, 50, 50, 2, false);
      leftEye.addKeyframe(26, 35, 37, 104, 2);
      rightEye.addKeyframe(117, 35, 37, 104, 2);
      faceBottom.addKeyframe(18, 178, 204, 63, 2);
      velocityFunc = easeInOut;
      anim_time = 2 * FRAME_RATE;
      break;

    case SAD:
      forehead.addKeyframe(60, 38, 156, 45, 2);
      leftEyelid.addKeyframe(83, 83, 50, 50, 2);
      rightEyelid.addKeyframe(215, 83, 50, 50, 2);
      leftEye.addKeyframe(83, 94, 37, 104, 2);
      rightEye.addKeyframe(175, 94, 37, 104, 2);
      faceBottom.addKeyframe(36, 203, 204, 63, 2);
      velocityFunc = easeInOut;
      anim_time = 2 * FRAME_RATE;
      break;
    
    case BLINK:
      keyframe snapshot = forehead.getCurrentKeyframe();
      forehead.addKeyframe(snapshot.x, snapshot.y, snapshot.w, snapshot.h + 120, 0.5);
      forehead.addKeyframe(snapshot.x, snapshot.y, snapshot.w, snapshot.h, 0.5);
      velocityFunc = linear;
      anim_time = FRAME_RATE;
      break;
  }
}

void Face::draw()
{
  tft.fillScreen(PRIMARY_COLOR);
  leftEye.draw();
  rightEye.draw();
  leftEyelid.draw();
  rightEyelid.draw();
  faceBottom.draw();
  forehead.draw();

  menu.draw();
}

