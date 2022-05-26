#include "menu.h"
#include "SDcard.h"

float linear(int t, int i)
{
  float i_flt = i * 1.0;
  float t_flt = t * 1.0;
  return i_flt/t_flt;
}

float easeIn(int t, int i)
{
  float i_flt = i * 1.0;
  float t_flt = t * 1.0;
  return (i_flt/t_flt) * (i_flt/t_flt);   // Quadratic Ease In
}

float easeOut(int t, int i)
{
  float i_flt = i * 1.0;
  float t_flt = t * 1.0;
  return 1 - (1 - i_flt/t_flt) * (1 - i_flt/t_flt);  // Quadratic Ease Out
}

float easeInOut(int t, int i)
{
  float i_flt = i * 1.0;
  float t_flt = t * 1.0;
  float func_x = (1 + i_flt/t_flt) * PI;
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
  float coef = velocityFunc(t, i);
  // Serial.print("coef: ");
  // Serial.println(coef);
  int delta_x = coef * (keyframes[target].x - keyframes[head].x);
  int delta_y = coef * (keyframes[target].y - keyframes[head].y);
  x = keyframes[head].x + delta_x;
  y = keyframes[head].y + delta_y;
  // Serial.print("\t y:");
  // Serial.print(y);
  // Serial.print("\t delta y:");
  // Serial.println(delta_y);
}

void UIElement::move(int16_t delta_x, int16_t delta_y)
{
  x += delta_x;
  y += delta_y;
}

void UIElement::scale(int t, int i, float (*velocityFunc)(int, int))
{
  float coef = velocityFunc(t, i);
  int delta_w = coef * (keyframes[target].w - keyframes[head].w);
  int delta_h = coef * (keyframes[target].h - keyframes[head].h);
  w = keyframes[head].w + delta_w;
  h = keyframes[head].h + delta_h;
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
  if (iterator == 0 && head != tail)  // Starting animation
  {
    target = (target + 1) % KEYFRAME_QUEUE_SIZE;
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
    // Serial.print("Inital h:");
    // Serial.print(keyframes[head].h);
    // Serial.print("Target h:");
    Serial.println(keyframes[target].h);
    if (keyframes[target].timestamp > anim_time) anim_time = keyframes[target].timestamp;
    if (keyframes[target].visible == true) setVisible(true);  // set visible before animation start
  }
  int t = keyframes[target].timestamp;
  if (iterator >= t + 1)    // Finish animation
  {
    // Serial.print("FINISHED FROM ");
    // Serial.print(head);
    // Serial.print(" TO ");
    // Serial.println(target);
    if (keyframes[target].visible == false) visible = false;
    head = (head + 1) % KEYFRAME_QUEUE_SIZE;
    // target = (head + 1) % KEYFRAME_QUEUE_SIZE;
    if (iterator >=  anim_time + 1) {   // animation done globally
      anim_iterator = 0;
      anim_time = 0;
    }
    iterator = 0;
    haveAnim = false;
  } else {
    // Serial.print("Iterating ");
    // Serial.print(iterator);
    // Serial.print("/");
    // Serial.println(t);

    if ((keyframes[target].w != w) || (keyframes[target].h != h)) scale(t, iterator, velocityFunc);
    if ((keyframes[target].x != x) || (keyframes[target].y != y)) move(t, iterator, velocityFunc);
    iterator++;
  }
}

void UIElement::addKeyframe(int16_t new_x, int16_t new_y, int16_t new_w, int16_t new_h, float new_timestamp, bool new_visible)
{
  tail = (tail + 1) % KEYFRAME_QUEUE_SIZE;
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
  return amount;
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
    if (primary) spr[sel].fillRoundRect(x, y, w, h, rad, PRIMARY_COLOR);
    else spr[sel].fillRoundRect(x, y, w, h, rad, SECONDARY_COLOR);
  }
}

void Circle::setFilled(bool newFilled)
{
  filled = newFilled;
}

void Circle::draw(bool sel)
{
  if (filled)
  {
    if (primary) spr[sel].fillCircle(x, y, w / 2, PRIMARY_COLOR);
    else spr[sel].fillCircle(x, y, w / 2, SECONDARY_COLOR);
  }
  else 
  {
    if (primary) spr[sel].drawCircle(x, y, w / 2, PRIMARY_COLOR);
    else spr[sel].drawCircle(x, y, w / 2, SECONDARY_COLOR);
  }
}

Triangle::Triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3) : UIElement(0,0,0,0)
{
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  this->x3 = x3;
  this->y3 = y3;
}

