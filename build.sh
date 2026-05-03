#!/bin/bash

# AlphaSign32 Build and Upload Script
# Usage: 
#   ./build.sh                    - Compile only, prompt for upload
#   ./build.sh -p PORT            - Compile and upload to PORT
#   ./build.sh --port PORT        - Compile and upload to PORT
#   ./build.sh --no-upload        - Compile only, skip upload prompt
#   ./build.sh --export           - Export compiled binary to project directory
#   ./build.sh --export-dir DIR   - Export compiled binary to specific directory

set -e  # Exit on error

# Configuration
FQBN="esp32:esp32:esp32"
SKETCH="AlphaSign32.ino"
PORT=""
SKIP_UPLOAD=false
MONITOR=false
UPLOADED=false
EXPORT_BINARY=false
EXPORT_DIR=""

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -p|--port)
      PORT="$2"
      shift 2
      ;;
    --no-upload)
      SKIP_UPLOAD=true
      shift
      ;;
    -m|--monitor)
      MONITOR=true
      shift
      ;;
    --export)
      EXPORT_BINARY=true
      shift
      ;;
    --export-dir)
      EXPORT_BINARY=true
      EXPORT_DIR="$2"
      shift 2
      ;;
    -h|--help)
      echo "Usage: $0 [OPTIONS]"
      echo ""
      echo "OPTIONS:"
      echo "  -p, --port PORT       Upload to specified port"
      echo "  -m, --monitor         Start serial monitor after upload"
      echo "  --no-upload           Skip upload prompt"
      echo "  --export              Export .bin file to project directory"
      echo "  --export-dir DIR      Export .bin file to specific directory"
      echo "  -h, --help            Show this help message"
      exit 0
      ;;
    *)
      echo "Unknown option: $1"
      echo "Use -h for help"
      exit 1
      ;;
  esac
done

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== AlphaSign32 Build Script ===${NC}"

# Get project directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Generate config.h from .env
echo -e "\n${BLUE}Generating configuration...${NC}"

