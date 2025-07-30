#!/bin/bash

# 추적할 포트 리스트
TARGET_PORTS=("12000" "13000" "14000")

# 각 포트별 누적 및 카운터 초기화
declare -A PORT_TOTAL
declare -A PORT_AVG
FOR_TIME=0

for port in "${TARGET_PORTS[@]}"; do
    PORT_TOTAL[$port]=0
done

# tcpdump 실행 (백그라운드)
sudo tcpdump -i lo udp -Q in -nnvv -l > ./udp.log &
TCPDUMP_PID=$!

# 종료 시 tcpdump도 같이 종료되도록 처리
trap "kill $TCPDUMP_PID; exit" INT TERM

# 무한 루프
while true; do
    sleep 1
    ((FOR_TIME++))

    echo -n "$(date +%H:%M:%S) - UDP 수신 패킷 수: "

    for port in "${TARGET_PORTS[@]}"; do
        # 목적지 포트가 있는 줄에서만 추출
	count=$(awk 'NR % 2 == 0' ./udp.log | grep -a -oE " > [0-9]+\.[0-9]+\.[0-9]+\.[0-9]+\.${port}:" | wc -l)
	# 누적합
        PORT_TOTAL[$port]=$(( PORT_TOTAL[$port] + count ))

        # 평균 (0 나누기 방지)
        if [ "$FOR_TIME" -gt 0 ]; then
            avg=$(( PORT_TOTAL[$port] / FOR_TIME ))
        else
            avg=0
        fi

        printf "port %s: %02d (avg: %02d/%d)  " "$port" "$count" "$avg" "${PORT_TOTAL[$port]}"
    done

    echo -e "\r"

    # 로그 초기화
    : > ./udp.log
done
