#ifndef _COMMS_WIFICLIENTBACKEND_HPP
#define _COMMS_WIFICLIENTBACKEND_HPP

#include "core/CommunicationBackend.hpp"
#include "core/InputSource.hpp"
#include "stdlib.hpp"

#include <WiFi.h>
#include <WiFiClient.h>

class WiFiClientBackend : public CommunicationBackend {
  public:
    WiFiClientBackend(InputSource **input_sources, size_t input_source_count);
    ~WiFiClientBackend();
    void SendReport();

  private:
    WiFiClient *_client;
};

#endif