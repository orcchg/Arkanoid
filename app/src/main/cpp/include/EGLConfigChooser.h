#ifndef __ARKANOID_EGL_CONFIG_CHOOSER__H__
#define __ARKANOID_EGL_CONFIG_CHOOSER__H__

#include <EGL/egl.h>
#include <GLES/gl.h>

namespace game {

class EGLConfigChooser {
public:
  EGLConfigChooser(
      EGLint red = 0, EGLint green = 0, EGLint blue = 0,
      EGLint alpha = 0, EGLint depth = 0, EGLint stencil = 0);
  virtual ~EGLConfigChooser();

  EGLConfig chooseConfig(EGLDisplay display);
  EGLint getNumberConfigs() const;

private:
  EGLConfig* m_configs;
  EGLint m_num_configs;

protected:
  EGLint* m_value;
  EGLint m_alpha_size;
  EGLint m_blue_size;
  EGLint m_depth_size;
  EGLint m_green_size;
  EGLint m_red_size;
  EGLint m_stencil_size;

private:
  EGLint findConfigAttrib(
      EGLDisplay display,
      EGLConfig config,
      EGLint attribute,
      EGLint defaultValue);
};

}

#endif  // __ARKANOID_EGL_CONFIG_CHOOSER__H__
