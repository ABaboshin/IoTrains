"""Single source of truth for both boards: parts, nets, schematic and PCB placement.

pins map a pin name or number to a net; None marks an intentional no-connect.
sch = (x, y) on the sheet, pcb = (x, y, rotation) relative to the board's top-left corner.
"""

R0402 = "Resistor_SMD:R_0402_1005Metric"
R0603 = "Resistor_SMD:R_0603_1608Metric"
C0402 = "Capacitor_SMD:C_0402_1005Metric"
C0603 = "Capacitor_SMD:C_0603_1608Metric"
C0805 = "Capacitor_SMD:C_0805_2012Metric"
S2B = "LIB_S2B-ZR_LF__SN_:S2BZRLFSN"
SM10B = "LIB_SM10B-SRSS-TB_LF__SN_:SM10B-SRSS-TB"

LINK = {"1": "GND", "2": "+3V3", "3": "GSCLK", "4": "BLANK", "5": "SIN", "6": "SCLK", "7": "XLAT", "8": "VPRG",
        "9": "MOT_A", "10": "MOT_B"}


def part(ref, sym, value, fp, lcsc, pins, sch, pcb=None):
    return dict(ref=ref, sym=sym, value=value, fp=fp, lcsc=lcsc, pins=pins, sch=sch, pcb=pcb)


def res(ref, value, lcsc, a, b, sch, pcb, fp=R0402):
    return part(ref, "Device:R", value, fp, lcsc, {"1": a, "2": b}, sch, pcb)


def cap(ref, value, lcsc, a, b, sch, pcb, fp=C0402):
    return part(ref, "Device:C", value, fp, lcsc, {"1": a, "2": b}, sch, pcb)


def conn2(ref, value, a, b, sch, pcb):
    return part(ref, "Connector_Generic:Conn_01x02", value, S2B, "C157930", {"1": a, "2": b}, sch, pcb)


def flag(n, net, sch):
    return part(f"#FLG0{n}", "power:PWR_FLAG", "PWR_FLAG", "", "", {"1": net}, sch)


ESP_PINS = {
    "3V3": "+3V3", "EN": "EN", "IO0": "BOOT",
    "IO11": "GSCLK", "IO12": "BLANK", "IO13": "SIN", "IO14": "SCLK", "IO15": "XLAT", "IO16": "VPRG",
    "IO17": "MOT_A", "IO18": "MOT_B", "IO19": "USB_D-", "IO20": "USB_D+",
    "IO21": "AMP_SD", "IO47": "I2S_DIN", "IO33": "I2S_BCLK", "IO34": "I2S_LRCLK",
}
ESP_PINS.update({f"IO{n}": None for n in (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 26, 35, 36, 37, 38, 39, 40, 41, 42, 45, 46, 48)})
ESP_PINS.update({"TXD0": None, "RXD0": None, "GND*": "GND"})

