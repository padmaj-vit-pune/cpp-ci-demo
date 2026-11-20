#!/usr/bin/env python3
import json
import subprocess
from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CALC_BIN = ROOT / "calc"
INDEX_HTML = ROOT / "web" / "index.html"

ALLOWED_OPS = {
    "add", "sub", "mul", "div", "pow",
    "sqrt", "exp", "ln", "log10",
    "sin_deg", "cos_deg", "tan_deg",
    "sin_rad", "cos_rad", "tan_rad",
}


class CalcHandler(BaseHTTPRequestHandler):
    def _send_json(self, status: int, payload: dict) -> None:
        body = json.dumps(payload).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self) -> None:
        if self.path in ("/", "/index.html"):
            content = INDEX_HTML.read_bytes()
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(content)))
            self.end_headers()
            self.wfile.write(content)
        else:
            self.send_error(404, "Not Found")

    def do_POST(self) -> None:
        if self.path != "/api/calc":
            self.send_error(404, "Not Found")
            return

        if not CALC_BIN.exists():
            self._send_json(500, {"error": "Binary ./calc not built. Run g++ first."})
            return

        length = int(self.headers.get("Content-Length", "0"))
        try:
            data = json.loads(self.rfile.read(length).decode("utf-8"))
        except json.JSONDecodeError:
            self._send_json(400, {"error": "Invalid JSON"})
            return

        op = data.get("op", "")
        a = data.get("a")
        b = data.get("b")

        if op not in ALLOWED_OPS or a is None:
            self._send_json(400, {"error": "Invalid operation or operand"})
            return

        cmd = [str(CALC_BIN), op, str(a)]
        if b is not None:
            cmd.append(str(b))

        proc = subprocess.run(cmd, capture_output=True, text=True, check=False)
        if proc.returncode != 0:
            err_msg = proc.stderr.strip() or "Calculation failed"
            self._send_json(400, {"error": err_msg})
            return

        self._send_json(200, {"result": proc.stdout.strip()})


if __name__ == "__main__":
    server = HTTPServer(("127.0.0.1", 8080), CalcHandler)
    print("Serving C++ Scientific Calculator at http://127.0.0.1:8080")
    server.serve_forever()
