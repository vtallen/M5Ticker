#include <M5Core2.h>

#include <theme.h>

namespace Picker {

bool screenActive = true;
static bool resetPressedBefore = false;

void drawTicker();

struct TickerChar {
  int pos{};
  char value{};
};

TickerChar ticker[5];

// This variable is used so that the reset button
int lastSelectedChar = 0;

void upDownPressed(Event &e) {
  static unsigned long lastPressTime = 0;
  if (millis() - lastPressTime > 500) {
    resetPressedBefore = false;

    String btnLabel = e.button->label();
    int pos = btnLabel.indexOf(" ");
    String direction = btnLabel.substring(0, pos);
    int charNum = btnLabel.substring(pos + 1).toInt();
    lastSelectedChar = charNum;

    if (direction == "UP") {
      if ((ticker[charNum].value >= 90) || (ticker[charNum].value == '-')) {
        ticker[charNum].value = 65;
      } else {
        ++ticker[charNum].value;
      }
    } else if (direction == "DN") {
      if (ticker[charNum].value <= 65) {
        ticker[charNum].value = 90;
      } else {
        --ticker[charNum].value;
      }
    }

    Serial.print(String(charNum));
    Serial.print(" Value: ");
    Serial.print(String(ticker[charNum].value));
    Serial.println();

    lastPressTime = millis();
    drawTicker();
  }
}

void addPressed(Event &e) {}

void cancelPressed(Event &e) {}

void resetPressed(Event &e) {
  static unsigned long lastPressTime = 0;

  if (millis() - lastPressTime > 500) {
    if (!resetPressedBefore) {
      resetPressedBefore = true;
      ticker[lastSelectedChar].value = '-';
      drawTicker();
    } else {
      Serial.println("BEFORE");

      for (int i = 0; i < sizeof(ticker) / (sizeof(ticker[0])); ++i) {
        Serial.println(String(ticker[i].value));
      }

      for (int i = 0; i < sizeof(ticker) / (sizeof(ticker[0])); ++i) {
        ticker[i].value = '-';
      }
      resetPressedBefore = false;

      Serial.println("AFTER");
      for (int i = 0; i < sizeof(ticker) / (sizeof(ticker[0])); ++i) {
        Serial.println(String(ticker[i].value));
      }
    }

    lastPressTime = millis();
  }
}

void drawTicker() {
  M5.Lcd.setTextSize(1.5);
  M5.Lcd.setFreeFont(&FreeSansBold12pt7b);
  M5.Lcd.drawString(String(ticker[0].value), 50, 75);
}

void loop() {
  Button up0(15, 0, 50, 50, true, "UP 0", theme::INVIS_BUTTON_OFF_COLORS,
             theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);
  Button down0(15, 100, 50, 50, true, "DN 0", theme::INVIS_BUTTON_OFF_COLORS,
               theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);

  Button up1(75, 0, 50, 50, true, "UP 1", theme::INVIS_BUTTON_OFF_COLORS,
             theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);
  Button down1(75, 100, 50, 50, true, "DN 1", theme::INVIS_BUTTON_OFF_COLORS,
               theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);

  Button up2(135, 0, 50, 50, true, "UP 2", theme::INVIS_BUTTON_OFF_COLORS,
             theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);
  Button down2(135, 100, 50, 50, true, "DN 2", theme::INVIS_BUTTON_OFF_COLORS,
               theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);

  Button up3(195, 0, 50, 50, true, "UP 3", theme::INVIS_BUTTON_OFF_COLORS,
             theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);
  Button down3(195, 100, 50, 50, true, "DN 3", theme::INVIS_BUTTON_OFF_COLORS,
               theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);

  Button up4(255, 0, 50, 50, true, "UP 4", theme::INVIS_BUTTON_OFF_COLORS,
             theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);
  Button down4(255, 100, 50, 50, true, "DN 4", theme::INVIS_BUTTON_OFF_COLORS,
               theme::INVIS_BUTTON_ON_COLORS, CC_DATUM);

  Button add(220, 180, 100, 50, true, "ADD", theme::CHECK_BUTTON_OFF_COLORS,
             theme::CHECK_BUTTON_ON_COLORS, CC_DATUM);

  Button cancel(0, 180, 100, 50, true, "CANCEL",
                theme::CANCEL_BUTTON_OFF_COLORS, theme::CANCEL_BUTTON_ON_COLORS,
                CC_DATUM);

  Button reset(110, 180, 100, 50, true, "RESET", theme::RESET_BUTTON_OFF_COLORS,
               theme::RESET_BUTTON_ON_COLORS, CC_DATUM);

  // Add the handler functions for all of the buttons
  up0.addHandler(upDownPressed);
  down0.addHandler(upDownPressed);
  up1.addHandler(upDownPressed);
  down1.addHandler(upDownPressed);
  up2.addHandler(upDownPressed);
  down2.addHandler(upDownPressed);
  up3.addHandler(upDownPressed);
  down3.addHandler(upDownPressed);
  up4.addHandler(upDownPressed);
  down4.addHandler(upDownPressed);

  add.addHandler(addPressed);
  cancel.addHandler(cancelPressed);
  reset.addHandler(resetPressed);

  M5.Buttons.draw();

  for (int i = 0; i < sizeof(ticker) / (sizeof(ticker[0])); ++i) {
    TickerChar tempChar;
    tempChar.pos = i;
    tempChar.value = '-';
    ticker[i] = tempChar;
    Serial.println("TickerChar " + String(i));
  }

  while (screenActive) {
    M5.update();
  }
}
} // namespace Picker