MCU = dict(
    name="mcu",
    title="IoTrains MCU board (ESP32-S3 + I2S amplifier)",
    size=(23.0, 40.0),
    keepouts=[(0.0, 0.0, 23.0, 5.3)],
    classes={"Power": (0.35, ["GND", "+3V3"]), "Audio": (0.3, ["/SPK+", "/SPK-"])},
    parts=[
        part("U1", "iotrains:ESP32-S3-MINI-1-N8", "ESP32-S3-MINI-1-N8", "iotrains:XCVR_ESP32-S3-MINI-1-N8", "C2913206",
             ESP_PINS, (109.22, 76.2), (11.9, 13.05, 0)),
        cap("C1", "10uF", "C19702", "+3V3", "GND", (165.1, 60.96), (1.0, 6.95, 90), C0603),
        cap("C2", "100nF", "C1525", "+3V3", "GND", (177.8, 60.96), (2.8, 6.95, 90)),
        res("R1", "10k", "C25744", "+3V3", "EN", (165.1, 96.52), (21.0, 7.1, 90)),
        cap("C3", "1uF", "C52923", "EN", "GND", (177.8, 96.52), (22.2, 7.1, 90)),
        part("SW1", "Switch:SW_Push", "BOOT", "iotrains:SW_SPST_TS-1088-xR020", "C720477",
             {"1": "BOOT", "2": "GND"}, (198.12, 81.28), (1.9, 11.6, 270)),
        part("SW2", "Switch:SW_Push", "RESET", "iotrains:SW_SPST_TS-1088-xR020", "C720477",
             {"1": "GND", "2": "EN"}, (198.12, 96.52), (1.9, 17.7, 270)),
        part("U2", "Audio:MAX98357A", "MAX98357AETE+T", "Package_DFN_QFN:TQFN-16-1EP_3x3mm_P0.5mm_EP1.23x1.23mm",
             "C910544",
             {"DIN": "I2S_DIN", "GAIN_SLOT": None, "GND": "GND", "~{SD_MODE}": "AMP_SD", "VDD": "+3V3",
              "OUTP": "SPK+", "OUTN": "SPK-", "LRCLK": "I2S_LRCLK", "BCLK": "I2S_BCLK", "PAD": "GND"},
             (254.0, 71.12), (18.6, 24.4, 0)),
        res("R2", "510k", "C11616", "+3V3", "AMP_SD", (223.52, 109.22), (15.2, 25.6, 90)),
        cap("C4", "10uF", "C19702", "+3V3", "GND", (236.22, 109.22), (21.9, 24.4, 90), C0603),
        cap("C5", "100nF", "C1525", "+3V3", "GND", (248.92, 109.22), (19.2, 27.3, 0)),
        part("J1", "Connector_Generic:Conn_01x10", "LINK", SM10B, "C160409", LINK, (330.2, 71.12), (5.4, 35.1, 0)),
        conn2("J2", "USB", "USB_D+", "USB_D-", (330.2, 109.22), (5.0, 23.9, 270)),
        conn2("J3", "3V3", "GND", "+3V3", (330.2, 129.54), (5.0, 30.6, 270)),
        conn2("J4", "SPK", "SPK+", "SPK-", (330.2, 149.86), (18.0, 32.1, 90)),
        flag(1, "+3V3", (289.56, 180.34)),
        flag(2, "GND", (302.26, 180.34)),
    ],
    notes=[
        (150, 40, "ESP32-S3-MINI-1-N8: USB on IO19/IO20, TLC5940 on IO11..IO16, motor on IO17/IO18"),
        (215, 46, "I2S amplifier: SD_MODE = 510k pull-up -> (L+R)/2, drive IO21 low to mute. Gain 9 dB (GAIN open)."),
        (300, 50, "LINK (1:1 to driver board): 1 GND, 2 3V3, 3..8 = IO11..IO16, 9/10 = motor A/B"),
        (300, 100, "USB: 1 D+, 2 D-   3V3: 1 GND, 2 +3V3   SPK: 4..8 ohm speaker"),
    ],
    silk=[
        (11.5, 10.0, "IoTrains MCU"), (11.5, 12.0, "ESP32-S3"),
        (11.0, 23.9, "USB 1 D+"), (11.0, 25.4, "2 D-"),
        (11.0, 30.6, "3V3 1 GND"), (11.0, 32.1, "2 3V3"),
        (19.5, 28.0, "SPK"), (11.4, 37.5, "LINK"), (6.9, 37.5, "1"), (15.9, 37.5, "10"),
        (4.5, 11.6, "BOOT"), (4.5, 17.7, "RST"),
    ],
    tracks=[],
)

TLC_PINS = {"GND": "GND", "PAD": "GND", "VCC": "+3V3", "DCPRG": "+3V3", "IREF": "IREF", "BLANK": "BLANK",
            "XLAT": "XLAT", "SCLK": "SCLK", "SIN": "SIN", "VPRG": "VPRG", "GSCLK": "GSCLK", "XERR": None, "SOUT": None}
TLC_PINS.update({f"OUT{n}": f"LED{n}" for n in range(8)})
TLC_PINS.update({f"OUT{n}": None for n in range(8, 16)})

