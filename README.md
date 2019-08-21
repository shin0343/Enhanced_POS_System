###### POS System Using Cortex-M3

- - -
###------------------ 개발환경 세팅 ------------------
1. IAR IDE 설치
2. TeraTerm 설치
3. 장치 관리자를 통해 M3 보드를 Silicon COM4(~)로 설정
4. M3 보드 리셋
5. IAR에서 코드 작성 후 빌드(단축키 F7)
6. TeraTerm에서 File->Transfer->Send->Y 누르고 3번에서 만들어진 .bin파일로 M3 보드 write
- - -

- - -
###------------------ 시뮬레이션 ------------------
- 지점(서버)에는 주문을 받는 POS(클라이언트)가 1대 이상 있음
- 각각의 POS에서 주문 받은 정보를 서버에 전달함
- 매일 마감 시간에 서버에서 정산 후 발주량 결정
- 일간, 월간 별로 서버에서 주문량 조회 및 발주량 계산
- 매일 마감 시간마다 DB에 판매액, 사용 재료량 저장
- - -

- - -
###------------------ 프로젝트 개요 ------------------

 - ARM M3 프로세서를 사용하는 SoC를 가지고 진행한 IOT 프로젝트
 - POS 시스템을 M3 보드 이용하여 구현
 - M3 보드는 POS 역할(클라이언트), PC는 지점의 POS들 관리 역할(서버)
 - M3는 IAR C환경에서 TCP이용하여 클라이언트 구현
 - PC는 Visual C 환경에서 TCP를 이용하여 서버 구현
 - 다중 클라이언트 접속을 지원하며, 전송 데이터의 Pending 및 Loss를 방지하고자 서버는 Non-Blocking 소켓을 이용
- - -

- - -
###------------------ 개발 과정 주요사항 ------------------

 - ARM Cortex M3보드로 구현된 POS(클라이언트)에서 3가지 제품을 n개 주문하는 기능을 Key 인터럽트와 TCP프로토콜 활용한 WiFi통신 구현 및 디버깅
 - POS마다 해당 POS에서 주문한 정보를 M3 보드의 LCD에 출력
 - 서버에서 지원하는 기능들(부족한 재료 자동 발주, 판매 통계 계산 및 보관)을 별도로 정의(.h) 및 구현(.c)
 - 다중 클라이언트 접속을 지원하는 TCP 서버를 Visual C 환경에서 Non-Blocking 개발
 - 서버에서 마감 시간이 되면 필요 정보들을 파일 출력으로 txt파일(DB)에 저장하고, 연결된 모든 POS(클라이언트)에 OFF 메시지를 보냄
 - 마감 시간에 OFF 메시지를 받은 POS들은 하루동안 판매된 정보들을 초기화
 - PO(클라이언트)S를 보조하도록 M3 보드의 센서 활용하는 기능 구현
 - M3 보드의 가속도 센서를 이용하여, Z축의 센서 값이 음수가 되면 POS(클라이언트)가 넘어졌거나 크게 기울었다는 것이므로 M3 보드의 부저음을 0.7초마다 울리고, 서버에 POS FLIPPED 메시지 전송
 - M3 보드의 조도 센서를 이용하여, ADC 값이 특정 값 이하가 되면 구매자가 POS(클라이언트)에 접근한 것으로 취급하여, 이 경우에만 서버에 POS Customer Arrived 메시지 전송하고 보드의 LCD를 출력(구매자가 없을 땐 LCD Cleaer하여 OFF상태)
- - -