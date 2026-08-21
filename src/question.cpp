#include "question.h"

#include <Arduino.h>

namespace {

// choices[] slot order fixed to these directions.
constexpr Direction kSlotDirection[4] = {Direction::Up, Direction::Right,
                                          Direction::Down, Direction::Left};

int randomOperand(Difficulty difficulty) {
  switch (difficulty) {
    case Difficulty::Easy:
      return random(0, 10);  // 0-9
    case Difficulty::Normal:
      return random(0, 2) == 0 ? random(0, 10) : random(10, 100);  // 0-9 or 10-99
    case Difficulty::Hard:
    default:
      return random(10, 100);  // 10-99
  }
}

void generateDummyChoices(int answer, int spread, int outValues[4]) {
  outValues[0] = answer;
  int count = 1;
  while (count < 4) {
    int offset = random(1, spread + 1);
    if (random(0, 2) == 0) offset = -offset;
    int candidate = answer + offset;
    if (candidate < 0) continue;
    bool duplicate = false;
    for (int i = 0; i < count; i++) {
      if (outValues[i] == candidate) {
        duplicate = true;
        break;
      }
    }
    if (duplicate) continue;
    outValues[count++] = candidate;
  }

  // Fisher-Yates shuffle so the correct answer isn't always in slot 0.
  for (int i = 3; i > 0; i--) {
    int j = random(0, i + 1);
    int tmp = outValues[i];
    outValues[i] = outValues[j];
    outValues[j] = tmp;
  }
}

}  // namespace

Question generateQuestion(Difficulty difficulty) {
  Question q;

  int a = randomOperand(difficulty);
  int b = randomOperand(difficulty);
  q.op = random(0, 2) == 0 ? '+' : '-';

  if (q.op == '-' && a < b) {
    int tmp = a;
    a = b;
    b = tmp;
  }

  q.operandA = a;
  q.operandB = b;
  q.answer = (q.op == '+') ? (a + b) : (a - b);

  int spread = max(3, q.answer / 4 + 3);
  int values[4];
  generateDummyChoices(q.answer, spread, values);

  for (int i = 0; i < 4; i++) {
    q.choices[i] = values[i];
    if (values[i] == q.answer) {
      q.answerDir = kSlotDirection[i];
    }
  }

  return q;
}