# Function to read env file and export variables
load_env_file() {
  local env_file="$1"
  while IFS='=' read -r key value || [ -n "$key" ]; do
    # Skip empty lines and comments
    [[ -z "$key" || "$key" =~ ^#.* ]] && continue
    # Remove leading/trailing whitespace
    key=$(echo "$key" | xargs)
    value=$(echo "$value" | xargs)
    # Export variable
    export "$key=$value"
  done < "$env_file"
}

# Always load defaults from .env.example first
if [ -f "$SCRIPT_DIR/.env.example" ]; then
  echo "Loading defaults from .env.example"
  load_env_file "$SCRIPT_DIR/.env.example"
else
  echo -e "${RED}Error: .env.example file not found${NC}"
  exit 1
fi

# Then override with .env if it exists
if [ -f "$SCRIPT_DIR/.env" ]; then
  echo "Loading custom configuration from .env"
  load_env_file "$SCRIPT_DIR/.env"
else
  echo "No custom .env found, using defaults"
fi

# Generate config.h from template
if [ -f "$SCRIPT_DIR/config.h.template" ]; then
  echo "Generating config.h from template"
  
  # Read template and replace variables
  template_content=$(<"$SCRIPT_DIR/config.h.template")
  
  # Replace all ${VAR} placeholders with environment variable values
  output_content="$template_content"
  for var in ALPHASIGN_RX_PIN ALPHASIGN_TX_PIN POWER_CONTROL_PIN DEBUG_SERIAL_BAUD ENABLE_DEBUG MONITOR_INTERVAL_MS SERIAL_TIMEOUT_MS WIFI_SSID WIFI_PASSWORD OTA_HOSTNAME; do
    value="${!var}"
    output_content="${output_content//\$\{$var\}/$value}"
  done
  
  # Write to config.h
  echo "$output_content" > "$SCRIPT_DIR/config.h"
  echo -e "${GREEN}✓ Configuration generated successfully${NC}"
else
  echo -e "${RED}Error: config.h.template not found${NC}"
  exit 1
fi

# Compile
echo -e "\n${BLUE}Compiling...${NC}"
BUILD_PATH="$SCRIPT_DIR/build"
mkdir -p "$BUILD_PATH"
arduino-cli compile \
  --fqbn "$FQBN" \
  --build-path "$BUILD_PATH" \
  --build-property "compiler.cpp.extra_flags=-I$SCRIPT_DIR/src" \
  "$SKETCH"

if [ $? -eq 0 ]; then
  echo -e "${GREEN}✓ Compilation successful!${NC}"
else
  echo -e "${RED}✗ Compilation failed!${NC}"
  exit 1
fi

# Export binary if requested
if [ "$EXPORT_BINARY" = true ]; then
  echo -e "\n${BLUE}Exporting compiled binary...${NC}"
  
  # Find the compiled .bin file
  BIN_FILE="$BUILD_PATH/${SKETCH%.ino}.ino.bin"
  
  if [ ! -f "$BIN_FILE" ]; then
    echo -e "${RED}✗ Binary file not found: $BIN_FILE${NC}"
    exit 1
  fi
  
  # Determine export destination
  if [ -z "$EXPORT_DIR" ]; then
    EXPORT_DIR="$BUILD_PATH"
  fi
  
  # Create export directory if it doesn't exist
  mkdir -p "$EXPORT_DIR"
  
  # Generate timestamp for filename
  TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
  EXPORT_NAME="AlphaSign32_${TIMESTAMP}.bin"
  EXPORT_PATH="$EXPORT_DIR/$EXPORT_NAME"
  
  # Copy the binary
  cp "$BIN_FILE" "$EXPORT_PATH"
  
  if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Binary exported to: $EXPORT_PATH${NC}"
    
    # Also create a symlink to latest version
    LATEST_LINK="$EXPORT_DIR/AlphaSign32_latest.bin"
    ln -sf "$EXPORT_NAME" "$LATEST_LINK"
    echo -e "${GREEN}✓ Latest version linked: $LATEST_LINK${NC}"
    
    # Show file size
    SIZE=$(ls -lh "$EXPORT_PATH" | awk '{print $5}')
    echo -e "${BLUE}  Binary size: $SIZE${NC}"
  else
    echo -e "${RED}✗ Failed to export binary${NC}"
    exit 1
  fi
fi

# Upload if port is provided
if [ -n "$PORT" ]; then
  echo -e "\n${BLUE}Uploading to $PORT...${NC}"
  arduino-cli upload \
    --fqbn "$FQBN" \
    --port "$PORT" \
    "$SKETCH"
  
  if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Upload successful!${NC}"
    UPLOADED=true
  else
    echo -e "${RED}✗ Upload failed!${NC}"
    exit 1
  fi
elif [ "$SKIP_UPLOAD" = true ]; then
  echo -e "\n${BLUE}Skipping upload as requested${NC}"
else
  # Prompt user to select a port
  echo -e "\n${BLUE}Available ports:${NC}"
  
  # Get list of ports (compatible with bash 3.2+)
  PORTS=()
  while IFS= read -r line; do
    PORTS+=("$line")
  done < <(arduino-cli board list | tail -n +2 | awk '{print $1}' | grep -v "^$")
  
  if [ ${#PORTS[@]} -eq 0 ]; then
    echo -e "${YELLOW}No serial ports detected${NC}"
    echo -e "Connect your ESP32 and try again, or use ${GREEN}./build.sh --no-upload${NC}"
    exit 0
  fi
  
  # Display numbered list
  for i in "${!PORTS[@]}"; do
    printf "${GREEN}%d${NC}) %s\n" $((i+1)) "${PORTS[$i]}"
  done
  echo -e "${GREEN}0${NC}) Skip upload"
  
  # Prompt for selection
  echo -e -n "\n${BLUE}Select port number [0-${#PORTS[@]}]: ${NC}"
  read -r selection
  
  if [ "$selection" = "0" ]; then
    echo -e "${BLUE}Skipping upload${NC}"
    exit 0
  elif [ "$selection" -ge 1 ] && [ "$selection" -le "${#PORTS[@]}" ] 2>/dev/null; then
    PORT="${PORTS[$((selection-1))]}"
    echo -e "\n${BLUE}Uploading to $PORT...${NC}"
    arduino-cli upload \
      --fqbn "$FQBN" \
      --port "$PORT" \
      "$SKETCH"
    
    if [ $? -eq 0 ]; then
      echo -e "${GREEN}✓ Upload successful!${NC}"
      UPLOADED=true
    else
      echo -e "${RED}✗ Upload failed!${NC}"
      exit 1
    fi
  else
    echo -e "${RED}Invalid selection${NC}"
    exit 1
  fi
fi

# Start monitor if requested and upload was successful
if [ "$UPLOADED" = true ]; then
  if [ "$MONITOR" = true ]; then
    echo -e "\n${BLUE}Starting serial monitor...${NC}"
    exec "$SCRIPT_DIR/monitor.sh" --port "$PORT"
  else
    # Prompt to start monitor
    echo -e -n "\n${BLUE}Start serial monitor? [y/N]: ${NC}"
    read -r answer
    if [[ "$answer" =~ ^[Yy]$ ]]; then
      exec "$SCRIPT_DIR/monitor.sh" --port "$PORT"
    fi
  fi
fi

echo -e "\n${GREEN}Done!${NC}"
