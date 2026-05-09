#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
  echo "Usage: $0 <template> <relative-output-path>"
  echo "Templates: single-c single-cpp pthread-c multifile-cpp async-cpp"
  echo "Example: $0 pthread-c lab04/ex03"
  exit 1
fi

TEMPLATE="$1"
OUT_REL="$2"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
SRC="$ROOT_DIR/templates/$TEMPLATE"
DST="$ROOT_DIR/workspace/$OUT_REL"

if [[ ! -d "$SRC" ]]; then
  echo "Unknown template: $TEMPLATE"
  exit 1
fi

if [[ -e "$DST" ]]; then
  echo "Destination already exists: $DST"
  exit 1
fi

mkdir -p "$(dirname "$DST")"
cp -R "$SRC" "$DST"

# Ensure VS Code settings exist even if template changes later.
if [[ ! -d "$DST/.vscode" ]]; then
  cp -R "$ROOT_DIR/templates/_vscode" "$DST/.vscode"
fi

printf "Created %s from template %s\n" "$DST" "$TEMPLATE"
printf "Next:\n  cd %s\n  make run\n  code .\n" "$DST"
