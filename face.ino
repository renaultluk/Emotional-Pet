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

  menu = new ScreenCol[3];
}

void Face::changeFaceState(faceState_t newFaceState)
{
  switch (newFaceState)
  {
    case NEUTRAL:
      forehead.addKeyframe(43, 0, 156, 69, 2 * FRAME_RATE);
      leftEyelid.addKeyframe(42, 20, 50, 50, 2 * FRAME_RATE);
      rightEyelid.addKeyframe(200, 20, 50, 50, 2 * FRAME_RATE);
      leftEye.addKeyframe(46, 69, 37, 104, 2 * FRAME_RATE);
      rightEye.addKeyframe(158, 69, 37, 104, 2 * FRAME_RATE);
      faceBottom.addKeyframe(18, 178, 204, 63, 2 * FRAME_RATE);
      anim_time = 2 * FRAME_RATE;
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

