#include "face.h"
#include "menu.h"

int main() {
  Face face;
  face.changeFaceState(SAD);
  face.update();
  face.draw();
  return 0;
}