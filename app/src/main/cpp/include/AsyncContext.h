#ifndef __ARKANOID_ASYNC_CONTEXT__H__
#define __ARKANOID_ASYNC_CONTEXT__H__

#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <jni.h>

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "ActiveObject.h"
#include "Ball.h"
#include "Bite.h"
#include "ExplosionPackage.h"
#include "LaserPackage.h"
#include "Level.h"
#include "LevelDimens.h"
#include "Prize.h"
#include "PrizePackage.h"
#include "Resources.h"
#include "rgbstruct.h"
#include "RowCol.h"
#include "Shader.h"

namespace game {

/**
 * @class AsyncContext AsyncContext.h "include/AsyncContext.h"
 * @brief Represents a render thread and provides an interface to interact
 * with User layer: input commands, gestures, graphic output and events to GUI.
 */
class AsyncContext : public ActiveObject {
public:
  typedef AsyncContext* Ptr;

  AsyncContext(JavaVM* jvm);
  virtual ~AsyncContext() noexcept;

  /** @defgroup Callbacks These methods are responses of incoming events
   *  which AsyncContext subscribed on.
   *  @{
   */
  /// @brief Called when a surface has been prepared for rendering
  /// and hence corresponding event has occurred.
  /// @param window Pointer to a window associated with the rendering surface.
  void callback_setWindow(ANativeWindow* window);
  /// @brief Called when load resources requested.
  void callback_loadResources(bool /* dummy */);
  /// @brief Called when user makes a motion gesture within the surface.
  /// @param distance Distance the user's pointer has passed.
  void callback_shiftGamepad(float distance);
  /// @brief Called when user sends a command to throw a ball.
  void callback_throwBall(float angle /* dummy */);
  /// @brief Called when user requests a level to be loaded
  void callback_loadLevel(Level::Ptr level);
  /// @brief Called when ball has moved to a new position.
  void callback_moveBall(Ball moved_ball);
  /// @brief Called when ball has been lost.
  void callback_lostBall(float is_lost);
  /// @brief Called when ball has been stopped.
  void callback_stopBall(bool /* dummy */);
  /// @brief Called when block has been impacted.
  void callback_blockImpact(RowCol block);
  /// @brief Called when level has been successfully finished.
  void callback_levelFinished(bool is_finished);
  /// @brief Called when requested to draw particle system explosion.
  void callback_explosion(ExplosionPackage package);
  /// @brief Called when prize has been generated.
  void callback_prizeReceived(PrizePackage package);
  /// @brief Called when prize has been caught.
  void callback_prizeCaught(PrizePackage package);
  /// @brief Called when drop ball's appearance to standard has been requested.
  void callback_dropBallAppearance(bool /* dummy */);
  /// @brief Called when bite's width has changed.
  void callback_biteWidthChanged(BiteEffect effect);
  /// @brief Called when laser beam changed visibility.
  void callback_laserBeamVisibility(bool is_visible);
  /// @brief Called when laser beam impacts block.
  void callback_laserBlockImpact(bool /* dummy */);
  /** @} */  // end of Callbacks group

  /** @defgroup GameStat Get game statistics
   * @{
   */
  /// @brief Gets current state of last loaded level.
  Level::Ptr getCurrentLevelState();
  /** @} */  // end of GameStat group

