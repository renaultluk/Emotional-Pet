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

Face::Face() : forehead(43, 0, 156, 69), leftEyelid(42, 20, 50, 50, false), rightEyelid(200, 20, 50, 50, true), leftEye(46, 69, 37, 104, false), rightEye(158, 69, 37, 104, false), faceBottom(18, 178, 204, 63), menu("menu", 4)
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
  menu.add(new ScreenCol("emotion", 3));

  UIElGroup* curr = static_cast<UIElGroup*>(menu["listen"]);
  curr->add(new UIElGroup("main", 2));
  curr->add(new UIElGroup("choose", 6));
  curr->add(new UIElGroup("audio", 9));
  curr->add(new UIElGroup("friends", 8));

  curr = static_cast<UIElGroup*>(menu["record"]);
  curr->add(new UIElGroup("main", 3));
  curr->add(new UIElGroup("record", 3));

  static_cast<UIElGroup*>(menu["settings"])->add(new UIElGroup("main", 4));

  curr = static_cast<UIElGroup*>(menu["emotion"]);
  curr->add(new UIElGroup("main", 11));
  curr->add(new UIElGroup("measure", 3));
  curr->add(new UIElGroup("feedback", 5));

  // Page Content
  curr = static_cast<UIElGroup*>(menu["listen"]);
  curr = static_cast<UIElGroup*>((*curr)["main"]);
  // curr->add(new Image(75, 75, "icons/headphones.jpg"));
  curr->add(new Text(120, 170, "Listen", true));

  curr = static_cast<UIElGroup*>(menu["listen"]);
  curr = static_cast<UIElGroup*>((*curr)["choose"]);
  curr->add(new Rounded(41, 55, 78, 130, true));
  curr->add(new Rounded(124, 55, 78, 130, true));
  // curr->add(new Image(71, 83, "icons/friends.jpg"));
  // curr->add(new Image(135, 92, "icons/explore.jpg"));
  curr->add(new Text(79, 148, "Friends", false, "frdsStr", 1));
  curr->add(new Text(163, 148, "Explore", false, "exploreStr", 1));

  // TODO: menu[listen][audio], menu[listen][friends]

  curr = static_cast<UIElGroup*>(menu["listen"]);
  curr = static_cast<UIElGroup*>((*curr)["audio"]);
  curr->add(new Rounded(41, 59, 159, 126, true));
  curr->add(new Bar(49, 112, 142, true));
  curr->add(new Bar(49, 112, 142, false, "progressBar"));
  curr->add(new Text(62, 124, "00:00", false, "audioProgress", 0));
  curr->add(new Text(185, 124, "00:00", false, "audioLength", 0));
  curr->add(new Rounded(49, 132, 68, 27, false, "replyBtn", 6));
  curr->add(new Rounded(123, 132, 68, 27, false, "frdBtn", 6));
  curr->add(new Text(84, 142, "Reply", true, "replyStr", 1));
  curr->add(new Text(155, 142, "+ Friend", true, "frdStr", 1));

  curr = static_cast<UIElGroup*>(menu["listen"]);
  curr = static_cast<UIElGroup*>((*curr)["friends"]);
  curr->add(new Text(82, 51, "Friends", true));
  curr->add(new Rounded(44, 70, 157, 116, true));
  curr->add(new ListItem(55, 70, "Test", "00:00", "frd0"));
  curr->add(new ListItem(55, 98, "", "", "frd1"));
  curr->add(new ListItem(55, 128, "", "", "frd2"));
  curr->add(new ListItem(55, 158, "", "", "frd3"));
  static_cast<ListItem*>((*curr)["frd0"])->setSelected(true);

  curr = static_cast<UIElGroup*>(menu["record"]);
  curr = static_cast<UIElGroup*>((*curr)["main"]);
  curr->add(new Circle(120, 105, 60, false));
  curr->add(new Circle(120, 105, 48, true));
  curr->add(new Text(120, 170, "Record", true));

  curr = static_cast<UIElGroup*>(menu["record"]);
  curr = static_cast<UIElGroup*>((*curr)["record"]);
  curr->add(new Circle(120, 105, 80, false));
  curr->add(new Circle(120, 105, 62, false, "innerCircle"));
  curr->add(new Text(120, 170, "00:00", true, "timer"));

  curr = static_cast<UIElGroup*>(menu["settings"]);
  curr = static_cast<UIElGroup*>((*curr)["main"]);
  // curr->add(new Image(93, 80, "icons/settings.jpg"));
  curr->add(new Circle(81, 105, 25, true));
  curr->add(new Circle(120, 105, 25, true));
  curr->add(new Circle(157, 105, 25, true));
  curr->add(new Text(120, 170, "Settings", true));

  curr = static_cast<UIElGroup*>(menu["emotion"]);
  curr = static_cast<UIElGroup*>((*curr)["main"]);
  curr->add(new Text(120, 62, "Are you feeling", true, "feel1", 1));
  curr->add(new Text(120, 89, "relaxed/stressed?", true, "feel2", 1));
  curr->add(new Circle(80, 156, 57, true));
  curr->add(new Circle(162, 156, 57, true));
  curr->add(new Rounded(64, 136, 9, 23, false, "eye", 4));
  curr->add(new Rounded(85, 136, 9, 23, false, "eye", 4));
  curr->add(new Rounded(147, 144, 9, 23, false, "eye", 4));
  curr->add(new Rounded(168, 144, 9, 23, false, "eye", 4));
  curr->add(new Block(57, 154, 43, 16));
  curr->add(new Eyelid(145, 143, 13, 11, false, true));
  curr->add(new Eyelid(166, 143, 13, 11, true, true));

  curr = static_cast<UIElGroup*>(menu["emotion"]);
  curr = static_cast<UIElGroup*>((*curr)["measure"]);
  curr->add(new Circle(120, 105, 80, false));
  curr->add(new Circle(120, 105, 62, false, "innerCircle"));
  curr->add(new Text(120, 170, "00:00", true, "timer"));

  curr = static_cast<UIElGroup*>(menu["emotion"]);
  curr = static_cast<UIElGroup*>((*curr)["feedback"]);
  curr->add(new Text(91, 87, "Your HRV: ", true, "yourhrv", 1));
  curr->add(new Text(171, 87, "00.00", true, "hrvLabel", 1));
  curr->add(new Text(120, 133, "You have", true, "have", 1));
  curr->add(new Text(79, 153, "MILD", true, "anxietyLevel", 1));
  curr->add(new Text(144, 153, "Anxiety", true, "anxiety", 1));
}

