#include "menu.h"

float linear(int t, int i)
{
  float a = (i + 1) * 1.0;
  float b = t * 1.0;
  return a/b;
}

float easeIn(int t, int i)
{
  return (i/t) * (i/t);   // Quadratic Ease In
}

float easeOut(int t, int i)
{
  float i_flt = i * 1.0;
  float t_flt = t * 1.0;
  return 1 - (1 - i_flt/t_flt) * (1 - i_flt/t_flt);  // Quadratic Ease Out
}

float easeInOut(int t, int i)
{
  float func_x = (1 + i/t) * PI;
  float res = cos(func_x);  // Sine Ease-In-Out
  return res;
}

UIElement::UIElement(int16_t init_x, int16_t init_y, int16_t init_w, int16_t init_h, String init_name)
{
  name = init_name;
  x = init_x;
  y = init_y;
  w = init_w;
  h = init_h;
  visible = true;

  keyframes[0] = {init_x, init_y, init_w, init_h, 0};
  head = 0;
  tail = 0;
  target = 0;
}

void UIElement::move(int t, int i, float (*velocityFunc)(int, int))
{
  float coef = linear(t, i);
  Serial.print("coef: ");
  Serial.println(coef);
  int delta_x = coef * (keyframes[target].x - keyframes[head].x);
  int delta_y = coef * (keyframes[target].y - keyframes[head].y);
  x = (abs(delta_x) > abs(keyframes[target].x - x)) ? keyframes[target].x : keyframes[head].x + delta_x;  // prevent overshooting
  y = (abs(delta_y) > abs(keyframes[target].y - y)) ? keyframes[target].y : keyframes[head].y + delta_y;
}

void UIElement::move(int16_t delta_x, int16_t delta_y)
{
  x += delta_x;
  y += delta_y;
}

void UIElement::scale(int t, int i, float (*velocityFunc)(int, int))
{
  float coef = linear(t, i);
  int delta_w = coef * (keyframes[target].w - keyframes[head].w);
  int delta_h = coef * (keyframes[target].h - keyframes[head].h);
  w = (abs(delta_w) > abs(keyframes[target].w - w)) ? keyframes[target].w : keyframes[head].w + delta_w;  // prevent overshooting
  h = (abs(delta_h) > abs(keyframes[target].h - h)) ? keyframes[target].h : keyframes[head].h + delta_h;
  Serial.print("coef: ");
  Serial.println(coef);
  Serial.print("\t h:");
  Serial.print(h);
  Serial.print("\t delta h:");
  Serial.println(delta_h);
}

void UIElement::update(float (*velocityFunc)(int, int))
{
  if (head == tail)   // No outstanding keyframes
  {
    // Serial.println("no change");
    return;
  }
  if (iterator == 0 && head != tail && anim_iterator == 0)  // Starting animation
  {
    target = (target + 1) % 10;
    // Serial.print("new target \t x = ");
    // Serial.print(keyframes[target].x);
    // Serial.print("\t y = ");
    // Serial.print(keyframes[target].y);
    // Serial.print("\t w = ");
    // Serial.print(keyframes[target].w);
    // Serial.print("\t h = ");
    // Serial.print(keyframes[target].h);
    // Serial.print("\t time = ");
    // Serial.println(keyframes[target].timestamp);
    Serial.print("Inital h:");
    Serial.print(keyframes[head].h);
    Serial.print("Target h:");
    Serial.println(keyframes[target].h);
    if (keyframes[target].timestamp > anim_time) anim_time = keyframes[target].timestamp;
    if (keyframes[target].visible == true) setVisible(true);
  }
  int t = keyframes[target].timestamp;
  if (iterator >= t + 1)    // Finish animation
  {
    Serial.print("FINISHED FROM ");
    Serial.print(head);
    Serial.print(" TO ");
    Serial.println(target);
    if (keyframes[target].visible == false) visible = false;
    head = (head + 1) % 10;
    target = (head + 1) % 5;
    if (iterator >=  anim_time + 1) {   // animation done globally
      anim_iterator = 0;
      anim_time = 0;
    }
    iterator = 0;
  } else {
    Serial.print("Iterating ");
    Serial.print(iterator);
    Serial.print("/");
    Serial.println(t);

    if ((keyframes[target].w != w) || (keyframes[target].h != h)) scale(t, iterator, velocityFunc);
    if ((keyframes[target].x != x) || (keyframes[target].y != y)) move(t, iterator, velocityFunc);
    iterator++;
  }
}

void UIElement::addKeyframe(int16_t new_x, int16_t new_y, int16_t new_w, int16_t new_h, float new_timestamp, bool new_visible)
{
  tail = (tail + 1) % 10;
  keyframes[tail] = {new_x, new_y, new_w, new_h, new_visible, (int)(new_timestamp * FRAME_RATE)};
  // Serial.print("{");
  // for (int i = 0; i < 10; i++) {
  //   Serial.print(keyframes[i].h);
  //   Serial.print(",");
  //   if (i == tail) Serial.print("//");
  // }
  // Serial.println("}");
}

