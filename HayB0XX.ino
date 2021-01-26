/*
  HayB0XX Version 0.0.3

  Some parts of this code were originally based on GCCPCB2 v1.208 code by Crane.

  This code utilizes
    Nicohood's Nintendo library
    MHeironimus' Arduino Joystick Library
    Arduino Keyboard Library
    Zac Staples' Arduino_Vector library
*/

#include "src/setup_gccpcb2.h"

#include "src/DefaultKeyboardMode.h"
#include "src/FgcMode.h"
#include "src/InputMode.h"
#include "src/Melee18Button.h"
#include "src/Melee20Button.h"

enum reportState : byte {
  ReportOff = 0x30,
  ReportOn = 0x31,
  ReportEnd = 0x0A,
  ReportInvalid = 0x00
};

// Used to count updates so we only send state to the input viewer every 3
// updates.
int gReportClock = 0;

void writeSerialReport();

extern CommunicationBackend *gCurrentBackend;
extern InputMode *gCurrentMode;
extern state::InputState gInputState;

void selectInputMode() {
  if (gInputState.mod_x && !gInputState.mod_y && gInputState.start) {
    if (gInputState.l) {
      delete gCurrentMode;
      gCurrentMode = new Melee20Button(socd::SOCD_2IP_NO_REAC, gInputState,
                                       gCurrentBackend);
    } else if (gInputState.down) {
      delete gCurrentMode;
      gCurrentMode =
          new FgcMode(socd::SOCD_NEUTRAL, gInputState, gCurrentBackend);
    }
  } else if (gInputState.mod_y && !gInputState.mod_x && gInputState.start) {
    if (gInputState.l) {
      delete gCurrentMode;
      gCurrentMode = new DefaultKeyboardMode(socd::SOCD_2IP, gInputState);
    }
  }
}

void readInputs() {
  gInputState.l = (digitalRead(pinout::L) == LOW);
  gInputState.left = (digitalRead(pinout::LEFT) == LOW);
  gInputState.down = (digitalRead(pinout::DOWN) == LOW);
  gInputState.right = (digitalRead(pinout::RIGHT) == LOW);
  gInputState.mod_x = (digitalRead(pinout::MODX) == LOW);
  gInputState.mod_y = (digitalRead(pinout::MODY) == LOW);
  gInputState.start = (digitalRead(pinout::START) == LOW);
  gInputState.select = (digitalRead(pinout::SELECT) == LOW);
  gInputState.home = (digitalRead(pinout::HOME) == LOW);
  gInputState.b = (digitalRead(pinout::B) == LOW);
  gInputState.x = (digitalRead(pinout::X) == LOW);
  gInputState.z = (digitalRead(pinout::Z) == LOW);
  gInputState.up = (digitalRead(pinout::UP) == LOW);
  gInputState.r = (digitalRead(pinout::R) == LOW);
  gInputState.y = (digitalRead(pinout::Y) == LOW);
  gInputState.lightshield = (digitalRead(pinout::LIGHTSHIELD) == LOW);
  gInputState.midshield = (digitalRead(pinout::MIDSHIELD) == LOW);
  gInputState.c_down = (digitalRead(pinout::CDOWN) == LOW);
  gInputState.a = (digitalRead(pinout::A) == LOW);
  gInputState.c_right = (digitalRead(pinout::CRIGHT) == LOW);
  gInputState.c_left = (digitalRead(pinout::CLEFT) == LOW);
  gInputState.c_up = (digitalRead(pinout::CUP) == LOW);
}

void setup() {
  pinMode(pinout::L, INPUT_PULLUP);
  pinMode(pinout::LEFT, INPUT_PULLUP);
  pinMode(pinout::DOWN, INPUT_PULLUP);
  pinMode(pinout::RIGHT, INPUT_PULLUP);
  pinMode(pinout::MODX, INPUT_PULLUP);
  pinMode(pinout::MODY, INPUT_PULLUP);
  pinMode(pinout::START, INPUT_PULLUP);
  pinMode(pinout::SELECT, INPUT_PULLUP);
  pinMode(pinout::HOME, INPUT_PULLUP);
  pinMode(pinout::B, INPUT_PULLUP);
  pinMode(pinout::X, INPUT_PULLUP);
  pinMode(pinout::Z, INPUT_PULLUP);
  pinMode(pinout::UP, INPUT_PULLUP);
  pinMode(pinout::R, INPUT_PULLUP);
  pinMode(pinout::Y, INPUT_PULLUP);
  pinMode(pinout::CDOWN, INPUT_PULLUP);
  pinMode(pinout::A, INPUT_PULLUP);
  pinMode(pinout::CRIGHT, INPUT_PULLUP);
  pinMode(pinout::CLEFT, INPUT_PULLUP);
  pinMode(pinout::CUP, INPUT_PULLUP);
  pinMode(pinout::LIGHTSHIELD, INPUT_PULLUP);
  pinMode(pinout::MIDSHIELD, INPUT_PULLUP);

  // Read inputs into gInputState initially to make backend selection logic in
  // initialise() a bit cleaner.
  readInputs();

  // Setup specific to each type of DIY.
  initialise();
}

void loop() {
  readInputs();

  /* Mode selection */
  selectInputMode();

  gCurrentMode->UpdateOutputs();

  // Only run input viewer on every 3 updates, to prevent lag.
  if (Serial.availableForWrite() > 32) {
    if (gReportClock == 0) {
      writeSerialReport();
      gReportClock++;
    } else if (gReportClock == 3) {
      gReportClock = 0;
    } else {
      gReportClock++;
    }
  }
}

/**
 * Write serial report for B0XX input viewer.
 */
void writeSerialReport() {
  byte report[25] = {gInputState.start ? ReportOn : ReportOff,
                     gInputState.y ? ReportOn : ReportOff,
                     gInputState.x ? ReportOn : ReportOff,
                     gInputState.b ? ReportOn : ReportOff,
                     gInputState.a ? ReportOn : ReportOff,
                     gInputState.l ? ReportOn : ReportOff,
                     gInputState.r ? ReportOn : ReportOff,
                     gInputState.z ? ReportOn : ReportOff,
                     gInputState.up ? ReportOn : ReportOff,
                     gInputState.down ? ReportOn : ReportOff,
                     gInputState.right ? ReportOn : ReportOff,
                     gInputState.left ? ReportOn : ReportOff,
                     gInputState.mod_x ? ReportOn : ReportOff,
                     gInputState.mod_y ? ReportOn : ReportOff,
                     gInputState.c_left ? ReportOn : ReportOff,
                     gInputState.c_right ? ReportOn : ReportOff,
                     gInputState.c_up ? ReportOn : ReportOff,
                     gInputState.c_down ? ReportOn : ReportOff,
                     gInputState.lightshield ? ReportOn : ReportOff,
                     gInputState.midshield ? ReportOn : ReportOff,
                     gInputState.e_1 ? ReportOn : ReportOff,
                     gInputState.e_2 ? ReportOn : ReportOff,
                     gInputState.e_3 ? ReportOn : ReportOff,
                     gInputState.e_4 ? ReportOn : ReportOff,
                     ReportEnd};

  Serial.write(report, 25);
}