faceState_t Face::getFaceState() const
{
  return faceState;
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
      forehead.addKeyframe(23, 0, 170, 35, 2);
      leftEyelid.addKeyframe(25, -4, 50, 50, 2, false);
      rightEyelid.addKeyframe(157, -4, 50, 50, 2, false);
      leftEye.addKeyframe(26, 35, 37, 104, 2);
      rightEye.addKeyframe(117, 35, 37, 104, 2);
      faceBottom.addKeyframe(18, 110, 204, 63, 2);
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

    case MENU: {
      forehead.addKeyframe(43, 0, 156, 0, 2, false);
      leftEyelid.addKeyframe(42, 20, 50, 0, 2, false);
      rightEyelid.addKeyframe(200, 20, 50, 0, 2, false);
      leftEye.addKeyframe(30, 30, 180, 180, 2);
      rightEye.addKeyframe(30, 30, 180, 180, 2, false);
      faceBottom.addKeyframe(18, 240, 204, 0, 2, false);
      menu.setVisible(true);
      // menu.addKeyframe(30, 30, 180, 180, 2);
      velocityFunc = linear;
      anim_time = 2 * FRAME_RATE;
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
  // Serial.print("Global iterating: ");
  // Serial.print(anim_iterator);
  // Serial.print("/");
  // Serial.println(anim_time);
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

