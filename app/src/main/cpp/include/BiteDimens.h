#ifndef __ARKANOID_BITE_DIMENS__H__
#define __ARKANOID_BITE_DIMENS__H__

#include <GLES/gl.h>

namespace game {

/// @brief Measured bite dimensions.
class BiteDimens {
public:
  friend class Bite;

  BiteDimens(GLfloat w, GLfloat h)
    : m_width(w)
    , m_height(h) {
  }

  inline GLfloat width() const { return m_width; }
  inline GLfloat height() const { return m_height; }
  inline GLfloat halfWidth() const { return m_width * 0.5f; }
  inline GLfloat halfHeight() const { return m_height * 0.5f; }
  inline GLfloat quarterWidth() const { return m_width * 0.25f; }
  inline GLfloat quarterHeight() const { return m_height * 0.25f; }

private:
  GLfloat m_width;
  GLfloat m_height;
};

}

#endif  // __ARKANOID_BITE_DIMENS__H__
