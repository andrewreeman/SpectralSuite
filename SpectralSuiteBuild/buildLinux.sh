#!/bin/bash

# Exit immediately on any error
set -e 

PROJECT_ROOT="$(pwd)"
OUTPUT_DIR="$PROJECT_ROOT/linux-x86_64"
RELEASE_DIR="$PROJECT_ROOT/../release"

GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

info() { echo -e "${BLUE}ℹ️ [INFO]${NC} $1"; }
success() { echo -e "${GREEN}✅ [SUCCESS]${NC} $1"; }

START_TIME=$(date +%s)

info "Building Docker image... 🐳"
cd ..
docker build . -t ss:latest
cd - > /dev/null

info "Preparing output directory... 📂"
rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"

info "Running Docker container and copying artifacts... 🚀"
docker run --rm -v "$PROJECT_ROOT:/output" ss:latest \
  sh -c "cp -r /ss/build-linux/artefacts/* /output/linux-x86_64"

info "Moving artifacts to release directory... 📦"
mkdir -p "$RELEASE_DIR"
rm -rf "$RELEASE_DIR/linux-x86_64"
mv "$OUTPUT_DIR" "$RELEASE_DIR/"

END_TIME=$(date +%s)
ELAPSED_TIME=$((END_TIME - START_TIME))

info "⏰ Execution time: ${ELAPSED_TIME}s"
success "Build and release process completed successfully! 🎉"

