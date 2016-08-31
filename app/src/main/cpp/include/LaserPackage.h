#ifndef __ARKANOID_LASER_PACKAGE__H__
#define __ARKANOID_LASER_PACKAGE__H__

#include <GLES2/gl2.h>

namespace game {

class LaserPackage {
public:
  LaserPackage(GLfloat x, GLfloat y)
    : x(x), y(y) {
  }

  inline GLfloat getX() const { return x; }
  inline GLfloat getY() const { return y; }

private:
  GLfloat x, y;
};

}

#endif  // __ARKANOID_LASER_PACKAGE__H__
