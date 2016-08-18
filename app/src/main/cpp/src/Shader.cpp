#include <GLES2/gl2.h>

#include "Exceptions.h"
#include "logger.h"
#include "Shader.h"

namespace shader {

ShaderHelper::ShaderHelper(const Shader& shader)
  : m_program(0)
  , m_vertex_location(0)
  , m_color_location(1)
  , m_texCoord_location(2) {

  DBG("enter ShaderHelper::ctor");
  GLuint vertex_shader = loadShader(GL_VERTEX_SHADER, shader.vertex);
  GLuint fragment_shader = loadShader(GL_FRAGMENT_SHADER, shader.fragment);

  m_program = glCreateProgram();
  if (m_program == 0) {
    const char* message = "Failed to create program object";
    ERR("%s", message);
    throw ShaderException(message);
  }

  glAttachShader(m_program, vertex_shader);
  glAttachShader(m_program, fragment_shader);

  // bind a_position to attribute with index m_vertex_location
  glBindAttribLocation(m_program, m_vertex_location, "a_position");
  shader.bindColorAttribLocation(m_program, m_color_location);
  shader.bindTexCoordAttribLocation(m_program, m_texCoord_location);
  glLinkProgram(m_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  GLint linked = 0;
  glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLint infoLen = 0;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1) {
      char* infoLog = new char[infoLen];
      glGetProgramInfoLog(m_program, infoLen, nullptr, infoLog);
      ERR("Error linking program: %s", infoLog);
      if (strcmp(infoLog, "--From Vertex Shader:\n--From Fragment Shader:\nLink was successful.")) {
        INF("No linker error !");
        delete [] infoLog;
        return;
      }
      delete [] infoLog;
    }
    glDeleteProgram(m_program);
    throw ShaderException("Error linking program");
  }
  DBG("exit ShaderHelper::ctor");
}

ShaderHelper::~ShaderHelper() {
  glDeleteProgram(m_program);
  m_program = 0;
}

void ShaderHelper::useProgram() const {
  if (m_program == 0) {
    ERR("Invalid program object !");
  }
  glUseProgram(m_program);
}

GLuint ShaderHelper::loadShader(GLenum type, const char* shader_src) {
  DBG("enter ShaderHelper::loadShader()");
  INF("GL version: %s", glGetString(GL_VERSION));
  GLuint shader = glCreateShader(type);
  if (shader == 0) {
    ERR("Failed to create shader of type %i", type);
    return 0;
  }

  glShaderSource(shader, 1, &shader_src, nullptr);  // load shader source
  glCompileShader(shader);

  GLint compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);  // get status
  if (compiled == 0) {
    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1) {
      char* infoLog = new char[infoLen];
      glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
      ERR("Error compiling shader: %s", infoLog);
      delete [] infoLog;
    }
    glDeleteShader(shader);
    return 0;
  }
  DBG("exit ShaderHelper::loadShader()");
  return shader;
}

/* Pre-made shaders */
// ----------------------------------------------------------------------------
Shader::Shader(const char* vertex, const char* fragment)
  : vertex(vertex)
  , fragment(fragment) {
}

Shader::~Shader() {
}

SimpleShader::SimpleShader()
  : Shader(
      "  attribute vec4 a_position;   \n"
      "  attribute vec4 a_color;      \n"
      "                               \n"
      "  varying vec4 v_color;        \n"
      "                               \n"
      "  void main() {                \n"
      "    v_color = a_color;         \n"
      "    gl_Position = a_position;  \n"
      "  }                            \n"
      ,
      "  precision mediump float;     \n"
      "                               \n"
      "  varying vec4 v_color;        \n"
      "                               \n"
      "  void main() {                \n"
      "    gl_FragColor = v_color;    \n"
      "  }                            \n") {
}

void SimpleShader::bindColorAttribLocation(GLuint program, GLuint color_location) const {
  glBindAttribLocation(program, color_location, "a_color");
}

