#include "wtf.h"
#include "cheapTricks.h"

bool wtf(int complaint) {
  static int lastWtf = 0;
  static int repeats = 0;
  if (complaint) {
    if (changed(lastWtf, complaint)) {
      repeats = 0;
      return false;
    } else {
      ++repeats;
      return true;
    }
  } else {
    return false;
  }
}
