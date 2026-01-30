#!/bin/bash

# Usage: ./output.sh <new_dir_name>

set -euo pipefail

cd /scratch/ss14729/apxs/build2/output

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <new_dir_name>"
  exit 1
fi

dest="$1"
mkdir -p "$dest"
mv -- *.csv "$dest"/
zip -r "${dest}.zip" "$dest"