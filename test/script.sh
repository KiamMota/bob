#!/usr/bin/env bash
URL="${1:-http://127.0.0.1:7192/}"
TOTAL="${2:-1000}"
CONCURRENCY="${3:-200}"
OUTFILE="${4:-results.log}"

: > "$OUTFILE"

start_ns=$(date +%s%N)

seq 1 "$TOTAL" | xargs -n1 -P "$CONCURRENCY" -I{} bash -c '
tmp=$(mktemp) || tmp="/tmp/quickcurl.$$.$RANDOM"
out=$(curl --http1.1 --silent --show-error --max-time 10 --connect-timeout 5 -o "$tmp" -w "%{http_code} %{time_total}s" "'"$URL"'" 2>/dev/null || true)
cat "$tmp"
printf "%s %s\n" "{}" "$out"
rm -f "$tmp"
' \
>> "$OUTFILE"

end_ns=$(date +%s%N)
elapsed_ns=$((end_ns - start_ns))
elapsed_sec=$(awk -v ns="$elapsed_ns" 'BEGIN{printf "%.3f", ns/1e9}')
printf "TOTAL_TIME %s\n" "$elapsed_sec" | tee -a "$OUTFILE"