void Triangle::draw(bool sel)
{
  if (visible)
  {
    spr[sel].fillTriangle(x1, y1, x2, y2, x3, y3, SECONDARY_COLOR);
  }
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

void Text::setContent(String newContent)
{
  content = newContent;
}

void Text::draw(bool sel)
{
  if (primary) spr[sel].setTextColor(PRIMARY_COLOR);
  else spr[sel].setTextColor(SECONDARY_COLOR);
  switch (size) {
    case 0:
      spr[sel].setFreeFont(TT1);
      break;
    case 1:
      spr[sel].setFreeFont(FSS9);
      break;
    case 2:
      spr[sel].setFreeFont(FSS12);
      break;
    case 3:
      spr[sel].setFreeFont(FSS18);
      break;
    case 4:
      spr[sel].setFreeFont(FSS24);
      break;
  }
  spr[sel].drawString(content, x, y, GFXFF);
}

void Bar::draw(bool sel)
{
  if (primary) spr[sel].drawFastHLine(x, y, w, PRIMARY_COLOR);
  else spr[sel].drawFastHLine(x, y, w, SECONDARY_COLOR);
}

void ListItem::draw(bool sel)
{
  if (visible)
  {
    spr[sel].setFreeFont(FSS9);
    tft.setTextDatum(TL_DATUM);
    if (selected)
    {
      spr[sel].fillRect(x, y, w, h, TFT_BLUE);
      spr[sel].setTextColor(PRIMARY_COLOR);
      spr[sel].drawString(title, x + 15, y + 15, GFXFF);
      spr[sel].drawString(subtitle, x + 98, y + 15, GFXFF);
    }
    else
    {
      spr[sel].setTextColor(SECONDARY_COLOR);
      spr[sel].drawString(title, x + 15, y + 15, GFXFF);
      spr[sel].setTextColor(GREY_COLOR);
      spr[sel].drawString(subtitle, x + 98, y + 15, GFXFF);
    }
    spr[sel].drawFastHLine(x, y + h, w, GREY_COLOR);
    tft.setTextDatum(MC_DATUM);
  }
}

void scrollList(bool up)
{
  int currItem = currFriend % 4;

  if (currItem == 0)
  {
    if (up && currFriend != 0)
    {
      friendHead -= 4;
      friendHead = friendHead < 0 ? 0 : friendHead;
      updateList();
    }
    else if (!up)
    {
      static_cast<ListItem*>((*currScreen)["frd0"])->setSelected(false);
      static_cast<ListItem*>((*currScreen)["frd1"])->setSelected(true);
    }
  }
  else if (currItem == 1)
  {
    static_cast<ListItem*>((*currScreen)["frd1"])->setSelected(false);
    if (up)
    {
      static_cast<ListItem*>((*currScreen)["frd0"])->setSelected(true);
    }
    else
    {
      static_cast<ListItem*>((*currScreen)["frd2"])->setSelected(true);
    }
  }
  else if (currItem == 2)
  {
    static_cast<ListItem*>((*currScreen)["frd2"])->setSelected(false);
    if (up)
    {
      static_cast<ListItem*>((*currScreen)["frd1"])->setSelected(true);
    }
    else
    {
      static_cast<ListItem*>((*currScreen)["frd3"])->setSelected(true);
    }
  }
  else
  {
    if (up)
    {
      static_cast<ListItem*>((*currScreen)["frd3"])->setSelected(false);
      static_cast<ListItem*>((*currScreen)["frd2"])->setSelected(true);
    }
    else if (!up && currFriend < numFriends)
    {
      friendHead += 4;
      updateList();
    }
  }

  if (up)
  {
    currFriend--;
    currFriend = currFriend < 0 ? 0 : currFriend;
  }
  else
  {
    currFriend++;
    currFriend = currFriend >= numFriends ? numFriends - 1 : currFriend;
  }

}

void updateList()
{
  for (int i = 0; i < 4; i++)
  {
    String query = "frd" + i;
    ListItem* currItem = static_cast<ListItem*>((*currScreen)[query]);
    int currIndex = friendHead + i;
    if (currIndex >= numFriends)
    {
      currItem->setText("", "");
    }
    else
    {
      currItem->setText(friendsList[currIndex].username, "");
    }
  }
  static_cast<ListItem*>((*currScreen)["frd0"])->setSelected(true);
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
  currScreen = static_cast<UIElGroup*>(elements[i]);
  listDir(SD, "/", 0);
  playAudio("/test.wav");
  PlayHaptic(1); //PlayHaptic(effect number)
  face.draw(0);
  face.draw(1);
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
  listDir(SD, "/", 0);
  playAudio("/test.wav");
  PlayHaptic(1); //PlayHaptic(effect number)
  face.draw(0);
  face.draw(1);
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
        int newIndex = curr->getColIndex();
        newIndex = newIndex > 0 ? newIndex - 1 : curr->getSize() - 1;
        curr->navigateTo(newIndex);
        break;
      }
    case 'l':
      {
        int newIndex = rowIndex > 0 ? (rowIndex - 1) % size : size - 1;
        navigateTo(newIndex);
        break;
      }
    case 'd':
      {
        ScreenCol* curr = static_cast<ScreenCol*>(elements[rowIndex]);
        int newIndex = curr->getColIndex();
        newIndex = newIndex < curr->getSize() ? newIndex + 1 : curr->getSize() - 1;
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

int ScreenRow::getRowIndex() const
{
  return rowIndex;
}

UIElGroup* ScreenRow::screen(int row, int col)
{
  ScreenCol* temp = static_cast<ScreenCol*>(elements[row]);
  return static_cast<UIElGroup*>((*temp)[col]);
}

UIElGroup* ScreenRow::screen(String row_name, String col_name)
{
  ScreenCol* temp = static_cast<ScreenCol*>(this->operator[](row_name));
  return static_cast<UIElGroup*>(this->operator[](col_name));
}

void ScreenRow::draw(bool sel)
{
  if (visible) elements[rowIndex]->draw(sel);
}

