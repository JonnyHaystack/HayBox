/* Ultimate profile rework by Zeronia */
#include "modes/Ultimate.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

Ultimate::Ultimate(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::up     },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };
}

    /* ---------------------------------------------- Button Config --------------------------------------------- */

void Ultimate::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    outputs.buttonR = inputs.z;
    outputs.buttonL = inputs.lightshield; // LS = Left Bumper.
    outputs.triggerLDigital = inputs.l;
    outputs.triggerRDigital = inputs.r;
    outputs.start = inputs.start;
    outputs.select = inputs.select;
    outputs.home = inputs.home;

    // Turn on D-Pad layer by holding Mod X + Mod Y, or Nunchuk C button.
    if ((inputs.mod_x && inputs.mod_y) || inputs.nunchuk_c) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right; 
        outputs.home = inputs.start; // MX+MY+Start = Home button.
    }
}

void Ultimate::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    /* ------------------ Shield Modifiers ------------------ */
        // Shield 1. Shield normally. Airdodge normally. (Affected by ModX/ModY).
        bool shield_1 = inputs.r;
        // Shield 2. Shield locked. Neutral airdodge with drift. Tech with Low LSI. (Overrides ModX/ModY).
        bool shield_2 = inputs.l;

        // Wavedash buttons
        bool wavedash_button_pressed = inputs.z || shield_1;
        // Z recommended due to Ultimate's SH-macro: https://youtu.be/1tOuq-dO85I?t=30.
        // R still needed for fighters who Zair. 
        // L excluded. Useful for "neutral airdodge" with drift.

        // Lock Shield_1 diagonally (without modifiers)
        if (shield_1 && directions.diagonal) {                      // Straight up/down can still jump/spotdodge.
            outputs.leftStickX = 128 + (directions.x * 51);         // Prevents roll when attempting reverse-UpB OoS,
            outputs.leftStickY = 128 + (directions.y * 52);         // or when reacting with shield mid-slingshot.
        }                                                           // Shield lock  = X:64, Y:64 (GCC = X:51 Y:52).

    /* ---------------------------------------------- MOD X Values ----------------------------------------------- */
    if (inputs.mod_x) {
        // MX + Horizontal                                          
        if (directions.horizontal) {                                // Fastest walk = 80.
            outputs.leftStickX = 128 + (directions.x * 53);         // Max value for walk out-of-dash and teeter = 71 (GCC = 53).
        }
        // MX + Vertical (1/3 DI). MX + Diagonal (Medium walk).
        if (directions.vertical) {
            outputs.leftStickX = 128 + (directions.x * 40);         // Max value for Tink/Yink passive shield = 50 (GCC = 40).
            outputs.leftStickY = 128 + (directions.y * 33);         // One-third upward/downward (like B0XX) = 33 (GCC = 33).
        }

        // MX Diaongal Shield Tilt / Airdodge / Wavedash
        if (directions.horizontal && wavedash_button_pressed) {     // Let go of MX to airdodge straight up/down.
            outputs.leftStickX = 128 + (directions.x * 51);         // Most horizontal for shield tilt, no roll = 67 (GCC = 51).
            outputs.leftStickY = 128 + (directions.y * 32);         // Minimum vertical for waveland on BF plat = 28... 
        }                                                           // Varies by character. More downward allows more characters, but less slide.

        // MX + Horizontal Attack (and angles)
        if (inputs.a && directions.horizontal | directions.diagonal) {
            outputs.leftStickX = 128 + (directions.x * 40);         // Max value for FTilt out-of-dash = 50 (GCC = 40) (going higher will dash attack).
            outputs.leftStickY = 128 + (directions.y * 35);         // Vertical range for angled FTilt/FAir/BAir = 26 to 58 (GCC = 26 to 44).
        }
        
        // MX Specials 
        if (inputs.b) {
            // MX Shortend Up B                                     // Diagonal overriden by Angled Tilt Specials.
            if (directions.vertical) {                              // For diagonal: Hold MX + UpB, and then Left/Right (keep all held).
                outputs.leftStickY = 128 + (directions.y * 42);     // Least vertical for Shortend UpB = 52 (GCC = 42) (51 does max UpB, 41 on GCC).
            }
            // MX Tilt SideB (and angles)                           
            if (directions.horizontal | directions.diagonal) {      // Hold B for steep angles, release B for 45°.
                outputs.leftStickX = 128 + (directions.x * 53);     // Horizontal range for tilt SideB = 59 to 80 (GCC = 47 to 59).
                outputs.leftStickY = 128 + (directions.y * 32);     // Least vertical for Yink angled boomerang = X:70 Y:26 (GCC: X:53 Y:26). 
            }                                                       // Least vertical for MinMin reversed angled arms = X:70 Y:27 (GCC: X:53 Y:31).
        }

        /* ----------------- Angled Up B (ModX) ----------------- */
        // MX + Diagonal = Medium Magnitude (80)
        if (directions.diagonal) {
            // MX + CRT = 52°
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 63);
                outputs.leftStickY = 128 + (directions.y * 49);
            }
            // MX + CUP = 59°
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 68);
                outputs.leftStickY = 128 + (directions.y * 41);
            }
            // MX + CLT = 66°
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 73);
                outputs.leftStickY = 128 + (directions.y * 33);
            }
            // MX + CDN = 73°
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 86);
                outputs.leftStickY = 128 + (directions.y * 23);
            }

            // MX + Diagonal + B = Max Magnitude
            if (inputs.b) {
                // MX + CRT + B = 52°
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 66);
                    outputs.leftStickY = 128 + (directions.y * 55);
                }
                // MX + CUP + B = 59°
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 66);
                    outputs.leftStickY = 128 + (directions.y * 46);
                }
                // MX + CLT + B = 66°
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 70);
                    outputs.leftStickY = 128 + (directions.y * 40);
                }
                // MX + CDN + B = 73°
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 73);
                    outputs.leftStickY = 128 + (directions.y * 33);
                }
            }

            // MY + Diagonal + Shield = Minimum Magnitude (differs per angle)
            if (shield_1) {
                // MX + CRT + Shield = 52° / mag 64
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 29);
                    outputs.leftStickY = 128 + (directions.y * 61);
                }
                // MX + CUP + Shield = 59° / mag 64
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 26);
                    outputs.leftStickY = 128 + (directions.y * 58);
                }
                // MX + CLT + Shield = 66° / mag 56
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 28);
                    outputs.leftStickY = 128 + (directions.y * 48);
                }
                // MX + CDN + Shield = 73° / mag 56
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 34);
                    outputs.leftStickY = 128 + (directions.y * 44);
                }
            }
        }
    }

    /* ---------------------------------------------- MOD Y Values ---------------------------------------------- */
    if (inputs.mod_y) {
        // MY + Horizontal
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 26);         // Slowest walk = 26 (GCC = 26).
            }
            // MY + Down
            if (directions.y == -1) {                               // Minimum for crouch/FF = 65 (GCC = 50).
                outputs.leftStickY = 128 + (directions.y * 50);     // Range for crouch on platform = 65 to 66 (GCC = 50 only).
            }
            // MY + Up / Diagonally Up or Down
            if (directions.y == +1 | directions.diagonal) {
                outputs.leftStickX = 128 + (directions.x * 33);     // One-third left/rightwards (like B0XX) = 33 (GCC = 33).
                outputs.leftStickY = 128 + (directions.y * 40);     // One-half up/down. Mirrors medium walk value = 50 (GCC = 40)
            }                                                       // Also prevents FF when attemping reverse DownB. 

        // MY Shield Tilt / Airdodge / Wavedash = 38°
        if (directions.vertical && wavedash_button_pressed) {
            outputs.leftStickY = 128 + (directions.x * 45);         // 38° angle = Y: 67 X: 53 (GCC = Y:52 X:~45)
            outputs.leftStickY = 128 + (directions.y * 52);         // Most vertical for shield tilt, no jump/spotdodge = 67 (GCC = 52).
        }

        // MY Reverse UpTilt/Dtilt
        if (directions.diagonal && inputs.a) {                      // Horizontal range for Reverse Utilt = Y:64, X:27 to 55.
            outputs.leftStickX = 128 + (directions.x * 38);         // Horizontal range for Reverse Dtilt = Y:64, X:37 to 54 (GCC = Y:49, X:33 to 43).
            outputs.leftStickY = 128 + (directions.y * 49);         // Most vertical, no jump/crouch/FF = 64 (GCC = 49).
        }

        // MY Specials
        if (inputs.b) {
            // MY Reverse Specials (Neutral)
            if (directions.horizontal) {
                outputs.leftStickX = 128 + (directions.x * 46);     // Most horizontal for NeutralB = 58 (GCC = 46).
            }
            // MY Reverse Specials (Up)
            if (directions.y == +1) {
                outputs.leftStickX = 128 + (directions.x * 100);    // Most horizontal for UpB (max drift).
                // MY Shortend Up B (medium)
                outputs.leftStickY = 128 + (directions.y * 60);     // 80 Magnitude for teleports = 80 (GCC = 60).
            }
            // MY Reverse Specials (Down)
            if (directions.y == -1) {                               // MinMin DownB = Y:64 X:41 (GCC = X:49 Y:35).
                outputs.leftStickX = 128 + (directions.x * 35);     // Range for grounded reverse DownB = 35 to 58 (GCC = 31 to 46).
                outputs.leftStickY = 128 + (directions.y * 49);     // Most vertical, no FF = 64 (GCC = 49).
            }
        }

        /* ----------------- Angled Up B (ModY) ----------------- */
        // MY + Diagonal = Medium Magnitude (80)
        if (directions.diagonal) {
            // MY + CRT = 17°
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 33); // Switch = 23, 68.
                outputs.leftStickY = 128 + (directions.y * 73); // GCC = ??
            }
            // MY + CUP = 24°
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 40); // Switch = 33, 73.
                outputs.leftStickY = 128 + (directions.y * 70);
            }
            // MY + CLT = 31°
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 46); // Switch = 41, 68.
                outputs.leftStickY = 128 + (directions.y * 66);
            }
            // MY + CDN = 38°
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 55); // Switch = 49, 63.
                outputs.leftStickY = 128 + (directions.y * 66); 
            }

            // MY + Diagonal + B = Max Magnitude
            if (inputs.b) {
                // MY + CRT + B = 17°
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 33); // Switch = 29, 95. 
                    outputs.leftStickY = 128 + (directions.y * 73); // GCC = 33, 73.
                }
                // MY + CUP + B = 24°
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 40); // Switch = 41, 91.
                    outputs.leftStickY = 128 + (directions.y * 70); // GCC = 40, 70.
                }
                // MY + CLT + B = 31°
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 46); // Switch = 52, 86.
                    outputs.leftStickY = 128 + (directions.y * 66); // GCC = 46, 66.
                }
                // MY + CDN + B = 38°
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 55); // Switch = 63, 80.
                    outputs.leftStickY = 128 + (directions.y * 66); // GCC = 55, 66.
                }
            }

            // MY + Diagonal + Shield = Minimum Magnitude (differs per angle)
            if (shield_1) {
                // MY + CRT + Shield = 17° / mag 64
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 29); // Switch = 29, 61.
                    outputs.leftStickY = 128 + (directions.y * 61);
                }
                // MY + CUP + Shield = 24° / mag 64
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 26); // Switch = 26, 58.
                    outputs.leftStickY = 128 + (directions.y * 58);
                }
                // MY + CLT + Shield = 31° / mag 56
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 28); // Switch = 28, 48.
                    outputs.leftStickY = 128 + (directions.y * 48);
                }
                // MY + CDN + Shield = 38° / mag 56
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 34); // Switch = 34, 44.
                    outputs.leftStickY = 128 + (directions.y * 44);
                }
            }
        }
    }

    /* ----------------------------------------------- Other Stuff ---------------------------------------------- */

    // Shield 2 Modifiers
    if (shield_2) {
        // Neutral Airdodge (also nearly max shield tilt without L+R mechanic)
        outputs.leftStickX = 128 + (directions.x * 49);             // Outmost for neutral airdodge = 64 (GCC = 49). 
        outputs.leftStickY = 128 + (directions.y * 49);             // Downmost for neutral airdodge = 64 (GCC = 49).
        // Neutral Airdodge diagonal
        if(directions.diagonal) {
        outputs.leftStickX = 128 + (directions.x * 39);             // Most diagonal for neutral airdodge = 47 (GCC = 39).
        outputs.leftStickY = 128 + (directions.y * 39);
        }

        // Shield2 + Special/Attack = Survival LSI (for tech OS: https://youtu.be/xG-O_Baj-wM?t=178)
        if (inputs.b | inputs.a && directions.vertical) {
            outputs.leftStickX = 128 + (directions.x * 46);         // Outmost for DownB, max DI = 58 (GCC = 46).
            outputs.leftStickY = 128 + (directions.y * 49);         // Downmost for DownB, no FF = 64 (GCC = 49).
        }

        // Shield 1+2 unlocks full tilt
        if (shield_1) {                                             // L+R activates Ultimate's shield lock mechanic:
            outputs.leftStickX = 128 + (directions.x * 100);        // https://youtu.be/HFYtrdcbDJI?t=66
            outputs.leftStickY = 128 + (directions.y * 100);        // (UpB/SH-aerials can still buffer OoS).
        }

        // Easier UpB OoS (with MY + Shield 2)
        if (inputs.mod_y && directions.y == +1) {
            outputs.leftStickY = 128 + (directions.y * 27);         // Partial shield tilt up (still considered neutral) = 28 (GCC = 27).
            if(inputs.b) {
            outputs.leftStickX = 128 + (directions.x * 100);        // Most horizontal for UpB (max drift).
            outputs.leftStickY = 128 + (directions.y * 60);         // Medium vertical for UpB = 80 (GCC = 60).
            }
        }
    }

    /* ------------------------ Misc ------------------------ */

    // Angled Horizontal Attacks (with C-Stick + L-Stick)           // Inputting C-Left/Right outputs Y-axis of L-stick.
    if (directions.vertical && directions.cx) {                     // Basically this, but regardless of modifier:
        outputs.rightStickY = (outputs.leftStickY);                 // https://youtu.be/uTYSgyca8cI?t=1000.
        if(inputs.mod_x) {
        outputs.rightStickX = 128 + (directions.cx * 40);           // Allows this to also work with ModX. 
        }                                                           // Mirrors medium walk and MX Horizontal Attack = 50 (GCC = 40).
        if(inputs.mod_y) {
        outputs.rightStickX = 128 + (directions.cx * 70);           // Allows this to also work with ModY on GamecubeBackend. 
        }                                                           // (GCC adapter scales C-Stick differertly from main stick, whereas SwitchPro does not).
    }
    // Disable C-Stick for Extended UpB                             // Prevents SideB/DownB if C-Left/Right/Down is 
    if (directions.y == +1 && directions.diagonal && inputs.b) {    // pressed on same frame as B button.
        outputs.rightStickX = 128;                                  // Only applies if main stick is upward, 
        outputs.rightStickY = 128;                                  // so Zelda can still C-Bounce DownB.
    }

    // GCC analog trigger values
    if (inputs.l) {
        outputs.triggerLAnalog = 140;
    }
    if (inputs.r) {
        outputs.triggerRAnalog = 140;
    }
    // Shut off C-stick when using D-Pad layer.
    if ((inputs.mod_x && inputs.mod_y) || inputs.nunchuk_c) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
        outputs.start = false; // MX+MY+Start = Home button.
    }
    // Shut off L-stick when using Nunchuk.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}

/* 
To test: 
 - Pikachu's "Double UpB".
 - C-Bounce sideB.

Extra features to add:
 - Make "MX + MY + Select" = Capture button.

Notes on firefox:
    B0XX manual has angles 9° to 81°...
    However firefox only uses 17° to 73°: https://i.imgur.com/BvHzlcf.png.
*/
