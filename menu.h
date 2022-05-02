#include "main.h"

#define FRAME_RATE      15

#define PI              3.14159

#define CORNER_RADIUS   5

#define PRIMARY_COLOR   TFT_WHITE
#define SECONDARY_COLOR TFT_BLUE

typedef enum {
  NEUTRAL,
  HAPPY,
  SAD,
  BLINK,
  MENU,
} faceState_t;

typedef struct keyframe {
  int x,
  int y,
  int w,
  int h,
  bool visible,
  int timestamp
} keyframe;

float linear(int t, int i);

float easeIn(int t, int i);

float easeOut(int t, int i);

float easeInOut(int t, int i);

class UIElement {
  protected:
    string name;
    int x;
    int y;
    int w;
    int h;
    bool visible;
    keyframe keyframes[5];
    int head;
    int tail;
    int target;

  public:
    UIElement(int init_x = 0, int init_y = 0, int init_w = 0, int init_h = 0, string init_name = "");
    void move(int t, int i, float (*velocityFunc)(int, int));
    void move(int delta_x, int delta_y);
    void scale(int t, int i, float (*velocityFunc)(int, int), int align = -1, int justify = -1);
    void update(int t, int i, float (*velocityFunc)(int, int), int align = -1, int justify = -1);
    void addKeyframe(int new_x, int new_y, int new_w, int new_h, int timestamp, bool new_visible = true);
    keyframe getCurrentKeyFrame() const;
    void setVisible(bool isVisible)
    {
      visible = isVisible;
    }
    virtual void draw();

    static int anim_iterator;
    static int anim_time;
};

class UIElGroup : public UIElement {
  protected:
    UIElement** elements;
    int size;
    int amount;

  public:
    UIElGroup() : UIElement() { amount = 0; };
    UIElGroup(string init_name, int init_size);
    ~UIElGroup();
    int getSize() const;
    void add(UIElement* element);
    void remove(UIElement* target);
    void updateAttr();
    UIElement* operator[](int i);
    UIElement* operator[](string query);
    void move(int t, int i, float (*velocityFunc)(int, int)) override;
    void draw();
};

class Block : public UIElement {
  public:
    Block(int init_x, int init_y, int init_w, int init_h) : UIElement(init_x, init_y, init_w, init_h) { };
    void draw();
};

class Rounded : public UIElement {
  private:
    bool primary;
  public:
    Rounded(int init_x, int init_y, int init_w, int init_h, bool isPrimary) : UIElement(init_x, init_y, init_w, init_h)
    {
      primary = isPrimary;
    };
    void draw();
};

class Circle : public UIElement {
  private:
    bool filled;

  public:
    Circle(int init_x, int init_y, int init_w, int init_h, bool isFilled) : UIElement(init_x, init_y, init_w, init_h)
    {
      filled = isFilled;
    }
    void draw();
};

class ScreenCol : public UIElGroup {
  private:
    int colIndex;
  public:
    ScreenCol();
    ScreenCol(string init_name, int init_size);
    void navigateTo(int i);
    int getColIndex() const;
    void draw() override;
}

class ScreenRow : public UIElGroup {
  private:
    int rowIndex;
  public:
    ScreenRow();
    ScreenRow(string init_name, int init_size);
    void navigateTo(int i);
    void navigateTo(int row, int col);
    int navigateTo(string row_name);
    void navigateTo(string row_name, string col_name);
    void navigateTo(char dir);
    void draw() override;
}