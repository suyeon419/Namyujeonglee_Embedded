# 옴짝달싹

본 작품은 사용자 ~~~~ 구현을 목표로 함.

# ✅ 프로젝트 전체 기능

- 모터 제어 : 버튼 입력을 통해 일정 시간 모터의 정방향/역방향 회전과 작동을 제어한다.
- RGB RED 점멸 : 모터의 상태에 따라 RGB LED 색상을 변경한다.
- 초음파 측정: 사용자가 자리에 있는지 판단해 모터를 일시정지하거나 작동한다.
- 무드등 제어: 주변 빛의 밝기에 따라 무드등 밝기를 조절한다.


# 자세한 기능 설명

## 1. 모터 제어

- 버튼 입력 기반 제어
    - 버튼을 누르면 각 버튼마다 정해진 일정 시간 동안 모터가 정방향으로 회전한다.
    - 동일 버튼을 다시 누르면 모터가 정지하고 역방향으로 진행된 거리만큼 회전한다.
    - 버튼에 따라 회전 시간이 다르게 설정되어, 다른 속도로 작동 가능하다.
- 공부-휴식 시간 모드
    - 공부 시간 : 모터가 설정된 시간 동안 회전해 총 3번 반복한다.
    - 휴식 시간 : 공부 시간이 종료된 후, 모터가 역방향으로 10분간 회전하며 휴식이 진행된다.
- Reverse 타이머 구현
    - 역방향 회전 시, 버튼을 통해 누적된 정방향 회전 스텝만큼 역방향으로 복구한다.
    - 휴식 시간 동안 역방향으로 돌아갈 때는 10분의 시간 동안 한 바퀴 회전한다.
- 강제 종료 구현
    - 타이머 실행 시 눌렀던 버튼을 다시 누르면 모터를 강제 종료할 수 있다.
    - 종료 시, 진행 중이던 상태를 초기화하고 역방향 회전으로 기존 상태로 돌아간다.
- 사이클 구현
    - 공부와 휴식을 1사이클로 동작한다.
    - 공부가 정방향으로 진행되고 끝나면, 휴식 시간이 역방향으로 진행된다.

## 2. 초음파 측정

- 시작 시 사용자 거리 측정
    - 기기 작동 시작 시 기기와 사용자 간의 거리를 측정해 기준 거리를 구한다.
- 사용자 거리 측정
    - 현재 거리와 기준 거리의 차이로 사용자가 자리에 있는지 없는 지를 판별한다.
- 일시 정지
    - 사용자가 자리에 없다 판단되면 모터 작동을 일시 정지해 시간이 가지 않도록 한다.
    - 사용자가 자리에 있으면 시간이 정상적으로 간다.
    - 쉬는 시간에는 사용자의 자리 유무를 작용하지 않는다.

## 3. RGB LED 제어

- RGB LED로 각 시간의 종료를 알림
    - 공부 종료 : 파란색 LED가 세 번 점멸하여 종료를 알린다.
    - 휴식 종료 : 분홍색 LED가 세 번 점멸하여 종료를 알린다.
- LED 점멸 구현
    - softPwmWrite를 사용해 LED 밝기를 조절하며 점멸 간격을 500ms로 설정했다.
    - 각 점멸은 3번 반복되어 상태를 알린다.

## 4. 무드등 기능

- 빛 측정
    - 조도 센서를 통하여 주변 빛의 밝기를 구한다.
- LED 제어
    - 주변 빛이 어두우면 LED를 밝히고, 주변 빛이 어두우면 LED를 어둡게 한다.

## 5. 멀티쓰레드 및 뮤텍스

- 멀티쓰레드 구현
    - switchThread : 버튼 입력을 감지하고 이에 따라 공유 자원을 변경해 모터 동작을 제어한다.
    - motorThread : 모터의 정방향/ 역방향 회전 및 공부-휴식 사이클 담당한다.
- 뮤텍스 사용
    - 공용 자원(motor_time, steps_run, isRun, isReverse 등) 접근 시 동기화를 위해 뮤텍스를 사용한다.
    - 여러 스레드가 동시에 자원에 접근할 때 발생할 수 있는 충돌을 방지한다.

---


# 👨‍🏫 하드웨어 구성