DRIVER = dict(
    name="driver",
    title="IoTrains driver board (12V in, 3.3V buck, motor driver, LED driver)",
    size=(23.0, 40.0),
    keepouts=[],
    classes={"Power": (0.35, ["GND", "+3V3", "/VM", "/SW"]),
             "Motor": (0.6, ["/AC1", "/AC2", "/MOTOR_A", "/MOTOR_B"])},
    parts=[
        conn2("J1", "12V", "AC1", "AC2", (40.64, 60.96), (2.9, 35.0, 0)),
        part("D1", "Diode_Bridge:ABS2", "ABS210", "Diode_SMD:Diode_Bridge_Diotec_ABS", "C84159",
             {"1": "VM", "2": "GND", "3": "AC1", "4": "AC2"}, (71.12, 60.96), (3.65, 28.0, 270)),
        cap("C1", "10uF 25V", "C15850", "VM", "GND", (96.52, 60.96), (7.7, 25.0, 90), C0805),
        part("U1", "iotrains:TPS629210DRLR", "TPS629210DRLR", "Package_TO_SOT_SMD:SOT-583-8", "C5219301",
             {"FB/VSET": "VSET", "PG": None, "VOS": "+3V3", "SW": "SW", "GND": "GND", "VIN": "VM", "EN": "VM",
              "MODE/S-CONF": "SCONF"}, (139.7, 60.96), (10.1, 25.5, 180)),
        part("L1", "Device:L", "2.2uH", "Inductor_SMD:L_1210_3225Metric", "C6021483",
             {"1": "SW", "2": "+3V3"}, (180.34, 60.96), (11.1, 22.4, 180)),
        cap("C3", "22uF", "C45783", "+3V3", "GND", (193.04, 60.96), (7.0, 22.2, 180), C0805),
        res("R1", "249k", "C22918", "GND", "VSET", (205.74, 60.96), (11.6, 28.0, 180), R0603),
        res("R2", "52.3k", "C23198", "SCONF", "GND", (218.44, 60.96), (8.6, 28.0, 180), R0603),
        part("U2", "iotrains:BDR6133", "BDR6133", "iotrains:SOIC-8-1EP_3.9x4.9mm_P1.27mm_EP2.41x3.3mm_ThermalVias",
             "C2687793",
             {"GND": "GND", "VCC": "+3V3", "VM": "VM", "OUTA": "MOTOR_A", "OUTB": "MOTOR_B", "INA": "MOT_A",
              "INB": "MOT_B"}, (71.12, 127.0), (18.3, 28.5, 0)),
        cap("C2", "10uF 25V", "C15850", "VM", "GND", (101.6, 127.0), (13.5, 30.6, 270), C0805),
        cap("C4", "100nF", "C1525", "+3V3", "GND", (119.38, 127.0), (13.5, 26.4, 180)),
        res("R3", "100k", "C25741", "MOT_A", "GND", (134.62, 127.0), (20.4, 24.2, 90)),
        res("R4", "100k", "C25741", "MOT_B", "GND", (147.32, 127.0), (21.8, 24.2, 90)),
        conn2("J2", "MOTOR", "MOTOR_A", "MOTOR_B", (175.26, 127.0), (18.6, 35.0, 0)),
        part("U3", "Driver_LED:TLC5940PWP", "TLC5940PWP",
             "Package_SO:HTSSOP-28-1EP_4.4x9.7mm_P0.65mm_EP3.4x9.5mm_Mask2.4x6.17mm_ThermalVias", "C181653",
             TLC_PINS, (254.0, 124.46), (12.0, 14.5, 0)),
        res("R5", "4.7k", "C25900", "IREF", "GND", (180.34, 177.8), (18.2, 11.3, 0)),
        res("R6", "10k", "C25744", "+3V3", "BLANK", (193.04, 177.8), (18.2, 12.8, 0)),
        res("R7", "10k", "C25744", "VPRG", "GND", (205.74, 177.8), (18.2, 14.3, 0)),
        cap("C5", "100nF", "C1525", "+3V3", "GND", (218.44, 177.8), (18.2, 9.8, 0)),
        part("J3", "Connector_Generic:Conn_01x10", "LINK", SM10B, "C160409", LINK, (330.2, 60.96), (17.5, 4.9, 180)),
        part("J4", "Connector_Generic:Conn_01x10", "LED", SM10B, "C160409",
             {"1": "VM", "10": "VM", **{str(n + 2): f"LED{n}" for n in range(8)}}, (330.2, 124.46), (4.9, 9.95, 270)),
        flag(1, "VM", (40.64, 180.34)),
        flag(2, "+3V3", (53.34, 180.34)),
        flag(3, "GND", (66.04, 180.34)),
    ],
    notes=[
        (35, 40, "12V DC in, either polarity -> bridge -> VM (about 10 V). Absolute max input 16 V."),
        (120, 35, "TPS629210: VSET 249k = 3.3 V, S-CONF 52.3k = 2.5 MHz forced PWM, output discharge off"),
        (35, 105, "BDR6133: INA/INB from ESP32 (100k pull-downs keep the motor off during boot)"),
        (175, 160, "TLC5940: Imax = 39.06 / R5 = 8.3 mA per channel. BLANK pull-up keeps LEDs off until firmware runs."),
        (300, 100, "LED: 1 and 10 = LED anodes (VM), 2..9 = OUT0..OUT7 (current sinks)"),
        (300, 40, "LINK: 1:1 cable to the MCU board (pin n to pin n)"),
    ],
    silk=[
        (15.5, 21.5, "IoTrains DRV"),
        (3.65, 37.5, "12V"), (19.35, 37.5, "MOTOR"),
        (11.5, 2.5, "LINK"), (16.0, 2.5, "1"), (7.0, 2.5, "10"),
        (3.0, 16.0, "LED"), (3.0, 11.45, "+"), (3.0, 20.45, "+"), (3.0, 12.6, "0"), (3.0, 19.3, "7"),
    ],
    # hand-routed buck converter: (net, width, points) on F.Cu, locked so the autorouter keeps them
    tracks=[
        ("/VM", 0.5, [(5.65, 25.03), (6.5, 25.95), (7.7, 25.95)]),
        ("/VM", 0.3, [(7.7, 25.95), (8.2, 25.75), (9.36, 25.75), (9.36, 25.25)]),
        ("GND", 0.3, [(9.36, 24.75), (8.7, 24.75), (8.2, 24.25), (7.7, 24.05)]),
        ("/SW", 0.3, [(10.84, 24.75), (11.7, 24.75), (12.5, 23.95), (12.5, 22.4)]),
        ("+3V3", 0.2, [(10.84, 25.25), (10.1, 25.25), (10.1, 23.4)]),
        ("+3V3", 0.5, [(9.7, 22.4), (7.95, 22.2)]),
        ("/SCONF", 0.2, [(9.36, 26.25), (9.36, 27.6), (9.38, 28.0)]),
        ("/VSET", 0.2, [(10.84, 26.25), (10.84, 27.6), (10.82, 28.0)]),
    ],
)

BOARDS = {"mcu": MCU, "driver": DRIVER}
