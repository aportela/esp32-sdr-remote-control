#include "SDRRadioTS2KSerialConnection.hpp"

SDRRadioTS2KSerialConnection::SDRRadioTS2KSerialConnection(HardwareSerial *serialPort, long speed, long timeout)
    : SerialConnection(serialPort, speed, timeout)
{
}

bool SDRRadioTS2KSerialConnection::tryConnection(void)
{
    this->flush();
    this->send("PS;");
    bool connected = false;
    while (this->serial->available() > 0 && !connected)
    {
        this->lastRXActivity = millis();
        String receivedData = this->serial->readStringUntil(';');
        if (receivedData == "PS1")
        {
            this->lastRXValidCommand = millis();
            this->flush();
            connected = true;
            break;
        }
    }
    return (connected);
}

void SDRRadioTS2KSerialConnection::loop(Transceiver *trx)
{
    if (millis() - this->lastTXActivity > MILLISECONDS_BETWEEN_LOOP)
    {
        if (trx != nullptr)
        {
            if (trx->powerStatus == TRX_PS_ON)
            {
                // connected => main
                /*
                this->send("FA;");   // get frequency
                this->send("MD;");   // get mode
                this->send("SL;");   // get low filter
                this->send("SH;");   // get high filter
                this->send("AG0;");  // get af gain (volume)
                this->send("MU;");   // get audio mute status
                this->send("SM0;");  // get signal meter level
                */
                // this works BETTER than sending separated commands (with own delay)
                this->send("FA;MD;SL;SH;AG0;MU;SM0;");
                while (this->serial->available() > 0)
                {
                    this->lastRXActivity = millis();
                    String receivedData = this->serial->readStringUntil(';');
                    if (receivedData.startsWith("FA") && receivedData != "FA")
                    {
                        this->lastRXValidCommand = millis();
                        trx->incSerialCommandCount();
                        trx->setVFOFrequency(trx->activeVFOIndex, receivedData.substring(2).toInt());
                    }
                    else if (receivedData.startsWith("MD") && receivedData != "MD")
                    {
                        this->lastRXValidCommand = millis();
                        trx->incSerialCommandCount();
                        trx->setVFOMode(trx->activeVFOIndex, (TRXVFOMode)receivedData.substring(2).toInt());
                    }
                    else if (receivedData.startsWith("SL") && receivedData != "SL")
                    {
                        this->lastRXValidCommand = millis();
                        trx->incSerialCommandCount();
                        trx->setVFOLowFilterHz(trx->activeVFOIndex, (TRXVFOMode)receivedData.substring(2).toInt());
                    }
                    else if (receivedData.startsWith("SH") && receivedData != "SH")
                    {
                        this->lastRXValidCommand = millis();
                        trx->incSerialCommandCount();
                        trx->setVFOHighFilterHz(trx->activeVFOIndex, (TRXVFOMode)receivedData.substring(2).toInt());
                    }
                    else if (receivedData.startsWith("AG") && receivedData != "AG")
                    {
                        this->lastRXValidCommand = millis();
                        trx->incSerialCommandCount();
                        trx->setAFGain(receivedData.substring(2).toInt());
                    }
                    else if (receivedData.startsWith("MU") && receivedData != "MU")
                    {
                        this->lastRXValidCommand = millis();
                        trx->incSerialCommandCount();
                        uint8_t newMutedStatus = receivedData.substring(2).toInt();
                        if (trx->audioMuted == TRX_AUDIO_MUTED && newMutedStatus == 0)
                        {
                            trx->setAudioMuted();
                        }
                        else if (trx->audioMuted == TRX_AUDIO_NOT_MUTED && newMutedStatus == 1)
                        {
                            trx->setAudioMuted();
                        }
                    }
                    else if (receivedData.startsWith("SM") && receivedData != "SM")
                    {
                        this->lastRXValidCommand = millis();
                        trx->incSerialCommandCount();
                        trx->setSignalMeterLevel(receivedData.substring(2).toInt());
                    }
                }
            }
        }
    }
}