#!/usr/bin/env bash
set -euo pipefail

VECTOR_SIZE="${1:-100000000}"
RUNS_PER_TEST=5
THREAD_COUNTS=(1 2 4 6 8 10 12 16 24 32 64 128 256)

make -s

printf "%-8s %-12s %-12s %-12s %-12s\n" "Threads" "Tempo_Min" "Tempo_Max" "Tempo_Médio" "Média_f(x)"
printf "%-8s %-12s %-12s %-12s %-12s\n" "-------" "---------" "---------" "-----------" "---------"

declare -A best_times
min_time=999999
best_thread_count=1

for threads in "${THREAD_COUNTS[@]}"; do
    times=()
    avg_fx=""
    
    for ((run=1; run<=RUNS_PER_TEST; run++)); do
        output=$(./bin/threads_parte1 "$threads" "$VECTOR_SIZE" 2>/dev/null | grep "threads=" || echo "threads=$threads elementos=$VECTOR_SIZE media=0.000000000000 tempo=999.999 s")
        
        time_val=$(echo "$output" | sed -n 's/.*tempo=\([0-9.]*\) s.*/\1/p')
        if [ -z "$time_val" ]; then
            time_val="999.999"
        fi
        
        if [ -z "$avg_fx" ]; then
            avg_fx=$(echo "$output" | sed -n 's/.*media=\([0-9.]*\).*/\1/p')
        fi
        
        times+=("$time_val")
        
        if (( $(echo "$time_val < $min_time" | bc -l) )); then
            min_time="$time_val"
            best_thread_count="$threads"
        fi
    done
    
    min_time_thread=$(printf '%s\n' "${times[@]}" | sort -n | head -1)
    max_time_thread=$(printf '%s\n' "${times[@]}" | sort -n | tail -1)
    
    sum=0
    for time in "${times[@]}"; do
        sum=$(echo "$sum + $time" | bc -l)
    done
    avg_time=$(echo "scale=6; $sum / $RUNS_PER_TEST" | bc -l)
    
    best_times[$threads]="$min_time_thread"
    
    printf "%-8s %-12s %-12s %-12s %-12s\n" "$threads" "$min_time_thread" "$max_time_thread" "$avg_time" "$avg_fx"
done

echo ""
echo "1. Valor médio de f(xi): $(echo "${best_times[1]}" | head -1)"
echo "2. Menor tempo de execução: ${min_time} segundos"
echo "3. Número de threads com menor tempo: $best_thread_count threads"

prev_time=""
stop_improving_at=""
for threads in "${THREAD_COUNTS[@]}"; do
    current_time="${best_times[$threads]}"
    if [ -n "$prev_time" ]; then
        if (( $(echo "$current_time >= $prev_time" | bc -l) )); then
            if [ -z "$stop_improving_at" ]; then
                stop_improving_at="$threads"
            fi
        fi
    fi
    prev_time="$current_time"
done

if [ -n "$stop_improving_at" ]; then
    echo "4. O tempo deixa de diminuir a partir de $stop_improving_at threads"
else
    echo "4. O tempo continua diminuindo até o limite testado"
fi

if command -v nproc &> /dev/null; then
    cores=$(nproc)
    echo "5. Número de núcleos: $cores"
else
    echo "5. Execute 'nproc' para verificar número de núcleos"
fi
