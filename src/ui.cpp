#include "ui.h"

#include <TFT_eSPI.h>

namespace {

TFT_eSPI tft;

constexpr int kScreenW = 320;
constexpr int kScreenH = 240;

void drawDifficultyRow(int y, const char* letter, int starCount, uint16_t color) {
  const int boxX = 40, boxY = y, boxW = 50, boxH = 44;
  tft.drawRoundRect(boxX, boxY, boxW, boxH, 8, color);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(color, TFT_BLACK);
  tft.drawString(letter, boxX + boxW / 2, boxY + boxH / 2, 4);

  int dotX = boxX + boxW + 30;
  int dotY = boxY + boxH / 2;
  for (int i = 0; i < starCount; i++) {
    tft.fillCircle(dotX + i * 26, dotY, 8, color);
  }
}

// Draws a choice box centered at (cx, cy) with the given value.
void drawChoiceBox(int cx, int cy, int value, bool highlight = false) {
  const int w = 74, h = 46;
  uint16_t color = highlight ? TFT_YELLOW : TFT_CYAN;
  tft.drawRoundRect(cx - w / 2, cy - h / 2, w, h, 8, color);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(color, TFT_BLACK);
  tft.drawNumber(value, cx, cy, 4);
}

}  // namespace

void uiBegin() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
}

void uiDrawTitleScreen() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("1 + 2 = ?", kScreenW / 2, 40, 4);

  drawDifficultyRow(80, "A", 1, TFT_GREEN);
  drawDifficultyRow(135, "B", 2, TFT_YELLOW);
  drawDifficultyRow(190, "C", 3, TFT_RED);
}

void uiDrawQuestionScreen(const Question& q, int questionNumber, int totalQuestions) {
  tft.fillScreen(TFT_BLACK);

  // Progress indicator, top-left.
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  char progress[16];
  snprintf(progress, sizeof(progress), "%d/%d", questionNumber, totalQuestions);
  tft.drawString(progress, 8, 8, 2);

  // Problem text, centered.
  char problem[24];
  snprintf(problem, sizeof(problem), "%d %c %d = ?", q.operandA, q.op, q.operandB);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(problem, kScreenW / 2, kScreenH / 2, 4);

  const int cx = kScreenW / 2;
  const int cy = kScreenH / 2;
  const int r = 90;
  drawChoiceBox(cx, cy - r, q.choices[0]);      // Up
  drawChoiceBox(cx + r, cy, q.choices[1]);      // Right
  drawChoiceBox(cx, cy + r, q.choices[2]);      // Down
  drawChoiceBox(cx - r, cy, q.choices[3]);      // Left
}

void uiDrawFeedback(bool correct, int correctAnswer) {
  tft.fillScreen(TFT_BLACK);
  const int cx = kScreenW / 2;
  const int cy = kScreenH / 2 - 10;

  if (correct) {
    tft.drawCircle(cx, cy, 50, TFT_GREEN);
    tft.drawCircle(cx, cy, 49, TFT_GREEN);
  } else {
    tft.drawLine(cx - 45, cy - 45, cx + 45, cy + 45, TFT_RED);
    tft.drawLine(cx - 45, cy + 45, cx + 45, cy - 45, TFT_RED);
    tft.drawLine(cx - 44, cy - 45, cx + 46, cy + 45, TFT_RED);
    tft.drawLine(cx - 44, cy + 45, cx + 46, cy - 45, TFT_RED);

    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    char buf[16];
    snprintf(buf, sizeof(buf), "= %d", correctAnswer);
    tft.drawString(buf, cx, cy + 80, 4);
  }
}

void uiDrawResultScreen(int correctCount, int totalQuestions) {
  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  char score[16];
  snprintf(score, sizeof(score), "%d / %d", correctCount, totalQuestions);
  tft.drawString(score, kScreenW / 2, 70, 6);

  // A = retry (play triangle icon), C = back to title (house icon).
  const int boxW = 90, boxH = 60;
  const int aX = 50, cX = kScreenW - 50 - boxW;
  const int y = 150;

  tft.drawRoundRect(aX, y, boxW, boxH, 8, TFT_GREEN);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("A", aX + boxW / 2 - 20, y + boxH / 2, 4);
  tft.fillTriangle(aX + boxW / 2 + 5, y + boxH / 2 - 12, aX + boxW / 2 + 5,
                    y + boxH / 2 + 12, aX + boxW / 2 + 22, y + boxH / 2, TFT_GREEN);

  tft.drawRoundRect(cX, y, boxW, boxH, 8, TFT_RED);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString("C", cX + boxW / 2 - 20, y + boxH / 2, 4);
  int hx = cX + boxW / 2 + 15, hy = y + boxH / 2;
  tft.fillTriangle(hx - 14, hy, hx, hy - 14, hx + 14, hy, TFT_RED);
  tft.fillRect(hx - 9, hy, 18, 12, TFT_RED);
}
