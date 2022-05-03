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

UIElement::UIElement(int init_x, int init_y, int init_w, int init_h, string init_name)
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

void UIElement::scale(int t, int i, float (*velocityFunc)(int, int))
{
  float coef = velocityFunc(t, i);
  int delta_w = coef * (keyframes[target].w - w);
  int delta_h = coef * (keyframes[target].h - h);
  w = (abs(delta_w) > abs(keyframes[target].w - w)) ? keyframes[target].w : keyframes[head].w + delta_w;  // prevent overshooting
  h = (abs(delta_h) > abs(keyframes[target].h - h)) ? keyframes[target].h : keyframes[head].h + delta_h;
}

void UIElement::update(float (*velocityFunc)(int, int))
{
  if (head == tail) return;
  if (iterator == 0 && keyframes[target].visible == true) visible = true;
  int t = keyframes[target].timestamp;
  if (iterator == t)
  {
    if (keyframes[target].visible == false) visible = false;
    head++;
    target = (head + 1) % 5;
    iterator = 0;
  } else {
    if ((keyframes[target].w != w) || (keyframes[target].h != h)) scale(t, i, velocityFunc);
    if ((keyframes[target].x != x) || (keyframes[target].y != y)) move(t, i, velocityFunc);
    iterator++;
  }
}

void UIElement::addKeyframe(int new_x, int new_y, int new_w, int new_h, float new_timestamp, bool new_visible)
{
  tail = (tail + 1) % 5;
  keyframes[tail] = {new_x, new_y, new_w, new_h, new_visible, new_timestamp * FRAME_RATE};
}

keyframe UIElement::getCurrentKeyframe() const
{
  return keyframes[head];
}

UIElGroup::UIElGroup(string init_name, int init_size) : UIElement(init_name)
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
  
  if (amount + 1 < size)
  {
    elements[amount] = element;
    amount++;
    updateAttr();
    return;
  }
  UIElGroup** newList = new UIElGroup* [size * 2];
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

UIElement* UIElGroup::operator[](string query)
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

void Image::draw()
{
  if (visible) {
    // Open the named file (the Jpeg decoder library will close it)
    File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
  
    if ( !jpegFile ) {
      Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
      return;
    }

    Serial.println("===========================");
    Serial.print("Drawing file: "); Serial.println(filename);
    Serial.println("===========================");

    // Use one of the following methods to initialise the decoder:
    bool decoded = JpegDec.decodeSdFile(jpegFile);  // Pass the SD file handle to the decoder,
    //bool decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)

    if (decoded) {
      // render the image onto the screen at given coordinates
      jpegRender(x, y);
    }
    else {
      Serial.println("Jpeg file format not supported!");
    }
  }
}

void jpegRender(int xpos, int ypos) {

  //jpegInfo(); // Print information from the JPEG file (could comment this line out)

  uint *pImg;
  uint mcu_w = JpegDec.MCUWidth;
  uint mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  bool swapBytes = tft.getSwapBytes();
  tft.setSwapBytes(true);
  
  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
  uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // Fetch data from the file, decode and display
  while (JpegDec.read()) {    // While there is more data in the file
    pImg = JpegDec.pImage ;   // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)

    // Calculate coordinates of top left corner of current MCU
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    else if ( (mcu_y + win_h) >= tft.height())
      JpegDec.abort(); // Image has run off bottom of screen so abort decoding
  }

  tft.setSwapBytes(swapBytes);

  showTime(millis() - drawTime); // These lines are for sketch testing only
}

void Text::draw()
{
  if (primary) tft.setTextColor(PRIMARY_COLOR);
  else tft.setTextColor(SECONDARY_COLOR);
  tft.drawString(content, x, y, 4);
}

ScreenCol::ScreenCol() : UIElGroup()
{
  colIndex = 0;
}

ScreenCol::ScreenCol(string init_name, int init_size) : UIElGroup(init_name, init_size)
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
  elements[colIndex]->draw();
}

ScreenRow::ScreenRow() : UIElGroup()
{
  rowIndex = 0;
  visible = false;
}

ScreenRow::ScreenRow(string init_name, int init_size) : UIElGroup(init_name, init_size)
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

void ScreenRow::navigateTo(string row_name)
{
  for (int i = 0; i < size; i++)
  {
    if (elements[i]->name == row_name)
    {
      navigateTo(i);
    }
  }
}

void ScreenRow::navigateTo(string row_name, string col_name)
{
  navigateTo(row_name);
  ScreenCol curr = elements[rowIndex];
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
      elements[rowIndex]->navigateTo(newIndex);
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
  if (visible) elements[rowIndex]->draw();
}