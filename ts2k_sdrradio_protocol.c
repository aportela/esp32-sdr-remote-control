#include "ts2k_sdrradio_protocol.h"

/*
  SDRRADIO PROTOCOL vs Original Kenwood TS-2000 protocol DIFFs

  WARNING
  As read on https://www.sdr-radio.com/SerialPort SDR Radio implementation changed some parts of TS-2000 protocol

  # AF Gain / volume (command "AG")

  SDR Radio TS-2000 protocol volume range is 0...100
  Original Kenwood TS-2000 protocol volume range is 000...255

  # DSP Filter High/Low (command "SH"/"SL")
  
  SDR Radio TS-2000 protocol value (hz) range is 0...99999
  Original Kenwood TS-2000 protocol value (custom) depends current mode 00...11 (each one has fixed hz corresponding values)
  
*/

// get power status
void ts2kReadCommandPowerStatus(char* command) {
  strcpy(command, "PS;");
}

// verify command type
bool ts2kIsPowerStatusCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 3 && strncmp(commandResponse, "PS", 2) == 0;
}

// check if power status command response is "ON"
bool ts2kParsePowerStatusCommandResponse(char* commandResponse) {
  return strcmp(commandResponse, "PS1") == 0;
}

// get AF gain (volume)
void ts2kReadCommandAFGain(char* command) {
  strcpy(command, "AG0;");
}

// verify command type
bool ts2kIsAFGainCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 5 && strncmp(commandResponse, "AG", 2) == 0;
}

// parse & return AF gain command response as unsigned integer
uint8_t ts2kParseAGFGainCommandResponse(char* commandResponse) {
  uint8_t volume;
  if (sscanf(commandResponse, "AG%d", &volume) == 1) {
    return volume;
  } else {
    return 0;
  }
}

// set AF gain (volume)
void ts2kWriteCommandAFGain(char* command, uint8_t volume) {
  sprintf(command, "AG0%03d;", volume < 100 ? volume : 100);
}

// get current frequency (Hz)
void ts2kReadCommandFrequency(char* command) {
  strcpy(command, "FA;");
}

// parse & return read current frequency command response as unsigned integer (64)
uint64_t ts2kParseFrequencyCommandResponse(char* commandResponse) {
  return 0;
}

// set current frequency (Hz)
void ts2kWriteCommandFrequency(char* command, uint64_t frequency) {
  sprintf(command, "FA%011llu;", frequency);
}

// get current mode
void ts2kReadCommandMode(char* command) {
  strcpy(command, "MD;");
}

// set current mode
void ts2kWriteCommandMode(char* command, ts2kMode mode) {
  sprintf(command, "MD%d;", (uint8_t)mode);
}

// get current signal meter level
void ts2kReadCommandSignalMeterLevel(char* command) {
  strcpy(command, "SM0;");
}

// get current filter high value (Hz)
void ts2kReadCommandFilterHighHz(char* command) {
  strcpy(command, "SH;");
}

// set current filter high value (Hz)
void ts2kWriteCommandFilterHighHz(char* command, uint32_t hz) {
  sprintf(command, "Sh%05u;", hz);
}

// get current filter low value (Hz)
void ts2kReadCommandFilterLowHz(char* command) {
  strcpy(command, "SL;");
}

// set current filter low value (Hz)
void ts2kWriteCommandFilterLowHz(char* command, uint32_t hz) {
  sprintf(command, "SL%05u;", hz);
}
