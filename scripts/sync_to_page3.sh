#!/usr/bin/env bash
set -euo pipefail

# Sync local source folders into page3 before publishing.
# Usage:
#   sync_to_page3.sh [--dry-run] [--delete]

DRY_RUN=0
DELETE=0

for arg in "$@"; do
  case "$arg" in
    --dry-run) DRY_RUN=1 ;;
    --delete) DELETE=1 ;;
    -h|--help)
      echo "Usage: $0 [--dry-run] [--delete]"
      exit 0
      ;;
    *)
      echo "Unknown arg: $arg" >&2
      exit 1
      ;;
  esac
done

BASE_DIR="/home/merv/Public/ai/page3"
CONF_FILE="$BASE_DIR/scripts/sync_sources.conf"
LOG_FILE="$BASE_DIR/scripts/sync_to_page3.log"

if [ ! -f "$CONF_FILE" ]; then
  echo "Missing config: $CONF_FILE" >&2
  exit 1
fi

RSYNC_FLAGS=("-a" "--info=stats2")
if [ "$DRY_RUN" -eq 1 ]; then
  RSYNC_FLAGS+=("--dry-run")
fi
if [ "$DELETE" -eq 1 ]; then
  RSYNC_FLAGS+=("--delete")
fi

: > "$LOG_FILE"

while IFS='|' read -r SRC DEST; do
  # Skip comments/blank lines
  [ -z "${SRC// }" ] && continue
  [[ "$SRC" =~ ^# ]] && continue

  SRC_TRIM=$(echo "$SRC" | xargs)
  DEST_TRIM=$(echo "$DEST" | xargs)

  if [ ! -d "$SRC_TRIM" ]; then
    echo "[SKIP] Source not found: $SRC_TRIM" | tee -a "$LOG_FILE"
    continue
  fi

  DEST_PATH="$BASE_DIR/$DEST_TRIM"
  mkdir -p "$DEST_PATH"

  echo "[SYNC] $SRC_TRIM -> $DEST_PATH" | tee -a "$LOG_FILE"
  rsync "${RSYNC_FLAGS[@]}" "$SRC_TRIM/" "$DEST_PATH/" | tee -a "$LOG_FILE"
  echo "" | tee -a "$LOG_FILE"

done < "$CONF_FILE"

echo "Done. Log: $LOG_FILE"
