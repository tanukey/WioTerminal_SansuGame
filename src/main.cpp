#include <Arduino.h>

#include "input.h"
#include "question.h"
#include "ui.h"

namespace {

enum class GameState {
  Title,
  Question,
  Feedback,
  Result,
};

constexpr int kTotalQuestions = 5;
constexpr unsigned long kFeedbackDurationMs = 1200;

GameState gState = GameState::Title;
Difficulty gDifficulty = Difficulty::Easy;
Question gCurrentQuestion;
int gQuestionIndex = 0;
int gCorrectCount = 0;
unsigned long gFeedbackStartMs = 0;

void startGame(Difficulty difficulty) {
  gDifficulty = difficulty;
  gQuestionIndex = 0;
  gCorrectCount = 0;
  gCurrentQuestion = generateQuestion(gDifficulty);
  uiDrawQuestionScreen(gCurrentQuestion, gQuestionIndex + 1, kTotalQuestions);
  gState = GameState::Question;
}

void goToTitle() {
  uiDrawTitleScreen();
  gState = GameState::Title;
}

void handleTitle() {
  switch (buttonPressed()) {
    case ButtonKey::A:
      startGame(Difficulty::Easy);
      break;
    case ButtonKey::B:
      startGame(Difficulty::Normal);
      break;
    case ButtonKey::C:
      startGame(Difficulty::Hard);
      break;
    default:
      break;
  }
}

void handleQuestion() {
  Direction moved = switchMoved();
  if (moved == Direction::None) return;

  bool correct = (moved == gCurrentQuestion.answerDir);
  if (correct) gCorrectCount++;

  uiDrawFeedback(correct, gCurrentQuestion.answer);
  gFeedbackStartMs = millis();
  gState = GameState::Feedback;
}

void handleFeedback() {
  if (millis() - gFeedbackStartMs < kFeedbackDurationMs) return;

  gQuestionIndex++;
  if (gQuestionIndex >= kTotalQuestions) {
    uiDrawResultScreen(gCorrectCount, kTotalQuestions);
    gState = GameState::Result;
  } else {
    gCurrentQuestion = generateQuestion(gDifficulty);
    uiDrawQuestionScreen(gCurrentQuestion, gQuestionIndex + 1, kTotalQuestions);
    gState = GameState::Question;
  }
}

void handleResult() {
  switch (buttonPressed()) {
    case ButtonKey::A:
      startGame(gDifficulty);
      break;
    case ButtonKey::C:
      goToTitle();
      break;
    default:
      break;
  }
}

}  // namespace

void setup() {
  randomSeed(micros());
  inputBegin();
  uiBegin();
  uiDrawTitleScreen();
}

void loop() {
  inputUpdate();

  switch (gState) {
    case GameState::Title:
      handleTitle();
      break;
    case GameState::Question:
      handleQuestion();
      break;
    case GameState::Feedback:
      handleFeedback();
      break;
    case GameState::Result:
      handleResult();
      break;
  }
}
