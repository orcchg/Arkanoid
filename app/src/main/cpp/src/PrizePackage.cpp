#include <algorithm>
#include "PrizePackage.h"

namespace game {

PrizePackage::PrizePackage()
  : id(-1)
  , x(0.0f)
  , y(0.0f)
  , prize(Prize::NONE)
  , has_gone(false)
  , has_caught(false) {
}

PrizePackage::PrizePackage(GLfloat x, GLfloat y, Prize prize)
  : id(-1)
  , x(x)
  , y(y)
  , prize(prize)
  , has_gone(false)
  , has_caught(false) {
}

PrizePackage::~PrizePackage() {
}

PrizePackage::PrizePackage(const PrizePackage& obj)
  : id(obj.id)
  , x(obj.x)
  , y(obj.y)
  , prize(obj.prize)
  , has_gone(obj.has_gone)
  , has_caught(obj.has_caught) {
}

PrizePackage::PrizePackage(PrizePackage&& rval_obj)
  : id(rval_obj.id)
  , x(rval_obj.x)
  , y(rval_obj.y)
  , prize(rval_obj.prize)
  , has_gone(rval_obj.has_gone)
  , has_caught(rval_obj.has_caught) {
  rval_obj.id = -1;
  rval_obj.x = 0.0f;
  rval_obj.y = 0.0f;
  rval_obj.prize = Prize::NONE;
  rval_obj.has_gone = false;
  rval_obj.has_caught = false;
}

PrizePackage& PrizePackage::operator= (PrizePackage temp) {
  std::swap(id, temp.id);
  std::swap(x, temp.x);
  std::swap(y, temp.y);
  std::swap(prize, temp.prize);
  std::swap(has_gone, temp.has_gone);
  std::swap(has_caught, temp.has_caught);
  return *this;
}

// http://stackoverflow.com/questions/18175962/cant-use-static-stdatomic-and-dont-know-how-to-initialize-it
PrizePackage::PrizePackage(int id, GLfloat x, GLfloat y, Prize prize)
  : id(id), x(x), y(y), prize(prize), has_gone(false), has_caught(false) {
}

}
