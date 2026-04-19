#!/usr/bin/env bash
set -eu

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

/usr/bin/python3 "$SCRIPT_DIR/cleanup.py" 
