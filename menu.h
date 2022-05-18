#ifndef MENU_H
#define MENU_H

#include "main.h"

#define FRAME_RATE      20

#define CORNER_RADIUS   18

#define KEYFRAME_QUEUE_SIZE 24

#define PRIMARY_COLOR   TFT_WHITE
#define SECONDARY_COLOR TFT_SKYBLUE

typedef enum {
  NEUTRAL,
  HAPPY,
  SAD,
  BLINK,
  MENU,
} faceState_t;

typedef struct keyframe {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  bool visible;
  int timestamp;
} keyframe;

int anim_iterator = 0;
int anim_time = 0;

float linear(int t, int i);

float easeIn(int t, int i);

float easeOut(int t, int i);

float easeInOut(int t, int i);

void jpegRender(int16_t xpos, int16_t ypos);

class UIElement {
  public:
    String name;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    bool visible;
    keyframe keyframes[KEYFRAME_QUEUE_SIZE];
    int head;
    int tail;
    int target;
    int iterator;

    UIElement(int16_t init_x = 0, int16_t init_y = 0, int16_t init_w = 0, int16_t init_h = 0, String init_name = "");
    void move(int t, int i, float (*velocityFunc)(int, int));
    void move(int16_t delta_x, int16_t delta_y);
    void scale(int t, int i, float (*velocityFunc)(int, int));
    virtual void update(float (*velocityFunc)(int, int));
    void addKeyframe(int16_t new_x, int16_t new_y, int16_t new_w, int16_t new_h, float new_timestamp, bool new_visible = true);
    keyframe getCurrentKeyFrame() const;
    void setVisible(bool isVisible)
    {
      visible = isVisible;
    }
    virtual void draw(bool sel) = 0;
};

class UIElGroup : public UIElement {
  protected:
    UIElement** elements;
    int size;
    int amount;

  public:
    UIElGroup() : UIElement() { amount = 0; iterator = 0; };
    UIElGroup(String init_name, int init_size);
    ~UIElGroup();
    int getSize() const;
    void add(UIElement* element);
    void remove(UIElement* target);
    void updateAttr();
    UIElement* operator[](int i);
    UIElement* operator[](String query);
    void update(float (*velocityFunc)(int, int)) override;
    void draw(bool sel);
};

class Block : public UIElement {
  public:
    Block(int16_t init_x, int16_t init_y, int16_t init_w, int16_t init_h, String name = "") : UIElement(init_x, init_y, init_w, init_h, name) { };
    void draw(bool sel);
};

class Rounded : public UIElement {
  private:
    bool primary;
  public:
    Rounded(int16_t init_x, int16_t init_y, int16_t init_w, int16_t init_h, bool isPrimary, String name = "") : UIElement(init_x, init_y, init_w, init_h)
    {
      primary = isPrimary;
    };
    void draw(bool sel);
};

class Circle : public UIElement {
  private:
    bool filled;

  public:
    Circle(int16_t init_x, int16_t init_y, int16_t init_r, bool isFilled, String name = "") : UIElement(init_x, init_y, init_r, init_r)
    {
      filled = isFilled;
    }
    void draw(bool sel);
};

class Image : public UIElement {
  private:
    String src;

  public:
    Image(int16_t init_x, int16_t init_y, String init_src) : UIElement(init_x, init_y)
    {
      src = init_src;
    }
    void draw(bool sel);
};

class Text : public UIElement {
  private:
    String content;
    bool primary;

  public:
    Text(int16_t init_x, int16_t init_y, String init_content, bool isPrimary, String name = "") : UIElement(init_x, init_y, 0, 0, name)
    {
      content = init_content;
      primary = isPrimary;
    }
    void draw(bool sel);
};

class ListItem : public UIElement {
  private:
    String title;
    String subtitle;
  
  public:
    ListItem(int16_t init_x, int16_t init_y, String init_title, String init_subtitle, String name = "") : UIElement(init_x, init_y, 130, 60, name)
    {
      title = init_title;
      subtitle = init_subtitle;
    }
    void draw(bool sel);
};

class ScreenCol : public UIElGroup {
  private:
    int colIndex;
  public:
    ScreenCol();
    ScreenCol(String init_name, int init_size);
    void navigateTo(int i);
    int getColIndex() const;
    void draw(bool sel) override;
};

class ScreenRow : public UIElGroup {
  private:
    int rowIndex;
  public:
    ScreenRow();
    ScreenRow(String init_name, int init_size);
    void navigateTo(int i);
    void navigateTo(int row, int col);
    void navigateTo(String row_name);
    void navigateTo(String row_name, String col_name);
    void navigateTo(char dir);
    void draw(bool sel) override;
};




#endif