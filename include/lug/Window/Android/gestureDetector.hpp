//--------------------------------------------------------------------------------
// gestureDetector.h
//--------------------------------------------------------------------------------
#ifndef GESTUREDETECTOR_H_
#define GESTUREDETECTOR_H_

#include <vector>

#include <android/sensor.h>
#include <android/log.h>
// #include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <android/configuration.h>
#include <android/input.h>
#include <android/keycodes.h>

#include <lug/Math/Vector.hpp>

// #include "JNIHelper.h"
// #include "vecmath.h"

namespace ndk_helper {
//--------------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------------
const int32_t DOUBLE_TAP_TIMEOUT = 300 * 1000000;
const int32_t TAP_TIMEOUT = 180 * 1000000;
const int32_t DOUBLE_TAP_SLOP = 100;
const int32_t TOUCH_SLOP = 8;

enum {
  GESTURE_STATE_NONE = 0,
  GESTURE_STATE_START = 1,
  GESTURE_STATE_MOVE = 2,
  GESTURE_STATE_END = 4,
  GESTURE_STATE_ACTION = (GESTURE_STATE_START | GESTURE_STATE_END),
};
typedef int32_t GESTURE_STATE;

/******************************************************************
 * Base class of Gesture Detectors
 * GestureDetectors handles input events and detect gestures
 * Note that different detectors may detect gestures with an event at
 * same time. The caller needs to manage gesture priority accordingly
 *
 */
class GestureDetector {
 protected:
  float dp_factor_;

 public:
  GestureDetector();
  virtual ~GestureDetector() {}
  virtual void SetConfiguration(AConfiguration* config);

  virtual GESTURE_STATE Detect(const AInputEvent* motion_event) = 0;
};

/******************************************************************
 * Tap gesture detector
 * Returns GESTURE_STATE_ACTION when a tap gesture is detected
 *
 */
class TapDetector : public GestureDetector {
 private:
  int32_t down_pointer_id_;
  float down_x_;
  float down_y_;

 public:
  TapDetector();
  virtual ~TapDetector() {}
  virtual GESTURE_STATE Detect(const AInputEvent* motion_event);
};

/******************************************************************
 * Pinch gesture detector
 * Returns GESTURE_STATE_ACTION when a double-tap gesture is detected
 *
 */
class DoubletapDetector : public GestureDetector {
 private:
  TapDetector tap_detector_;
  int64_t last_tap_time_{0};
  float last_tap_x_{0.0f};
  float last_tap_y_{0.0f};

 public:
  DoubletapDetector();
  virtual ~DoubletapDetector() {}
  virtual GESTURE_STATE Detect(const AInputEvent* motion_event);
  virtual void SetConfiguration(AConfiguration* config);
};

/******************************************************************
 * Double gesture detector
 * Returns pinch gesture state when a pinch gesture is detected
 * The class handles multiple touches more than 2
 * When the finger 1,2,3 are tapped and then finger 1 is released,
 * the detector start new pinch gesture with finger 2 & 3.
 */
class PinchDetector : public GestureDetector {
 private:
  int32_t FindIndex(const AInputEvent* event, int32_t id);
  const AInputEvent* event_;
  std::vector<int32_t> vec_pointers_;

 public:
  PinchDetector() {}
  virtual ~PinchDetector() {}
  virtual GESTURE_STATE Detect(const AInputEvent* event);
  bool GetPointers(lug::Math::Vec2f& v1, lug::Math::Vec2f& v2);
};

/******************************************************************
 * Drag gesture detector
 * Returns drag gesture state when a drag-tap gesture is detected
 *
 */
class DragDetector : public GestureDetector {
 private:
  int32_t FindIndex(const AInputEvent* event, int32_t id);
  const AInputEvent* event_;
  std::vector<int32_t> vec_pointers_;

 public:
  DragDetector() : event_(nullptr) {}
  virtual ~DragDetector() {}
  virtual GESTURE_STATE Detect(const AInputEvent* event);
  bool GetPointer(lug::Math::Vec2f& v);
};

}  // namespace ndkHelper
#endif /* GESTUREDETECTOR_H_ */