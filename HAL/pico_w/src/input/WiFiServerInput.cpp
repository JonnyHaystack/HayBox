#include "input/WiFiServerInput.hpp"

#include "core/InputSource.hpp"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

WiFiServerInput::WiFiServerInput() {
    WiFi.beginAP(HAYBOX_WIFI_SSID, HAYBOX_WIFI_KEY);
    _server = new WiFiServer(6969);
    _server->begin();
    Serial.begin(115200);
}

WiFiServerInput::~WiFiServerInput() {
    delete _server;
    WiFi.end();
}

InputScanSpeed WiFiServerInput::ScanSpeed() {
    return InputScanSpeed::FAST;
}

void WiFiServerInput::UpdateInputs(InputState &inputs) {
    // if (!_client || !_client->connected()) {
    Serial.println("Updating inputs");
    WiFiClient client = _server->available();
    Serial.println("Got first available client");
    if (!client.connected()) {
        Serial.println("No clients connected");
        sleep_ms(1);
        return;
    }

    Serial.println("Flushing client");
    client.flush();
    // }

    uint8_t report[3];

    // Send poll command.
    Serial.println("Sending poll command");
    uint8_t buf[] = { 0 };
    client.write(buf, sizeof(buf));
    Serial.println("Flushing client");
    client.flush();

    // Wait for response.
    Serial.println("Waiting for response");
    while (client.available() < sizeof(report)) {
        Serial.printf("Still waiting %d...\n", client.available());
        // sleep_ms(50);
        tight_loop_contents();
    }
    Serial.println("                                     READING RESPONSE");
    client.readBytes(report, sizeof(report));

    Serial.println("Updating input struct from response");

    Serial.printf("%02x %02x %02x\n", report[0], report[1], report[2]);

    inputs.left = report[0] & (1 << 7);
    inputs.right = report[0] & (1 << 6);
    inputs.down = report[0] & (1 << 5);
    inputs.up = report[0] & (1 << 4);
    inputs.c_left = report[0] & (1 << 3);
    inputs.c_right = report[0] & (1 << 2);
    inputs.c_down = report[0] & (1 << 1);
    inputs.c_up = report[0] & 1;
    inputs.a = report[1] & (1 << 7);
    inputs.b = report[1] & (1 << 6);
    inputs.x = report[1] & (1 << 5);
    inputs.y = report[1] & (1 << 4);
    inputs.l = report[1] & (1 << 3);
    inputs.r = report[1] & (1 << 2);
    inputs.z = report[1] & (1 << 1);
    inputs.lightshield = report[1] & (1 << 0);
    inputs.midshield = report[2] & (1 << 7);
    inputs.select = report[2] & (1 << 6);
    inputs.start = report[2] & (1 << 5);
    inputs.home = report[2] & (1 << 4);
    inputs.mod_x = report[2] & (1 << 3);
    inputs.mod_y = report[2] & (1 << 2);

    Serial.printf("Inputs update complete %d\n", client.read());
}