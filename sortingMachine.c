#include <macros.h>
#include <iom128v.h>

void initTimer3(void);
void servo_motor1(int motor1);
void servo_motor2(int motor2);
void delay_ms(long int ms1);
void initRegister(void);
unsigned long start_edge, end_edge, edge_count;
unsigned long edge_count_array[20] = {0};   //Red Mode
unsigned long edge_count_array1[20] = {0};  //Green Mode
unsigned long edge_count_array2[20] = {0};  //Blue Mode
unsigned long edge_count_array3[20] = {0};  //White Mode
unsigned long edge_count=0;
int timer_ovf_count = 0,count =0,count1=0,count2=0,count3=0;
unsigned long color_check;
#define DELAY_ROTATION 500
int sum=0;
int mode=0;

int main(void)
{
    initTimer3();
    initRegister();
    servo_motor1(375);
    delay_ms(50);

    while(1)
    {
        mode=0;    //Red mode
        PORTD=0b00000010;
        SREG |= 0x80;
        delay_ms(500);
        SREG|=0x00;
        delay_ms(15);

        mode=1;    //Green mode
        PORTD=0b00001110;
        SREG|=0x80;
        delay_ms(500);
        SREG|=0x00;
        delay_ms(15);

        mode=2;   //Blue mode
        PORTD=0b00001010;
        SREG|=0x80;
        delay_ms(500);
        SREG|=0x00;
        delay_ms(15);

        mode=3;   //White mode
        PORTD=0b00000110;
        SREG|=0x80;
        delay_ms(500);
        SREG|=0x00;
        delay_ms(15);

        if (edge_count_array[15]<=608 && edge_count_array1[15]<=2018 && edge_count_array2[15]<=586 && edge_count_array3[15]<2100){
            servo_motor2(590);
            delay_ms(DELAY_ROTATION);
        }
        else if (edge_count_array[15] <= 642 && edge_count_array1[15]<=2150 && edge_count_array2[15]<=588 && edge_count_array3[15]<2128){
            servo_motor2(420);
            delay_ms(DELAY_ROTATION);
        }
        else if (edge_count_array[15] <= 650 && edge_count_array1[15]<=2200 && edge_count_array2[15]<=600 &&e dge_count_array3[15]<2228){
            servo_motor2(250);
            delay_ms(DELAY_ROTATION);
        }
        else{
            servo_motor2(119);
            delay_ms(DELAY_ROTATION);
        }
        SREG = 0x00;

        servo_motor1(367);
        delay_ms(952);

        servo_motor1(375);
        delay_ms(DELAY_ROTATION);
    }
    return 0;
}
void initRegister(void)
{
    DDRE = 0x18;
    DDRD = 0b00001111;

    TCCR1A = 0x00;
    TCCR1B = 0x03;
    TCCR1C = 0x00;
    TIMSK = 0x24;
    SREG |= 0x80;
}
void initTimer3(void)
{
    TCCR3A = 0xA2;
    TCCR3B = 0x1B;
    TCCR3C = 0x00;
    ICR3 = 9999;
}
void servo_motor1(int motor1)
{
    TCCR3A = 0x82;//0b10000010
    OCR3A = motor1;
}
void servo_motor2(int motor2)
{
    TCCR3A = 0x22;//0b00100010
    OCR3B = motor2;
}
void delay_ms(long int ms1)
{
    int i, j;

    for(i=0; i<ms1; i++){
        for(j=0; j<2100; j++);
    }
}

#pragma interrupt_handler timer_cap_isr:iv_TIMER1_CAPT
void timer_cap_isr(void)
{
    if(mode==0){ //Red Mode
        end_edge = ICR1;

        edge_count = end_edge - start_edge + timer_ovf_count*0x10000;
        timer_ovf_count = 0;
        edge_count_array[count++] = edge_count;
    }
    else if(mode==1){ //Green mode
        end_edge = ICR1;
        edge_count = end_edge - start_edge + timer_ovf_count*0x10000;
        timer_ovf_count = 0;
        edge_count_array1[count1++] = edge_count;
    }
    else if(mode==2){ //Blue mode
        end_edge = ICR1;
        edge_count = end_edge - start_edge + timer_ovf_count*0x10000;
        timer_ovf_count = 0;
        edge_count_array2[count2++] = edge_count;
    }
    else{ //White mode
        end_edge = ICR1;
        edge_count = end_edge - start_edge + timer_ovf_count*0x10000;
        timer_ovf_count = 0;
        edge_count_array3[count3++] = edge_count;
    }

    if(count>100){
        count = 0;
    }
    if(count1>100){
        count1 = 0;
    }
    if(count2>100){
        count2 = 0;
    }
    if(count3>100){
        count3 = 0;
    }

    start_edge = end_edge;
}

#pragma interrupt_handler timer_ovf_isr:iv_TIMER1_OVF

void timer_ovf_isr(void)
{
    timer_ovf_count++;
}
