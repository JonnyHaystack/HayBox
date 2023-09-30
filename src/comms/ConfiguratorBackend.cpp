#include "comms/ConfiguratorBackend.hpp"

#include "core/InputSource.hpp"
#include "core/persistence.hpp"
#include "serial.hpp"

#include <pb_decode.h>
#include <pb_encode.h>

ConfiguratorBackend::ConfiguratorBackend(InputSource **input_sources, size_t input_source_count)
    : CommunicationBackend(input_sources, input_source_count) {
    _in = new packetio::COBSStream(Serial);
    _out = new packetio::COBSPrint(Serial);
    serial::init(115200);
}

ConfiguratorBackend::~ConfiguratorBackend() {
    serial::close();
    delete _in;
    delete _out;
}

void ConfiguratorBackend::SendReport() {
    uint8_t buffer[EEPROM_SIZE];
    size_t packet_len = ReadPacket(buffer, sizeof(buffer));
    Command command = (Command)buffer[0];
    switch (command) {
        case CMD_GET_DEVICE_INFO:
            HandleGetDeviceInfo();
            break;
        case CMD_GET_CONFIG:
            HandleGetConfig();
            break;
        case CMD_SET_CONFIG:
            HandleSetConfig(buffer, packet_len);
            break;
        case CMD_REBOOT_FIRMWARE:
            watchdog_enable(1, 1);
            while (1) {
                tight_loop_contents();
            }
            break;
        case CMD_REBOOT_BOOTLOADER:
            reset_usb_boot(0, 0);
            break;
        case CMD_UNSPECIFIED:
        default:
            HandleUnknownCommand(command);
            break;
    }
}

size_t ConfiguratorBackend::ReadPacket(uint8_t *buffer, size_t max_len) {
    size_t bytes_read = 0;
    while (!Serial.available()) {
        delay(1);
    }
    while (true) {
        int result = _in->read();
        if (result == _in->EOF) {
            continue;
        }
        if (result == _in->EOP) {
            break;
        }
        buffer[bytes_read++] = result;
        if (bytes_read >= max_len) {
            _in->next();
            return bytes_read;
        }
    }
    _in->next();

    return bytes_read;
}

bool ConfiguratorBackend::WritePacket(Command command_id, uint8_t *buffer, size_t len) {
    _out->write((uint8_t)command_id);
    for (size_t i = 0; i < len; i++) {
        _out->write(buffer[i]);
    }
    return _out->end();
}

bool ConfiguratorBackend::HandleGetDeviceInfo() {
    static const DeviceInfo device_info = {
        FIRMWARE_NAME,
        FIRMWARE_VERSION,
        DEVICE_NAME,
    };

    uint8_t buffer[sizeof(DeviceInfo)];
    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&ostream, &DeviceInfo_msg, &device_info)) {
        char errmsg[] = "Failed to encode device info";
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, sizeof(errmsg));
        return false;
    }

    return WritePacket(CMD_SET_DEVICE_INFO, buffer, ostream.bytes_written);
}

bool ConfiguratorBackend::HandleGetConfig() {
    uint8_t buffer[EEPROM_SIZE - CONFIG_OFFSET];
    size_t config_size = persistence::load_config_raw(buffer, sizeof(buffer));
    return WritePacket(CMD_SET_CONFIG, buffer, config_size);
}

bool ConfiguratorBackend::HandleSetConfig(uint8_t *buffer, size_t len) {
    Config config = Config_init_zero;
    pb_istream_t istream = pb_istream_from_buffer(&buffer[1], len - 1);

    if (!pb_decode(&istream, &Config_msg, &config)) {
        char errmsg[] = "Failed to decode config";
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, sizeof(errmsg));
        return false;
    }

    if (!persistence::save_config(config)) {
        char errmsg[] = "Failed to save config to memory";
        WritePacket(CMD_ERROR, (uint8_t *)errmsg, sizeof(errmsg));
        return false;
    }

    WritePacket(CMD_SUCCESS, nullptr, 0);
    return true;
}

bool ConfiguratorBackend::HandleUnknownCommand(Command command) {
    char errmsg[50];
    size_t errmsg_len =
        snprintf(errmsg, sizeof(errmsg), "Unknown command ID: %d", (uint8_t)command);
    return WritePacket(CMD_ERROR, (uint8_t *)errmsg, errmsg_len);
}