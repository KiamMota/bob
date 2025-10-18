# quick_parallel_curl.sh
# lança TOTAL requisições curl (concurrentes em lotes) e grava resultados em results.log
#!/usr/bin/env bash
URL="${1:-http://127.0.0.1:7192/}"   # passe URL como primeiro argumento
TOTAL="${2:-1000}"                   # total de conexões a simular
CONCURRENCY="${3:-200}"              # quantas concorrentes ao mesmo tempo
OUTFILE="${4:-results.log}"

: > "$OUTFILE"

seq 1 "$TOTAL" | xargs -n1 -P "$CONCURRENCY" -I{} \
  sh -c 'curl --http1.1 --silent --show-error --max-time 10 --connect-timeout 5 -o /dev/null -w "%{http_code} %{time_total}s\n" "'"$URL"'" 2>/dev/null' \
  >> "$OUTFILE"

