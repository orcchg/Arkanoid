#ifndef __ARKANOID_RGBSTRUCT__H__
#define __ARKANOID_RGBSTRUCT__H__

#include <GLES/gl.h>

namespace util {

template <typename T>
struct BGRA {
  T b, g, r, a;

  BGRA()
    : b(0), g(0), r(0), a(0) {
  }

  BGRA(const T* const bgra)
    : b(bgra[0]), g(bgra[1]), r(bgra[2]), a(bgra[3]) {
  }

  BGRA(T b, T g, T r, T a)
    : b(b), g(g), r(r), a(a) {
  }

  BGRA(const BGRA& obj)
    : b(obj.b), g(obj.g), r(obj.r), a(obj.a) {
  }

  BGRA& operator = (const BGRA& obj) {
    b = obj.b; g = obj.g; r = obj.r; a = obj.a;
    return *this;
  }
};

// @see http://stackoverflow.com/questions/13865842/does-static-constexpr-variable-make-sense
constexpr static GLfloat BLACK[4]   = { 0.f,     0.f,  0.f,  1.f };
constexpr static GLfloat RED[4]     = { 1.f,     0.f,  0.f,  1.f };
constexpr static GLfloat GREEN[4]   = { 0.f,     1.f,  0.f,  1.f };
constexpr static GLfloat BLUE[4]    = { 0.f,     0.f,  1.f,  1.f };
constexpr static GLfloat CYAN[4]    = { 0.f,     1.f,  1.f,  1.f };
constexpr static GLfloat MAGENTA[4] = { 0.5961f, 0.f,  1.f,  1.f };
constexpr static GLfloat PURPLE[4]  = { 0.5961f, 0.f,  0.4f, 1.f };
constexpr static GLfloat YELLOW[4]  = { 1.f,     1.f,  0.f,  1.f };
constexpr static GLfloat ORANGE[4]  = { 1.f,     0.4f, 0.f,  1.f };
constexpr static GLfloat MESHLAB[4] = { 0.8118f, 0.4f, 0.4f, 1.f };
constexpr static GLfloat TRANSPARENT[4] = { 0.f, 0.f, 0.f,   0.f};
constexpr static GLfloat SALMON[4]  = {0.8039f, 0.4392f, 0.3294f, 1.f};
constexpr static GLfloat BROWN[4]   = {0.5451f, 0.2706f, 0.0745f, 1.f};
constexpr static GLfloat SIENNA_LIGHT[4] = {0.9333f, 0.4745f, 0.2588f, 1.f};
constexpr static GLfloat SIENNA[4]       = {0.8039f, 0.4078f, 0.2235f, 1.f};
constexpr static GLfloat SIENNA_DARK[4]  = {0.5451f, 0.2784f, 0.1490f, 1.f};
constexpr static GLfloat MIRROR[4]       = {0.8784f, 0.9333f, 0.9333f, 1.f};
constexpr static GLfloat MIRROR_EDGE[4]  = {0.5137f, 0.5451f, 0.5451f, 1.f};

// http://www.color-hex.com/color-names.html
constexpr static GLfloat ALUMINIUM[4]      = {0.8509f, 0.8509f, 0.8509f, 1.f};  // 217 217 217
constexpr static GLfloat ALUMINIUM_EDGE[4] = {0.65f,   0.65f,   0.65f,   1.f};  // 166 166 166

constexpr static GLfloat ARTIFICAL[4]      = {1.f,     0.0784f, 0.5765f, 1.f};  // 255 20 147
constexpr static GLfloat ARTIFICAL_EDGE[4] = {0.2784f, 0.2784f, 0.2784f, 1.f};  // 71  71 71

constexpr static GLfloat BRICK[4]      = {0.8039f, 0.1490f, 0.1490f, 1.f};  // 205 38 38
constexpr static GLfloat BRICK_EDGE[4] = {0.5451f, 0.1020f, 0.1020f, 1.f};  // 139 26 26

constexpr static GLfloat CLAY[4]      = {1.f,     0.8275,  0.6078f, 1.f};  // 255 211 155
constexpr static GLfloat CLAY_EDGE[4] = {0.8759f, 0.7216f, 0.5294f, 1.f};  // 222 184 135

constexpr static GLfloat DESTROY[4]      = {0.8039f, 0.8039f, 0.f, 1.f};  // 205 205 0
constexpr static GLfloat DESTROY_EDGE[4] = {0.8039f, 0.f,     0.f, 1.f};  // 205 0 0

constexpr static GLfloat ELECTRO[4]      = {1.f,     0.4980f, 0.f,     1.f};  // 255 127 0
constexpr static GLfloat ELECTRO_EDGE[4] = {0.9333f, 0.1726f, 0.1726f, 1.f};  // 238 44 44

constexpr static GLfloat FOG[4]       = {0.8784f, 0.9333f, 0.9333f, 1.f};  // 224 238 238
constexpr static GLfloat FOG_EDGE[4]  = {0.9412f, 0.9726f, 1.f,     1.f};  // 240 248 255

constexpr static GLfloat GLASS[4]      = {0.4980f, 1.f,     0.8314f, 1.f};  // 127 255 212
constexpr static GLfloat GLASS_EDGE[4] = {0.2706f, 0.5451f, 0.4549f, 1.f};  // 69  139 116

constexpr static GLfloat HYPER[4]      = {0.6902f, 0.8784f, 0.9020f, 1.f};  // 176 224 230
constexpr static GLfloat HYPER_EDGE[4] = {0.2824f, 0.4628f, 1.f,     1.f};  // 72  118 255

constexpr static GLfloat IRON[4]      = {0.6902f, 0.6902f, 0.6902f, 1.f};  // 176 176 176
constexpr static GLfloat IRON_EDGE[4] = {0.5412f, 0.5412f, 0.5412f, 1.f};  // 138 138 138

constexpr static GLfloat JELLY[4]      = {0.9333f, 0.0706f, 0.5373f, 1.f};  // 238 18 137
constexpr static GLfloat JELLY_EDGE[4] = {0.5451f, 0.0392f, 0.3137f, 1.f};  // 139 10 80

constexpr static GLfloat KNOCK[4]      = {1.f,     0.5490f, 0.f, 1.f};  // 255 140 0
constexpr static GLfloat KNOCK_EDGE[4] = {0.5451f, 0.2706f, 0.f, 1.f};  // 139 69  0

constexpr static GLfloat STEEL[4]      = {0.6902f, 0.7686f, 0.8706f, 1.f};  // 176 196 222
constexpr static GLfloat STEEL_EDGE[4] = {0.4314f, 0.4824f, 0.5451f, 1.f};  // 110 123 139

constexpr static GLfloat MAGIC[4]      = {0.8157f, 0.1255f, 0.5647f, 1.f};  // 208 32 144
constexpr static GLfloat MAGIC_EDGE[4] = {0.4902f, 0.1490f, 0.8039f, 1.f};  // 125 38 205

constexpr static GLfloat MIDAS[4]      = {0.9333f, 0.7882f, 0.f,     1.f};  // 238 201 0
constexpr static GLfloat MIDAS_EDGE[4] = {0.5451f, 0.4118f, 0.0784f, 1.f};  // 139 105 20

constexpr static GLfloat NETWORK[4]      = {0.f,     0.9333f, 0.f,     1.f};  // 0 238 0
constexpr static GLfloat NETWORK_EDGE[4] = {0.1804f, 0.5451f, 0.3418f, 1.f};  // 46 139 87

constexpr static GLfloat ORIGIN[4]      = {1.f,     0.4157f, 0.4157f, 1.f};  // 255 106 106
constexpr static GLfloat ORIGIN_EDGE[4] = {0.5451f, 0.2275f, 0.3843f, 1.f};  // 139 58  98

constexpr static GLfloat PLUMBUM[4]      = {0.7569f, 0.8039f, 0.8039f, 1.f};  // 193 205 205
constexpr static GLfloat PLUMBUM_EDGE[4] = {0.5137f, 0.5451f, 0.5451f, 1.f};  // 131 139 139

constexpr static GLfloat QUICK[4]      = {0.f, 1.f,     1.f,     1.f};  // 0 255 255
constexpr static GLfloat QUICK_EDGE[4] = {0.f, 0.5451f, 0.5451f, 1.f};  // 0 139 139

constexpr static GLfloat ROLLING[4]      = {0.8039f, 0.3569f, 0.2706f, 1.f};  // 205 91 69
constexpr static GLfloat ROLLING_EDGE[4] = {0.5451f, 0.2431f, 0.1843f, 1.f};  // 139 62 47

constexpr static GLfloat SIMPLE[4]      = {0.6353f, 0.8039f, 0.3529f, 1.f};  // 162 205 90
constexpr static GLfloat SIMPLE_EDGE[4] = {0.4314f, 0.5451f, 0.2392f, 1.f};  // 110 139 61

constexpr static GLfloat TITAN[4]      = {1.f,     0.7567f, 0.1451f, 1.f};  // 255 193 37
constexpr static GLfloat TITAN_EDGE[4] = {0.8039f, 0.6078f, 0.1137f, 1.f};  // 205 155 29

constexpr static GLfloat ULTRA[4]      = {0.7490f, 0.2431f, 1.f,     1.f};  // 191 62 255
constexpr static GLfloat ULTRA_EDGE[4] = {0.4078f, 0.1333f, 0.5451f, 1.f};  // 104 34 139

constexpr static GLfloat INVUL[4]      = {0.4118f, 0.5451f, 0.4118f, 1.f};  // 105 139 105
constexpr static GLfloat INVUL_EDGE[4] = {0.1843f, 0.3098f, 0.3098f, 1.f};  // 47  79  79

constexpr static GLfloat WATER[4]      = {0.5294f, 0.8078f, 0.9216f, 1.f};  // 135 206 235
constexpr static GLfloat WATER_EDGE[4] = {0.4941f, 0.7529f, 0.9333f, 1.f};  // 126 192 238

constexpr static GLfloat EXTRA[4]      = {0.6078f, 0.8039f, 0.6078f, 1.f};  // 155 205 155
constexpr static GLfloat EXTRA_EDGE[4] = {0.4118f, 0.5451f, 0.4118f, 1.f};  // 105 139 105

constexpr static GLfloat YOGURT[4]      = {1.f,     0.9726f, 0.8628f, 1.f};  // 255 248 220
constexpr static GLfloat YOGURT_EDGE[4] = {0.8039f, 0.7843f, 0.6941f, 1.f};  // 205 200 177

constexpr static GLfloat ZYGOTE[4]      = {0.2628f, 0.8039f, 0.5020f, 1.f};  // 67 205 128
constexpr static GLfloat ZYGOTE_EDGE[4] = {0.1804f, 0.5451f, 0.3412f, 1.f};  // 46 139 87

constexpr static GLfloat ZYGOTE_SPAWN[4]      = {0.4f,    0.8039f, 0.f,     1.f};  // 104 205 128
constexpr static GLfloat ZYGOTE_SPAWN_EDGE[4] = {0.2706f, 0.5451f, 0.f, 1.f};  // 69  139 0

}

#endif  // __ARKANOID_RGBSTRUCT__H__
