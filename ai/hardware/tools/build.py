"""Regenerate everything: python build.py [board ...]   (place + autoroute + checks + review image)"""
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
KICAD_PY = r"C:\Program Files\KiCad\10.0\bin\python.exe"
boards = sys.argv[1:] or ["mcu", "driver"]


def run(python, script, *args):
    out = subprocess.run([python, str(HERE / script), *args], capture_output=True, text=True)
    lines = [line for line in (out.stdout + out.stderr).splitlines() if "image handler" not in line]
    print("\n".join(lines))
    if out.returncode:
        sys.exit(f"{script} failed")


run(sys.executable, "gen_sch.py", *boards)
run(KICAD_PY, "gen_pcb.py", "place", "route", *boards)
run(sys.executable, "check.py", *boards)
