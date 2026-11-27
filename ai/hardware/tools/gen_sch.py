"""Generate <board>/<board>.kicad_sch plus the project and library-table files."""
import json
import subprocess
import sys
import uuid
from pathlib import Path

import symlib
from design import BOARDS
from sexp import Q, dump

ROOT = Path(__file__).resolve().parents[1]
TEMPLATE_PRO = ROOT.parents[1] / "hardware/esp32/esp32.kicad_pro"
KICAD_CLI = r"C:\Program Files\KiCad\10.0\bin\kicad-cli.exe"
NS = uuid.UUID("6f1d2a9e-5c0b-4f43-9a55-0b1d7e5a1c01")
STUB = 2.54
POWER = {"GND": "power:GND", "+3V3": "power:+3V3"}
# rotation of a power symbol so that it points along the stub direction (dx, dy)
POWER_ROT = {
    "power:GND": {(0, 1): 0, (1, 0): 90, (0, -1): 180, (-1, 0): 270},
    "power:+3V3": {(0, -1): 0, (-1, 0): 90, (0, 1): 180, (1, 0): 270},
}
LABEL = {(-1, 0): (180, "right"), (1, 0): (0, "left"), (0, -1): (90, "left"), (0, 1): (270, "right")}
OUTWARD = {0: (-1, 0), 180: (1, 0), 90: (0, 1), 270: (0, -1)}


def uid(*key):
    return Q(str(uuid.uuid5(NS, "/".join(str(k) for k in key))))


def num(v):
    return f"{round(v, 4):g}"


def font(size=1.27, justify=None, hide=False):
    eff = ["effects", ["font", ["size", num(size), num(size)]]]
    if justify:
        eff.append(["justify"] + justify.split())
    out = [eff]
    if hide:
        out.insert(0, ["hide", "yes"])
    return out


def prop(name, value, x, y, hide=False, justify=None):
    return ["property", Q(name), Q(value), ["at", num(x), num(y), "0"]] + font(justify=justify, hide=hide)


def net_of(part, pin):
    pins = part["pins"]
    for key in (pin["number"], pin["name"]):
        if key in pins:
            return True, pins[key]
    for key, net in pins.items():
        if key.endswith("*") and pin["name"].startswith(key[:-1]):
            return True, net
    if pin["type"] == "no_connect":
        return False, None
    raise KeyError(f'{part["ref"]}: pin {pin["number"]} ({pin["name"]}) has no net assigned')


