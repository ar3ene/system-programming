#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./tools/new.sh <template> <relative-output-path>
# Examples:
#   ./tools/new.sh single-cpp lab04/ex01
#   ./tools/new.sh pthread-c lab05/ex02
#
# This script copies a template into workspace/<relative-output-path>
# and injects a local ./run.sh into that exercise folder.
# It does NOT require modifying/replacing the whole template directory.

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
  echo "Available templates:"
  find "$ROOT_DIR/templates" -mindepth 1 -maxdepth 1 -type d \
    ! -name '_vscode' -exec basename {} \; | sort | sed 's/^/  - /'
  exit 1
fi

if [[ -e "$DST" ]]; then
  echo "Destination already exists: $DST"
  exit 1
fi

mkdir -p "$(dirname "$DST")"
cp -R "$SRC" "$DST"

# Ensure VS Code settings exist even if the chosen template is minimal.
if [[ ! -d "$DST/.vscode" && -d "$ROOT_DIR/templates/_vscode" ]]; then
  cp -R "$ROOT_DIR/templates/_vscode" "$DST/.vscode"
fi

# Inject/overwrite the one-command runner for this exercise.
# The runner intentionally supports arbitrary input/config filenames and extensions:
#   ./run.sh                         -> build, auto-load .env, auto-detect input/config if unique
#   ./run.sh data.bin                -> build, run ./bin/app data.bin
#   ./run.sh config.yaml data.csv    -> build, run ./bin/app config.yaml data.csv
#   ./run.sh --stdin any.file        -> build, run ./bin/app < any.file
#   ./run.sh --env .env.dev config.json
#   ./run.sh --clean
#
# Optional zero-typing config files in the exercise folder:
#   .env        shell-compatible env vars, auto-loaded if present
#   .run-args   one line of default argv, e.g. "config.yaml data/input.bin output.bin"
#   .run-stdin  path to default stdin file, e.g. "tests/case01.in"
cat > "$DST/run.sh" <<'RUNNER'
#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"

APP="${APP:-./bin/app}"
MAKE_TARGET="${MAKE_TARGET:-}"
DO_BUILD=1
CLEAN_ONLY=0
STDIN_FILE=""
ENV_FILES=()
USER_ARGS=()

usage() {
  cat <<'USAGE'
Usage:
  ./run.sh                         Build and run. Auto-loads .env. Uses .run-args/.run-stdin or unique input/config file if present.
  ./run.sh <file-or-args...>       Build and run with exact argv. Any extension is allowed: .txt, .bin, .csv, .json, .yaml, .conf, etc.
  ./run.sh --stdin <file>          Build and run with file redirected to stdin.
  ./run.sh --env <file> <args...>  Load an env/config file before running. Can be repeated.
  ./run.sh --no-build <args...>    Run without make.
  ./run.sh --clean                 make clean.

Zero-typing local config files:
  .env        Auto-loaded if present. Shell-compatible dotenv syntax, e.g. KEY=value, export KEY=value.
  .run-args   Default argv for ./bin/app when no args are given.
  .run-stdin  Default stdin file when no args are given.

Examples:
  ./run.sh
  ./run.sh input.bin
  ./run.sh config.yaml data.csv output.bin
  ./run.sh --stdin tests/case01.in
  ./run.sh --env .env.local config.json
USAGE
}

