#pragma once

#include "question.h"

void uiBegin();
void uiDrawTitleScreen();
void uiDrawQuestionScreen(const Question& q, int questionNumber, int totalQuestions);
void uiDrawFeedback(bool correct, int correctAnswer);
void uiDrawResultScreen(int correctCount, int totalQuestions);