![Frame 191 (1)](https://github.com/user-attachments/assets/3f92ae35-6ffe-4b38-b1aa-0e05885d6056)


**타이머의 내부 배치도는 프로젝트의 구성 요소와 배치를 시각적으로 나타낸다.** 

&nbsp;각 부품은 다음과 같은 역할을 수행한다. 각 층을 명확히 구분하기 위해 부품을 안정적으로 지탱할 수 있는 바닥이 설계되어 있다. 
 
&nbsp;1층에는 차례대로 라즈베리파이가 배치된다. 전체 시스템의 중앙 처리 장치로, 다른 구성 요소들의 제어를 담당한다. 오른쪽 하단에 전원 단자의 홈이 존재한다. 이는 라즈베리파이 시스템에 전원을 공급하는 역할을 한다. 
 
&nbsp;2층에는 브레드보드가 배치되며, 각 부품의 회로 연결을 담당한다. 보드에 연결되는 내부 구성 요소로 스텝모터, 스텝모터 드라이버, RGB LED, 조도센서, YL-40모듈, 초음파센서가 있다. 스텝모터는 타이머의 핵심 부품으로 시간이 흐르는 것을 사용자에게 시각적으로 알리기 위해 이미지와 같이 동그란 홈을 통해 외부로 돌출된다. 점멸용 RGB LED도 스텝모터와 동일한 방식으로 디자인되어 있으며, 사용자가 공부 시간과 쉬는 시간을 명확히 구별하고, 타이머의 시간이 흐르는 것을 시각적으로 인지할 수 있도록 하는 데 중점을 두었다. 무드등용 RGB LED는 타이머 상단에 배치되는 조도센서의 외부 밝기 측정 값에 따라 적절한 밝기를 유지하도록 한다. 조도 센서와 초음파 센서는 선을 길게 빼서 상단에 위치시켜 현재 공간의 빛과 사용자와의 거리를 파악할 수 있도록 하였다.
 
&nbsp;3층에도 브레드보드가 배치되며, 시간을 입력 받는 버튼이 있다. 이 버튼은 각각 25분, 35분, 45분으로 설정 가능하도록 하였다.
 
&nbsp;타이머의 뒷면은 투명한 아크릴로 제작되어 무드등의 빛이 잘 보이도록 디자인되었다. 또한, 사용자가 쉽게 조작할 수 있도록 위에서 아래로 슬라이드할 수 있는 홈이 마련되어 있어, 부품을 쉽게 빼고 넣을 수 있도록 설계되었다.

![Frame 192 (1)](https://github.com/user-attachments/assets/d0f5b168-cd6b-4b61-b3d1-9f1065586a56)

**다음은 타이머의 상단 배치도로 각 부품의 구성 요소와 배치를 시각적으로 나타낸다.**

&nbsp;조도센서는 왼쪽에 위치하며, 주변의 밝기를 감지하여 무드등의 밝기를 자동으로 조절하는 역할을 한다. 이미지와 같이 디자인한 이유는 조도 센서를 안정적으로 고정시키는 역할을 하며, 외부 밝기를 측정하는 데 있어 오차를 최소화하기 위해 디자인했다. 
 
&nbsp;초음파 센서는 앞쪽에 배치되어 있으며, 사용자가 자리에 앉아있는지 판별할 때 사용된다. 구멍은 초음파 센서를 안정적으로 고정시키는 역할을 하며, 센서와 연결된 전선을 외부로 쉽게 빼낼 수 있도록 디자인되었다. 시간 조절용 스위치는 중앙 하단에 위치하며, 사용자가 타이머의 시간을 조절할 수 있는 버튼이다. 브레드보드에 연결된 버튼이 돌출될 수 있도록 구멍이 설계되어 있으며, 간편한 조작을 통해 원하는 공부 시간을 쉽게 설정할 수 있다. 


![Frame 193 (1)](https://github.com/user-attachments/assets/e38c0c73-41ca-4788-989e-63f34809528f)

**초음파센서의 파츠 디자인이다.**

&nbsp;초음파센서가 보다 안정적으로 배치될 수 있도록 부품 사이즈에 맞게 디자인되어 있다.

![Frame 194](https://github.com/user-attachments/assets/3505b28b-9d22-48c0-b468-babaf5525574)

**스텝모터에 시계바늘 파츠 디자인이다.**

&nbsp;타이머 컨셉에 맞게 시계바늘 파츠로 모터의 돌출 부분과 사이즈가 맞도록 디자인되어 있다.

---

## 사용한 액추에이터 및 센서

![RGB LED](https://github.com/user-attachments/assets/471e4198-2b71-48e8-948a-9fb57b35e25a)
![스텝 모터](https://github.com/user-attachments/assets/f820cee6-50d7-4b7d-8ac7-d0d683e9e57b)

&nbsp;액추에이터는 RGB LED 3개와 스텝모터를 사용하였다. RGB LED중 2개는 무드등용으로 사용하고 1개는 점멸용으로 사용하였다. 스텝모터는 타이머의 시계바늘을 돌리기 위해 사용하였다.

![초음파 센서](https://github.com/user-attachments/assets/3f5330a1-d7e1-4292-8486-06c4b3b3d18c)
![조도 센서](https://github.com/user-attachments/assets/c0f0fec4-b3a6-4217-85e2-cd201c923a58)

&nbsp;센서는 초음파 센서와 조도 센서를 사용하였다. 초음파 센서는 사람이 앞에 있는 지 확인하기 위해 사용하였고 조도 센서는 현재 공간의 밝기를 확인하기 위해 사용하였다.

![버튼](https://github.com/user-attachments/assets/52103e50-5a45-4f7c-a698-f46922f1425b)
![저항](https://github.com/user-attachments/assets/b860af45-4968-43e0-a77b-5c3ff4b22d36)
![YL-40모듈](https://github.com/user-attachments/assets/0f095a0e-08a9-480d-8386-21657b2e1f5b)
![스템모터 드라이버](https://github.com/user-attachments/assets/4599dc88-7950-4e2e-b53e-73fa6b245a77)

&nbsp;이 외에는 버튼 3개, 저항 7개, YL-40모듈 그리고 스텝 모터 드라이버를 사용하였다. 버튼 3개는 타이머의 시간을 지정하기 위해 사용하였다. 저창은 초음파 센서에 1KΩ을 3개, 버튼에 10KΩ을 각 1개씩, 조도 센서에 10KΩ을 1개 사용하였다. YL-40 모듈은 외부 조도 센서를 사용하기 위해 사용하였다. 스텝 모터 드라이버는 스텝모터를 사용하기 위해 사용하였다.

## 회로 구성

![회로 구성](https://github.com/user-attachments/assets/1638c8c6-d07c-4f13-8642-3b4d1d534ada)


### 회로도

![조도 회로](https://github.com/user-attachments/assets/e1e8810e-62a5-4b30-a05e-bb8120d973bd)

**YL-40 모듈 및 조도 센서**

![버튼 회로](https://github.com/user-attachments/assets/edd5176d-139f-4f81-ae36-5556996a4047)

**버튼**

![무드등 회로](https://github.com/user-attachments/assets/583bcf8b-c4b5-41c8-94cc-38e51b377abb)

**RGB LED(무드등용)**

![점멸 회로](https://github.com/user-attachments/assets/f1a87311-4bb2-4e6c-a782-cff492fe14fe)

**RGB LED(점멸용)**

![스텝 모터 회로](https://github.com/user-attachments/assets/a66b2c10-40b6-44fc-84d1-d6c452421570)

**스텝 모터 드라이버 및 스텝 모터**

![초음파 회로](https://github.com/user-attachments/assets/ffe278d8-e1cc-4526-8a99-f856b46e3ff1)

**초음파 센서**


# 💻 소프트웨어 코드 구성

## SwitchThread

<details>
<summary>코드 보기</summary>

```c
void* switchThread(void* arg) {
    while (1) {
        for (int i = 0; i < BTN_COUNT; i++) {
            if (digitalRead(btn_gpio[i]) == LOW) {
                delay(200);
                if (digitalRead(btn_gpio[i]) == HIGH) {
                    printf("Button pressed: %d\n", btn_gpio[i]);
                    
                    if (!isRun) {
                        pthread_mutex_lock(&mid);
                        motor_time = 25 + (i * 10);
                        motor_time = motor_time * 60; // 실제 시간 용임(데모는 주석처리)
                        isRun = true;
                        btn_state[i] = true;
                        active_btn = i;
                        steps_run = 0;
                        pthread_mutex_unlock(&mid);
                    }
                    else if (active_btn == i && !isReverse) {
                        pthread_mutex_lock(&mid);
                        isRun = false;
                        printf("Steps completed: %d\n", steps_run);
                        pthread_mutex_unlock(&mid);
                        reverseMotor(steps_run);
                        pthread_mutex_lock(&mid);
                        motor_time = 0;
                        steps_run = 0;
                        active_btn = -1;
                        btn_state[i] = false;
                        pthread_mutex_unlock(&mid);
                    }
                    delay(1000);
                }
            }
        }
    }
    return NULL;
}
```

</details>

- 설명  

버튼 입력을 지속적으로 감지하고, 버튼 상태에 따라 모터의 회전 방향을 제어합니다.


#### 동작 흐름  

1. **버튼 상태 확인**  

```c
for (int i = 0; i < BTN_COUNT; i++) {
    if (digitalRead(btn_gpio[i]) == LOW) {
        delay(200);
        if (digitalRead(btn_gpio[i]) == HIGH) {
            printf("Button pressed: %d\n", btn_gpio[i]);
```

- `for` 루프를 통해 `BTN_COUNT(3)` 만큼 반복하며 모든 버튼 상태를 확인합니다.  
- `digitalRead(btn_gpio[i])`로 현재 버튼의 입력 신호를 읽습니다.  
- `LOW` 상태 후 신호를 지연시키고, 이후 `HIGH` 상태를 확인해 버튼이 눌렸는지 검증합니다.  

2. **버튼 이벤트 처리**  

(1) **모터가 동작 중이 아닐 때**  

```c
if (!isRun) {
    pthread_mutex_lock(&mid);
    motor_time = 25 + (i * 10);
    motor_time = motor_time * 60;
    isRun = true;
    btn_state[i] = true;
    active_btn = i;
    steps_run = 0;
    pthread_mutex_unlock(&mid);
}
```

- 버튼에 따라 `motor_time`을 설정합니다.  
  - 예: 버튼 0 → 25분, 버튼 1 → 35분, 버튼 2 → 45분.  
- **상태 플래그 업데이트**  
  - `isRun → True`: 모터가 동작 중임.  
  - `btn_state[i] → True`: 어떤 버튼이 활성화되었는지 저장.  
  - `active_btn`: 눌린 버튼의 인덱스를 저장.  
- **뮤텍스 사용**으로 공유 자원을 보호합니다.  

(2) **동일한 버튼을 다시 눌렀을 때**  

```c
else if (active_btn == i && !isReverse) {
    pthread_mutex_lock(&mid);
    isRun = false;
    printf("Steps completed: %d\n", steps_run);
    pthread_mutex_unlock(&mid);
    reverseMotor(steps_run);
    pthread_mutex_lock(&mid);
    motor_time = 0;
    steps_run = 0;
    active_btn = -1;
    btn_state[i] = false;
    pthread_mutex_unlock(&mid);
}
```

- `isRun → False`: 모터를 정지 상태로 전환(`motorThread`에서 플래그를 읽고 반응).  
- `reverseMotor(steps_run)`을 호출해 수행한 스텝만큼 역방향으로 복구합니다.  
- **상태 초기화**  
  - `motor_time = 0`  
  - `steps_run = 0`  
  - `active_btn = -1`  
  - `btn_state[i] = false`.  

3. **버튼 이벤트 후 딜레이**  

```c
delay(1000);
```

- 1초 동안 대기해 동일 버튼이 연속으로 눌리는 것을 방지합니다.



</details> 

## motorThread
 
<details>
<summary>코드 보기</summary>

```c
void* motorThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mid);
        int time_left = motor_time;
        pthread_mutex_unlock(&mid);

        if (isRun) {
            int steps = 2048;
            int delay_time = (time_left * 1000) / steps;

            for (int c = 0; c < 3; c++) {
                printf("[%d] Start of STUDY Time\n", c + 1);
                for (int i = steps - 1; i >= 0; i--) {
                    if (!isRun) {
                        printf("Motor stopped prematurely\n");
                        break;
                    }
                    pthread_mutex_lock(&mid);
                    bool personDetected = isPerson;
                    pthread_mutex_unlock(&mid);

                    // 사람이 감지되지 않았을 때 대기
                    while (!personDetected) {
                        printf("Motor paused. Waiting for person to be detected...\n");
                        delay(100); // 100ms 간격으로 상태 확인

                        pthread_mutex_lock(&mid);
                        personDetected = isPerson;
                        pthread_mutex_unlock(&mid);
                    }

                    for (int j = 0; j < 4; j++) {
                        digitalWrite(pin_arr[j], one_phase[i % 4][j]);
                    }
                    delay(delay_time);

                    pthread_mutex_lock(&mid);
                    steps_run++;
                    pthread_mutex_unlock(&mid);
                }

                if (!isRun) {
                    printf("Motor stopped prematurely\n");
                    break;
                }

                pthread_mutex_lock(&mid);
                steps_run = 0;
                pthread_mutex_unlock(&mid);

                printf("[%d] End of STUDY Time\n", c + 1);
                turnOnLight(1);

                if (c < 2) {
                    printf("Start of BREAK Time\n");
                    reverseMotor(0);
                    turnOnLight(0);
                }
            }

            pthread_mutex_lock(&mid);
            if (isRun) {
                motor_time = 0;
                isRun = false;
                steps_run = 0;
                active_btn = -1;
                for (int i = 0; i < BTN_COUNT; i++) {
                    btn_state[i] = false;
                }
            }
            pthread_mutex_unlock(&mid);
        }
    }
    return NULL;
}
```

</details>

- 설명  

모터의 정방향 회전, 역방향 회전, 사람 감지 대기, 공부-휴식 사이클 제어를 담당합니다.


#### 동작 흐름

1. **초기화 및 상태 확인**

```c
while (1) {
    pthread_mutex_lock(&mid);
    int time_left = motor_time;
    pthread_mutex_unlock(&mid);

    if (isRun) {
        int steps = 2048;
        int delay_time = (time_left * 1000) / steps;
```

- `motor_time` 값을 확인해 모터의 동작 시간을 가져옵니다.  
- `isRun` 플래그를 확인해 모터가 동작 중인지 판단합니다.  
- 동작 중이라면 1회전을 위해 필요한 스텝 수를 지정하고, 설정된 `motor_time`에 따라 각 스텝 간 지연 시간을 계산해 `delay_time`에 할당합니다.

2. **사이클 반복**

```c
for (int c = 0; c < 3; c++) {
    printf("[%d] Start of STUDY Time\n", c + 1);
    for (int i = steps - 1; i >= 0; i--) {
        if (!isRun) {
            printf("Motor stopped prematurely\n");
            break;
        }

        for (int j = 0; j < 4; j++) {
            digitalWrite(pin_arr[j], one_phase[i % 4][j]);
        }
        delay(delay_time);

        pthread_mutex_lock(&mid);
        steps_run++;
        pthread_mutex_unlock(&mid);
    }
}
```

- **공부 및 휴식 시간 총 3번 반복**  
- 각 스텝마다 `one_phase[i % 4][j]` 배열을 통해 현재 스텝에 해당하는 GPIO 핀 상태를 설정해 신호를 전달합니다.  
- `isRun` 플래그를 확인하여 강제 종료 여부를 확인합니다. 강제 종료 시 `break`로 루프를 빠져나옵니다.

3. **상태 초기화**

```c
pthread_mutex_lock(&mid);
if (isRun) {
    motor_time = 0;
    isRun = false;
    steps_run = 0;
    active_btn = -1;
    for (int i = 0; i < BTN_COUNT; i++) {
        btn_state[i] = false;
    }
}
pthread_mutex_unlock(&mid);
```

- 모든 작업이 종료되면 모터와 버튼 상태를 초기화합니다.


## reverseMotor

<details>
<summary>코드 보기</summary>

```c
void reverseMotor(int steps) {
    pthread_mutex_lock(&mid);
    isReverse = true;
    pthread_mutex_unlock(&mid);
    int delay_time = 0;
    int breakTime = 10 * 60;

    if (steps == 0) {
        steps = 2048;
        delay_time = (breakTime * 1000) / steps;
    }
    else {
        delay_time = 5;
    }

    for (int i = 0; i < steps; i++) {
        for (int j = 0; j < 4; j++) {
            digitalWrite(pin_arr[j], one_phase[i % 4][j]);
        }
        delay(delay_time);
    }
    pthread_mutex_lock(&mid);
    isReverse = false;
    pthread_mutex_unlock(&mid);
    printf("Reverse complete for %d steps\n", steps);
}
```

</details>

- 설명  

현재 동작에 적합한 `delay_time`을 설정하고 모터를 역방향으로 회전시키는 코드입니다.


#### 동작 흐름

1. **역방향 동작 상태 설정**

```c
pthread_mutex_lock(&mid);
isReverse = true;
pthread_mutex_unlock(&mid);
```

- `isReverse` 플래그를 활성화하여 현재 모터가 역방향으로 동작 중임을 표시합니다.

2. **딜레이 시간 설정**

```c
int delay_time = 0;
int breakTime = 10 * 60;

if (steps == 0) {
    steps = 2048;
    delay_time = (breakTime * 1000) / steps;
}
else {
    delay_time = 5;
}
```

- `steps`의 값에 따라 각 스텝 간의 딜레이를 다르게 설정합니다.
  - `steps == 0`: 휴식 시간으로 판단하여 10분 기준으로 `delay_time` 계산.
  - `steps ≠ 0`: 스텝 수가 지정된 경우 딜레이를 5ms로 고정.

3. **역방향 회전 및 상태 복구**

```c
for (int i = 0; i < steps; i++) {
    for (int j = 0; j < 4; j++) {
        digitalWrite(pin_arr[j], one_phase[i % 4][j]);
    }
    delay(delay_time);
}
pthread_mutex_lock(&mid);
isReverse = false;
pthread_mutex_unlock(&mid);
```

- `motorThread`의 정방향 회전과 동일한 방식이지만, `i` 값이 반대로 진행되어 회전 방향이 반대로 동작합니다.  
- 동작 완료 후 `isReverse` 플래그를 `false`로 설정하여 상태를 초기화합니다.


## turnOnLight

<details>
<summary>코드 보기</summary>

```c
void turnOnLight(int now) {
    for (int i = 0; i < 3; i++) {
        if (now == 0) { // 핑크색임 (휴식끝)
            softPwmWrite(T_R, 0);
            softPwmWrite(T_G, 50);
            softPwmWrite(T_B, 0);
        }
        else { // 파란색 (공부끝)
            softPwmWrite(T_R, 100);
            softPwmWrite(T_G, 100);
            softPwmWrite(T_B, 0);
        }
        delay(500);
        softPwmWrite(T_R, 100);
        softPwmWrite(T_G, 100);
        softPwmWrite(T_B, 100);
        delay(500);
    }
}
```

</details>

- 설명  

LED를 제어하여 공부 및 휴식 상태의 종료를 알리는 역할을 합니다.


- `now` 값에 따라 핑크색(휴식 종료) 또는 파란색(공부 종료)으로 LED 색상을 설정합니다.  
- LED 점멸은 3회 반복되며, `softPwmWrite`를 통해 부드러운 색상 전환과 밝기 제어를 구현합니다.


## 초음파 센서 ##

 - 설명
: 공부하는 시간 동안 그 자리에 앉아서 집중하도록 사람이 자리에 있는지 없는 지를 판단한다.

- 기능 구현

기능 구현을 위한 함수는 총 4가지다.

- sonicFun(): 초음파 센서로 거리를 측정하는 함수
    
    ```jsx
    double sonicFun() {
        long long start, end;
        double distance;
    
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
    
        while (digitalRead(ECHO) == LOW);
        start = micros();
        while (digitalRead(ECHO) == HIGH);
        end = micros();
    
        double timeElapsed = (end - start) / 1000000.0;
        distance = (timeElapsed * 34300) / 2.0;
    
    	printf("Sonic distance: %.2f cm \n", distance);
        return distance;
    }
    ```
    
    - 초음파는 trig와 echo가 있다. trig 핀에서 초음파를 보내고 물체에 부딪혀서 다시 돌아오는 신호를 echo핀으로 수신한다. 걸린 시간을 계산해 거리를 구한다.
    - digitalWrite와 digitalRead를 사용하였다. digitalWrite로 Trig핀에 HIGH 신호를 주면 10μs 동안 초음파를 보낸다. 그 후 초음파가 다시 ECHO핀으로 돌아오면 ECHO는 LOW에서 HIGH가 된다. start 변수와 end 변수로 신호 시작 시간과 종료 시간을 측정한다.
    - timeElapse에 end-start로 초음파가 돌아오는데 걸린 시간을 계산하고, μs단위를 초 단위로 변환해 저장한다.
    - distance에 음속(34300 cm/s)를 기준으로 반사된 신호의 왕복 시간을 편도 거리로 변환한다. 단위는 cm이다.
    - distance를 반환하여 다른 곳에서 sonicFun()을 호출해 사용 시 distance 값을 받을 수 있도록 하였다.

- averageDistanceThread(): calculateAverageDistance로 평균값을 구해 calculateDistance 변수에 넣는 함수. 프로그램 시작 시 한번 호출된다. thread로 만드려고 하였지만 해당 함수가 전체 기능 작동의 시작할 때 한 번만 실행되는 점을 고려해 thread가 아닌 일반 함수로 하였다. ← 이거 지울까 말까
    
    ```jsx
    void averageDistanceThread() {
        printf("Measuring average distance for 10 sec...\n");
        pthread_mutex_lock(&mid);  // Mutex lock 시작
        calculatedAverageDistance = calculateAverageDistance(5);  // 평균 거리 계산
        printf("Average Distance: %.2f cm\n", calculatedAverageDistance);
        pthread_mutex_unlock(&mid);  // Mutex lock 해제
    
    }
    ```
    
    - 기기로부터 사용자가 앉은 거리의 평균 값을 구하는 함수를 호출해 변수에 저장하고, 이를 mutex를 사용해 동기화한다.

- calculateAverageDistance(): 사람이 앉아 있는 경우를 판단하기 위해 평균 값을 측정하는 함수
    
    ```jsx
    double calculateAverageDistance(int durationInSeconds) {
        int samples = durationInSeconds * 10;
        double totalDistance = 0.0;
    
        for (int i = 0; i < samples; i++) {
    	    if( i > 10){
    	        double distance = sonicFun();
    	        totalDistance += distance;
    	        printf("Sample %d: %.2f cm\n", i + 1, distance);
    	        delay(100);
            }
        }
        return totalDistance / (samples - 10);
    }
    ```
    
    - 초 당 10번 측정하되, 사용자가 기기 시작 버튼을 누르고 자리로 돌아가는 것을 고려해 초기 10번 값은 제외한다.
    - sonicFun()으로 구한 거리를 totalDistance에 저장하고, 100ms 단위로 측정한다.
    - 구한 거리를 전부 합친 값을 나눠 평균 거리를 반환한다.

- ultrasonicThread(void *arg): 사용자가 자리에 있는지 없는지 판단하는 함수
    
    ```jsx
    void *ultrasonicThread(void *arg) {
        while (1) {
            pthread_mutex_lock(&mid);  // Mutex lock 시작
            double distance = sonicFun();
            printf("Distance: %.2f cm\n", distance);
    
            // isPerson 값 읽기
            if (distance > calculatedAverageDistance + 20.0) {
                isPerson = false;  // 값 쓰기
                printf("Person has moved away. isPerson: false\n");
            } else {
                isPerson = true;  // 값 쓰기
                printf("Person is present. isPerson: true\n");
            }
            pthread_mutex_unlock(&mid);  // Mutex lock 해제
    
            delay(500);
        }
        return NULL;
    }
    ```
    
    - sonicFun()을 이용해 사용자의 거리를 꾸준히 측정한다.
    - 측정한 거리가 위의 두 함수로 구한 평균 값보다 20cm 더 크면, 사용자가 자리에서 일어났다고 생각해 isPerson을 false로 반환한다.
    - 500ms 간격으로 초음파 측정을 반복한다.
    - Mutex를 이용해 calculatedAverageDistance와 isPerson을 사용하고, 멀티 쓰레드를 통해 모터가 돌아가는 동시에 실행되도록 한다.

- 진행 순서
    1. 타이머를 실행하면 5초 간 초음파와 사람이 앉은 간격을 측정해 평균 값을 구한다.
    2. 모터가 돌아가기 시작하면 ultrasonicThread가 함께 작동된다.
    3. ultrasonicThread에서 현재 거리가 평균 값보다 20 이상 크다면, 사람이 없다고 판단해 isPerson을 false로 바꾼다. isPerson은 모터스레드로 전달되며, false면 모터 작동을 중지한다.
    4. 다시 평균 값과의 차이가 20 이하가 되면 isPerson은 true가 되어 모터를 다시 작동 시킨다.

## 일시정지 구현 ##
- 코드
    
    ```jsx
    void *motorThread(void *arg) {
        while (1) {
            pthread_mutex_lock(&mid);
            int time_left = motor_time;
            pthread_mutex_unlock(&mid);
    
            if (isRun) {
                int steps = 2048;
                int delay_time = (time_left * 1000) / steps;
    
                for (int c = 0; c < 3; c++) { //study time 3번 반복
                    printf("[%d] Start of STUDY Time\n", c + 1);
                    for (int i = 0; i < steps; i++) {
                        pthread_mutex_lock(&mid);
                        bool personDetected = isPerson;
                        pthread_mutex_unlock(&mid);
    
                        // 사람이 감지되지 않았을 때 대기
                        while (!personDetected) {
                            printf("Motor paused. Waiting for person to be detected...\n");
                            delay(100); // 100ms 간격으로 상태 확인
                            
                            pthread_mutex_lock(&mid);
                            personDetected = isPerson;
                            pthread_mutex_unlock(&mid);
                        }
                        
                        // 공부시간 모터가 돌아가는 함수
                        // 생략 ...
                    }
                    
                    // 생략 ...
    }
    ```
    
- 설명
: 초음파 센서를 이용해 얻은 정보로 사용자가 자리에 없으면 멈추는 일시 정지 기능이다.
    - 모터는 공부 시간과 쉬는 시간으로 나뉜다. 이때 쉬는 시간에는 사람이 자리에 없어도 되므로 공부 시간에만 personDetected를 사용한다.
    - 모터 스레드에서 공부 시간 사이클을 담당하는 for(int c = 0; c < 3; c++) 의 안에 초음파에서 얻은 isPerson 정보를 personDetected로 불러왔다.
    - false일 때는 사람이 없으므로 모터가 멈춰야 하고, true일 때는 사람이 있으므로 모터가 작동해야 한다. 그래서 모터가 돌아가기 전 while(!personDetected)로 false이면 while문이 계속 작동되며 그 뒤의 코드 진행이 되지 못한다. delay를 통해 100ms 단위로 다시 true/false를 받아오고, true가 된다면 while 반복이 끝나고 모터가 돌아가는 아래 코드가 실행된다.
    - isPerson을 구하는 기능 또한 멀티 스레드로 작동하기 때문에, 해당 스레드가 작동하는 동안에도 계속 isPerson 값은 변한다.

## 무드등 구현 ##
- 코드
    
    ```jsx
    void *moodLampThread(void *arg) {
    	int i2c_fd;
        int curLight = 0;
        int preLight = 0;
        int threshold = 230;
        int adcChannel = 2;
        
        i2c_fd = wiringPiI2CSetupInterface(I2C_DEV, SLAVE_ADDR_01);
        
        printf("I2C start....\n");
    
        while (1) {
            wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel);
            preLight = wiringPiI2CRead(i2c_fd);
            curLight = wiringPiI2CRead(i2c_fd);
    
            if (curLight < threshold) {
                //printf("Bright!\n");
                softPwmWrite(M_R, 80);
                softPwmWrite(M_G, 80);
                softPwmWrite(M_B, 100);
            } else {
                //printf("Dark!\n");
                softPwmWrite(M_R, 0);
                softPwmWrite(M_G, 0);
                softPwmWrite(M_B, 100);
            }
    
            delay(500);
        }
    }
    ```
    
- 설명
: 공부를 돕기 위해 주변 밝기에 맞춰 무드등의 밝기를 제어한다.
    
    ---
    
    - 동작 호출
    1. 초기 상태 설정
        
        ```jsx
        	int i2c_fd;
            int curLight = 0;
            int preLight = 0;
            int threshold = 230;
            int adcChannel = 2;
            
            i2c_fd = wiringPiI2CSetupInterface(I2C_DEV, SLAVE_ADDR_01);
            
            printf("I2C start....\n");
        ```
        
        - i2c_fd: I2C를 초기화한다.
        - PerLight: 코드에서는 사용하지 않지만, wiringPiI2CRead()로 값을 읽어오면 처음에는 이전 값을 저장하기 때문에 정확한 측정을 위하여 이전 조도 값을 따로 저장한다.
        - curLight: 현재 조도 값을 저장한다.
        - threshold: 밝기 기준 값으로, 이 변수를 바꿔 기준을 정할 수 있다. 해당 스크립트에서는 실내 조명을 기준으로 하드코딩했다.
        - I2C 인터페이스로 조도 센서에서 데이터를 읽는다.
    2. 조도 측정 및 LED 제어 반복
        
        ```jsx
        while (1) {
                wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel);
                preLight = wiringPiI2CRead(i2c_fd);
                curLight = wiringPiI2CRead(i2c_fd);
        
                if (curLight < threshold) {
                    //printf("Bright!\n");
                    softPwmWrite(M_R, 80);
                    softPwmWrite(M_G, 80);
                    softPwmWrite(M_B, 100);
                } else {
                    //printf("Dark!\n");
                    softPwmWrite(M_R, 0);
                    softPwmWrite(M_G, 0);
                    softPwmWrite(M_B, 100);
                }
        
                delay(500);
            }
        ```
        
        - 무한 반복문으로 설정해 특별한 조건 없이 기기가 작동하는 내내 무드등이 작동하도록 하였다.
        - wiringPiI2CRead로 I2C 버스에서 데이터를 읽어와 ADC 값을 출력한다.
        - 코드에서 PerLight는 사용하지 않지만, wiringPiI2CRead()로 값을 읽어올 경우 처음에는 이전 값을 받아오기 때문에 제대로 된 조도 측정을 위하여 값을 두 번 받아온다.
        - 기준 값 이하일 경우 led를 밝게, 이상일 경우 led를 어둡게 설정한다. led는 일반 led가 아닌 rgb를 조절할 수 있는 led로, softPWM을 통해 각 RGB 값을 조절하여 색상과 밝기를 조절하였다.
        - 무드등은 0.5초 동안 빛나고, 상태를 업데이트한다.
