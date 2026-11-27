"""Load schematic symbols from the stock KiCad libraries and the project library."""
import copy
from pathlib import Path

from sexp import Q, find, find_all, parse

ROOT = Path(__file__).resolve().parents[1]
KICAD_SYM = Path(r"C:\Program Files\KiCad\10.0\share\kicad\symbols")
_libs = {}


def _lib(name):
    if name not in _libs:
        path = ROOT / "lib/iotrains.kicad_sym" if name == "iotrains" else KICAD_SYM / f"{name}.kicad_sym"
        tree = parse(path.read_text(encoding="utf-8"))
        _libs[name] = {s[1]: s for s in find_all(tree, "symbol")}
    return _libs[name]


def _flatten(lib, name):
    sym = copy.deepcopy(_lib(lib)[name])
    ext = find(sym, "extends")
    if ext is None:
        return sym
    base = _flatten(lib, ext[1])
    base[1] = Q(name)
    for unit in find_all(base, "symbol"):
        unit[1] = Q(name + unit[1][len(ext[1]):])
    for prop in find_all(sym, "property"):
        base[:] = [x for x in base if not (isinstance(x, list) and x[0] == "property" and x[1] == prop[1])]
        base.insert(2, prop)
    return base


def load(lib_id):
    """Return (lib_symbols entry, pins) where pins are dicts in symbol coordinates."""
    lib, name = lib_id.split(":")
    sym = _flatten(lib, name)
    sym[1] = Q(lib_id)
    pins = []
    for unit in find_all(sym, "symbol"):
        for pin in find_all(unit, "pin"):
            at = find(pin, "at")
            pins.append({
                "number": str(find(pin, "number")[1]),
                "name": str(find(pin, "name")[1]),
                "type": pin[1],
                "x": float(at[1]),
                "y": float(at[2]),
                "angle": int(float(at[3])),
            })
    return sym, pins


if __name__ == "__main__":
    import sys

    for lib_id in sys.argv[1:]:
        _, pins = load(lib_id)
        xs = [p["x"] for p in pins]
        ys = [p["y"] for p in pins]
        print(f"== {lib_id} x[{min(xs)},{max(xs)}] y[{min(ys)},{max(ys)}]")
        print("  " + "  ".join(f'{p["number"]}:{p["name"]}/{p["type"][:4]}@{p["x"]:g},{p["y"]:g},{p["angle"]}' for p in pins))
