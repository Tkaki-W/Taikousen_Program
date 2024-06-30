/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#define WAIT_TIME_MS 500

    
    AnalogIn Vx(PA_1);//ジョイスティックのx軸方向の押し具合を入力
    AnalogIn Vy(PA_0); //ジョイスティックのy軸方向の押し具合を入力
    AnalogIn Vr(PB_1);//ジョイスディックの回転(x方向)の押し具合を入力
    DigitalIn servo_1_CONTROL(PF_0 ,PullUp);
    DigitalIn servo_2_CONTROL(PB_6 ,PullUp);
    PwmOut pwm1(PA_7); //モーター１のpwmを設定
    PwmOut pwm2(PA_4);//モーター２のpwmを設定
    PwmOut pwm3(PA_3);//モーター３のpwmを設定
    DigitalOut ain1_1(PB_0);//モーター１のbin1を設定
    DigitalOut ain2_1(PB_7);//モーター１のbin2を設定
    DigitalOut ain1_2(PF_1);//モーター2のain1を設定
    DigitalOut ain2_2(PA_8);//モーター2のain2を設定
    DigitalOut ain1_3(PA_11);//モーター３のain1を設定
    DigitalOut ain2_3(PB_5);//モーター３のain2を設定
    //サーボのピンの指定
    PwmOut servo1(PA_9);
    PwmOut servo2(PB_4);

//サーボの角度を指定する関数
    float servo_RB(int range);

    //わざわざ置かなくてもいいが見やすいしわかりやすい
    const float ARG0 = servo_RB(0); //放出するときのサーボ
    const float ARG60 = servo_RB(60); //サーボの角度を6０°
    const float ARG90 = servo_RB(90);
    int state_a = 0;
    int state_b = 0;

    //サーボを動かす関数
    void servo1_move(float range);
    void servo2_move(float range);


    void move(float duty, float move_time){
        pwm1 = duty;
        pwm2 = duty;
        pwm3 = duty; 
        thread_sleep_for(move_time);
        pwm1 = 0.0;
        pwm2 = 0.0;
        pwm3 = 0.0;
        ain1_1 = 1;
        ain2_1 = 0;
        ain1_2 = 1;
        ain2_2 = 0;
        ain1_3 = 1;
        ain2_3 = 0;//モーターをすべて時計回りに回転させる
        pwm1 = 0;
        pwm2 = 0;
        pwm3 = 0;
    } 
int main(){
    float corx =0.5 ;//ジョイスティックを押していないときのx軸の補正項
    float cory =0.5 ;//ジョイスティックを押していないときのy軸の補正項
    float corr =0.5 ;//ジョイスティックを押していないときの回転の補正項
    
    pwm1.period(0.002);
    pwm2.period(0.002);
    pwm3.period(0.002);
    //サーボの周期を指定 20ms
    printf("start\n");
    servo1.period(0.002);
    servo2.period(0.002);

    servo2_move(ARG0);

    //STARTが書かれた時点で開始する
    printf("START\r\n");
while(true){
    float V1=-(Vx.read()-corx)-(Vr.read()-corr);
    float V2=(0.5)*(Vx.read()-corx)+0.86602*(Vy.read()-cory)-(Vr.read()-corr);
    float V3=(0.5)*(Vx.read()-corx)-0.86602*(Vy.read()-cory)-(Vr.read()-corr);
    if(V1<0){
        ain1_1 = 0;
        ain2_1 = 1; //モーター１を反時計回りに回転にする
        pwm1 = abs(V1);//モーター１を動かす
    }else{
        ain1_1 = 1;
        ain2_1 = 0;//モーター２を時計回りに回転させる
        pwm1 = abs(V1);//モーター１を動かす
            }
    if(V2<0){
        ain1_2 =0;
        ain2_2 = 1; //モーター2を反時計回りに回転にする
        pwm2= abs(V2);//モーター２を動かす
    }else{
        ain1_2 = 1;
        ain2_2 = 0;//モーター２を時計回りに回転させる
        pwm2 = abs(V2);//モーター２を動かす
            }
    if(V3<0){
        ain1_3 = 0;
        ain2_3 = 1; //モーター3を反時計回りに回転にする
        pwm3 = abs(V3);//モーター３を動かす
    }else{
        ain1_3 = 1;
        ain2_3 = 0;//モーター3を時計回りに回転させる
        pwm3 = abs(V3);//モーター３を動かす
    }
    pwm1 = pwm1*2;
    pwm2 = pwm2*2;
    pwm3 = pwm3*2;
    printf("Vx = %f\n",Vx.read()-corx);
    printf("Vy = %f\n",Vy.read()-cory);
    printf("Vr = %f\n",Vr.read()-corr);

    printf("pwm1=%f\n", pwm1.read());
    printf("pwm2=%f\n" ,pwm2.read());
    printf("pwm3=%f\n" ,pwm3.read());

    
    if(servo_1_CONTROL.read() == 1){
        servo1_move(ARG0);
        printf("ARG0\n");
        }else{
        servo1_move(ARG60);
        printf("ARG60\n");
    }
    
    if(servo_2_CONTROL.read() == 1){
        servo2_move(ARG0);
        printf("ARG0\n");
    }else{
        servo2_move(ARG90);
        printf("ARG90\n");
    }
    /*if(state_a == 0){
        if(servo_1_CONTROL.read() == 0){//押されたとき（プルアップ抵抗は感覚が逆）
            servo1_move(ARG0);
            printf("ARG0\n");
            state_a = 1;
        }else{
            state_a = 0;
            printf("ARG60\n");
        }
    }else if(state_a == 1){
        if(servo_1_CONTROL.read() == 0){//押されたとき（プルアップ抵抗は感覚が逆）
            servo1_move(ARG60);
            printf("ARG60\n");
            state_a = 0;
        }else{
            state_a = 1;
            printf("ARG0\n");
        }
    }
    if(state_b == 0){
        if(servo_2_CONTROL.read() == 0){
            servo2_move(ARG90);
            printf("ARG90\n");
            state_b = 1;
        }else{
            state_b = 0;
            servo2_move(ARG0);
            printf("ARG0\n");
        }
    }else if(state_b == 1){
        if(servo_2_CONTROL.read() == 0){
            servo2_move(ARG0);
            printf("ARG0\n");
            state_b = 0;
        }else{
            state_b = 1;
            servo2_move(ARG90);
            printf("ARG90\n");
        }
    }*/
    thread_sleep_for(50);
    
}

}

//サーボを動かす関数
void servo1_move(float range) {
    servo1.pulsewidth_us(range);
}
void servo2_move(float range){
    servo2.pulsewidth_us(range);
}


//サーボの角度をパルス幅に変換して指定する関数
float servo_RB(int range) {
    return ((float) (2400 - 500) / 180) * range + 500;
}
