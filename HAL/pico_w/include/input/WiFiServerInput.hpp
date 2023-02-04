#ifndef _INPUT_WIFISERVERINPUT_HPP
#define _INPUT_WIFISERVERINPUT_HPP

#include "core/InputSource.hpp"

#include <WiFi.h>
#include <WiFiServer.h>

class WiFiServerInput : public InputSource {
  public:
    WiFiServerInput();
    ~WiFiServerInput();
    InputScanSpeed ScanSpeed();
    void UpdateInputs(InputState &inputs);

  protected:
    WiFiServer *_server;
    // WiFiClient *_client;
};

#endif