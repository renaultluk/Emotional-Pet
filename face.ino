#include "face.h"

float linear(int t, int i)
{
  float res = 1/t;
  if (i == t)
  {
    i = 0;
    t = 0;
  }
  return res;
}

float easeIn(int t, int i)
{
  float res = (i/t) * (i/t);  // Quadratic Ease In
  if (i == t)
  { 
    i = 0;
    t = 0;
  }
  return res;
}

float easeOut(int t, int i)
{
  float res = 1 - (1 - i/t) * (1 - i/t);  // Quadrtic Ease Out
  if (i == t)
  {
    i = 0;
    t = 0;
  }
  return res;
}

float easeInOut(int t, int i)
{
  float func_x = (1 + i/t) * PI;
  float res = cos(func_x);  // Sine Ease-In-Out
  if (i == t)
  {
    i = 0;
    t = 0;
  }
  return res;
}

void UIElement::move(int t, int i, float (*velocityFunc)(int, int))
{
  float coef = velocityFunc(t, i);
  int delta_x = coef * (target_x - x);
  int delta_y = coef * (target_y - y);
  x = (abs(delta_x) > abs(target_x - x)) ? target_x : x + delta_x;  // prevent overshooting
  y = (abs(delta_y) > abs(target_y - y)) ? target_y : y + delta_y;
}

void UIElement::scale(int t, int i, float (*velocityFunc)(int, int), int align, int justify)
{
  float coef = velocityFunc(t, i);
  int delta_w = coef * (target_w - w);
  int delta_h = coef * (target_h - h);
  delta_w = (abs(delta_w) > abs(target_w - w)) ? target_w - w : delta_w;  // prevent overshooting
  delta_h = (abs(delta_h) > abs(target_h - h)) ? target_h - h : delta_h;
  w += delta_w;
  h += delta_h;
  switch (align)
  {
    case 0:   // center alignment
      y -= delta_h / 2;
      target_y = y;
      break;
    case 1:   // bottom alignment
      y -= delta_h;
      target_y = y;
      break;
    default:  // top alignment
      break;
  }
  switch (justify)
  {
    case 0:   // center justification
      x -= delta_w / 2;
      target_x = x;
      break;
    case 1:   // right justification
      x -= delta_w;
      target_x = x;
      break;
    default:  // left justification
      break; 
  }
}

void UIElement::transform(int t, int i, float (*velocityFunc)(int, int), int align, int justify)
{
  if ((target_w != w) || (target_h != h)) scale(t, i, velocityFunc, align, justify);
  else if ((target_x != x) || (target_y != y)) move(t, i, velocityFunc);
}

UIElGroup::~UIElGroup()
{
  delete[] elements;
}

void UIElGroup::add(UIElement* element)
{
  UIElGroup** newList = new UIElGroup* [size + 1];
  for (int i = 0; i < size; i++)
  {
    newList[i] = elements[i];
  }
  newList[size] = element;
  elements = newList;
  size++;
  updateAttr();
}

void UIElGroup::remove(UIElement* element)
{
  UIElGroup** newList = new UIElGroup* [size - 1];
  int j = 0;
  for (int i = 0; i < size; i++)
  {
    if (elements[i] == element)
    {
      newList[j] = elements[i];
      j++;
    }
    elements[i] = nullptr;
  }
  elements = newList;
}

void UIElGroup::updateAttr()
{
  int min_x = elements[i].x;
  int min_y = elements[i].y;
  int max_x = elements[i].x;
  int max_y = elements[i].y;

  for (int i = 1; i < size; i++)
  {
    if (elements[i].x + elements[i].w > max_x) max_x = elements[i].x + elements[i].w;
    if (elements[i].x < min_x) min_x = elements [i].x;
    if (elements[i].y + elements[i].h > max_y) max_y = elements[i].y + elements[i].h;
    if (elements[i].y < min_y) min_y = elements [i].y;
  }

  x = min_x;
  y = min_y;
  w = max_x - min_x;
  h = max_y - min_y;
}

void UIElGroup::move()
{
  float coef = velocityFunc(t, i);
  int delta_x = coef * (target_x - x);
  int delta_y = coef * (target_y - y);
  delta_x = (abs(delta_x) > abs(target_x - x)) ? target_x - x : delta_x;  // prevent overshooting
  delta_y = (abs(delta_y) > abs(target_y - y)) ? target_y - y : delta_y;

  for (int i = 0; i < size; i++)
  {
    elements[i].move(delta_x, delta_y);
  }
}


void Block::draw()
{
  if (visible) tft.fillRect(x, y, w, h, PRIMARY_COLOR);
}


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

void Rounded::draw()
{
  if (visible) 
  {
    if (primary) tft.fillRoundRect(x, y, w, h, CORNER_RADIUS, PRIMARY_COLOR);
    else tft.fillRoundRect(x, y, w, h, CORNER_RADIUS, SECONDARY_COLOR);
  }
}

void Circle::draw()
{
  if (filled) tft.fillCircle(x, y, w / 2, PRIMARY_COLOR);
  else void Rounded::draw()
{
  if (visible) 
  {
    if (primary) tft.fillCircle(x, y, w, h, CORNER_RADIUS, PRIMARY_COLOR);
    else tft.fillCircle(x, y, w, h, CORNER_RADIUS, SECONDARY_COLOR);
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
}

void Face::changeFaceState(faceState_t newFaceState)
{
  switch (newFaceState)
  {
    case NEUTRAL:
      forehead.setTarget(43, 0, 156, 69);
      leftEyelid.setTarget(42, 20, 50, 50);
      rightEyelid.setTarget(200, 20, 50, 50);
      leftEye.setTarget(46, 69, 37, 104);
      rightEye.setTarget(158, 69, 37, 104);
      faceBottom.setTarget(18, 178, 204, 63);
      anim_time = 3 * FRAME_RATE;
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
}

