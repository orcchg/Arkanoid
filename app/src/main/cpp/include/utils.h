#ifndef __ARKANOID_UTILS__H__
#define __ARKANOID_UTILS__H__

#include <vector>
#include <cstdlib>

#include "logger.h"
#include "rgbstruct.h"

namespace util {

// http://habrahabr.ru/company/abbyy/blog/252871/
constexpr float _3PI = 9.4250778f;
constexpr float _2PI = 6.2831852f;
constexpr float _3PI2 = 4.7123889f;
constexpr float PI = 3.1415926f;
constexpr float PI2 = 1.5707963f;
constexpr float PI4 = 0.78539815f;
constexpr float PI6 = 0.5235988f;
constexpr float PI8 = 0.392711575f;
constexpr float PI10 = 0.31416926f;
constexpr float PI12 = 0.261799383f;
constexpr float PI16 = 0.1963495375f;
constexpr float PI30 = 0.10471975f;

constexpr float epsilon = 1e-4;

template <typename T>
void setColor(const BGRA<T>& bgra, T* const color_buffer, size_t size) {
  for (size_t i = 0; i < size; i += 4) {
    color_buffer[i + 0] = bgra.b;
    color_buffer[i + 1] = bgra.g;
    color_buffer[i + 2] = bgra.r;
    color_buffer[i + 3] = bgra.a;
  }
}

void setColor(const GLfloat* const bgra, GLfloat* const color_buffer, size_t size);

void setRectangleVertices(
    GLfloat* const vertices,
    GLfloat width,
    GLfloat height,
    GLfloat x_offset,
    GLfloat y_offset,
    size_t cols,
    size_t rows);

void setOctagonVertices(
    GLfloat* const vertices,
    GLfloat width,
    GLfloat height,
    GLfloat x_offset,
    GLfloat y_offset,
    size_t cols,
    size_t rows);

void rectangleIndices(GLushort* const indices, size_t size);

void printBuffer2D(const GLfloat* const buffer, size_t size);
void printBuffer3D(const GLfloat* const buffer, size_t size);
void printBuffer4D(const GLfloat* const buffer, size_t size);

template <typename T>
size_t getRandomElement(const std::vector<T>& array) {
  return std::rand() % array.size();
}

}

#endif  // __ARKANOID_UTILS__H__