void SimpleShader::bindTexCoordAttribLocation(GLuint program, GLuint texCoord_location) const {
  // no-op
}

SimpleTextureShader::SimpleTextureShader()
  : Shader(
      "  attribute vec4 a_position;                                            \n"
      "  attribute vec2 a_texCoord;                                            \n"
      "                                                                        \n"
      "  varying vec2 v_texCoord;                                              \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    v_texCoord = a_texCoord;                                            \n"
      "    gl_Position = a_position;                                           \n"
      "  }                                                                     \n"
      ,
      "  precision mediump float;                                              \n"
      "                                                                        \n"
      "  varying vec2 v_texCoord;                                              \n"
      "  uniform sampler2D s_texture;                                          \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    gl_FragColor = texture2D(s_texture, v_texCoord);                    \n"
      "  }                                                                     \n") {
}

void SimpleTextureShader::bindColorAttribLocation(GLuint program, GLuint color_location) const {
  // no-op
}

void SimpleTextureShader::bindTexCoordAttribLocation(GLuint program, GLuint texCoord_location) const {
  glBindAttribLocation(program, texCoord_location, "a_texCoord");
}

ParticleSystemShader::ParticleSystemShader()
  : Shader(
      "  uniform float u_time;                                                 \n"
      "  uniform vec2 u_centerPosition;                                        \n"
      "                                                                        \n"
      "  attribute float a_lifetime;                                           \n"
      "  attribute vec2 a_startPosition;                                       \n"
      "  attribute vec2 a_endPosition;                                         \n"
      "                                                                        \n"
      "  varying float v_lifetime;                                             \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    if (u_time <= a_lifetime) {                                         \n"
      "      gl_Position.xy = a_startPosition + (u_time * a_endPosition);      \n"
      "      gl_Position.xy += u_centerPosition;                               \n"
      "      gl_Position.z = 0.0;                                              \n"
      "      gl_Position.w = 1.0;                                              \n"
      "    } else {                                                            \n"
      "      gl_Position = vec4(-1000, -1000, 0, 0);                           \n"
      "    }                                                                   \n"
      "    v_lifetime = 1.0 - (u_time / a_lifetime);                           \n"
      "    v_lifetime = clamp(v_lifetime, 0.0, 1.0);                           \n"
      "    gl_PointSize = (v_lifetime * v_lifetime) * 40.0;                    \n"
      "  }                                                                     \n"
      ,
      "  precision mediump float;                            \n"
      "                                                      \n"
      "  uniform vec4 u_color;                               \n"
      "  varying float v_lifetime;                           \n"
      "  uniform sampler2D s_texture;                        \n"
      "                                                      \n"
      "  void main() {                                       \n"
      "    vec4 texColor;                                    \n"
      "    texColor = texture2D(s_texture, gl_PointCoord);   \n"
      "    gl_FragColor = vec4(u_color) * texColor;          \n"
      "    gl_FragColor.a *= v_lifetime;                     \n"
      "  }                                                   \n") {
}

void ParticleSystemShader::bindColorAttribLocation(GLuint program, GLuint color_location) const {
  // no-op
}

void ParticleSystemShader::bindTexCoordAttribLocation(GLuint program, GLuint texCoord_location) const {
  // no-op
}

VerticalFallShader::VerticalFallShader()
  : Shader(
      "  uniform float u_time;                                                 \n"
      "  uniform float u_velocity;                                             \n"
      "  uniform int u_visible;                                                \n"
      "                                                                        \n"
      "  attribute vec4 a_position;                                            \n"
      "  attribute vec2 a_texCoord;                                            \n"
      "                                                                        \n"
      "  varying vec2 v_texCoord;                                              \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    if (u_visible != 0 && u_time <= 3.0) {                              \n"
      "      gl_Position = a_position;                                         \n"
      "      gl_Position.y -= u_time * u_velocity;                             \n"
      "    } else {                                                            \n"
      "      gl_Position = vec4(-1000, -1000, 0, 0);                           \n"
      "    }                                                                   \n"
      "    v_texCoord = a_texCoord;                                            \n"
      "  }                                                                     \n"
      ,
      "  precision mediump float;                                              \n"
      "                                                                        \n"
      "  varying vec2 v_texCoord;                                              \n"
      "  uniform sampler2D s_texture;                                          \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    gl_FragColor = texture2D(s_texture, v_texCoord);                    \n"
      "  }                                                                     \n") {
}

