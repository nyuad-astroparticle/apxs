#!/bin/bash

# Usage: ./output.sh <zip_name>

set -euo pipefail

build_dir="/scratch/ss14729/apxs/build_ne"

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <zip_name>"
  exit 1
fi

zip_name="${1%.zip}"

cd "$build_dir"
zip -r "${zip_name}.zip" output/
