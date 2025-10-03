#!/usr/bin/env bash
set -euo pipefail

VECTOR_SIZE="${1:-100000000}"
RUNS_PER_TEST=3
THREAD_COUNTS=(1 2 4 6 8 10 12 16 24 32 64 128 256)

make -C .. parte1 -s

declare -A best_times
min_time=999999
best_thread_count=1

for threads in "${THREAD_COUNTS[@]}"; do
    times=()
    
    for ((run=1; run<=RUNS_PER_TEST; run++)); do
        output=$(../bin/threads_parte1 "$threads" "$VECTOR_SIZE" 2>/dev/null | grep "threads=" || echo "threads=$threads elementos=$VECTOR_SIZE media=0.000000000000 tempo=999.999 s")
        time_val=$(echo "$output" | sed -n 's/.*tempo=\([0-9.]*\) s.*/\1/p')
        if [ -z "$time_val" ]; then time_val="999.999"; fi
        times+=("$time_val")
        if (( $(echo "$time_val < $min_time" | bc -l) )); then
            min_time="$time_val"
            best_thread_count="$threads"
        fi
    done
    
    min_time_thread=$(printf '%s\n' "${times[@]}" | sort -n | head -1)
    best_times[$threads]="$min_time_thread"
    echo "$threads threads: ${min_time_thread}s"
done

echo ""
echo "Menor tempo: ${min_time}s com $best_thread_count threads"

if command -v nproc &> /dev/null; then
    echo "Núcleos: $(nproc)"
fi