void VerticalFallShader::bindColorAttribLocation(GLuint program, GLuint color_location) const {
  // no-op
}

void VerticalFallShader::bindTexCoordAttribLocation(GLuint program, GLuint texCoord_location) const {
  glBindAttribLocation(program, texCoord_location, "a_texCoord");
}

VerticalClimbShader::VerticalClimbShader()
  : Shader(
      "  uniform float u_time;                                                 \n"
      "  uniform float u_velocity;                                             \n"
      "  uniform int u_visible;                                                \n"
      "                                                                        \n"
      "  attribute vec4 a_position;                                            \n"
      "  attribute vec2 a_texCoord;                                            \n"
      "                                                                        \n"
      "  varying vec2 v_texCoord;                                              \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    if (u_visible != 0 && u_time <= 0.6) {                              \n"
      "      gl_Position = a_position;                                         \n"
      "      gl_Position.y += u_time * u_velocity;                             \n"
      "    } else {                                                            \n"
      "      gl_Position = vec4(-1000, -1000, 0, 0);                           \n"
      "    }                                                                   \n"
      "    v_texCoord = a_texCoord;                                            \n"
      "  }                                                                     \n"
      ,
      "  precision mediump float;                                              \n"
      "                                                                        \n"
      "  varying vec2 v_texCoord;                                              \n"
      "  uniform sampler2D s_texture;                                          \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    gl_FragColor = texture2D(s_texture, v_texCoord);                    \n"
      "  }                                                                     \n") {
}

void VerticalClimbShader::bindColorAttribLocation(GLuint program, GLuint color_location) const {
  // no-op
}

void VerticalClimbShader::bindTexCoordAttribLocation(GLuint program, GLuint texCoord_location) const {
  glBindAttribLocation(program, texCoord_location, "a_texCoord");
}

ParticleMoveShader::ParticleMoveShader()
  : Shader(
      "  uniform float u_time;                                                 \n"
      "  uniform vec2 u_centerPosition;                                        \n"
      "                                                                        \n"
      "  attribute vec2 a_startPosition;                                       \n"
      "  attribute vec2 a_endPosition;                                         \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    if (u_time <= 1.0) {                                                \n"
      "      gl_Position.xy = a_startPosition + (u_time * a_endPosition);      \n"
      "      gl_Position.xy += u_centerPosition;                               \n"
      "      gl_Position.z = 0.0;                                              \n"
      "      gl_Position.w = 1.0;                                              \n"
      "    } else {                                                            \n"
      "      gl_Position = vec4(-1000, -1000, 0, 0);                           \n"
      "    }                                                                   \n"
      "    gl_PointSize = 5.0;                                                 \n"
      "  }                                                                     \n"
      ,
      "  precision mediump float;                                              \n"
      "                                                                        \n"
      "  uniform vec4 u_color;                                                 \n"
      "  uniform sampler2D s_texture;                                          \n"
      "                                                                        \n"
      "  void main() {                                                         \n"
      "    vec4 texColor;                                                      \n"
      "    texColor = texture2D(s_texture, gl_PointCoord);                     \n"
      "    gl_FragColor = vec4(u_color) * texColor;                            \n"
      "  }                                                                     \n") {
}

void ParticleMoveShader::bindColorAttribLocation(GLuint program, GLuint color_location) const {
  // no-op
}

void ParticleMoveShader::bindTexCoordAttribLocation(GLuint program, GLuint texCoord_location) const {
  // no-op
}

}
