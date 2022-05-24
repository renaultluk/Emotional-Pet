#ifndef FACE_H
#define FACE_H

#include "main.h"
#include "menu.h"

class Eyelid : public UIElement {
  private:
    bool dir_right; // true if vertical edge is on right side, false if on left
  
  public:
    Eyelid(int16_t init_x, int16_t init_y, int16_t init_w, int16_t init_h, bool right, bool init_visible = false) : UIElement(init_x, init_y, init_w, init_h)
    {
      dir_right = right;
      visible = init_visible;
    };
    void setDir(bool newDir) { dir_right = newDir; };
    void draw(bool sel);
};

class Face {
  private:
    Block forehead;
    Eyelid leftEyelid;
    Eyelid rightEyelid;
    Rounded leftEye;
    Rounded rightEye;
    Block faceBottom;

    faceState_t faceState;

    float (*velocityFunc)(int, int);

  public:
    ScreenRow menu;

    Face();
    faceState_t getFaceState() const;
    void changeFaceState(faceState_t newFaceState);
    void update();
    void draw(bool sel);
};


#endif