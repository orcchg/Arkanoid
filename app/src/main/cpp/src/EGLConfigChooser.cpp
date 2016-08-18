#include <climits>

#include "EGLConfigChooser.h"
#include "logger.h"


namespace game {

EGLConfigChooser::EGLConfigChooser(
    EGLint red, EGLint green, EGLint blue,
    EGLint alpha, EGLint depth, EGLint stencil)
  : m_configs(nullptr)
  , m_num_configs(0)
  , m_value(new GLint[1])
  , m_alpha_size(alpha)
  , m_blue_size(blue)
  , m_depth_size(depth)
  , m_green_size(green)
  , m_red_size(red)
  , m_stencil_size(stencil) {
}

EGLConfigChooser::~EGLConfigChooser() {
  delete [] m_configs;
  delete [] m_value;
}

EGLConfig EGLConfigChooser::chooseConfig(EGLDisplay display) {
  const EGLint attributes[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_TRANSPARENT_TYPE, EGL_NONE,
        EGL_NONE
    };

  EGLint* arg = new EGLint[1];
  eglChooseConfig(display, attributes, nullptr, 0, arg);
  m_num_configs = arg[0];
  DBG("Total %i configurations available", m_num_configs);

  if (m_num_configs <= 0) {
    WRN("No available configurations for such device and minimum attributes");
    return nullptr;
  }

  m_configs = new EGLConfig[m_num_configs];
  eglChooseConfig(display, attributes, m_configs, m_num_configs, arg);
  delete [] arg;

  /* The hard work */
  //
  EGLConfig best_match = nullptr;

  EGLint best_alpha   = INT_MAX;
  EGLint best_blue    = INT_MAX;
  EGLint best_depth   = INT_MAX;
  EGLint best_green   = INT_MAX;
  EGLint best_red     = INT_MAX;
  EGLint best_stencil = INT_MAX;

  for (int i = 0; i < m_num_configs; ++i) {
    EGLint alpha   = findConfigAttrib(display, m_configs[i], EGL_ALPHA_SIZE, 0);
    EGLint blue    = findConfigAttrib(display, m_configs[i], EGL_BLUE_SIZE, 0);
    EGLint depth   = findConfigAttrib(display, m_configs[i], EGL_DEPTH_SIZE, 0);
    EGLint green   = findConfigAttrib(display, m_configs[i], EGL_GREEN_SIZE, 0);
    EGLint red     = findConfigAttrib(display, m_configs[i], EGL_RED_SIZE, 0);
    EGLint stencil = findConfigAttrib(display, m_configs[i], EGL_STENCIL_SIZE, 0);

    if (alpha   <= best_alpha   && alpha   >= m_alpha_size &&
        blue    <= best_blue    && blue    >= m_blue_size  &&
        depth   <= best_depth   && depth   >= m_depth_size &&
        green   <= best_green   && green   >= m_green_size &&
        red     <= best_red     && red     >= m_red_size   &&
        stencil <= best_stencil && stencil >= m_stencil_size) {

      best_alpha   = alpha;
      best_blue    = blue;
      best_depth   = depth;
      best_green   = green;
      best_red     = red;
      best_stencil = stencil;

      best_match = m_configs[i];
    }
  }

  return best_match;
}

EGLint EGLConfigChooser::getNumberConfigs() const {
  return m_num_configs;
}

/* Private members */
//
EGLint EGLConfigChooser::findConfigAttrib(
    EGLDisplay display,
    EGLConfig config,
    EGLint attribute,
    EGLint defaultValue) {

  if (eglGetConfigAttrib(display, config, attribute, m_value)) {
    return m_value[0];
  }
  return defaultValue;
}

}
