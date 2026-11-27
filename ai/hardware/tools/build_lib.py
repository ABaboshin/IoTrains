"""Collect the non-stock symbols and footprints into ai/hardware/lib."""
from pathlib import Path

from sexp import Q, dump, find, find_all, parse

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
LIB = ROOT / "lib"
KICAD_FP = Path(r"C:\Program Files\KiCad\10.0\share\kicad\footprints")

SYMBOLS = [
    (REPO / "hardware/esp32/esp32.kicad_sch", "ESP32-S3-MINI-1-N8:ESP32-S3-MINI-1-N8"),
    (REPO / "hardware/tps6292/tps6292.kicad_sch", "RP2040-Decoder_Additional_Symbols:BDR6133"),
    (REPO / "hardware/tps6292/tps6292.kicad_sch", "TPS629210DRLR:TPS629210DRLR"),
]

FOOTPRINTS = [
    REPO / "hardware/esp32v2/Symbols/XCVR_ESP32-S3-MINI-1-N8.kicad_mod",
    REPO / "hardware/esp32v2/footprints/Library.pretty/SOIC-8-1EP_3.9x4.9mm_P1.27mm_EP2.41x3.3mm_ThermalVias.kicad_mod",
    KICAD_FP / "Button_Switch_SMD.pretty/SW_SPST_TS-1088-xR020.kicad_mod",
]


def main():
    (LIB / "iotrains.pretty").mkdir(parents=True, exist_ok=True)
    out = ["kicad_symbol_lib", ["version", "20251024"], ["generator", Q("iotrains")], ["generator_version", Q("10.0")]]
    cache = {}
    for sch, lib_id in SYMBOLS:
        if sch not in cache:
            cache[sch] = find(parse(sch.read_text(encoding="utf-8")), "lib_symbols")
        sym = next(s for s in find_all(cache[sch], "symbol") if s[1] == lib_id)
        sym[1] = Q(lib_id.split(":")[1])
        out.append(sym)
    (LIB / "iotrains.kicad_sym").write_text(dump(out) + "\n", encoding="utf-8")
    for fp in FOOTPRINTS:
        text = fp.read_text(encoding="utf-8")
        if "ThermalVias" in fp.name:
            text = text.replace("(drill 0.2)", "(drill 0.3)")  # JLCPCB 2-layer minimum
        (LIB / "iotrains.pretty" / fp.name).write_text(text, encoding="utf-8")


if __name__ == "__main__":
    main()
