cmake_minimum_required(VERSION 3.4.1)

# zlib
#set( TARGET_ZLIB zlib )
#set( SOURCE_ZLIB
#    src/main/cpp/zlib/adler32.c
#    src/main/cpp/zlib/compress.c
#    src/main/cpp/zlib/crc32.c
#    src/main/cpp/zlib/deflate.c
#    src/main/cpp/zlib/gzclose.c
#    src/main/cpp/zlib/gzlib.c
#    src/main/cpp/zlib/gzread.c
#    src/main/cpp/zlib/gzwrite.c
#    src/main/cpp/zlib/infback.c
#    src/main/cpp/zlib/inffast.c
#    src/main/cpp/zlib/inflate.c
#    src/main/cpp/zlib/inftrees.c
#    src/main/cpp/zlib/trees.c
#    src/main/cpp/zlib/uncompr.c
#    src/main/cpp/zlib/zutil.c
#)
#add_library( ${TARGET_ZLIB} STATIC ${SOURCE_ZLIB} )

# png
set( TARGET_PNG png )
set( SOURCE_PNG
    src/main/cpp/png/png.c
    src/main/cpp/png/pngerror.c
    src/main/cpp/png/pngget.c
    src/main/cpp/png/pngmem.c
    src/main/cpp/png/pngpread.c
    src/main/cpp/png/pngread.c
    src/main/cpp/png/pngrio.c
    src/main/cpp/png/pngrtran.c
    src/main/cpp/png/pngrutil.c
    src/main/cpp/png/pngset.c
    src/main/cpp/png/pngtest.c
    src/main/cpp/png/pngtrans.c
    src/main/cpp/png/pngwio.c
    src/main/cpp/png/pngwrite.c
    src/main/cpp/png/pngwtran.c
    src/main/cpp/png/pngwutil.c
)
add_library( ${TARGET_PNG} STATIC ${SOURCE_PNG} )

# Arkanoid library
include_directories(
    src/main/cpp/include
    src/main/cpp/png
)
set( TARGET_ARKANOID Arkanoid )
set( SOURCE_ARKANOID
    src/main/cpp/src/AssetStorage.cpp
    src/main/cpp/src/AsyncContext.cpp
    src/main/cpp/src/AsyncContextHelper.cpp
    src/main/cpp/src/Block.cpp
    src/main/cpp/src/EGLConfigChooser.cpp
    src/main/cpp/src/ExplosionPackage.cpp
    src/main/cpp/src/GameProcessor.cpp
    src/main/cpp/src/Level.cpp
    src/main/cpp/src/LevelDimens.cpp
    src/main/cpp/src/Prize.cpp
    src/main/cpp/src/PrizePackage.cpp
    src/main/cpp/src/PrizeProcessor.cpp
    src/main/cpp/src/Resources.cpp
    src/main/cpp/src/Shader.cpp
    src/main/cpp/src/SoundBuffer.cpp
    src/main/cpp/src/SoundPlayer.cpp
    src/main/cpp/src/SoundProcessor.cpp
    src/main/cpp/src/Texture.cpp
    src/main/cpp/src/utils.cpp
)
add_library( ${TARGET_ARKANOID} SHARED ${SOURCE_ARKANOID} )
target_link_libraries( ${TARGET_ARKANOID} log dl z png android EGL GLESv2 OpenSLES )