  /** @defgroup Resources Bind with external resources.
   * @{
   */
  /// @brief Sets the pointer to external resources.
  void setResourcesPtr(Resources* resources);
  /** @} */  // end of Resources group

// ----------------------------------------------
/* Private member-functions */
private:
  /** @defgroup JNIEnvironment Native glue between core and GUI.
   * @{
   */
  /// @brief Attaches this thread to an existing JVM.
  void attachToJVM();
  /// @brief Detaches this thread from an existing JVM it had been
  /// previously attached.
  void detachFromJVM();
  /** @} */  // end of JNIEnvironment group

public:
  /** @addtogroup JNIEnvironment
   * @{
   */
  inline void setMasterObject(jobject object) { master_object = object; }
  inline void setOnErrorTextureLoadMethodID(jmethodID id) { fireJavaEvent_errorTextureLoad_id = id; }
  /** @} */  // end of JNIEnvironment group

// ----------------------------------------------
/* Public data-members */
public:
  /** @defgroup Event Outcoming events and listeners for incoming events.
   * @{
   */
  /// @brief Listens for event which occurs when surface will be prepared.
  EventListener<ANativeWindow*> surface_received_listener;
  /// @brief Listens for load resources request.
  EventListener<bool> load_resources_listener;
  /// @brief Listens for event which occurs when user performs a motion gesture.
  EventListener<float> shift_gesture_listener;
  /// @brief Listens for event which occurs when user sends throw ball command.
  EventListener<float> throw_ball_listener;
  /// @brief Listens for event which occurs when user requests a level to be loaded.
  EventListener<Level::Ptr> load_level_listener;
  /// @brief Listens for event which occurs when ball has moved to a new position.
  EventListener<Ball> move_ball_listener;
  /// @brief Listens for event which occurs when ball has been lost.
  EventListener<bool> lost_ball_listener;
  /// @brief Listens for event which occurs when ball has been stopped.
  EventListener<bool> stop_ball_listener;
  /// @brief Listens for event which occurs when block has been impacted.
  EventListener<RowCol> block_impact_listener;
  /// @brief Listens for event which occurs when level has been successfully finished.
  EventListener<bool> level_finished_listener;
  /// @brief Listens for event which occurs when particle system explosion has been requested.
  EventListener<ExplosionPackage> explosion_listener;
  /// @brief Listens for event which occurs when prize has been generated.
  EventListener<PrizePackage> prize_listener;
  /// @brief Listens for event which occurs when prize has been caught.
  EventListener<PrizePackage> prize_caught_listener;
  /// @brief Listens for event which drop ball's appearance to standard has been requested.
  EventListener<bool> drop_ball_appearance_listener;
  /// @brief Listens for event which changes bite's width.
  EventListener<BiteEffect> bite_width_changed_listener;
  /// @brief Listens for laser beam visibility.
  EventListener<bool> laser_beam_visibility_listener;
  /// @brief Listens for laser block impact.
  EventListener<bool> laser_block_impact_listener;

  /// @brief Notifies for measured aspect ratio.
  Event<float> aspect_ratio_event;
  /// @brief Notifies ball has been placed to it's initial position.
  Event<Ball> init_ball_position_event;
  /// @brief Notifies bite's dimensions have been measured.
  Event<Bite> init_bite_event;
  /// @brief Notifies for measured dimensions of loaded level.
  Event<LevelDimens> level_dimens_event;
  /// @brief Notifies bite location has changed.
  Event<Bite> bite_location_event;
  /// @brief Notifies prize visual location has changed.
  Event<PrizePackage> prize_location_event;
  /// @brief Notifies prize has gone.
  Event<int> prize_gone_event;
  /// @brief Notifies laser beam has changed it's position.
  Event<LaserPackage> laser_beam_event;
  /// @brief Notifies laser beam pulse has emerged.
  Event<bool> laser_pulse_event;
  /** @} */  // end of Event group

// ----------------------------------------------
/* Private data-members */
private:
  /** @addtogroup JNIEnvironment
   * @{
   */
  JavaVM* m_jvm;  //!< Pointer to Java Virtual Machine in current session.
  JNIEnv* m_jenv;  //!< Pointer to environment local within this thread.
  jobject master_object;
  jmethodID fireJavaEvent_errorTextureLoad_id;
  /** @} */  // end of JNIEnvironment group

  /** @defgroup WindowSurface Rendering surface stuff.
   * @see https://www.khronos.org/registry/egl/sdk/docs/man/html/eglIntro.xhtml
   * @{
   */
  /// Pointer to a window associated with the rendering surface.
  ANativeWindow* m_window;
  EGLDisplay m_egl_display;  //!< Connection between EGL API and native display.
  EGLSurface m_egl_surface;  //!< Native window extension with auxiliary buffers.
  EGLContext m_egl_context;  //!< Bind client API rendering to an EGL surface.
  EGLint m_width, m_height;  //!< Surface sizes.
  GLfloat m_aspect;  //!< Surface aspect ratio.
  EGLConfig m_config;  //!< Selected frame buffer configuration, best one.
  EGLint m_num_configs;  //!< Number of available frame buffer configurations.
  EGLint m_format;  //!< Frame buffer native visual type (actual configuration).
  /** @} */  // end of WindowSurface group

