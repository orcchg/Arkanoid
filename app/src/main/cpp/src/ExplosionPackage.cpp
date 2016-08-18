#include <algorithm>
#include "ExplosionPackage.h"

namespace game {

ExplosionPackage::ExplosionPackage()
  : id(-1)
  , x(0.0f)
  , y(0.0f)
  , color(util::TRANSPARENT)
  , kind(Kind::DIVERGE) {
}

ExplosionPackage::ExplosionPackage(
    GLfloat x,
    GLfloat y,
    const util::BGRA<GLfloat>& color,
    Kind kind)
  : id(-1)
  , x(x)
  , y(y)
  , color(color)
  , kind(kind) {
}

ExplosionPackage::~ExplosionPackage() {
}

ExplosionPackage::ExplosionPackage(const ExplosionPackage& obj)
  : id(obj.id)
  , x(obj.x)
  , y(obj.y)
  , color(obj.color)
  , kind(obj.kind) {
}

ExplosionPackage::ExplosionPackage(ExplosionPackage&& rval_obj)
  : id(rval_obj.id)
  , x(rval_obj.x)
  , y(rval_obj.y)
  , color(rval_obj.color)
  , kind(rval_obj.kind) {
  rval_obj.id = -1;
  rval_obj.x = 0.0f;
  rval_obj.y = 0.0f;
  rval_obj.color = util::TRANSPARENT;
  rval_obj.kind = Kind::DIVERGE;
}

ExplosionPackage& ExplosionPackage::operator= (ExplosionPackage temp) {
  std::swap(id, temp.id);
  std::swap(x, temp.x);
  std::swap(y, temp.y);
  std::swap(color, temp.color);
  std::swap(kind, temp.kind);
  return *this;
}

ExplosionPackage::ExplosionPackage(
    int id,
    GLfloat x,
    GLfloat y,
    const util::BGRA<GLfloat>& color,
    Kind kind)
  : id(id)
  , x(x)
  , y(y)
  , color(color)
  , kind(kind) {
}
}
