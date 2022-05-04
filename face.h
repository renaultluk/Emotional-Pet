#ifndef FACE_H
#define FACE_H

#include "main.h"
#include "menu.h"

class Eyelid : public UIElement {
  private:
    bool dir_right; // true if vertical edge is on right side, false if on left
  
  public:
    Eyelid(int16_t init_x, int16_t init_y, int16_t init_w, int16_t init_h, bool right) : UIElement(init_x, init_y, init_w, init_h)
    {
      dir_right = right;
      visible = false;
    };
    void setDir(bool newDir) { dir_right = newDir; };
    void draw();
};

class Face {
  private:
    Block forehead;
    Eyelid leftEyelid;
    Eyelid rightEyelid;
    Rounded leftEye;
    Rounded rightEye;
    Block faceBottom;

    ScreenRow menu;
    
    faceState_t faceState;

    float (*velocityFunc)(int, int);

  public:
    Face();
    void changeFaceState(faceState_t newFaceState);
    void update();
    void draw();
};

int UIElement::anim_iterator = 0;
int UIElement::anim_time = 0;

#endif