  /** @defgroup LogicData Game logic related data members.
   * @{
   */
  constexpr static int particleSize = 5;
  constexpr static int particleSystemSize = 1000;

  constexpr static int particleSpiralSize = 4;
  constexpr static int particeSpiralSystemBranches = 4;
  constexpr static int particleSpiralSystemBranchSize = 100;
  constexpr static int particleSpiralSystemSize = particleSpiralSystemBranchSize * particeSpiralSystemBranches;

  GLfloat m_position;  //!< Last received position value of user's motion gesture.
  Bite m_bite;  //!< Physical bite's representation.
  BiteEffect m_bite_effect;  //!< Changed width of bite due to prize.
  Ball m_ball;  //!< Physical ball's representation.
  std::queue<RowCol> m_impact_queue;  //!< Queue of impacted blocks' indices.

  GLfloat* m_bite_vertex_buffer;  //!< Re-usable buffer for vertices of bite.
  GLfloat* m_bite_color_buffer;   //!< Re-usable buffer for colors of bite.
  GLfloat* m_ball_vertex_buffer;  //!< Re-usable buffer for vertices of ball.
  GLfloat* m_ball_color_buffer;   //!< Re-usable buffer for color of ball.
  GLfloat* m_bg_vertex_buffer;    //!< Re-usable buffer for background vertices.
  GLfloat* m_particle_diverge_buffer;    //!< Re-usable buffer for diverging particle system.
  GLfloat* m_particle_converge_buffer;   //!< Re-usable buffer for converging particle system.
  GLfloat* m_particle_spiral_buffer;     //!< Re-usable buffer for particle spiral system.
  GLushort* m_rectangle_index_buffer;    //!< Re-usable buffer for indices of rectangle.
  GLushort* m_octagon_index_buffer;      //!< Re-usable buffer for indices of octagon.
  GLfloat* m_rectangle_texCoord_buffer;  //!< Re-usable buffer for texture coords of rectangle.

  Level::Ptr m_level;  //!< Last loaded game level.
  GLfloat* m_level_vertex_buffer;  //!< Re-usable buffer for vertices of level.
  GLfloat* m_level_color_buffer;   //!< Re-usable buffer for colors of level.
  GLushort* m_level_index_buffer;  //!< Re-usable buffer for indices of level's blocks.

  std::default_random_engine m_generator;
  std::uniform_real_distribution<float> m_particle_distribution;
  clock_t m_last_time;
  float m_particle_time;
  bool m_render_explosion;
  std::vector<ExplosionPackage> m_explosion_packages;

  std::unordered_map<int, PrizePackage> m_prize_packages;
  std::unordered_map<int, clock_t> m_prize_last_timers;
  std::unordered_map<int, float> m_prize_timers;
  std::unordered_set<int> m_removed_prizes;

  clock_t m_prize_catch_last_time;
  float m_prize_catch_time;
  bool m_render_prize_catch;
  std::vector<GLfloat> m_caught_prizes_x_coords;

  clock_t m_laser_last_time;
  float m_laser_time;
  bool m_render_laser;
  bool m_laser_interruption;
  /** @} */  // end of LogicData group

  /** @defgroup Shaders Shaders for rendering game components.
   * @{
   */
  shader::ShaderHelper::Ptr m_level_shader;
  shader::ShaderHelper::Ptr m_bite_shader;
  shader::ShaderHelper::Ptr m_ball_shader;
  shader::ShaderHelper::Ptr m_explosion_shader;
  shader::ShaderHelper::Ptr m_sample_shader;
  shader::ShaderHelper::Ptr m_prize_shader;
  shader::ShaderHelper::Ptr m_prize_catch_shader;
  shader::ShaderHelper::Ptr m_laser_shader;
  /** @} */  // end of Shaders group

