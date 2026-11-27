"""Read a KiCad s-expression netlist exported by kicad-cli."""
from pathlib import Path

from sexp import find, find_all, parse


def read(path):
    tree = parse(Path(path).read_text(encoding="utf-8"))
    comps = {}
    for c in find_all(find(tree, "components"), "comp"):
        tstamps = find(c, "tstamps")
        comps[str(find(c, "ref")[1])] = {
            "value": str(find(c, "value")[1]),
            "footprint": str(find(c, "footprint")[1]) if find(c, "footprint") else "",
            "uuid": str(tstamps[1]),
            "fields": {str(p[1][1]): str(p[2][1]) for p in find_all(c, "property") if len(p) > 2},
        }
    nets = {}
    for n in find_all(find(tree, "nets"), "net"):
        nets[str(find(n, "name")[1])] = [(str(find(node, "ref")[1]), str(find(node, "pin")[1]))
                                         for node in find_all(n, "node")]
    return comps, nets


if __name__ == "__main__":
    import sys

    comps, nets = read(sys.argv[1])
    for name, nodes in nets.items():
        if not name.startswith("unconnected-"):
            print(f"{name}: " + " ".join(f"{r}.{p}" for r, p in nodes))
    print("unconnected:", sum(n.startswith("unconnected-") for n in nets), "| parts:", len(comps))
