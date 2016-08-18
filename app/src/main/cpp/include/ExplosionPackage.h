#ifndef __ARKANOID_EXPLOSION_PACKAGE__H__
#define __ARKANOID_EXPLOSION_PACKAGE__H__

#include <GLES/gl.h>
#include "rgbstruct.h"

namespace game {

enum class Kind : int {
  DIVERGE = 0,
  CONVERGE = 1
};

class ExplosionPackage {
public:
  friend class GameProcessor;

  ExplosionPackage();
  ExplosionPackage(GLfloat x, GLfloat y, const util::BGRA<GLfloat>& color, Kind kind);
  virtual ~ExplosionPackage();
  ExplosionPackage(const ExplosionPackage& obj);
  ExplosionPackage(ExplosionPackage&& rval_obj);
  ExplosionPackage& operator= (ExplosionPackage rhs);

  inline int getID() const { return id; }
  inline GLfloat getX() const { return x; }
  inline GLfloat getY() const { return y; }
  inline const util::BGRA<GLfloat>& getColor() const { return color; }
  inline Kind getKind() const { return kind; }

private:
  int id;
  GLfloat x, y;
  util::BGRA<GLfloat> color;
  Kind kind;

  ExplosionPackage(int id, GLfloat x, GLfloat y, const util::BGRA<GLfloat>& color, Kind kind);
};

}

#endif  // __ARKANOID_EXPLOSION_PACKAGE__H__
