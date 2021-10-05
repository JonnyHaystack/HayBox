#include "NasbMode.h"
#include "ControllerMode.h"

#define ANALOG_STICK_MIN 48
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 208

NasbMode::NasbMode(state::InputState &rInputState, CommunicationBackend *communicationBackend)
    : ControllerMode(socd::SOCD_NEUTRAL, rInputState, communicationBackend)
{
  mSocdPairs.push_back(socd::SocdPair{&rInputState.left, &rInputState.right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.down, &rInputState.up});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_left, &rInputState.c_right});
  mSocdPairs.push_back(socd::SocdPair{&rInputState.c_down, &rInputState.c_up});
}

void NasbMode::HandleSocd()
{
  InputMode::HandleSocd();
}

void NasbMode::UpdateDigitalOutputs()
{
  mOutputState.dpadUp = mrInputState.midshield;
  mOutputState.start = mrInputState.start;
  mOutputState.a = mrInputState.a;
  mOutputState.b = mrInputState.b;
  mOutputState.y = mrInputState.x || mrInputState.y;
  mOutputState.x = mrInputState.mod_y || mrInputState.lightshield;
  mOutputState.buttonR = mrInputState.z;
  mOutputState.triggerRDigital = mrInputState.l || mrInputState.r;
  mOutputState.triggerLDigital = mrInputState.mod_x;
}

void NasbMode::UpdateAnalogOutputs()
{
  HandleVectors(mrInputState.left, mrInputState.right, mrInputState.down,
                mrInputState.up, mrInputState.c_left, mrInputState.c_right,
                mrInputState.c_down, mrInputState.c_up, ANALOG_STICK_MIN,
                ANALOG_STICK_NEUTRAL, ANALOG_STICK_MAX);
}
