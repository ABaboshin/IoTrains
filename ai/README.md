# IoTrains train controller hardware (AI-generated)

Two 23 x 40 mm, 2-layer KiCad 10 boards for controlling a model train from an ESP32, joined by a 10-wire cable.

**Status:** schematics and PCBs are complete and fully routed, but nothing has been built or reviewed by a person. Routing is autorouted (Freerouting) except the buck converter, which is laid out by hand. Review both boards in KiCad before ordering.

| Board | Project | Contents |
|---|---|---|
| MCU | [hardware/mcu/mcu.kicad_pro](hardware/mcu/mcu.kicad_pro) | ESP32-S3-MINI-1-N8, MAX98357A I2S amplifier, BOOT and RESET buttons |
| Driver | [hardware/driver/driver.kicad_pro](hardware/driver/driver.kicad_pro) | Bridge rectifier, 3.3 V buck converter, BDR6133 motor driver, TLC5940 LED driver |

All parts are on the top side. Connector labels are on the bottom silkscreen. There are no mounting holes.

Every connector sits on a short edge, so all cables leave along the length of the board:

| Board | Top edge | Bottom edge (left to right) |
|---|---|---|
| MCU | Antenna, no connectors | `LINK`, `USB`, `SPK` |
| Driver | `LINK`, `3V3` | `12V`, `LED`, `MOTOR` |

## Requirements and how they are met

| Requirement | Result |
|---|---|
| Max 23 x 40 mm, up to 3 boards | 2 boards, each 23 x 40 mm |
| ESP32, module or bare chip, whichever is cheaper | ESP32-S3-MINI-1-N8 module (see below) |
| S2B-ZR and SM10B-SRSS-TB connectors | Used for every connector |
| Cables must run parallel to the long side | Every connector is on a short edge |
| 12 V DC input, unknown polarity | ABS210 bridge rectifier on the input |
| USB pins on an S2B connector | `USB` on the MCU board (IO19/IO20) |
| One BDR6133 motor driver, outputs exposed | On the driver board, `MOTOR` connector |
| 6 LED outputs, TLC5940 allowed | TLC5940 with 8 outputs on the `LED` connector |
| Audio chip with speaker output, otherwise expose I2S | MAX98357A with `SPK` connector; I2S is not exposed |

**Module vs bare chip:** at JLCPCB on 2026-11-27 the module was $4.72 and the bare ESP32-S3FN8 $3.27. The $1.45 difference does not cover a crystal, RF matching parts and an antenna that needs tuning, so the module is used.

## Connectors

### MCU board

| Connector | Type | Pins |
|---|---|---|
| `LINK` (J1) | SM10B | See link pinout below |
| `USB` (J2) | S2B | 1 = D+ (IO20), 2 = D- (IO19) |
| `SPK` (J4) | S2B | 1 = SPK+, 2 = SPK- |

The MCU board has no power connector of its own. It gets 3.3 V over `LINK` pin 2.

### Driver board

| Connector | Type | Pins |
|---|---|---|
| `12V` (J1) | S2B | 12 V DC, either polarity |
| `MOTOR` (J2) | S2B | 1 = OUTA, 2 = OUTB |
| `LINK` (J3) | SM10B | See link pinout below |
| `LED` (J4) | SM10B | 1 and 10 = LED anodes (bridge output, about 10 V), 2-9 = OUT0-OUT7 |
| `3V3` (J5) | S2B | 1 = GND, 2 = +3V3 |

### Link cable

The cable must be wired 1:1 (pin n to pin n).

| Pin | Signal | ESP32 pin | Driver board |
|---|---|---|---|
| 1 | GND | | |
| 2 | +3V3 | | From the buck converter |
| 3 | GSCLK | IO11 | TLC5940 |
| 4 | BLANK | IO12 | TLC5940, 10k pull-up |
| 5 | SIN | IO13 | TLC5940 |
| 6 | SCLK | IO14 | TLC5940 |
| 7 | XLAT | IO15 | TLC5940 |
| 8 | VPRG | IO16 | TLC5940, 10k pull-down |
| 9 | MOT_A | IO17 | BDR6133 INA, 100k pull-down |
| 10 | MOT_B | IO18 | BDR6133 INB, 100k pull-down |

