#include "menu.h"

float linear(int t, int i)
{
  return 1/t;
}

float easeIn(int t, int i)
{
  return (i/t) * (i/t);   // Quadratic Ease In
}

float easeOut(int t, int i)
{
  return 1 - (1 - i/t) * (1 - i/t);  // Quadratic Ease Out
}

float easeInOut(int t, int i)
{
  float func_x = (1 + i/t) * PI;
  float res = cos(func_x);  // Sine Ease-In-Out
  return res;
}

UIElement::UIElement(int init_x, int init_y, int init_w, int init_h)
{
  x = init_x;
  y = init_y;
  w = init_w;
  h = init_h;
  target_x = init_x;
  target_y = init_y;
  target_w = init_w;
  target_h = init_h;
  visible = true;

  keyframes[0] = {init_x, init_y, init_w, init_h, 0};
  for (int i = 1; i < 5; i++)
  {
    keyframes[i] = nullptr;
  }
  head = 0;
  tail = 0;
  target = 0;
}

void UIElement::move(int t, int i, float (*velocityFunc)(int, int))
{
  float coef = velocityFunc(t, i);
  int delta_x = coef * (keyframes[target].x - keyframes[head].x);
  int delta_y = coef * (keyframes[target].y - keyframes[head].y);
  x = (abs(delta_x) > abs(keyframes[target].x - x)) ? keyframes[target].x : keyframes[head].x + delta_x;  // prevent overshooting
  y = (abs(delta_y) > abs(keyframes[target].y - y)) ? keyframes[target].y : keyframes[head].y + delta_y;
}

void UIElement::move(int delta_x, int delta_y)
{
  x += delta_x;
  y += delta_y;
}

void UIElement::scale(int t, int i, float (*velocityFunc)(int, int), int align, int justify)
{
  float coef = velocityFunc(t, i);
  int delta_w = coef * (keyframes[target].w - w);
  int delta_h = coef * (keyframes[target].h - h);
  delta_w = (abs(delta_w) > abs(keyframes[target].w - w)) ? keyframes[target].w - w : delta_w;  // prevent overshooting
  delta_h = (abs(delta_h) > abs(keyframes[target].h - h)) ? keyframes[target].h - h : delta_h;
  w += delta_w;
  h += delta_h;
  switch (align)
  {
    case 0:   // center alignment
      y -= delta_h / 2;
      break;
    case 1:   // bottom alignment
      y -= delta_h;
      break;
    default:  // top alignment
      break;
  }
  switch (justify)
  {
    case 0:   // center justification
      x -= delta_w / 2;
      break;
    case 1:   // right justification
      x -= delta_w;
      break;
    default:  // left justification
      break; 
  }
}

void UIElement::update(int t, int i, float (*velocityFunc)(int, int), int align, int justify)
{
  if ((keyframes[target].w != w) || (keyframes[target].h != h)) scale(t, i, velocityFunc, align, justify);
  else if ((keyframes[target].x != x) || (keyframes[target].y != y)) move(t, i, velocityFunc);
  if (i == t)
  {
    head++;
    if (head != tail) target = (head + 1) % 5;
  }
}

void UIElement::addKeyframe(int new_x, int new_y, int new_w, int new_h, int new_timestamp)
{
  tail = (tail + 1) % 5;
  keyframes[tail] = {new_x, new_y, new_w, new_h, new_timestamp};
}

UIElGroup::~UIElGroup()
{
  delete[] elements;
}

int UIElGroup::getSize() const
{
  return size;
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
  size--;
  updateAttr();
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
    if (elements[i].x < min_x) min_x = elements[i].x;
    if (elements[i].y + elements[i].h > max_y) max_y = elements[i].y + elements[i].h;
    if (elements[i].y < min_y) min_y = elements[i].y;
  }

  x = min_x;
  y = min_y;
  w = max_x - min_x;
  h = max_y - min_y;
}

void UIElGroup::operator[](int i)
{
  return elements[i];
}

void UIElGroup::move()
{
  float coef = velocityFunc(t, i);
  int delta_x = coef * (keyframes[target].x - keyframes[head].x);
  int delta_y = coef * (keyframes[target].y - keyframes[head].y);
  delta_x = (abs(delta_x) > abs(keyframes[target].x - x)) ? keyframes[target].x - x : delta_x;  // prevent overshooting
  delta_y = (abs(delta_y) > abs(keyframes[target].y - y)) ? keyframes[target].y - y : delta_y;

  for (int i = 0; i < size; i++)
  {
    elements[i].move(delta_x, delta_y);
  }
}


void Block::draw()
{
  if (visible) tft.fillRect(x, y, w, h, PRIMARY_COLOR);
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
  else tft.drawCircle(x, y, w / 2, PRIMARY_COLOR);
}

ScreenCol::ScreenCol() : UIElGroup()
{
  colIndex = 0;
}

void ScreenCol::navigateTo(int i)
{
  colIndex = i;
}

int getColIndex() const
{
  return colIndex;
}

void ScreenCol::draw()
{
  elements[colIndex].draw();
}

ScreenRow::ScreenRow() : UIElGroup()
{
  rowIndex = 0;
  visible = false;
}

void ScreenRow::navigateTo(int i)
{
  rowIndex = i;
}

void ScreenRow::navigateTo(int row, int col)
{
  rowIndex = row;
  elements[row]->navigateTo(col);
}

void ScreenRow::navigateTo(char dir)
{
  switch (char)
  {
    case 'u':
      UIElement* curr = elements[rowIndex];
      int newIndex = curr->getColIndex() - 1;
      if (newIndex > 0) {
        if (newIndex < curr->getSize());
        else newIndex = curr->getSize();
      } else {
        newIndex = 0;
      }
      curr->navigateTo(newIndex);
      break;
    case 'l':
      int newIndex = (rowIndex - 1) % size;
      navigateTo(newIndex);
      break;
    case 'd':
      UIElement* curr = elements[rowIndex];
      int newIndex = curr->getColIndex() + 1;
      if (newIndex > 0) {
        if (newIndex < curr->getSize());
        else newIndex = curr->getSize();
      } else {
        newIndex = 0;
      }
      elements[rowIndex].navigateTo(newIndex);
      break;
    case 'r':
      int newIndex = (rowIndex + 1) % size;
      navigateTo(newIndex);
      break;
    default:
      break;
  }
}

void ScreenRow::draw()
{
  if (visible) elements[rowIndex].draw();
}