class Sheet:
    def __init__(self, board):
        self.board = board
        self.name = board["name"]
        self.root = uid(self.name, "root")
        self.lib = {}
        self.items = []
        self.pwr = 0

    def symbol(self, lib_id, ref, value, x, y, rot=0, fp="", lcsc="", hide_ref=False, ref_at=None, val_at=None):
        if lib_id not in self.lib:
            self.lib[lib_id] = symlib.load(lib_id)
        pins = self.lib[lib_id][1]
        rx, ry, rj = ref_at or (x, y - 2.54, None)
        vx, vy, vj = val_at or (x, y + 2.54, None)
        sym = [
            "symbol", ["lib_id", Q(lib_id)], ["at", num(x), num(y), str(rot)], ["unit", "1"], ["body_style", "1"],
            ["exclude_from_sim", "no"], ["in_bom", "yes"], ["on_board", "yes"], ["in_pos_files", "yes"],
            ["dnp", "no"], ["uuid", uid(self.name, ref)],
            prop("Reference", ref, rx, ry, hide_ref, rj),
            prop("Value", value, vx, vy, hide_ref, vj),
            prop("Footprint", fp, x, y, True),
            prop("Datasheet", "", x, y, True),
            prop("Description", "", x, y, True),
        ]
        if lcsc:
            sym.append(prop("LCSC", lcsc, x, y, True))
        for number in dict.fromkeys(p["number"] for p in pins):
            sym.append(["pin", Q(number), ["uuid", uid(self.name, ref, "pin", number)]])
        sym.append(["instances", ["project", Q(self.name),
                                  ["path", Q("/" + self.root), ["reference", Q(ref)], ["unit", "1"]]]])
        self.items.append(sym)
        return pins

    def place(self, part):
        x, y = part["sch"]
        pins = symlib.load(part["sym"])[1]
        xs, ys = [p["x"] for p in pins], [p["y"] for p in pins]
        dirs = {OUTWARD[p["angle"]] for p in pins}
        if part["ref"].startswith("#"):
            ref_at = val_at = None
        elif min(xs) == max(xs) == 0:
            ref_at, val_at = (x + 2.54, y - 1.27, "left"), (x + 2.54, y + 1.27, "left")
        else:
            up = 8.89 if (0, -1) in dirs else 5.08
            down = 8.89 if (0, 1) in dirs else 5.08
            ref_at = (x + min(xs) + 2.54, y - max(ys) - up, "left")
            val_at = (x + min(xs) + 2.54, y - min(ys) + down, "left")
        self.symbol(part["sym"], part["ref"], part["value"], x, y, 0, part["fp"], part["lcsc"],
                    part["ref"].startswith("#"), ref_at, val_at)
        done = set()
        for pin in pins:
            px, py = x + pin["x"], y - pin["y"]
            if (px, py) in done:
                continue
            done.add((px, py))
            known, net = net_of(part, pin)
            if not known:
                continue
            if net is None:
                self.items.append(["no_connect", ["at", num(px), num(py)], ["uuid", uid(self.name, "nc", px, py)]])
                continue
            dx, dy = OUTWARD[pin["angle"]]
            ex, ey = px + dx * STUB, py + dy * STUB
            self.items.append(["wire", ["pts", ["xy", num(px), num(py)], ["xy", num(ex), num(ey)]],
                               ["stroke", ["width", "0"], ["type", "default"]],
                               ["uuid", uid(self.name, "w", px, py)]])
            if net in POWER:
                self.pwr += 1
                lib_id = POWER[net]
                self.symbol(lib_id, f"#PWR{self.pwr:03d}", net, ex, ey, POWER_ROT[lib_id][(dx, dy)], hide_ref=True)
            else:
                angle, just = LABEL[(dx, dy)]
                self.items.append(["label", Q(net), ["at", num(ex), num(ey), str(angle)]]
                                  + font(justify=f"{just} bottom") + [["uuid", uid(self.name, "l", px, py)]])

    def write(self):
        for part in self.board["parts"]:
            self.place(part)
        for i, (x, y, text) in enumerate(self.board["notes"]):
            self.items.append(["text", Q(text), ["exclude_from_sim", "no"], ["at", num(x), num(y), "0"]]
                              + font(1.5, "left bottom") + [["uuid", uid(self.name, "t", i)]])
        tree = [
            "kicad_sch", ["version", "20260306"], ["generator", Q("eeschema")], ["generator_version", Q("10.0")],
            ["uuid", self.root], ["paper", Q("A3")],
            ["title_block", ["title", Q(self.board["title"])], ["rev", Q("1")]],
            ["lib_symbols"] + [sym for sym, _ in self.lib.values()],
        ] + self.items + [["sheet_instances", ["path", Q("/"), ["page", Q("1")]]], ["embedded_fonts", "no"]]
        out = ROOT / self.name
        out.mkdir(exist_ok=True)
        (out / f"{self.name}.kicad_sch").write_text(dump(tree) + "\n", encoding="utf-8")
        # re-save in KiCad's own formatting
        subprocess.run([KICAD_CLI, "sch", "upgrade", "--force", str(out / f"{self.name}.kicad_sch")],
                       check=True, capture_output=True)
        write_project(out, self.board, str(self.root))


def write_project(out, board, root_uuid):
    name = board["name"]
    pro = json.loads(TEMPLATE_PRO.read_text(encoding="utf-8"))
    pro["meta"]["filename"] = f"{name}.kicad_pro"
    pro["sheets"] = [[root_uuid, "Root"]]
    ds = pro["board"]["design_settings"]
    ds["drc_exclusions"] = []
    ds["rules"].update(min_clearance=0.15, min_track_width=0.15, min_copper_edge_clearance=0.3,
                       min_through_hole_diameter=0.3, min_via_diameter=0.5)
    ds["track_widths"] = [0.0, 0.2, 0.35, 0.6]
    ds["via_dimensions"] = [{"diameter": 0.0, "drill": 0.0}, {"diameter": 0.6, "drill": 0.3}]
    default = pro["net_settings"]["classes"][0]
    default.update(clearance=0.15, track_width=0.2, via_diameter=0.6, via_drill=0.3)
    classes = [default]
    patterns = []
    for cls, (width, nets) in board["classes"].items():
        classes.append(dict(default, name=cls, track_width=width, priority=0))
        patterns += [{"netclass": cls, "pattern": net} for net in nets]
    pro["net_settings"]["classes"] = classes
    pro["net_settings"]["netclass_patterns"] = patterns
    (out / f"{name}.kicad_pro").write_text(json.dumps(pro, indent=2) + "\n", encoding="utf-8")
    for kind, ext in (("sym", "kicad_sym"), ("fp", "pretty")):
        (out / f"{kind}-lib-table").write_text(
            f'({kind}_lib_table\n  (version 7)\n  (lib (name "iotrains")(type "KiCad")'
            f'(uri "${{KIPRJMOD}}/../lib/iotrains.{ext}")(options "")(descr ""))\n)\n', encoding="utf-8")


if __name__ == "__main__":
    for name in sys.argv[1:] or BOARDS:
        Sheet(BOARDS[name]).write()
