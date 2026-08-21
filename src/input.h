#pragma once

enum class Direction {
  None,
  Up,
  Right,
  Down,
  Left,
};

enum class ButtonKey {
  None,
  A,
  B,
  C,
};

void inputBegin();
void inputUpdate();

// Edge-triggered (true only on the frame the input becomes pressed).
ButtonKey buttonPressed();
Direction switchMoved();
bool switchPressed();
