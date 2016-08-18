#ifndef __ARKANOID_EXCEPTIONS__H__
#define __ARKANOID_EXCEPTIONS__H__

#include <jni.h>
#include <exception>

struct JNI_exception : public std::exception {
  JNI_exception(
      JNIEnv* jenv,
      jclass exception,
      const char* message = "Thrown from native") {
    jenv->ExceptionDescribe();
    jenv->ThrowNew(exception, message);
  }
  JNI_exception() {
    // in case JNIEnv pointer is null
  }
  JNI_exception(JNIEnv* jenv) {
    jenv->ExceptionDescribe();  // in static methods
  }

  const char* what() const noexcept override final {
    return "Thrown from native\n";
  }
};

struct GraphicsNotConfiguredException : public std::exception {
  const char* what() const noexcept override final {
    return "Graphics not configured\n";
  }
};

struct ShaderException : public std::exception {
public:
  ShaderException(const char* message) : message(message) {}
  const char* what() const noexcept override final {
    return message;
  }

private:
  const char* message;
};

struct GameProcessorException : public std::exception {
public:
  GameProcessorException(const char* message) : message(message) {}
  const char* what() const noexcept override final {
    return message;
  }

private:
  const char* message;
};

struct SoundProcessorException : public std::exception {
public:
  SoundProcessorException(const char* message) : message(message) {}
  const char* what() const noexcept override final {
    return message;
  }

private:
  const char* message;
};

#endif  // __ARKANOID_EXCEPTIONS__H__
