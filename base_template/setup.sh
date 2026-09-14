#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Setting up editor integration for: $SCRIPT_DIR"

if command -v jadidi-hub >/dev/null 2>&1; then
    jadidi-hub setup-editor "$SCRIPT_DIR"
else
    echo "Error: jadidi-hub not found in PATH." >&2
    echo "Install: https://github.com/MHoseinJ/jadidi-hub" >&2
    exit 1
fi

echo ""
echo "Setup complete."