keyframe UIElement::getCurrentKeyFrame() const
{
  return keyframes[head];
}

UIElGroup::UIElGroup(String init_name, int init_size) : UIElement(0, 0, 0, 0, init_name)
{
  size = init_size;
  amount = 0;
  elements = new UIElement* [size];
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
  
  if (amount < size)
  {
    elements[amount] = element;
    amount++;
    updateAttr();
    return;
  }
  UIElement** newList = new UIElement* [size * 2];
  for (int i = 0; i < size; i++)
  {
    newList[i] = elements[i];
  }
  newList[size] = element;
  elements = newList;
  size *= 2;
  amount++;
  updateAttr();
}

void UIElGroup::remove(UIElement* element)
{
  UIElement** newList = new UIElement* [size - 1];
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
  int min_x = elements[0]->x;
  int min_y = elements[0]->y;
  int max_x = elements[0]->x;
  int max_y = elements[0]->y;

  for (int i = 1; i < amount; i++)
  {
    if (elements[i]->x + elements[i]->w > max_x) max_x = elements[i]->x + elements[i]->w;
    if (elements[i]->x < min_x) min_x = elements[i]->x;
    if (elements[i]->y + elements[i]->h > max_y) max_y = elements[i]->y + elements[i]->h;
    if (elements[i]->y < min_y) min_y = elements[i]->y;
  }

  x = min_x;
  y = min_y;
  w = max_x - min_x;
  h = max_y - min_y;
}

UIElement* UIElGroup::operator[](int i)
{
  return elements[i];
}

UIElement* UIElGroup::operator[](String query)
{
  for (int i = 0; i < size; i++)
  {
    if (elements[i]->name == query) return elements[i];
  }
  return nullptr;
}


void UIElGroup::update(float (*velocityFunc)(int, int))
{
  int t = keyframes[target].timestamp;
  if ((iterator != t) && (head != target)) {
    float coef = velocityFunc(t, iterator);
    int delta_x = coef * (keyframes[target].x - keyframes[head].x);
    int delta_y = coef * (keyframes[target].y - keyframes[head].y);
    delta_x = (abs(delta_x) > abs(keyframes[target].x - x)) ? keyframes[target].x - x : delta_x;  // prevent overshooting
    delta_y = (abs(delta_y) > abs(keyframes[target].y - y)) ? keyframes[target].y - y : delta_y;

    for (int i = 0; i < size; i++)
    {
      elements[i]->move(delta_x, delta_y);
    }
    iterator++;
  }
}

void UIElGroup::draw(bool sel)
{
  for (int i = 0; i < amount; i++)
  {
    elements[i]->draw(sel);
  }
}


void Block::draw(bool sel)
{
  if (visible) spr[sel].fillRect(x, y, w, h, PRIMARY_COLOR);
}

void Rounded::draw(bool sel)
{
  if (visible) 
  {
    if (primary) spr[sel].fillRoundRect(x, y, w, h, CORNER_RADIUS, PRIMARY_COLOR);
    else spr[sel].fillRoundRect(x, y, w, h, CORNER_RADIUS, SECONDARY_COLOR);
  }
}

void Circle::draw(bool sel)
{
  if (filled) spr[sel].fillCircle(x, y, w / 2, PRIMARY_COLOR);
  else spr[sel].drawCircle(x, y, w / 2, PRIMARY_COLOR);
}

void Image::draw(bool sel)
{
  if (visible) {
    // Open the named file (the Jpeg decoder library will close it)
    File jpegFile = SD.open( src, FILE_READ);  // or, file handle reference for SD library
  
    if ( !jpegFile ) {
      Serial.print("ERROR: File \""); Serial.print(src); Serial.println ("\" not found!");
      return;
    }

    Serial.println("===========================");
    Serial.print("Drawing file: "); Serial.println(src);
    Serial.println("===========================");

    // Use one of the following methods to initialise the decoder:
    bool decoded = JpegDec.decodeSdFile(jpegFile);  // Pass the SD file handle to the decoder,
    //bool decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)

    if (decoded) {
      // render the image onto the screen at given coordinates
      jpegRender(x, y, sel);
    }
    else {
      Serial.println("Jpeg file format not supported!");
    }
  }
}