  /** @defgroup Mutex Thread-safety variables
   * @{
   */
  std::mutex m_jnienvironment_mutex;  //!< Sentinel for thread attach to JVM.
  std::mutex m_surface_mutex;  //!< Sentinel for window setting.
  std::mutex m_load_resources_mutex;  //!< Sentinel for load resources.
  std::mutex m_shift_gamepad_mutex;  //!< Sentinel for shifting gesture event.
  std::mutex m_throw_ball_mutex;  //!< Sentinel for throw ball user command.
  std::mutex m_load_level_mutex;  //!< Sentinel for load level user request.
  std::mutex m_move_ball_mutex;  //!< Sentinel for move ball to a new position.
  std::mutex m_lost_ball_mutex;  //!< Sentinel for lost ball flag.
  std::mutex m_stop_ball_mutex;
  std::mutex m_block_impact_mutex;  //!< Sentinel for block impact event.
  std::mutex m_level_finished_mutex;  //!< Sentinel for level has been successfully finished.
  std::mutex m_explosion_mutex;  //!< Sentinel for particle system explosion.
  std::mutex m_prize_mutex;  //!< Sentinel for prize receiving.
  std::mutex m_prize_caught_mutex;  //!< Sentinel for prize has been caught.
  std::mutex m_drop_ball_appearance_mutex;
  std::mutex m_bite_width_changed_mutex;
  std::mutex m_laser_beam_visibility_mutex;
  std::mutex m_laser_block_impact_mutex;
  std::atomic_bool m_surface_received;  //!< Window has been set.
  std::atomic_bool m_load_resources_received;  //!< Load resources requested.
  std::atomic_bool m_shift_gamepad_received;  //!< Shift gesture has occurred.
  std::atomic_bool m_throw_ball_received;  //!< Throw ball command has been received.
  std::atomic_bool m_load_level_received;  //!< Load level request has been received.
  std::atomic_bool m_move_ball_received;  //!< Move ball event has been received.
  std::atomic_bool m_lost_ball_received;  //!< Ball has been lost received.
  std::atomic_bool m_stop_ball_received;
  std::atomic_bool m_block_impact_received;  //!< Block impact has been received.
  std::atomic_bool m_level_finished_received;  //!< Level has been successfully finished.
  std::atomic_bool m_explosion_received;  //!< Request for explosion received.
  std::atomic_bool m_prize_received;  //!< Prize has been received.
  std::atomic_bool m_prize_caught_received;  //!< Prize has been caught received.
  std::atomic_bool m_drop_ball_appearance_received;
  std::atomic_bool m_bite_width_changed_received;
  std::atomic_bool m_laser_beam_visibility_received;
  std::atomic_bool m_laser_block_impact_received;
  /** @} */  // end of Mutex group

  /** @defgroup SafetyFlag Logic-safety variables
   * @{
   */
  bool m_window_set;
  /** @} */  // end of SafetyFlag group

  /** @addtogroup Resources
   * @{
   */
  Resources* m_resources;
  const native::Texture* m_bg_texture;
  /** @} */  // end of Resources group

// ----------------------------------------------
/* Private member-functions */
private:
  /** @defgroup ActiveObject Basic thread lifecycle and operation functions.
   * @{
   */
  void onStart() override final;  //!< Right after thread has been launched.
  void onStop() override final;   //!< Right before thread has been stopped.
  /// @brief Automatic check whether this thread should continue to operate.
  /// @return Whether this thread should continue sleeping (false)
  /// or working (true).
  bool checkForWakeUp() override final;
  /// @brief Operate the data or do some job as a response of incoming
  /// outer event.
  void eventHandler() override final;
  /** @} */  // end of ActiveObject group

