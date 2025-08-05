# 🐮 온습도 관리를 위한 스마트 축사

## 개요
이상기후로 이내 가축의 폐사가 급격히 증가,
가축의 폐사가 곧 생산량 저하로 이어짐

이를 방지하고자 적절한 온습도를 제어하는 축사를 구축하고자 함,
가축에게 쾌적한 환경을 제공하고 생산량 증가 기대



- 개발기간 : 3주
- 팀장 : 이종희
- 팀원 : 김민성, 오준택, 이윤성

## 프로젝트 핵심 기능
- 내외부 온습도 센서 제어
- 블루투스 통신을 통한 STM32, 라즈베리파이4 데이터 송수신 및 데이터 파싱
- 실내외 온도에 따라 FAN 제어
- 실내 습도에 따라 가습기 ON/OFF
- GUI 구현

## 하드웨어
<img width="400" alt="image" src="https://github.com/user-attachments/assets/ff234533-fa1e-4f53-81cb-4a3e72714f10" />

### 보드 및 칩
STM32F411RE(ARM Cortex-M4), 라즈베리파이4B+ , 시프트레지스터칩(SN74H595N)

### 송수신
- 블루투스 모듈(HC-06)

### 정보 표현
- LCD(LCD1602), 8x8 도트매트릭스(1088BS)
  
### 온습도 데이터 수집
- 온습도 센서(DHT11)
  
### 온습도 값에 의한 동작 센서
- 가습기 모듈, FAN(EZ모터 300), RGB LED_제습, 히터 역할 대체(SZH-EK058)



## USECASE
<img width="400" alt="image" src="https://github.com/user-attachments/assets/8ef71d1b-6525-4486-b611-866c25614168" />

### 데이터 수신 및 파싱
<img width="200" alt="image" src="https://github.com/user-attachments/assets/81496d85-5d1b-4e95-ad5e-47c246ef0fcc" />



## Flow Chart
<img width="1000" alt="image" src="https://github.com/user-attachments/assets/8286215e-5a55-403e-a16a-0b6217649674" />



## 테스트
유튜브 링크 : https://youtu.be/lQedsSYqslA
![스마트축사_readme용](https://github.com/user-attachments/assets/866fb942-42ba-4ab1-b665-774e15bbe141)


<img width="400" alt="image" src="https://github.com/user-attachments/assets/6859aabb-3117-479b-af87-1067b29d99ed" />
<img width="400" alt="image" src="https://github.com/user-attachments/assets/f67c8c0f-f8d2-45f1-affd-1cbfc44b7652" />

