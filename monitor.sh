#!/bin/bash

# AlphaSign32 Serial Monitor Script
# Usage: 
#   ./monitor.sh                    - Prompt for port selection
#   ./monitor.sh -p PORT            - Connect to PORT
#   ./monitor.sh --port PORT        - Connect to PORT

set -e  # Exit on error

# Configuration
FQBN="esp32:esp32:esp32"
BAUD_RATE=115200
PORT=""

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -p|--port)
      PORT="$2"
      shift 2
      ;;
    -b|--baud)
      BAUD_RATE="$2"
      shift 2
      ;;
    -h|--help)
      echo "Usage: $0 [OPTIONS]"
      echo ""
      echo "OPTIONS:"
      echo "  -p, --port PORT    Connect to specified port"
      echo "  -b, --baud RATE    Set baud rate (default: 115200)"
      echo "  -h, --help         Show this help message"
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

echo -e "${BLUE}=== AlphaSign32 Serial Monitor ===${NC}"

# If port not provided, prompt user to select
if [ -z "$PORT" ]; then
  echo -e "\n${BLUE}Available ports:${NC}"
  
  # Get list of ports (compatible with bash 3.2+)
  PORTS=()
  while IFS= read -r line; do
    PORTS+=("$line")
  done < <(arduino-cli board list | tail -n +2 | awk '{print $1}' | grep -v "^$")
  
  if [ ${#PORTS[@]} -eq 0 ]; then
    echo -e "${RED}No serial ports detected${NC}"
    echo -e "Connect your ESP32 and try again"
    exit 1
  fi
  
  # Display numbered list
  for i in "${!PORTS[@]}"; do
    printf "${GREEN}%d${NC}) %s\n" $((i+1)) "${PORTS[$i]}"
  done
  
  # Prompt for selection
  echo -e -n "\n${BLUE}Select port number [1-${#PORTS[@]}]: ${NC}"
  read -r selection
  
  if [ "$selection" -ge 1 ] && [ "$selection" -le "${#PORTS[@]}" ] 2>/dev/null; then
    PORT="${PORTS[$((selection-1))]}"
  else
    echo -e "${RED}Invalid selection${NC}"
    exit 1
  fi
fi

echo -e "\n${BLUE}Connecting to $PORT at ${BAUD_RATE} baud...${NC}"
echo -e "${YELLOW}Press Ctrl+C to exit${NC}\n"

# Use arduino-cli monitor
arduino-cli monitor --port "$PORT" --config "baudrate=$BAUD_RATE"