load_env_file() {
  local f="$1"
  if [[ ! -f "$f" ]]; then
    echo "run.sh: env file not found: $f" >&2
    exit 1
  fi
  # Shell-compatible dotenv loader. Lines such as KEY=value and export KEY=value work.
  # Do not put arbitrary commands in .env unless you trust the file.
  set -a
  # shellcheck disable=SC1090
  source "$f"
  set +a
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --help|-h)
      usage
      exit 0
      ;;
    --clean)
      CLEAN_ONLY=1
      shift
      ;;
    --no-build)
      DO_BUILD=0
      shift
      ;;
    --stdin)
      [[ $# -ge 2 ]] || { echo "run.sh: --stdin needs a file" >&2; exit 1; }
      STDIN_FILE="$2"
      shift 2
      ;;
    --env)
      [[ $# -ge 2 ]] || { echo "run.sh: --env needs a file" >&2; exit 1; }
      ENV_FILES+=("$2")
      shift 2
      ;;
    --)
      shift
      USER_ARGS+=("$@")
      break
      ;;
    *)
      USER_ARGS+=("$1")
      shift
      ;;
  esac
done

if [[ "$CLEAN_ONLY" -eq 1 ]]; then
  make clean
  exit 0
fi

# Always auto-load .env if present. This supports environment-variable based labs.
if [[ -f .env ]]; then
  load_env_file .env
fi
for env_file in "${ENV_FILES[@]}"; do
  load_env_file "$env_file"
done

if [[ "$DO_BUILD" -eq 1 ]]; then
  if [[ -n "$MAKE_TARGET" ]]; then
    make --quiet "$MAKE_TARGET"
  else
    make --quiet
  fi
fi

ARGS=("${USER_ARGS[@]}")

# If no command-line args were given, allow zero-typing defaults from config files.
if [[ ${#ARGS[@]} -eq 0 && -f .run-args ]]; then
  # Whitespace-separated argv. Good enough for lab filenames/configs without spaces.
  # For filenames with spaces, pass them explicitly: ./run.sh "file with spaces.txt"
  # shellcheck disable=SC2207
  ARGS=($(grep -v '^[[:space:]]*#' .run-args | tr '\n' ' '))
fi

if [[ ${#ARGS[@]} -eq 0 && -z "$STDIN_FILE" && -f .run-stdin ]]; then
  STDIN_FILE="$(grep -v '^[[:space:]]*#' .run-stdin | head -n 1 | xargs)"
fi

# If still no args, auto-detect exactly one likely input/config file.
# This is intentionally extension-agnostic and includes config-like files, but avoids
# source/build/editor files and generated outputs.
if [[ ${#ARGS[@]} -eq 0 && -z "$STDIN_FILE" ]]; then
  mapfile -t candidates < <(
    find . -maxdepth 2 -type f \
      ! -path './src/*' \
      ! -path './include/*' \
      ! -path './build/*' \
      ! -path './bin/*' \
      ! -path './.vscode/*' \
      ! -name 'Makefile' \
      ! -name 'run.sh' \
      ! -name '.env' \
      ! -name '.DS_Store' \
      \( \
        -name 'input*' -o -name 'Input*' -o \
        -name '*.in' -o -name '*.dat' -o -name '*.bin' -o -name '*.csv' -o \
        -name 'config*' -o -name '*.conf' -o -name '*.cfg' -o \
        -name '*.json' -o -name '*.yaml' -o -name '*.yml' -o -name '*.toml' -o -name '*.ini' \
      \) \
      | sed 's#^./##' | sort
  )

  if [[ ${#candidates[@]} -eq 1 ]]; then
    ARGS+=("${candidates[0]}")
  elif [[ ${#candidates[@]} -gt 1 ]]; then
    echo "run.sh: multiple possible input/config files found; choose one explicitly or create .run-args:" >&2
    printf '  %s\n' "${candidates[@]}" >&2
    echo "Example:" >&2
    echo "  ./run.sh ${candidates[0]}" >&2
    echo "or put default args in .run-args" >&2
    exit 2
  fi
fi

if [[ -n "$STDIN_FILE" ]]; then
  [[ -f "$STDIN_FILE" ]] || { echo "run.sh: stdin file not found: $STDIN_FILE" >&2; exit 1; }
  echo "+ $APP ${ARGS[*]-} < $STDIN_FILE"
  exec "$APP" "${ARGS[@]}" < "$STDIN_FILE"
else
  echo "+ $APP ${ARGS[*]-}"
  exec "$APP" "${ARGS[@]}"
fi
RUNNER

chmod +x "$DST/run.sh"

printf "Created %s from template %s\n" "$DST" "$TEMPLATE"
printf "Next:\n  cd %s\n  ./run.sh\n  code .\n" "$DST"
printf "Notes:\n  - Put env vars in .env; it is auto-loaded.\n  - Put default argv in .run-args for zero-typing file/config args.\n  - Put a stdin filename in .run-stdin if your program reads stdin.\n"
