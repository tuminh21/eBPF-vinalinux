#!/bin/bash
# trace_containers.sh — Start 2 Docker containers and trace their file I/O
#
# Usage: sudo ./trace_containers.sh
#
# This script:
#   1. Starts container-a and container-b via docker compose
#   2. Launches the eBPF tracer in --docker mode
#   3. You exec into the containers from other terminals to trigger events
#
# Example (from other terminals):
#   docker exec -it container-a bash -c "cat /etc/hostname; echo hi > /tmp/test"
#   docker exec -it container-b bash -c "ls /usr; echo world > /tmp/test2"

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== eBPF Docker Container File I/O Tracer ==="
echo ""

# Step 1: Start containers
echo "[1/3] Starting Docker containers..."
docker compose up -d 2>/dev/null || docker-compose up -d 2>/dev/null
echo ""

# Verify containers are running
for name in container-a container-b; do
    pid=$(docker inspect -f '{{.State.Pid}}' "$name" 2>/dev/null || echo "0")
    if [ "$pid" = "0" ] || [ -z "$pid" ]; then
        echo "ERROR: Container '$name' is not running!"
        echo "  Try: docker compose up -d"
        exit 1
    fi
    echo "  $name → host PID $pid"
done
echo ""

# Step 2: Show instructions
echo "[2/3] Containers are ready. Open other terminals and run:"
echo ""
echo "  docker exec -it container-a bash"
echo "  docker exec -it container-b bash"
echo ""
echo "  Then do file operations (cat, ls, echo >, etc.)"
echo "  Events will appear below tagged by container name."
echo ""

# Step 3: Start the tracer
echo "[3/3] Starting tracer..."
echo ""
exec "$SCRIPT_DIR/open_trace" --docker container-a container-b
