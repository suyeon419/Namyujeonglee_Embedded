#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include <time.h>

#define BTN_COUNT 3
#define M_R 27
#define M_G 22
#define M_B 23
#define T_R 6
#define T_G 26
#define T_B 19


#define SLAVE_ADDR_01 0x48
static const char* I2C_DEV = "/dev/i2c-1";

int btn_gpio[BTN_COUNT] = { 17, 24, 25 };
bool btn_state[BTN_COUNT] = { false, false, false };
int motor_time = 0;
int steps_run = 0;
bool isRun = false;
bool isReverse = false;
int active_btn = -1;

pthread_mutex_t mid;

int pin_arr[4] = { 12, 16, 20, 21 };
int one_phase[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1} };

void* switchThread(void* arg);
void* motorThread(void* arg);
void* moodLampThread(void* arg);
void reverseMotor(int steps);
void turnOnLight(int now);
void pinInit();

//sonic                                                                                                                                                                      
#define TRIG 15
#define ECHO 18

//pthread_mutex_t lock;
bool isPerson = true;
double calculatedAverageDistance = 0.0;


double sonicFun();
double calculateAverageDistance(int durationInSeconds);
void* ultrasonicThread(void* arg);
void averageDistanceThread();

int main() {
    pinInit();
    averageDistanceThread();
    
    pthread_t ptSwitch, ptMotor, ptLamp, ptUltrasonic;

    pthread_mutex_init(&mid, NULL);

    pthread_create(&ptSwitch, NULL, switchThread, NULL);
    pthread_create(&ptMotor, NULL, motorThread, NULL);
    pthread_create(&ptLamp, NULL, moodLampThread, NULL);
    
    // After averageDistanceThread finishes, start ultrasonicThread
    printf("Starting ultrasonicThread...\n");
    pthread_create(&ptUltrasonic, NULL, ultrasonicThread, NULL);
    pthread_join(ptUltrasonic, NULL);

    pthread_join(ptSwitch, NULL);
    pthread_join(ptMotor, NULL);
    pthread_join(ptLamp, NULL);

    pthread_mutex_destroy(&mid);
    //pthread_mutex_destroy(&lock);

    return 0;
}


void pinInit()
{
    wiringPiSetupGpio();
    for (int i = 0; i < BTN_COUNT; i++) {
        pinMode(btn_gpio[i], INPUT);
    }
    for (int i = 0; i < 4; i++) {
        pinMode(pin_arr[i], OUTPUT);
    }
    softPwmCreate(M_R, 0, 100);
    softPwmCreate(M_G, 0, 100);
    softPwmCreate(M_B, 0, 100);

    softPwmCreate(T_R, 0, 100);
    softPwmCreate(T_G, 0, 100);
    softPwmCreate(T_B, 0, 100);

    softPwmWrite(T_R, 100);
    softPwmWrite(T_G, 100);
    softPwmWrite(T_B, 100);

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    digitalWrite(TRIG, LOW);
}


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

void* moodLampThread(void* arg) {
    int i2c_fd;
    int cnt = 0;
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
        }
        else {
            //printf("Dark!\n");
            softPwmWrite(M_R, 0);
            softPwmWrite(M_G, 0);
            softPwmWrite(M_B, 100);
        }

        delay(500);
        cnt++;
    }
}

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

// ultrasonic

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

void averageDistanceThread() {
    printf("Measuring average distance for 10 sec...\n");
    pthread_mutex_lock(&mid);  // Mutex lock 시작
    calculatedAverageDistance = calculateAverageDistance(5);  // 평균 거리 계산
    printf("Average Distance: %.2f cm\n", calculatedAverageDistance);
    pthread_mutex_unlock(&mid);  // Mutex lock 해제

    //pthread_mutex_lock(&cond_lock);
    //pthread_cond_signal(&cond);
    //pthread_mutex_unlock(&cond_lock);

}

double calculateAverageDistance(int durationInSeconds) {
    int samples = durationInSeconds * 10;
    double totalDistance = 0.0;

    for (int i = 0; i < samples; i++) {
        if (i > 10) {
            double distance = sonicFun();
            totalDistance += distance;
            printf("Sample %d: %.2f cm\n", i + 1, distance);
            delay(100);
        }
    }

    return totalDistance / (samples - 10);
}

// ultrasonicThread 예시
void* ultrasonicThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mid);  // Mutex lock 시작
        double distance = sonicFun();
        printf("Distance: %.2f cm\n", distance);

        // isPerson 값 읽기
        if (distance > calculatedAverageDistance + 20.0) {
            isPerson = false;  // 값 쓰기
            printf("Person has moved away. isPerson: false\n");
        }
        else {
            isPerson = true;  // 값 쓰기
            printf("Person is present. isPerson: true\n");
        }
        pthread_mutex_unlock(&mid);  // Mutex lock 해제

        delay(500);
    }
    return NULL;
}
