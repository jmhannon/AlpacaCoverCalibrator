#pragma once
#include <Arduino.h>
#include "AlpacaCoverCalibrator.h"
#include "ESP32_FastPWM.h"
// #define _PWM_LOGLEVEL_ 4  // what does this do??
#define MaxBrightness 100 // must be 100

class Flat : public AlpacaCoverCalibrator
{
private:
    int _bright_target;

public:
    void pwr_on(void);
    void pwr_off(void);
    bool pwr_stat(void);
    void setdimlevel(int dim);
    void begin(void);
    int dimlevel(void);

    // alpaca getters
    void aGetBrightness(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, dimlevel());
    } // calls a function to get response value

    // return 1 if off or 3 if ready (on)
    void aGetCalibratorState(AsyncWebServerRequest *request)
    {
        int cstate = COff;
        if (pwr_stat() == true)
        {
            cstate = CReady;
        }
        _alpacaServer->respond(request, cstate);
    }

    void aGetCalibratorChanging(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, "false");
        // True whenever the Calibrator is not ready to be
        // used (illumination not yet stabilized), or not completely shut down.
    }

    void aGetConnecting(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, "false");
    }

    void aGetCoverStatus(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, "false"); // correct response ??
    }
    void aGetCoverMoving(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, "false"); // correct response ??
    }

    void aGetCoverState(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, CNotPresent);
    } // calls a function to get response value

    void aGetDeviceState(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, "false"); // correct response ??
    }

    void aGetMaxBrightness(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, MaxBrightness);
    } // calls a function to get response value

    // alpaca setters
    // test for valid input range 0 to max brightness
    void aPutCalibratorOn(AsyncWebServerRequest *request)
    {
        _alpacaServer->getParam(request, "Brightness", _bright_target);

        if ((_bright_target >= 0) && (_bright_target <= MaxBrightness))
        {
            Serial.print("brightness level "); // debug jmh
            Serial.println(_bright_target);
            setdimlevel(_bright_target);
            pwr_on(); // maybe turn power off if _bright_target == 0
            _alpacaServer->respond(request, nullptr);
        }
        else
        {
            _alpacaServer->respond(request, nullptr, InvalidValue, "Invalid value for brightness");
        }
    }

    void aPutCalibratorOff(AsyncWebServerRequest *request)
    {
        pwr_off();
        setdimlevel(0);
        _alpacaServer->respond(request, nullptr);
    }

    void aPutCloseCover(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, nullptr, NotImplemented, "CloseCover not implemented");
    }

    void aPutHaltCover(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, nullptr, NotImplemented, "HaltCover not implemented");
    }

    void aPutOpenCover(AsyncWebServerRequest *request)
    {
        _alpacaServer->respond(request, nullptr, NotImplemented, "OpenCover not implemented");
    }
};