void jpegRender(int16_t xpos, int16_t ypos, bool sel) {

  // //jpegInfo(); // Print information from the JPEG file (could comment this line out)

  // uint16_t *pImg;
  // uint16_t mcu_w = JpegDec.MCUWidth;
  // uint16_t mcu_h = JpegDec.MCUHeight;
  // uint32_t max_x = JpegDec.width;
  // uint32_t max_y = JpegDec.height;

  // bool swapBytes = spr[sel].getSwapBytes();
  // spr[sel].setSwapBytes(true);
  
  // // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // // Typically these MCUs are 16x16 pixel blocks
  // // Determine the width and height of the right and bottom edge image blocks
  // uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
  // uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

  // // save the current image block size
  // uint32_t win_w = mcu_w;
  // uint32_t win_h = mcu_h;

  // // record the current time so we can measure how long it takes to draw an image
  // uint32_t drawTime = millis();

  // // save the coordinate of the right and bottom edges to assist image cropping
  // // to the screen size
  // max_x += xpos;
  // max_y += ypos;

  // // Fetch data from the file, decode and display
  // while (JpegDec.read()) {    // While there is more data in the file
  //   pImg = JpegDec.pImage ;   // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)

  //   // Calculate coordinates of top left corner of current MCU
  //   int mcu_x = JpegDec.MCUx * mcu_w + xpos;
  //   int mcu_y = JpegDec.MCUy * mcu_h + ypos;

  //   // check if the image block size needs to be changed for the right edge
  //   if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
  //   else win_w = min_w;

  //   // check if the image block size needs to be changed for the bottom edge
  //   if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
  //   else win_h = min_h;

  //   // copy pixels into a contiguous block
  //   if (win_w != mcu_w)
  //   {
  //     uint16_t *cImg;
  //     int p = 0;
  //     cImg = pImg + win_w;
  //     for (int h = 1; h < win_h; h++)
  //     {
  //       p += mcu_w;
  //       for (int w = 0; w < win_w; w++)
  //       {
  //         *cImg = *(pImg + w + p);
  //         cImg++;
  //       }
  //     }
  //   }

  //   // calculate how many pixels must be drawn
  //   uint32_t mcu_pixels = win_w * win_h;

  //   // draw image MCU block only if it will fit on the screen
  //   if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
  //     tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
  //   else if ( (mcu_y + win_h) >= tft.height())
  //     JpegDec.abort(); // Image has run off bottom of screen so abort decoding
  // }

  // tft.setSwapBytes(swapBytes);

  // showTime(millis() - drawTime); // These lines are for sketch testing only
}

void Text::draw(bool sel)
{
  if (primary) spr[sel].setTextColor(PRIMARY_COLOR);
  else spr[sel].setTextColor(SECONDARY_COLOR);
  spr[sel].drawString(content, x, y, 4);
}

void ListItem::draw(bool sel)
{
  if (visible)
  {
    spr[sel].setTextColor(PRIMARY_COLOR);
    spr[sel].drawString(title, x + 7, y + 5, 4);
    spr[sel].setTextColor(SECONDARY_COLOR);
    spr[sel].drawString(subtitle, x + 98, y + 5, 4);
    spr[sel].drawFastHLine(x, y + h, w, SECONDARY_COLOR);
  }
}

ScreenCol::ScreenCol() : UIElGroup()
{
  colIndex = 0;
}

ScreenCol::ScreenCol(String init_name, int init_size) : UIElGroup(init_name, init_size)
{
  colIndex = 0;
}

void ScreenCol::navigateTo(int i)
{
  colIndex = i;
}

int ScreenCol::getColIndex() const
{
  return colIndex;
}

void ScreenCol::draw(bool sel)
{
  elements[colIndex]->draw(sel);
}

ScreenRow::ScreenRow() : UIElGroup()
{
  rowIndex = 0;
  visible = false;
}

ScreenRow::ScreenRow(String init_name, int init_size) : UIElGroup(init_name, init_size)
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
  static_cast<ScreenCol*>(elements[row])->navigateTo(col);
}

void ScreenRow::navigateTo(String row_name)
{
  for (int i = 0; i < size; i++)
  {
    if (elements[i]->name == row_name)
    {
      navigateTo(i);
    }
  }
}

void ScreenRow::navigateTo(String row_name, String col_name)
{
  navigateTo(row_name);
  ScreenCol* curr = static_cast<ScreenCol*>(elements[rowIndex]);
  for (int i = 0; i < curr->getSize(); i++)
  {
    if ((*curr)[i]->name == col_name)
    {
      curr->navigateTo(i);
    }
  }
}

void ScreenRow::navigateTo(char dir)
{
  switch (dir)
  {
    case 'u':
      {
        ScreenCol* curr = static_cast<ScreenCol*>(elements[rowIndex]);
        int newIndex = curr->getColIndex() - 1;
        if (newIndex > 0) {
          if (newIndex < curr->getSize());
          else newIndex = curr->getSize();
        } else {
          newIndex = 0;
        }
        curr->navigateTo(newIndex);
        break;
      }
    case 'l':
      {
        int newIndex = (rowIndex - 1) % size;
        navigateTo(newIndex);
        break;
      }
    case 'd':
      {
        ScreenCol* curr = static_cast<ScreenCol*>(elements[rowIndex]);
        int newIndex = curr->getColIndex() + 1;
        if (newIndex > 0) {
          if (newIndex < curr->getSize());
          else newIndex = curr->getSize();
        } else {
          newIndex = 0;
        }
        curr->navigateTo(newIndex);
        break;
      }
    case 'r':
      {
        int newIndex = (rowIndex + 1) % size;
        navigateTo(newIndex);
        break;
      }
    default:
      break;
  }
}

void ScreenRow::draw(bool sel)
{
  if (visible) elements[rowIndex]->draw(sel);
}