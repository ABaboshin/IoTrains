"""Build <board>/<board>.kicad_pcb: placement from design.py, nets from the schematic, autorouted tracks.

Run with KiCad's Python:  python.exe gen_pcb.py [place|route] [board ...]
"""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

import pcbnew

import netlist
from design import BOARDS
from gen_sch import uid, write_project
from sexp import find_all, parse

ROOT = Path(__file__).resolve().parents[1]
KICAD = Path(sys.executable).resolve().parents[1]
KICAD_CLI = KICAD / "bin/kicad-cli.exe"
KICAD_FP = KICAD / "share/kicad/footprints"
GLOBAL_FP_TABLE = Path(os.environ["APPDATA"]) / "kicad/10.0/fp-lib-table"
TMP = Path(tempfile.gettempdir())
FREEROUTING = TMP / "freerouting-1.9.0.jar"  # github.com/freerouting/freerouting/releases/tag/v1.9.0
ORIGIN = (100.0, 60.0)
EDGE_INSET = 0.25


def mm(x, y):
    return pcbnew.VECTOR2I_MM(ORIGIN[0] + x, ORIGIN[1] + y)


def fp_dirs():
    dirs = {"iotrains": ROOT / "lib/iotrains.pretty"}
    table = parse(GLOBAL_FP_TABLE.read_text(encoding="utf-8"))
    for lib in find_all(table, "lib"):
        entry = {x[0]: str(x[1]) for x in lib[1:]}
        if "${" not in entry["uri"]:
            dirs[entry["name"]] = Path(entry["uri"])
    return dirs


def rect(board, layer, x0, y0, x1, y1, width=0.1):
    shape = pcbnew.PCB_SHAPE(board)
    shape.SetShape(pcbnew.SHAPE_T_RECT)
    shape.SetStart(mm(x0, y0))
    shape.SetEnd(mm(x1, y1))
    shape.SetLayer(layer)
    shape.SetWidth(pcbnew.FromMM(width))
    board.Add(shape)


def zone(board, x0, y0, x1, y1, layers, net=None, fills=False):
    z = pcbnew.ZONE(board)
    lset = pcbnew.LSET()
    for layer in layers:
        lset.AddLayer(layer)
    z.SetLayerSet(lset)
    outline = z.Outline()
    outline.NewOutline()
    for x, y in ((x0, y0), (x1, y0), (x1, y1), (x0, y1)):
        p = mm(x, y)
        outline.Append(p.x, p.y)
    if net is None:
        z.SetIsRuleArea(True)
        z.SetDoNotAllowTracks(True)
        z.SetDoNotAllowVias(True)
        z.SetDoNotAllowZoneFills(not fills)
        z.SetDoNotAllowPads(False)
        z.SetDoNotAllowFootprints(False)
    else:
        z.SetNet(net)
        z.SetLocalClearance(pcbnew.FromMM(0.25))
        z.SetMinThickness(pcbnew.FromMM(0.2))
        z.SetPadConnection(pcbnew.ZONE_CONNECTION_THT_THERMAL)
        z.SetThermalReliefGap(pcbnew.FromMM(0.25))
        z.SetThermalReliefSpokeWidth(pcbnew.FromMM(0.3))
    board.Add(z)
    return z


