/* Ultimate profile rework by Zeronia*/

#include "modes/Ultimate.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

Ultimate::Ultimate(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
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

    /* ------------------- Alike Buttons ------------------- */

    // Different shield buttons
    bool shield_1 = inputs.r;    // Shield normally. Airdodge normally. Affected by ModX/ModY.

    bool shield_2 = inputs.l;    // Shield with low tilt. Unaffected by ModX/ModY.
                                 // Mostly for neutral airdodge with drift, and tech with Low LSI. 

    // Button "duplicates" to make modifier handling simpler.
    bool shield_button_pressed = shield_1 || shield_2;
    bool attack_pressed = inputs.a || inputs.c_left || inputs.c_right || inputs.c_down || inputs.c_up;
    bool jump_pressed   = inputs.x || inputs.y;

    // Wavedash buttons
    bool wavedash_button_pressed = inputs.z || shield_1;
    // Z recommended due to Ultimate's SH-macro: https://youtu.be/1tOuq-dO85I?t=30.
    // R still needed for fighters who Zair.
    // L excluded. Useful for "neutral airdodge" with drift.

    /* ------------------ Shield Modifiers ------------------ */

        // Lock Shield_1 diagonally (without modifiers)
        if (shield_1 && directions.diagonal) {                      // Most vertical, no jump = 65.
            outputs.leftStickX = 128 + (directions.x * 65);         // Prevents roll when attempting reverse-UpB OoS,
            outputs.leftStickY = 128 + (directions.y * 65);         // or when reacting with shield mid-slingshot.
        }
        // Shield release on SH-Aerial
        if (shield_button_pressed && jump_pressed && attack_pressed) {
            outputs.triggerLDigital = false;                        // Prevents Zair instead of SH-Aerial OoS with A.
            outputs.triggerRDigital = false;                        // This is more so an issue with Smash Ultimate.
        }                                                           // Should only affect fighters with a Zair.

    /* ---------------------------------------------- MOD X Values ----------------------------------------------- */

    if (inputs.mod_x) {
        // MX + Horizontal
        if (directions.horizontal) {                            // Max value for Tink/Yink passive shield = 51.
            outputs.leftStickX = 128 + (directions.x * 50);     // Max value for FTilt out-of-dash = 50
        }                                                       // (going higher will dash attack).
        // MX + Vertical
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 33);     // One-third upward/downward (like B0XX).
        }
        // MX Wavedash / Shield Tilt
        if (directions.diagonal && wavedash_button_pressed) {
            outputs.leftStickX = 128 + (directions.x * 68);     // Close to CL-FW and Arte's firmware.
            outputs.leftStickY = 128 + (directions.y * 28);
        }

        // MX Angled Horizontal Tilts/Aerials
        if (directions.diagonal && inputs.a) {
            outputs.leftStickY  = 128 + (directions.y * 57);    // Most vertical FTilt/FAir/BAir = (X,Y: 51,59 or 50,57)
        }
        // MX Shortend Up B                                     // Diagonal overriden by Angled Tilt Specials.
        if (directions.vertical && inputs.b) {                  // For Diagonal UpB: press UpB, then MX diagonal.
            outputs.leftStickY = 128 + (directions.y * 51);     // Least vertical UpB (like B0XX).
        }
        // MX Angled Horizontal Tilt Specials
        if (directions.horizontal && inputs.b) {
            outputs.leftStickX = 128 + (directions.x * 80);     // Most horizontal for tilt-SideB.
            outputs.leftStickY = 128 + (directions.y * 26);     // Least vertical for BotW/Yink boomerang (must hold B).
        }                                                       // 5 angles: 1 straight, 2 up, 2 down.

        /* ----------------- Angled Up B (ModX) ----------------- */
        if (directions.diagonal && inputs.b) {
            // MX + CDN = 54°
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 80);
                outputs.leftStickY = 128 + (directions.y * 58);
            }
            // MX + CLT = 63°
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 86);
                outputs.leftStickY = 128 + (directions.y * 44);
            }
            // MX + CUP = 72°
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 92);
                outputs.leftStickY = 128 + (directions.y * 30);
            }
            // MX + CRT = 81°
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 98);
                outputs.leftStickY = 128 + (directions.y * 16);
            }
        }
    }

    /* ---------------------------------------------- MOD Y Values ---------------------------------------------- */

    if (inputs.mod_y) {
        // MY + Horizontal
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 25);         // Slowest walk.
            }
            // MY + Up
            if (directions.y == +1) {
                outputs.leftStickY = 128 + (directions.y * 50);     // Mirrors MX Horizontal (like B0XX?).
            }
            // MY + Down
            if (directions.y == -1) {
                outputs.leftStickY = 128 + (directions.y * 65);     // Minimum for crouch/FF.
            }
            // MY + Diagonal
            if (directions.diagonal) {
                outputs.leftStickX = 128 + (directions.x * 33);     // One-third left/rightwards (like B0XX).
                outputs.leftStickY = 128 + (directions.y * 50);     // Prevents FF when attemping reverse DownB
            }                                                       // (Overrides crawl. Let go of MY to crawl).

        // MY Shield_1 Tilt
        if (shield_1) {                                         // 29 = more consistent UpB OoS.
            outputs.leftStickY = 128 + (directions.y * 29);     // Most vertical, no jump = 65.
        }
        // MY Reverse UpTilt/Dtilt
        if (directions.diagonal && inputs.a) {
            outputs.leftStickX = 128 + (directions.x * 38);     // Minimum horizontal for Reverse Dtilt.
        }
        // MY Reverse Specials (Neutral)
        if (directions.horizontal && inputs.b) {
            outputs.leftStickX = 128 + (directions.x * 59);     // Most horizontal for NeutralB.
        }
        // MY Reverse Specials (Up/Down)
        if (directions.vertical && inputs.b) {
            outputs.leftStickX = 128 + (directions.x * 100);    // Most horizontal for Up/DownB.
            outputs.leftStickY = 128 + (directions.y *  65);    // Medium vertical for Up/DownB (like B0XX).
        }

        /* ----------------- Angled Up B (ModY) ----------------- */
        if (directions.diagonal && inputs.b) {
            // MY + CRT = 9°
            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 16);
                outputs.leftStickY = 128 + (directions.y * 98);
            }
            // MY + CUP = 18°
            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 30);
                outputs.leftStickY = 128 + (directions.y * 92);
            }
            // MY + CLT = 27°
            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 44);
                outputs.leftStickY = 128 + (directions.y * 86);
            }
            // MY + CDN = 36°
            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 58);
                outputs.leftStickY = 128 + (directions.y * 80);
            }
        }
    }



    /* ----------------------------------------------- Other Stuff ---------------------------------------------- */

    /* ----------------- Shield 2 Modifiers ----------------- */

    // Shield2 = Neutral Airdodge
    if (shield_2) {
        outputs.leftStickX = 128 + (directions.x * 65);            // Outmost for neutral airdodge = 65. 
        outputs.leftStickY = 128 + (directions.y * 64);            // Downmost for neutral airdodge = 64.
        // Neutral Airdodge diagonal input
        if(directions.diagonal)
        outputs.leftStickX = 128 + (directions.x * 48);            // Most diagonal for neutral airdodge = 48.
        outputs.leftStickY = 128 + (directions.y * 48);

        // Shield2 + Special/Attack = Survival LSI (for tech OS: https://youtu.be/xG-O_Baj-wM?t=178)
        if (inputs.b | inputs.a) {
            // Shield2 SideB
            if (directions.horizontal) {
                outputs.leftStickX = 128 + (directions.x * 65);     // Outmost for tilt SideB (high DI).
            }
            // Shield2 UpB
            if (directions.y == +1) {
                outputs.leftStickX = 128 + (directions.x * 100);     // Outmost for UpB/DownB (max DI).
                outputs.leftStickY = 128 + (directions.y *  51);     // Downmost for UpB (minimizes LSI).
            }
            // Shield2 DownB
            if (directions.y == -1) {
                outputs.leftStickX = 128 + (directions.x * 100);     // Outmost for UpB/DownB (max DI).
                outputs.leftStickY = 128 + (directions.y *  64);     // Downmost for DownB, no FF (minimizes LSI).
            }
        }
        // Shield 1+2 unlocks full tilt
        if (shield_1) {                                            // L+R activates Ultimate's shield lock mechanic:
            outputs.leftStickX = 128 + (directions.x * 100);       // https://youtu.be/HFYtrdcbDJI?t=66
            outputs.leftStickY = 128 + (directions.y * 100);       // (UpB/SH-aerials can still buffer OoS).
        }
    }
    
    // Angled Horizontal Attacks (with C-Stick diagonal)
    if (directions.cx && directions.cy) {                       // Not consistent.       
        outputs.rightStickX = 128 + (directions.cx * 70);       // Tbh, just do Angled Fsmash with the A button 
        outputs.rightStickY = 128 + (directions.cy * 59);       // like the B0XX manual says to.
    }
    // Angled Horizontal Attacks (with C-Stick + L-Stick)       // Up   + C Horizontal = Upward angled.
    if (directions.vertical && directions.cx) {                 // Down + C Horizontal = Downward angled.
        outputs.rightStickY = (outputs.leftStickY);             // Inputting C-Left/Right outputs Y-axis of L-stick.
        if(inputs.mod_x) {
        outputs.rightStickX = 128 + (directions.cx * 50);       // Allows this to also work with ModX. 
        }                                                       // Issue: Reverse down-angled Ftilt while
    }                                                           // fully crouched does reverse Dtilt?

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
    }
    // Shut off L-stick when using Nunchuk.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}


// Todo: 
// - Check if MY + CRT is even useful. Angle is picked up by Switch, but firefox just goes straight up.
// - Make L+R shield tilt overriden by ModX/ModY. But L alone overrides ModX/ModY. 
// - Check if SwitchBackend's right stick scaling is equal to left stick.
// - Make "Shield release on SH-Aerial" work with GameCubeBackend.
// - Scale the GamecubeBackend to match to SwitchBackend (0 = Min. 100 = Max).



/* 
Todo for later:
 
Change C-Stick modifiers (for Extended Up B Angles) order to: 
CUp, CLeft, CRight, CDown.

Basically:
 9° = CUp = Slighest horizontal (most upward)
18° = CLeft = slight more 
27° = CRight = even more horizontal
36° = CDown = most horizontal (most downward)

Also make Left/Right swap if stick is leftwards?
And make Up/Down swap if stick is downwards?


Or cut down on angles to remove need of MX:
15° = CUp
30° = CLeft
45°
60° = CRight
75° = CDown
90°
*/
