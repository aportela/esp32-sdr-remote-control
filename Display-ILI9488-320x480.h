#ifndef SDR_REMOTE_CONTROL_DISPLAY_ILI9488_H
#define SDR_REMOTE_CONTROL_DISPLAY_ILI9488_H

#include <SPI.h>
#include <TFT_eSPI.h>

#include "IDisplay.h"
#include "SSWAnimationILI9488.h"

#include "Transceiver.h"

class DisplayILI9488 : public IDisplay {
public:
  DisplayILI9488(uint16_t width, uint16_t height, uint8_t rotation, bool invertDisplayColors);
  void clearScreen(uint8_t color) override;

  void showConnectScreen(uint16_t serialBaudRate, float currentVersion) override;
  void hideConnectScreen(void) override;
  void refreshConnectScreen(void) override;

  void showMainScreen() override;
  void refreshMainScreen(Transceiver* trx, float fps) override;

  void debugBottomStr(char* str, uint64_t value);
  void debugBottomStr2(String, uint64_t value);
private:
  uint16_t width;
  uint16_t height;
  TFT_eSPI screen;
  SSWAnimationILI9488* animatedScreenPtr = nullptr;
  uint8_t oldSignal;
  uint8_t currentSignal = 0;
  uint8_t peakSignal = 0;
  long lastPeakChange;

  void refreshTransmitStatus(bool isTransmitting);
  void refreshActiveVFO(uint8_t number);
  void refreshVFOMode(TRXVFOMode mode);
  void refreshFPS(float fps);
  void refreshVFOFreq(uint64_t frequency);
  void createDigitalSMeter();
  void refreshRNDDigitalSMeter(int newSignal);
};

#endif