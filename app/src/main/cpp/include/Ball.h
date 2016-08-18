#ifndef __ARKANOID_BALL__H__
#define __ARKANOID_BALL__H__

#include "BallDimens.h"
#include "BallPosition.h"
#include "Params.h"

namespace game {

enum class BallEffect : int {
  NONE = 0,      //!
  EASY = 1,      //! block effect
  EASY_T = 2,    //! timed effect ; block effect
  EXPLODE = 3,   //! block effect
  GOO = 4,       //! timed effect
  JUMP = 5,      //! timed effect ; block effect
  MIRROR = 6,    //! timed effect
  PIERCE = 7,    //! timed effect ; block effect
  PROTECT = 8,   //! timed effect, no-op for ball, only for bite
  RANDOM = 9,    //! timed effect
  UPGRADE = 10,  //! block effect
  DEGRADE = 11,  //! block effect
  ZYGOTE = 12
};

class Ball {
public:
  Ball(GLfloat width = 0.f, GLfloat height = 0.f)
    : m_dimens(width, height)
    , m_pose()
    , m_angle(0.0f)
    , m_velocity(BallParams::ballSpeed)
    , m_effect(BallEffect::NONE) {
  }

  inline const BallDimens& getDimens() const { return m_dimens; }
  inline const BallPosition& getPose() const { return m_pose; }
  inline GLfloat getAngle() const { return m_angle; }
  inline GLfloat getVelocity() const { return m_velocity; }
  inline BallEffect getEffect() const { return m_effect; }

  inline void setXPose(GLfloat x_pose) { m_pose.setX(x_pose); }
  inline void setYPose(GLfloat y_pose) { m_pose.setY(y_pose); }
  inline void setAngle(GLfloat angle) { m_angle = angle; }
  inline void fastSpeed() { m_velocity = BallParams::ballFastSpeed; }
  inline void normalSpeed() { m_velocity = BallParams::ballSpeed; }
  inline void slowSpeed() { m_velocity = BallParams::ballSlowSpeed; }
  inline void setEffect(BallEffect effect) { m_effect = effect; }

 private:
  BallDimens m_dimens;
  BallPosition m_pose;  //!< Location of ball's center.
  GLfloat m_angle;  //!< Angle (radian) between velocity and positive X axis.
  GLfloat m_velocity;
  BallEffect m_effect;
};

}

#endif  // __ARKANOID_BALL__H__