The TLC5940 pins match the mapping in the comment in `firmware/basic_train/src/main.cpp`.

## Circuit notes

### Power

- The 12 V input goes through the ABS210 bridge, so polarity does not matter. The bridge output (`VM`, about 10 V) feeds the motor driver, the LED anodes and the buck converter.
- A TPS629210 makes 3.3 V. It is configured the same way as the existing `hardware/tps6292` board: 249k on VSET (3.3 V) and 52.3k on S-CONF (2.5 MHz, forced PWM, output discharge off).
- Keep the input at 16 V or below. The buck converter and the TLC5940 outputs are rated to 17 V.
- The S2B connectors are rated 1 A, which limits motor current.
- The `3V3` connector on the driver board is for powering both boards without 12 V, for example from a USB adapter while flashing. Do not feed 3.3 V into it while 12 V is connected.

### Motor

- BDR6133 logic: INA high / INB low = forward, INA low / INB high = reverse, both high = brake, both low = stop.
- The pull-downs keep the motor off while the ESP32 boots.

### LEDs

- Each output is a constant-current sink of 8.3 mA (39.06 / 4.7k), so LEDs need no series resistors. Connect each LED between an anode pin and an output pin.
- BLANK is pulled high so the LEDs stay off until the firmware runs.
- DCPRG is tied to 3.3 V, as on the existing `hardware/tlc5940` board. The dot-correction register is undefined after power-up, so the firmware must write it (with VPRG high) before using the outputs.
- OUT8-OUT15 are not connected.

### Audio

| Signal | ESP32 pin |
|---|---|
| I2S DIN | IO47 |
| I2S BCLK | IO33 |
| I2S LRCLK | IO34 |
| Amplifier SD_MODE | IO21, 510k pull-up to 3.3 V |

- With IO21 floating, the amplifier plays (L+R)/2. Driving IO21 low shuts it down; driving it high selects the left channel only.
- Gain is fixed at 9 dB.
- The amplifier runs from 3.3 V. Expect roughly 0.6 W into 8 ohm; this is extrapolated from the datasheet's 0.77 W at 3.7 V.
- Use a 4-8 ohm speaker. The output is bridge-tied, so neither speaker wire may be connected to GND.
- The existing firmware does not drive these pins yet (the I2S code in `MP3Player.cpp` is commented out).

### ESP32

- BOOT button on IO0 (left of the module), RESET button on EN (right of the module).
- No copper is placed under the module's antenna (top 5.3 mm of the board).

## Bill of materials

Every part carries an `LCSC` field in the schematic and on the PCB footprint.

### MCU board

| Ref | Part | Package | LCSC |
|---|---|---|---|
| U1 | ESP32-S3-MINI-1-N8 | Module | C2913206 |
| U2 | MAX98357AETE+T | TQFN-16 | C910544 |
| SW1, SW2 | TS-1088 tactile switch | SMD | C720477 |
| R1 | 10k | 0402 | C25744 |
| R2 | 510k | 0402 | C11616 |
| C1, C4 | 10 uF | 0603 | C19702 |
| C2, C5 | 100 nF | 0402 | C1525 |
| C3 | 1 uF | 0402 | C52923 |
| J1 | SM10B-SRSS-TB | | C160409 |
| J2, J4 | S2B-ZR | | C157930 |

### Driver board

