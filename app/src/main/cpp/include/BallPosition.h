#ifndef __ARKANOID_BALLPOSITION__H__
#define __ARKANOID_BALLPOSITION__H__

#include <GLES/gl.h>

namespace game {

class BallPosition {
public:
  BallPosition(GLfloat x = 0.0f, GLfloat y = 0.0f)
    : m_x(x)
    , m_y(y) {
  }

  inline GLfloat getX() const { return m_x; }
  inline GLfloat getY() const { return m_y; }
  inline void setX(GLfloat x) { m_x = x; }
  inline void setY(GLfloat y) { m_y = y; }

private:
  GLfloat m_x;
  GLfloat m_y;
};

}

#endif /* __ARKANOID_BALLPOSITION__H__ */
