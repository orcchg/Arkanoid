#include <string>

#include "AsyncContextHelper.h"
#include "Level.h"
#include "Resources.h"

static JavaVM* jvm = nullptr;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  jvm = vm;
  return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* vm, void* reserved) {}

/* init */
// ----------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_init
  (JNIEnv *jenv, jobject object) {
  auto ptr = new AsyncContextHelper(jenv, object);
  jlong descriptor = (jlong)(intptr_t) ptr;

  /* Subscribe on events incoming from outside */
  ptr->acontext->surface_received_listener = ptr->surface_received_event.createListener(&game::AsyncContext::callback_setWindow, ptr->acontext);
  ptr->acontext->load_resources_listener = ptr->load_resources_event.createListener(&game::AsyncContext::callback_loadResources, ptr->acontext);
  ptr->acontext->shift_gesture_listener = ptr->shift_gesture_event.createListener(&game::AsyncContext::callback_shiftGamepad, ptr->acontext);
  ptr->acontext->throw_ball_listener = ptr->throw_ball_event.createListener(&game::AsyncContext::callback_throwBall, ptr->acontext);
  ptr->acontext->load_level_listener = ptr->load_level_event.createListener(&game::AsyncContext::callback_loadLevel, ptr->acontext);
  ptr->acontext->move_ball_listener = ptr->processor->move_ball_event.createListener(&game::AsyncContext::callback_moveBall, ptr->acontext);
  ptr->acontext->lost_ball_listener = ptr->processor->lost_ball_event.createListener(&game::AsyncContext::callback_lostBall, ptr->acontext);
  ptr->acontext->stop_ball_listener = ptr->processor->stop_ball_event.createListener(&game::AsyncContext::callback_stopBall, ptr->acontext);
  ptr->acontext->block_impact_listener = ptr->processor->block_impact_event.createListener(&game::AsyncContext::callback_blockImpact, ptr->acontext);
  ptr->acontext->level_finished_listener = ptr->processor->level_finished_event.createListener(&game::AsyncContext::callback_levelFinished, ptr->acontext);
  ptr->acontext->explosion_listener = ptr->processor->explosion_event.createListener(&game::AsyncContext::callback_explosion, ptr->acontext);
  ptr->acontext->prize_listener = ptr->processor->prize_event.createListener(&game::AsyncContext::callback_prizeReceived, ptr->acontext);
  ptr->acontext->prize_caught_listener = ptr->prize_processor->prize_caught_event.createListener(&game::AsyncContext::callback_prizeCaught, ptr->acontext);
  ptr->acontext->drop_ball_appearance_listener = ptr->processor->drop_ball_appearance_event.createListener(&game::AsyncContext::callback_dropBallAppearance, ptr->acontext);
  ptr->acontext->bite_width_changed_listener = ptr->processor->bite_width_changed_event.createListener(&game::AsyncContext::callback_biteWidthChanged, ptr->acontext);
  ptr->acontext->laser_beam_visibility_listener = ptr->processor->laser_beam_visibility_event.createListener(&game::AsyncContext::callback_laserBeamVisibility, ptr->acontext);
  ptr->acontext->laser_block_impact_listener = ptr->processor->laser_block_impact_event.createListener(&game::AsyncContext::callback_laserBlockImpact, ptr->acontext);

  ptr->processor->aspect_ratio_listener = ptr->acontext->aspect_ratio_event.createListener(&game::GameProcessor::callback_aspectMeasured, ptr->processor);
  ptr->processor->load_level_listener = ptr->load_level_event.createListener(&game::GameProcessor::callback_loadLevel, ptr->processor);
  ptr->processor->throw_ball_listener = ptr->throw_ball_event.createListener(&game::GameProcessor::callback_throwBall, ptr->processor);
  ptr->processor->init_ball_position_listener = ptr->acontext->init_ball_position_event.createListener(&game::GameProcessor::callback_initBall, ptr->processor);
  ptr->processor->init_bite_listener = ptr->acontext->init_bite_event.createListener(&game::GameProcessor::callback_initBite, ptr->processor);
  ptr->processor->level_dimens_listener = ptr->acontext->level_dimens_event.createListener(&game::GameProcessor::callback_levelDimens, ptr->processor);
  ptr->processor->bite_location_listener = ptr->acontext->bite_location_event.createListener(&game::GameProcessor::callback_biteMoved, ptr->processor);
  ptr->processor->prize_caught_listener = ptr->prize_processor->prize_caught_event.createListener(&game::GameProcessor::callback_prizeCaught, ptr->processor);
  ptr->processor->laser_beam_listener = ptr->acontext->laser_beam_event.createListener(&game::GameProcessor::callback_laserBeam, ptr->processor);

  ptr->prize_processor->aspect_ratio_listener = ptr->acontext->aspect_ratio_event.createListener(&game::PrizeProcessor::callback_aspectMeasured, ptr->prize_processor);
  ptr->prize_processor->bite_location_listener = ptr->acontext->bite_location_event.createListener(&game::PrizeProcessor::callback_biteMoved, ptr->prize_processor);
  ptr->prize_processor->init_bite_listener = ptr->acontext->init_bite_event.createListener(&game::PrizeProcessor::callback_initBite, ptr->prize_processor);
  ptr->prize_processor->prize_listener = ptr->processor->prize_event.createListener(&game::PrizeProcessor::callback_prizeReceived, ptr->prize_processor);
  ptr->prize_processor->prize_location_listener = ptr->acontext->prize_location_event.createListener(&game::PrizeProcessor::callback_prizeLocated, ptr->prize_processor);
  ptr->prize_processor->prize_gone_listener = ptr->acontext->prize_gone_event.createListener(&game::PrizeProcessor::callback_prizeHasGone, ptr->prize_processor);

  ptr->sound_processor->load_resources_listener = ptr->load_resources_event.createListener(&native::sound::SoundProcessor::callback_loadResources, ptr->sound_processor);
  ptr->sound_processor->lost_ball_listener = ptr->processor->lost_ball_event.createListener(&native::sound::SoundProcessor::callback_lostBall, ptr->sound_processor);
  ptr->sound_processor->bite_impact_listener = ptr->processor->bite_impact_event.createListener(&native::sound::SoundProcessor::callback_biteImpact, ptr->sound_processor);
  ptr->sound_processor->block_impact_listener = ptr->processor->block_impact_event.createListener(&native::sound::SoundProcessor::callback_blockImpact, ptr->sound_processor);
  ptr->sound_processor->wall_impact_listener = ptr->processor->wall_impact_event.createListener(&native::sound::SoundProcessor::callback_wallImpact, ptr->sound_processor);
  ptr->sound_processor->level_finished_listener = ptr->processor->level_finished_event.createListener(&native::sound::SoundProcessor::callback_levelFinished, ptr->sound_processor);
  ptr->sound_processor->explosion_listener = ptr->processor->explosion_event.createListener(&native::sound::SoundProcessor::callback_explosion, ptr->sound_processor);
  ptr->sound_processor->prize_caught_listener = ptr->prize_processor->prize_caught_event.createListener(&native::sound::SoundProcessor::callback_prizeCaught, ptr->sound_processor);
  ptr->sound_processor->laser_beam_visibility_listener = ptr->processor->laser_beam_visibility_event.createListener(&native::sound::SoundProcessor::callback_laserBeamVisibility, ptr->sound_processor);
  ptr->sound_processor->laser_block_impact_listener = ptr->processor->laser_block_impact_event.createListener(&native::sound::SoundProcessor::callback_laserBlockImpact, ptr->sound_processor);
  ptr->sound_processor->laser_pulse_listener = ptr->acontext->laser_pulse_event.createListener(&native::sound::SoundProcessor::callback_laserPulse, ptr->sound_processor);
  ptr->sound_processor->ball_effect_listener = ptr->processor->ball_effect_event.createListener(&native::sound::SoundProcessor::callback_ballEffect, ptr->sound_processor);

  return descriptor;
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_start
  (JNIEnv *jenv, jobject, jlong descriptor) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
  ptr->acontext->launch();
  ptr->processor->launch();
  ptr->prize_processor->launch();
  ptr->sound_processor->launch();
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_stop
  (JNIEnv *jenv, jobject, jlong descriptor) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
  ptr->acontext->stop();
  ptr->processor->stop();
  ptr->prize_processor->stop();
  ptr->sound_processor->stop();
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_destroy
  (JNIEnv *jenv, jobject, jlong descriptor) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
  delete ptr;
  ptr = nullptr;
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_setSurface
  (JNIEnv *jenv, jobject, jlong descriptor, jobject surface) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;

  if (surface == nullptr) {
    DBG("Releasing window...");
    ANativeWindow_release(ptr->window);
    ptr->window = nullptr;
  } else {
    DBG("Setting window...");
    ptr->window = ANativeWindow_fromSurface(jenv, surface);
  }

  ptr->surface_received_event.notifyListeners(ptr->window);
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_setResourcesPtr
  (JNIEnv *, jobject, jlong descriptor, jlong resources) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
  game::Resources* res_ptr = (game::Resources*) resources;
  ptr->acontext->setResourcesPtr(res_ptr);
  ptr->sound_processor->setResourcesPtr(res_ptr);
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_loadResources
  (JNIEnv *, jobject, jlong descriptor) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
  ptr->load_resources_event.notifyListeners(true);
}

