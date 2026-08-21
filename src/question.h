#pragma once

#include "input.h"

enum class Difficulty {
  Easy,
  Normal,
  Hard,
};

struct Question {
  int operandA;
  int operandB;
  char op;  // '+' or '-'
  int answer;
  int choices[4];      // indexed by Direction: Up, Right, Down, Left
  Direction answerDir;
};

Question generateQuestion(Difficulty difficulty);
