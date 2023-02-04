#include "comms/WifiClientBackend.hpp"

#include <WiFi.h>
#include <WiFiClient.h>
#include <hardware/watchdog.h>

WiFiClientBackend::WiFiClientBackend(InputSource **input_sources, size_t input_source_count)
    : CommunicationBackend(input_sources, input_source_count) {
    Serial.begin(115200);
    // WiFi.begin(HAYBOX_WIFI_SSID, HAYBOX_WIFI_KEY);
    while (WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECTING) {
        Serial.println("Connecting to WiFi network...");
        WiFi.begin(HAYBOX_WIFI_SSID, HAYBOX_WIFI_KEY);
        sleep_ms(500);
    }
    Serial.println("Connected to WiFi network");

    Serial.println("Connecting to TCP server...");
    _client = new WiFiClient();
    // _client->connect("192.168.42.1", 6969);
    while (!_client->connect("192.168.42.1", 6969)) {
        Serial.println("Connecting to TCP server...");
        // _client->connect("192.168.42.1", 6969);
        // sleep_ms(1000);
    }
    Serial.println("Connected to TCP server");
}

WiFiClientBackend::~WiFiClientBackend() {
    delete _client;
    WiFi.end();
}

void WiFiClientBackend::SendReport() {
    if (!WiFi.connected()) {
        WiFi.begin(HAYBOX_WIFI_SSID, HAYBOX_WIFI_KEY);
        return;
    }
    if (!_client->connected() && !_client->connect("192.168.42.1", 6969)) {
        // if (!_client->connect("192.168.42.1", 6969)) {
        return;
        // }
    }

    ScanInputs(InputScanSpeed::SLOW);
    ScanInputs(InputScanSpeed::MEDIUM);

    // Wait for server to send poll command.
    Serial.println("Waiting for poll");
    uint8_t available = _client->available();
    if (!available) {
        sleep_ms(1);
        return;
    }
    Serial.println("Received poll command. Scanning inputs...");

    ScanInputs(InputScanSpeed::FAST);

    Serial.println("Scanned inputs");

    uint8_t buf[20];
    _client->readBytes(buf, available);

    Serial.println("Reading the rest of the request from the server");

    uint8_t report[] = {
        (uint8_t
        )(_inputs.left << 7 | _inputs.right << 6 | _inputs.down << 5 | _inputs.up << 4 |
          _inputs.c_left << 3 | _inputs.c_right << 2 | _inputs.c_down << 1 | _inputs.c_up),
        (uint8_t
        )(_inputs.a << 7 | _inputs.b << 6 | _inputs.x << 5 | _inputs.y << 4 | _inputs.l << 3 |
          _inputs.r << 2 | _inputs.z << 1 | _inputs.lightshield),
        (uint8_t
        )(_inputs.midshield << 7 | _inputs.select << 6 | _inputs.start << 5 | _inputs.home << 4 |
          _inputs.mod_x << 3 | _inputs.mod_y << 2),
    };

    Serial.println("Waiting for TX FIFO to have space");
    while (_client->availableForWrite() < sizeof(report)) {
        tight_loop_contents();
    }
    Serial
        .printf("Sending input report %02x %02x %02x to server\n", report[0], report[1], report[2]);
    _client->write(report, sizeof(report));
    _client->flush();
}