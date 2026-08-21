#include "input.h"

#include <Arduino.h>

namespace {

constexpr uint8_t kDebounceMs = 30;

struct DigitalInput {
  explicit DigitalInput(uint8_t p) : pin(p) {}

  uint8_t pin;
  bool lastStable = true;   // buttons/switch are active LOW with pull-ups
  bool lastReading = true;
  unsigned long lastChangeMs = 0;
  bool risingEdge = false;  // became pressed this update
};

DigitalInput gKeyA{WIO_KEY_A};
DigitalInput gKeyB{WIO_KEY_B};
DigitalInput gKeyC{WIO_KEY_C};
DigitalInput gUp{WIO_5S_UP};
DigitalInput gDown{WIO_5S_DOWN};
DigitalInput gLeft{WIO_5S_LEFT};
DigitalInput gRight{WIO_5S_RIGHT};
DigitalInput gPress{WIO_5S_PRESS};

DigitalInput* const kAllInputs[] = {
    &gKeyA, &gKeyB, &gKeyC, &gUp, &gDown, &gLeft, &gRight, &gPress,
};

void updateDebounced(DigitalInput& in, unsigned long nowMs) {
  bool reading = digitalRead(in.pin) == HIGH;
  in.risingEdge = false;

  if (reading != in.lastReading) {
    in.lastReading = reading;
    in.lastChangeMs = nowMs;
  }

  if (nowMs - in.lastChangeMs >= kDebounceMs && reading != in.lastStable) {
    bool wasPressed = !in.lastStable;
    in.lastStable = reading;
    bool isPressed = !in.lastStable;
    if (isPressed && !wasPressed) {
      in.risingEdge = true;
    }
  }
}

}  // namespace

void inputBegin() {
  for (DigitalInput* in : kAllInputs) {
    pinMode(in->pin, INPUT_PULLUP);
    in->lastStable = digitalRead(in->pin) == HIGH;
    in->lastReading = in->lastStable;
    in->lastChangeMs = millis();
    in->risingEdge = false;
  }
}

void inputUpdate() {
  unsigned long nowMs = millis();
  for (DigitalInput* in : kAllInputs) {
    updateDebounced(*in, nowMs);
  }
}

ButtonKey buttonPressed() {
  if (gKeyA.risingEdge) return ButtonKey::A;
  if (gKeyB.risingEdge) return ButtonKey::B;
  if (gKeyC.risingEdge) return ButtonKey::C;
  return ButtonKey::None;
}

Direction switchMoved() {
  if (gUp.risingEdge) return Direction::Up;
  if (gRight.risingEdge) return Direction::Right;
  if (gDown.risingEdge) return Direction::Down;
  if (gLeft.risingEdge) return Direction::Left;
  return Direction::None;
}

bool switchPressed() {
  return gPress.risingEdge;
}
