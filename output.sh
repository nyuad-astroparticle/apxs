#!/bin/bash

# Fixed source directory path
SRC_DIR="/scratch/ss14729/geant4/apxs/build/output" 
DEST_DIR="$SRC_DIR/latest_run_change_name"

# Create destination directory if it doesn’t exist
mkdir -p "$DEST_DIR"

# Move all .csv files (non-recursive) into the new directory
find "$SRC_DIR" -maxdepth 1 -type f -name "*.csv" -exec mv {} "$DEST_DIR" \;

# # Zip the new directory
# cd "$DEST_DIR" || exit
# zip "$SRC_DIR/$ZIP_NAME" *.csv


