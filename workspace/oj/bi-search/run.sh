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
RUN_VERBOSE="${RUN_VERBOSE:-0}"

usage() {
  cat <<'USAGE'
Usage:
  ./run.sh                         Build and run. Auto-loads .env. Uses .run-args/.run-stdin or unique input/config file if present.
  ./run.sh <file-or-args...>       Build and run with exact argv. Any extension is allowed: .txt, .bin, .csv, .json, .yaml, .conf, etc.
  ./run.sh --stdin <file>          Build and run with file redirected to stdin.
  ./run.sh --env <file> <args...>  Load an env/config file before running. Can be repeated.
  ./run.sh --no-build <args...>    Run without make.
  ./run.sh --clean                 make clean.

Optional debug mode:
  RUN_VERBOSE=1 ./run.sh            Print the exact command before running.

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
  RUN_VERBOSE=1 ./run.sh input.bin
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
  if [[ "$RUN_VERBOSE" == "1" || "$RUN_VERBOSE" == "true" || "$RUN_VERBOSE" == "yes" ]]; then
    echo "+ $APP ${ARGS[*]-} < $STDIN_FILE" >&2
  fi
  exec "$APP" "${ARGS[@]}" < "$STDIN_FILE"
else
  if [[ "$RUN_VERBOSE" == "1" || "$RUN_VERBOSE" == "true" || "$RUN_VERBOSE" == "yes" ]]; then
    echo "+ $APP ${ARGS[*]-}" >&2
  fi
  exec "$APP" "${ARGS[@]}"
fi
