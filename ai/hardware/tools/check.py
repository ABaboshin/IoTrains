"""Run ERC/DRC on the boards and print a compact summary.  Usage: python check.py [board ...]"""
import json
import subprocess
import sys
import tempfile
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
KICAD_CLI = r"C:\Program Files\KiCad\10.0\bin\kicad-cli.exe"
TMP = Path(tempfile.gettempdir())


def run(kind, name, extra):
    ext = "kicad_sch" if kind == "sch" else "kicad_pcb"
    out = TMP / f"iotrains_{name}_{kind}.json"
    cmd = [KICAD_CLI, kind, "erc" if kind == "sch" else "drc", "--format", "json", "--severity-all", "-o", str(out)]
    subprocess.run(cmd + extra + [str(ROOT / name / f"{name}.{ext}")], capture_output=True, check=True)
    return json.loads(out.read_text(encoding="utf-8"))


def describe(v):
    where = "; ".join(f'{i["description"]} @({i["pos"]["x"]:.2f},{i["pos"]["y"]:.2f})' for i in v.get("items", []))
    return f'    [{v["severity"]}] {v["type"]}: {v["description"]} :: {where}'[:260]


for name in sys.argv[1:] or ["mcu", "driver"]:
    erc = run("sch", name, [])
    violations = [v for sheet in erc.get("sheets", []) for v in sheet.get("violations", [])]
    print(f"== {name}: ERC {len(violations)}")
    for v in violations:
        print(describe(v))
    drc = run("pcb", name, ["--schematic-parity"])
    for key in ("violations", "unconnected_items", "schematic_parity"):
        items = drc.get(key, [])
        print(f"   DRC {key}: {len(items)} {dict(Counter(v['type'] for v in items))}")
        for v in items[:12]:
            print(describe(v))
