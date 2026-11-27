"""Minimal KiCad s-expression reader/writer."""


class Q(str):
    """A quoted string atom."""


def parse(text):
    pos = 0
    n = len(text)
    stack = [[]]
    while pos < n:
        c = text[pos]
        if c in " \t\r\n":
            pos += 1
        elif c == "(":
            stack.append([])
            pos += 1
        elif c == ")":
            done = stack.pop()
            stack[-1].append(done)
            pos += 1
        elif c == '"':
            pos += 1
            buf = []
            while text[pos] != '"':
                if text[pos] == "\\":
                    buf.append(text[pos : pos + 2])
                    pos += 2
                else:
                    buf.append(text[pos])
                    pos += 1
            pos += 1
            stack[-1].append(Q("".join(buf)))
        else:
            start = pos
            while pos < n and text[pos] not in ' \t\r\n()"':
                pos += 1
            stack[-1].append(text[start:pos])
    return stack[0][0]


def dump(node, indent=0):
    if isinstance(node, Q):
        return '"' + node + '"'
    if isinstance(node, str):
        return node
    flat = "(" + " ".join(dump(x) for x in node) + ")"
    if len(flat) <= 100 or not any(isinstance(x, list) for x in node):
        return flat
    pad = "\t" * (indent + 1)
    head = []
    rest = list(node)
    while rest and not isinstance(rest[0], list):
        head.append(dump(rest.pop(0)))
    lines = ["(" + " ".join(head)]
    for x in rest:
        lines.append(pad + dump(x, indent + 1))
    lines.append("\t" * indent + ")")
    return "\n".join(lines)


def find(node, name):
    for x in node:
        if isinstance(x, list) and x and x[0] == name:
            return x
    return None


def find_all(node, name):
    return [x for x in node if isinstance(x, list) and x and x[0] == name]