def place(name):
    spec = BOARDS[name]
    out = ROOT / name
    net_file = TMP / f"iotrains_{name}.net"
    subprocess.run([str(KICAD_CLI), "sch", "export", "netlist", "--format", "kicadsexpr", "-o", str(net_file),
                    str(out / f"{name}.kicad_sch")], check=True, capture_output=True)
    comps, nets = netlist.read(net_file)
    dirs = fp_dirs()
    pcb_path = out / f"{name}.kicad_pcb"
    pcb_path.unlink(missing_ok=True)
    write_project(out, spec, str(uid(name, "root")))
    board = pcbnew.NewBoard(str(pcb_path))
    w, h = spec["size"]
    rect(board, pcbnew.Edge_Cuts, 0, 0, w, h)
    for x0, y0, x1, y1 in spec["keepouts"]:
        zone(board, x0, y0, x1, y1, (pcbnew.F_Cu, pcbnew.B_Cu))
    for x0, y0, x1, y1 in spec.get("route_keepouts", []):
        zone(board, x0, y0, x1, y1, (pcbnew.B_Cu,), fills=True)

    fps = {}
    for p in spec["parts"]:
        if p["pcb"] is None:
            continue
        comp = comps[p["ref"]]
        nick, fp_name = comp["footprint"].split(":")
        fp = pcbnew.FootprintLoad(str(dirs.get(nick, KICAD_FP / f"{nick}.pretty")), fp_name)
        fp.SetFPID(pcbnew.LIB_ID(nick, fp_name))
        fp.SetReference(p["ref"])
        fp.SetValue(comp["value"])
        fp.SetPath(pcbnew.KIID_PATH("/" + comp["uuid"]))
        fp.SetSheetname("/")
        fp.SetSheetfile(f"{name}.kicad_sch")
        fp.SetField("LCSC", comp["fields"].get("LCSC", ""))
        fp.GetField("LCSC").SetVisible(False)
        fp.Reference().SetLayer(pcbnew.F_Fab)
        fp.Reference().SetTextSize(pcbnew.VECTOR2I_MM(0.6, 0.6))
        fp.Reference().SetTextThickness(pcbnew.FromMM(0.1))
        fp.Value().SetVisible(False)
        x, y, rot = p["pcb"]
        fp.SetPosition(mm(x, y))
        fp.SetOrientationDegrees(rot)
        board.Add(fp)
        fps[p["ref"]] = fp

    for net_name, nodes in nets.items():
        net = pcbnew.NETINFO_ITEM(board, net_name)
        board.Add(net)
        for ref, pin in nodes:
            hits = [pad for pad in fps[ref].Pads() if pad.GetNumber() == pin]
            if not hits:
                raise KeyError(f"{name}: {ref} has no pad {pin}")
            for pad in hits:
                pad.SetNet(net)

    for net_name, width, points in spec["tracks"]:
        for a, b in zip(points, points[1:]):
            track = pcbnew.PCB_TRACK(board)
            track.SetStart(mm(*a))
            track.SetEnd(mm(*b))
            track.SetWidth(pcbnew.FromMM(width))
            track.SetLayer(pcbnew.F_Cu)
            track.SetNet(board.FindNet(net_name))
            track.SetLocked(True)
            board.Add(track)

    pcbnew.SaveBoard(str(pcb_path), board)
    print(f"    placed {len(fps)} footprints")


def remove_dangling(board):
    """Drop the stubs the autorouter leaves behind."""
    removed = 0
    while True:
        board.BuildConnectivity()
        conn = board.GetConnectivity()
        stubs = [t for t in board.GetTracks()
                 if not t.IsLocked() and t.Type() != pcbnew.PCB_VIA_T and conn.TestTrackEndpointDangling(t, True)]
        if not stubs:
            return removed
        for t in stubs:
            board.Remove(t)
        removed += len(stubs)


def route(name, timeout=420):
    spec = BOARDS[name]
    path = ROOT / name / f"{name}.kicad_pcb"
    board = pcbnew.LoadBoard(str(path))
    dsn, ses = TMP / f"iotrains_{name}.dsn", TMP / f"iotrains_{name}.ses"
    ses.unlink(missing_ok=True)
    w, h = spec["size"]
    edge = next(d for d in board.GetDrawings() if d.GetLayer() == pcbnew.Edge_Cuts)
    edge.SetStart(mm(EDGE_INSET, EDGE_INSET))  # the router ignores the board's edge clearance rule
    edge.SetEnd(mm(w - EDGE_INSET, h - EDGE_INSET))
    if not pcbnew.ExportSpecctraDSN(board, str(dsn)):
        raise RuntimeError("DSN export failed")
    edge.SetStart(mm(0, 0))
    edge.SetEnd(mm(w, h))
    cmd = ["java", "-jar", str(FREEROUTING), "-de", str(dsn), "-do", str(ses), "-mp", "100", "-mt", "1", "-da"]
    try:
        log = subprocess.run(cmd, cwd=TMP, capture_output=True, text=True, timeout=timeout).stdout
    except subprocess.TimeoutExpired:
        log = "ERROR router timed out"
    summary = [line for line in log.splitlines() if "completed" in line or "ERROR" in line]
    print("   ", " | ".join(line.split("] ")[-1] for line in summary))
    if not pcbnew.ImportSpecctraSES(board, str(ses)):
        raise RuntimeError("SES import failed")
    print(f"    removed {remove_dangling(board)} dangling stubs")

    gnd = board.FindNet("GND")
    for layer in (pcbnew.F_Cu, pcbnew.B_Cu):
        zone(board, 0, 0, w, h, (layer,), gnd)
    pcbnew.ZONE_FILLER(board).Fill(board.Zones())

    for x, y, text in spec["silk"]:
        t = pcbnew.PCB_TEXT(board)
        t.SetText(text)
        t.SetLayer(pcbnew.B_SilkS)
        t.SetMirrored(True)
        t.SetTextSize(pcbnew.VECTOR2I_MM(0.9, 0.9))
        t.SetTextThickness(pcbnew.FromMM(0.15))
        t.SetPosition(mm(x, y))
        board.Add(t)
    pcbnew.SaveBoard(str(path), board)
    print(f"  {name}: {len(board.GetTracks())} track segments/vias")


if __name__ == "__main__":
    args = sys.argv[1:]
    stages = [a for a in args if a in ("place", "route")] or ["place", "route"]
    for board_name in [a for a in args if a in BOARDS] or list(BOARDS):
        print(f"== {board_name}")
        for stage in stages:
            globals()[stage](board_name)