  /** @defgroup Processors Actions being performed by AsyncContext when
   *  corresponding event occurred and has been caught.
   *  @{
   */
  /// @brief Given a rendering surface in Java, performs setting of native
  /// window to interact with during actual rendering.
  void process_setWindow();
  /// @brief Loads external resources into Graphic memory.
  void process_loadResources();
  /// @brief Performs visual translation of the gamepad by given distance.
  void process_shiftGamepad();
  /// @brief Performs visual ball throwing.
  void process_throwBall();
  /// @brief Performs visual refreshing of current level.
  void process_loadLevel();
  /// @brief Performs visual displacement of the ball to a new position.
  void process_moveBall();
  /// @brief Processing when ball has been lost.
  void process_lostBall();
  /// @brief Processing when ball has been stopped.
  void process_stopBall();
  /// @brief Performs visual block impact.
  void process_blockImpact();
  /// @brief Performs visual level finalization.
  void process_levelFinished();
  /// @brief Performs visual particle system explosion.
  void process_explosion();
  /// @brief Performs visual prize generation.
  void process_prizeReceived();
  /// @brief Performs visual prize catching.
  void process_prizeCaught();
  /// @brief Drops ball's appearance to standard.
  void process_dropBallAppearance();
  /// @brief Performs visual change of bite's width.
  void process_biteWidthChanged();
  /// @brief Performs laser beam visibility changes.
  void process_laserBeamVisibility();
  /// @brief Processing laser block impact.
  void process_laserBlockImpact();
  /** @} */  // end of Processors group

private:
  /** @defgroup LogicFunc Game logic related member functions.
   * @{
   */
  /// @brief Initialize game state.
  void initGame();
  /// @brief Sets the bite into shifted state.
  /// @param position Normalized position the bite should move at.
  /// @note Position should be within [-1, 1] segment.
  void moveBite(float position);
  /// @brief Sets the ball into shifted state.
  /// @param x_position Normalized position along X axis the ball should move at.
  /// @param y_position Normalized position along Y axis the ball should move at.
  /// @note Positions should both be within [-1, 1] segment.
  void moveBall(float x_position, float y_position);
  /// @brief Adds prize to be removed later.
  void addPrizeToRemoved(int prize_id);
  /// @brief Clears removed prizes.
  void clearRemovedPrizes();
  /// @brief Clean-up prize structures and counters.
  void clearPrizeStructures();
  /// @brief Checks whether specified block is present in current level.
  /// @return TRUE is block is present in newly loaded level, FALSE otherwise.
  /// @detail This allows to avoid crash when load level event is followed by
  /// block impact event, but it's processing is performed before the former's
  /// processing due to asynchronous event handling.
  bool checkBlockPresense(int row, int col);
  /// @brief Sets bite's and ball's appearance according to current ball's effect.
  void setBiteBallAppearance(BallEffect effect);
  /** @} */  // end of LogicFunc group

private:
  /** @defgroup GraphicsContext Low-layers functions to setup graphics.
   * @{
   */
  /// @brief Configures rendering surface, context and display.
  /// @return false in case of error, true in case of success.
  bool displayConfig();
  /// @brief Sets gl options for already prepared context.
  /// @note Different contexts could have different gl options.
  void glOptionsConfig();
  /// @brief Releases surface, context and display resources.
  void destroyDisplay();
  /// @brief Render a frame.
  void render();
  /// @brief Initializes particle system.
  void initParticleSystem();
  /// @brief Continue rendering for specified delay in ms.
  /// @param ms Time in ms.
  void delay(int ms);
  /** @} */  // end of GraphicsContext group

  /** @defgroup Drawings Draw routines.
   * @{
   */
  /// @brief Draws current level's state.
  void drawLevel();
  /// @brief Draws block of current level.
  void drawBlock(int row, int col);
  /// @brief Draws textured block of current level,
  void drawTexturedBlock(int row, int col, const std::string& texture);
  /// @brief Draws bite at it's current position.m_load_resources_received
  void drawBite();
  /// @brief Draws ball at it's current position.
  void drawBall();
  /// @brief Draws particle system explosion.
  void drawExplosion(GLfloat x, GLfloat y, const util::BGRA<GLfloat>& bgra, Kind kind);
  /// @brief Draws textured background.
  void drawBackground();
  /// @brief Draws prize of specified type at given location.
  void drawPrize(const PrizePackage& prize);
  /// @brief Draws prize catch animation.
  void drawPrizeCatch(GLfloat x, GLfloat y, const util::BGRA<GLfloat>& bgra);
  /// @brief Draws laser sprite originated at specified point.
  void drawLaser(GLfloat x, GLfloat y);
  /** @} */  // end of Drawings group
};

}  // namespace game

#endif  // __ARKANOID_ASYNC_CONTEXT__H__