| Ref | Part | Package | LCSC |
|---|---|---|---|
| D1 | ABS210 bridge | ABS | C84159 |
| U1 | TPS629210DRLR | SOT-583 | C5219301 |
| U2 | BDR6133 | ESOP-8 | C2687793 |
| U3 | TLC5940PWPR | HTSSOP-28 | C181653 |
| L1 | 2.2 uH, 3.3 A | 1210 | C6021483 |
| C1, C2 | 10 uF 25 V | 0805 | C15850 |
| C3 | 22 uF | 0805 | C45783 |
| C4, C5 | 100 nF | 0402 | C1525 |
| R1 | 249k | 0603 | C22918 |
| R2 | 52.3k | 0603 | C23198 |
| R3, R4 | 100k | 0402 | C25741 |
| R5 | 4.7k | 0402 | C25900 |
| R6, R7 | 10k | 0402 | C25744 |
| J1, J2, J5 | S2B-ZR | | C157930 |
| J3, J4 | SM10B-SRSS-TB | | C160409 |

## Verification

Checked with `kicad-cli` (KiCad 10.0.1):

| Board | ERC | DRC | Unconnected | Schematic parity |
|---|---|---|---|---|
| MCU | 1 warning | 1 error, 10 warnings | 0 | OK |
| Driver | 0 | 8 warnings | 0 | OK |

- The MCU ERC warning is a pin-type mismatch on the exposed pad of the stock MAX98357A symbol.
- The MCU DRC error is a courtyard overlap between `USB` (J2) and `SPK` (J4). It is expected: the three connectors only fit on the 23 mm edge with their bodies about 0.2 mm apart.
- All DRC warnings are silkscreen overlaps between neighbouring connectors, for the same reason.
- IC pinouts were checked against the datasheets for the BDR6133, TPS629210, TLC5940 and MAX98357A.
- Design rules: 0.2 mm tracks (0.3-0.6 mm for power, motor and speaker), 0.15 mm clearance, 0.6 / 0.3 mm vias, 0.3 mm copper-to-edge.

## Before ordering

- **TLC5940PWPR (C181653):** JLCPCB showed 1 in stock on 2026-11-27, so it would be a pre-order.
- **ESP32 module:** JLCPCB lists it as "Standard PCBA only", not Economic.
- **100 nF capacitor (C1525):** LCSC's own site showed 0 stock on 2026-11-27. JLCPCB's assembly stock for it was not checked.
- **Production files:** gerbers, BOM and placement files have not been generated. Use the Fabrication Toolkit plugin as on the other boards in `hardware/`.
- **Connector spacing:** neighbouring connectors are 0.2-0.25 mm apart, and the outer ones on the bottom edges are 0.2-0.25 mm from the board edge. Check this against real parts before ordering in quantity.
- **LED tracks:** on the driver board the eight LED tracks run under the body of the bridge rectifier, between its pads.
- **Review:** look over the autorouted tracks, especially the motor and power paths.

## Regenerating the design

The KiCad files are generated from [hardware/tools/design.py](hardware/tools/design.py), which holds the parts, nets and placement for both boards.

```
python ai/hardware/tools/build.py            # both boards
python ai/hardware/tools/build.py driver     # one board
```

This rewrites the schematic and PCB, re-runs the autorouter and prints the ERC/DRC summary. Manual edits made in KiCad are lost, and the routing differs on every run.

It needs:

- KiCad 10 in `C:\Program Files\KiCad\10.0`
- Java 21 and `freerouting-1.9.0.jar` in `%TEMP%` (from the Freerouting GitHub releases; 2.1.0 failed to route these boards)
- The JST footprint libraries `LIB_S2B-ZR_LF__SN_` and `LIB_SM10B-SRSS-TB_LF__SN_` in KiCad's global footprint table
- `hardware/esp32/esp32.kicad_pro` in this repo, used as the project template

| File | Purpose |
|---|---|
| `design.py` | Parts, nets, schematic and PCB positions, hand-routed tracks |
| `gen_sch.py` | Writes the schematic, project file and library tables |
| `gen_pcb.py` | Places footprints, autoroutes, adds ground pours and labels |
| `check.py` | Runs ERC and DRC and prints a summary |
| `build.py` | Runs the three steps above |
| `build_lib.py` | Rebuilds `hardware/lib` from symbols and footprints in the repo's existing boards |
| `sexp.py`, `symlib.py`, `netlist.py` | Helpers for KiCad file formats |

The shared symbols and footprints are in [hardware/lib](hardware/lib).