/* User actions */
// ----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_shiftGamepad
  (JNIEnv *jenv, jobject, jlong descriptor, jfloat position) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
  ptr->shift_gesture_event.notifyListeners(position);
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_throwBall
  (JNIEnv *jenv, jobject, jlong descriptor, jfloat angle) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
  ptr->throw_ball_event.notifyListeners(angle);
}

/* Tools */
// ----------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_loadLevel
  (JNIEnv *jenv, jobject, jlong descriptor, jobjectArray in_level_Java) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;

  jsize length = jenv->GetArrayLength(in_level_Java);
  std::vector<std::string> array;
  array.reserve(length);

  for (jsize i = 0; i < length; ++i) {
    jstring java_str = (jstring) jenv->GetObjectArrayElement(in_level_Java, i);
    const char* raw_str = jenv->GetStringUTFChars(java_str, nullptr);
    array.emplace_back(raw_str);  // copy chars
    jenv->ReleaseStringUTFChars(java_str, raw_str);
  }

  auto level = game::Level::fromStringArray(array, length);
  ptr->load_level_event.notifyListeners(level);
}

JNIEXPORT jobjectArray JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_saveLevel
  (JNIEnv *jenv, jobject, jlong descriptor) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;

  game::Level::Ptr level_ptr = ptr->acontext->getCurrentLevelState();
  int size = level_ptr->numRows();

  jobjectArray out_level_Java =
      (jobjectArray) jenv->NewObjectArray(
          (jsize) size,
          ptr->String_clazz,
          jenv->NewStringUTF(""));

  std::vector<std::string> array;
  array.reserve(level_ptr->numRows());
  level_ptr->toStringArray(&array);

  for (jsize i = 0; i < size; ++i) {
    jobject str = jenv->NewStringUTF(array[i].c_str());
    jenv->SetObjectArrayElement(out_level_Java, i, str);
  }
  return out_level_Java;
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_setBonusBlocks
  (JNIEnv *, jobject, jlong descriptor, jboolean flag) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
  ptr->processor->setBonusBlocks(flag);
}

JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_drop
  (JNIEnv *jenv, jobject, jlong descriptor) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
}

JNIEXPORT jint JNICALL Java_com_orcchg_arkanoid_surface_AsyncContext_getScore
  (JNIEnv *jenv, jobject, jlong descriptor) {
  AsyncContextHelper* ptr = (AsyncContextHelper*) descriptor;
}

/* Core */
// ----------------------------------------------------------------------------
AsyncContextHelper::AsyncContextHelper(JNIEnv* jenv, jobject object)
  : jenv(jenv)
  , window(nullptr) {

  DBG("enter AsyncContextHelper ctor");
  acontext = new game::AsyncContext(jvm);
  processor = new game::GameProcessor(jvm);
  prize_processor = new game::PrizeProcessor(jvm);
  sound_processor = new native::sound::SoundProcessor(jvm);

  global_object = jenv->NewGlobalRef(object);
  jclass clazz = jenv->FindClass("java/lang/String");
  String_clazz = (jclass) jenv->NewGlobalRef(clazz);
  jenv->DeleteLocalRef(object);
  jclass class_id = jenv->FindClass("com/orcchg/arkanoid/surface/AsyncContext");
  fireJavaEvent_lostBall_id = jenv->GetMethodID(class_id, "fireJavaEvent_lostBall", "()V");
  fireJavaEvent_levelFinished_id = jenv->GetMethodID(class_id, "fireJavaEvent_levelFinished", "()V");
  fireJavaEvent_scoreUpdated_id = jenv->GetMethodID(class_id, "fireJavaEvent_onScoreUpdated", "(I)V");
  fireJavaEvent_angleChanged_id = jenv->GetMethodID(class_id, "fireJavaEvent_angleChanged", "(I)V");
  fireJavaEvent_cardinalityChanged_id = jenv->GetMethodID(class_id, "fireJavaEvent_cardinalityChanged", "(I)V");
  fireJavaEvent_prizeCatch_id = jenv->GetMethodID(class_id, "fireJavaEvent_prizeCatch", "(I)V");
  fireJavaEvent_errorTextureLoad_id = jenv->GetMethodID(class_id, "fireJavaEvent_errorTextureLoad", "()V");
  fireJavaEvent_errorSoundLoad_id = jenv->GetMethodID(class_id, "fireJavaEvent_errorSoundLoad", "()V");
  fireJavaEvent_debugMessage_id = jenv->GetMethodID(class_id, "fireJavaEvent_debugMessage", "(Ljava/lang/String;)V");

  acontext->setMasterObject(global_object);
  acontext->setOnErrorTextureLoadMethodID(fireJavaEvent_errorTextureLoad_id);

  processor->setMasterObject(global_object);
  processor->setOnLostBallMethodID(fireJavaEvent_lostBall_id);
  processor->setOnLevelFinishedMethodID(fireJavaEvent_levelFinished_id);
  processor->setOnScoreUpdatedMethodID(fireJavaEvent_scoreUpdated_id);
  processor->setOnAngleChangedMethodID(fireJavaEvent_angleChanged_id);
  processor->setOnCardinalityChangedMethodID(fireJavaEvent_cardinalityChanged_id);
  processor->setOnDebugMessageMethodID(fireJavaEvent_debugMessage_id);

  prize_processor->setMasterObject(global_object);
  prize_processor->setOnPrizeCatchMethodID(fireJavaEvent_prizeCatch_id);

  sound_processor->setMasterObject(global_object);
  sound_processor->setOnErrorSoundLoadMethodID(fireJavaEvent_errorSoundLoad_id);
  DBG("exit AsyncContextHelper ctor");
}

AsyncContextHelper::~AsyncContextHelper() {
  DBG("enter AsyncContextHelper ~dtor");
  delete acontext; acontext = nullptr;
  delete processor; processor = nullptr;
  delete prize_processor; prize_processor = nullptr;
  delete sound_processor; sound_processor = nullptr;
  jenv->DeleteGlobalRef(global_object);
  global_object = nullptr;
  jenv->DeleteGlobalRef(String_clazz);
  String_clazz = nullptr;
  DBG("exit AsyncContextHelper ~dtor");
}
