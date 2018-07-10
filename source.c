#include<LPC2103.H>
#include<string.h>
#define GPIO_Port0s_IODIR IODIR
#define Set_Port0s IOSET
#define Clear_Port0s IOCLR
#define Port0_Set IOPIN
#define Logic_Low 0
#define Port0 0
#define Port1 1
#define Soil_Input (1<<29)
#define T_Cooler_fan (1<<4)
#define H_Spinkr_motor (1<<5)
#define L_Light (1<<6)
#define Soil_pump (1<<7)
#include "LCD.c"
#include "Serial_Uart0.c"
#include "adc0.c"
#include "GSM.c"
#include "App.c"
int PinStatus_Port(unsigned char ,unsigned int);
void Multichannel_Data_Display(void);
void LCD_Lable_display(void);
void Registering_Mobile_NO(void);
unsigned char x;
main()
{
GPIO_Port0s_IODIR = ~Soil_Input;
GPIO_Port0s_IODIR =
(LCD_Data|RS|EN|T_Cooler_fan|H_Spinkr_motor|L_Light|Soil_pump);
//output=1 // input=0
Clear_Port0s=T_Cooler_fan;
Clear_Port0s=H_Spinkr_motor;
Clear_Port0s=L_Light;
Clear_Port0s=Soil_pump;
Lcd_Init();
Init_UART0 (9600);
//Init_UART1 (9600);
Init_UART0_Interrupt();
ADC0_Init();
GSM_Modem_Init();
Sim_Registering();
Registering_Mobile_NO();
LCD_Lable_display();
while(1)
{
Multichannel_Data_Display();
if(Misscall_flag==1)
{
Misscall();
}
if(lite_flag==1)
{
lite_Message_Sending();
lite_flag=0;
}
}
}
void Multichannel_Data_Display(void)
{
ADC0_Channel_3();
Delay(25);
ADC0_Channel_4();
Delay(25);
ADC0_Channel_2();
Delay(25);
Soil();
Delay(100);
}
void Soil_Message_Sending(void);
void lite_Message_Sending(void);
void Humid_Message_Sending(void);
void Temp_Message_Sending(void);
void String_Compare_Home_Auto(void);
void clear_mess_buffer(void);
void Multichannel_GetData(void);
int PinStatus_Port(unsigned char ,unsigned int);
void Multichannel_Data_Display(void);
void LCD_Lable_display(void);
void Soil(void);
unsigned char v1,v2,v3,k,t1,t2,t3,h1,h2,h3,l1,l2,l3,l4;
unsigned char mess[20],Message_string[13],soil_flag=0;
void Multichannel_GetData(void)
{
UART0_TX_Str("Temperature:");
UART0_TX_Chr(t2+48);
UART0_TX_Chr(t3+48);
UART0_TX_Chr('C');
UART0_TX_Str("Humidity:");
UART0_TX_Chr(h2+48);
UART0_TX_Chr(h3+48);
UART0_TX_Chr('%');
UART0_TX_Str("\r\n");
Delay(100);
UART0_TX_Str("Light:");
UART0_TX_Chr(l1+48);
UART0_TX_Chr(l2+48);
UART0_TX_Chr(l3+48);
UART0_TX_Chr(l4+48);
Delay(100);
UART0_TX_Str("LUX");
UART0_TX_Str("\r\n");
Delay(100);
UART0_TX_Str("Soil:");
if(!PinStatus_Port(0,29))
{
UART0_TX_Str("Wet\r\n");
Delay(100);
}
else
{
UART0_TX_Str("Dry\r\n");
Delay(100);
}
UART0_TX_Str("Status Of Devices\r\n");
if(!PinStatus_Port(0,4))
{
UART0_TX_Str("Fan:Off\r\n");
Delay(100);
}
if(PinStatus_Port(0,4))
{
UART0_TX_Str("Fan:On\r\n");
Delay(100);
}
if(!PinStatus_Port(0,5))
{
UART0_TX_Str("Spinklr:Off\r\n");
Delay(100);
}
if(PinStatus_Port(0,5))
{
UART0_TX_Str("Spinklr:On\r\n");
Delay(100);
}
if(!PinStatus_Port(0,6))
{
UART0_TX_Str("Light:Off\r\n");
Delay(100);
}
if(PinStatus_Port(0,6))
{
UART0_TX_Str("Light:On\r\n");
Delay(100);
}
if(!PinStatus_Port(0,7))
{
UART0_TX_Str("Pump:Off\r\n");
Delay(100);
}
if(PinStatus_Port(0,7))
{
UART0_TX_Str("Pump:On\r\n");
Delay(300);
}
}
void Soil(void) //(0:wet, 1:dry)
{
if(!PinStatus_Port(0,29))
{
Lcd_Data_Str(1,5,"Wet");
Delay(10);
Clear_Port0s=Soil_pump;
Delay(100);
}
else
{
Lcd_Data_Str(1,5,"Dry");
Delay(10);
Set_Port0s=Soil_pump;
Soil_Message_Sending();
}
}
void LCD_Lable_display(void)
{
Lcd_Data_Chr(0,0,0,LCD_CLEAR);
Lcd_Data_Str(1,1,"sol:");
Delay(10);
Lcd_Data_Str(1,10,"Temp:");
Delay(10);
Lcd_Data_Str(2,1,"Hum:");
Delay(10);
Lcd_Data_Str(2,10,"L:");
Delay(10);
}
void Misscall_Message_Sending(void)
{
Lcd_Data_Str(2,1,"message sending");
while(UART0_RX_Chr()!='>');
UART0_TX_Str ("GSM Green House MT & ACT\r\n");
Delay(200);
Multichannel_GetData();
UART0_TX_Chr(0x1a);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Lcd_Data_Str(2,1," message sent ");
Delay(300);
LCD_Lable_display();
}
void Soil_Message_Sending(void)
{
Disable_UART0_Interrupt();
Lcd_Data_Chr(0,0,0,LCD_CLEAR);
Lcd_Data_Str(2,1,"message sending");
Delay(100);
UART0_TX_Str ("AT\r\n");
Delay(200);
UART0_TX_Str ("Alert !! Soil Was Dry And Motor Was ON");
UART0_TX_Chr(0x1A);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Delay(100);
Lcd_Data_Str(2,1," message sent ");
Delay(300);
LCD_Lable_display();
Enable_UART0_Interrupt();
}
void Temp_Message_Sending(void)
{
Disable_UART0_Interrupt();
Lcd_Data_Chr(0,0,0,LCD_CLEAR);
Lcd_Data_Str(2,1,"message sending");
UART0_TX_Str ("AT\r\n");
Delay(200);
UART0_TX_Str ("AT+CMGS=")
Delay(100);;
UART0_TX_Str(misscall_phone);
UART0_TX_Chr(0x0D);
while(UART0_RX_Chr()!='>');
Delay(100);
UART0_TX_Str ("Alert!! High Temperature Cooler Was On");
Lcd_Data_Str(2,1," message sent ");
Delay(300);
LCD_Lable_display();
Enable_UART0_Interrupt();
}
void Humid_Message_Sending(void)
{
Disable_UART0_Interrupt();
Lcd_Data_Chr(0,0,0,LCD_CLEAR);
Lcd_Data_Str(2,1,"message sending");
UART0_TX_Str ("AT\r\n");
Delay(200);
UART0_TX_Str ("AT+CMGS=");
UART0_TX_Str(misscall_phone);
UART0_TX_Chr(0x0D);
Delay(100);
while(UART0_RX_Chr()!='>');
UART0_TX_Str ("Alert!! High Humidity Sprinkler Was On");
UART0_TX_Chr(0x1A);
Delay(100);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Delay(100);
Lcd_Data_Str(2,1," message sent ");
Delay(300);
LCD_Lable_display();
Enable_UART0_Interrupt();
}
void lite_Message_Sending(void)
{
Disable_UART0_Interrupt();
Lcd_Data_Chr(0,0,0,LCD_CLEAR);
Lcd_Data_Str(2,1,"message sending");
UART0_TX_Str ("AT\r\n");
Delay(200);
UART0_TX_Str ("AT+CMGS=");
Delay(100);
UART0_TX_Str(misscall_phone);
UART0_TX_Chr(0x0D);
while(UART0_RX_Chr()!='>');
Delay(100);
UART0_TX_Str ("Alert!! Low Intensity Of Light. Light Was On");
UART0_TX_Chr(0x1A);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Delay(100);
Lcd_Data_Str(2,1," message sent ");
Delay(300);
LCD_Lable_display();
Enable_UART0_Interrupt();
}
void voltage_Message_Sending(void);
void lite_Message_Sending(void);
void Humid_Message_Sending(void);
void Temp_Message_Sending(void);
unsigned int Divisor;
unsigned char v1,v2,v3,k,t1,t2,t3,h1,h2,h3,l1,l2,l3,l4;\
unsigned char samples;
unsigned int RH,lux;
unsigned int r;
float ADC_VALUE;
unsigned char lite_flag,humid_flag,temp_flag;
void ADC0_Init(void);
//void ADC0_Channel_1(void);
void ADC0_Channel_2(void);
void ADC0_Channel_3(void);
void ADC0_Channel_4(void);
void ADC0_Channel_5(void);
void ADC0_Init(void)
{
PCONP|=(1<<12);
PINSEL0|=0X03F30005;
PINSEL1|=0X00030000;
ADCR &=0X00000000;
ADCR |=0X00000600;
ADCR |=(1<<24);
}
void ADC0_Channel_2(void) //light //P0.28
{
unsigned int a1,b1,c1,p;
ADC_VALUE=0X00000000;
Delay(10);
while((ADDR2&0X80000000)==0);
Delay(10);
a1=2500/ADC_VALUE;
b1=a1-500;
c1=b1/2.2;
lux= c1-115; //((2500/r1)-500)/2.2;
l1=lux/1000;
k=lux%1000;
l2=k/100;
p=k%100;
l3=p/10;
l4=p%10;
if(lux<10)
{
Set_Port0s=L_Light;
lite_flag=1;
}
else
{
Clear_Port0s=L_Light;
lite_flag=0;
}
}
void ADC0_Channel_4(void) //humidity P0.11
{
ADCR &=0xFFFFFFF0;
ADCR |=0X00000010;
ADC_VALUE=0X00000000;
Delay(10);
while((ADDR4&0X80000000)==0);
ADC_VALUE=0.003222*((ADDR4&0X0000FFC0)>>6);
Delay(10);
RH=ADC_VALUE/0.0330; //%RH=(Vout-0.453)/0.0308
h1=RH/100;
k=RH%100;
h2=k/10;
h3=k%10;
Delay(10);
Lcd_Data_Chr(1,2,5,h2+48);
Lcd_Data_Chr(1,2,6,h3+48);
Lcd_Data_Chr(1,2,7,'%');
if(RH<30)
{
Set_Port0s=H_Spinkr_motor;
if(humid_flag==0)
{
Humid_Message_Sending();
humid_flag=1;
}
}
else
{
Clear_Port0s=H_Spinkr_motor;
humid_flag=0;
}
}
void ADC0_Channel_3(void) //temp P0.10
{
unsigned char temp;
ADCR &=0xFFFFFFF0;
Delay(10);
while((ADDR3&0X80000000)==0);
ADC_VALUE=0.003222*((ADDR3&0X0000FFC0)>>6);
Delay(10);
temp=ADC_VALUE*100;
k=temp/10;
t1=k/10;
t2=k%10;
t3=temp%10;
Delay(10);
Lcd_Data_Chr(1,1,15,t2+48);
Lcd_Data_Chr(1,1,16,t3+48);
Lcd_Data_Chr(1,1,17,0xdf);
if(temp>55)
{
Set_Port0s=T_Cooler_fan;
if(temp_flag==0)
{
Temp_Message_Sending();
temp_flag=1;
}
}
else
{
Clear_Port0s=T_Cooler_fan;
temp_flag=0;
}
}
/*
void ADC0_Channel_6(void)
{
Delay(10);
while((AD0DR6&0X80000000)==0);
ADC_VALUE=0.003222*((AD0DR6&0X0000FFC0)>>6);
Delay(10);
r=ADC_VALUE*100;
k=r/10;
v1=k/10;
v2=k%10;
v3=r%10;
UART0_TX_Chr (v1+48);
UART0_TX_Str (".");
UART0_TX_Chr (v2+48);
UART0_TX_Chr (v3+48);
Delay(25);
}
void ADC0_Channel_7(void)
{
Delay(10);
while((AD0DR7&0X10000000)==0);
ADC_VALUE=0.003222*((AD0DR7&0X0000FFC0)>>6);
Delay(10);
r=ADC_VALUE*100;
k=r/10;
v1=k/10;
v2=k%10;
v3=r%10;
UART0_TX_Chr (v1+48);
UART0_TX_Str (".");
UART0_TX_Chr (v2+48);
UART0_TX_Chr (v3+48);
Delay(25);
}
*/
#define Restg 0
#define Restd 1
void GSM_Modem_Init(void);
void Sim_Registering(void);
void Message_Sending(void);
void Message_Reading(void);
void Message_Deleting(void);
void Display_Message(void);
void Misscall(void);
void Misscall_Message_Sending(void);
void clear_mess_buffer(void);
void Ph_no_Reg_Message_Sending(void);
void Enable_UART0_Interrupt(void);
void Disable_UART0_Interrupt(void);
void String_Copy(void);
void String_Compare_Home_Auto(void);
unsigned char mess[20],
mess_phone[16],s[5],misscall_phone[16],sim_reg[4],Message_string[13];
//misscall_phone="+919866200201"
unsigned char j,k=0,l,ptr,ptr1,cnt,cnt1;
unsigned char LCD_CLEAR=0X01;
//unsigned char Reg_Phno_flag=0;
//const char mess[20];
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
void GSM_Modem_Init(void)
{
Lcd_Data_Str(1,1,"GSM Modem Initng");
//Delay(4000);
Disable_UART0_Interrupt();
UART0_TX_Str("AT\r\n");
Delay(500);
UART0_TX_Str("AT\r\n");
Delay(500);
UART0_TX_Str("AT\r\n");
Delay(500);
UART0_TX_Str("AT\r\n");
Delay(500);
UART0_TX_Str("AT+CNMI=1,1,0,0,0\r\n");
UART0_TX_Chr(0x0d);
Delay(500);
UART0_TX_Str("AT+CNMI=1,1,0,0,0\r\n");
UART0_TX_Chr(0x0d);
Delay(500);
}
void Sim_Registering(void)
{
Disable_UART0_Interrupt();
start:
UART0_TX_Str("AT+CREG?\r\n");
while(UART0_RX_Chr()!=':');
for(j=0;j<4;j++)
{
sim_reg[j]=UART0_RX_Chr();
}
ptr=0;
for(j=0;j<4;j++)
{
Lcd_Data_Chr(1,2,ptr++,sim_reg[j]);
}
if(sim_reg[3]==0x31)
{
Lcd_Data_Str(1,1,"GSM Modem Inited");
Lcd_Data_Str(2,1,"SIM Registered ");
Delay(1000);
Enable_UART0_Interrupt();
}
else
{
Lcd_Data_Str(2,1,"SIM Registering");
Delay(900);
Lcd_Data_Str(2,1," ");
goto start;
}
}
void Project_Label(void)
{
Lcd_Data_Str(1,1," GSM Based ");
Lcd_Data_Str(2,1,"GreenHouse Mt&ct");
}
void Give_Misscall(unsigned char ph)
{
if(!ph)
{
Lcd_Data_Str(1,1," Give Misscall ");
Lcd_Data_Str(2,1,"To Reg Phone No ");
}
if(ph)
{
Lcd_Data_Str(1,1," Your Phone No ");
Lcd_Data_Str(2,1,"Reg Successfully");
}
}
void Registering_Mobile_NO(void)
{
Project_Label();
Delay(150);
/********************************* Phone No
Registering***************************/
/***********************************************************************
**********/
Give_Misscall(Restg);
while(1)
{
if(Reg_Phno_flag==1)
{
Ph_no_Reg_Message_Sending();
Reg_Phno_flag=0;
Misscall_flag=0;

Give_Misscall(Restd);
break;
}
}
/***********************************************************************
**********/
/***********************************************************************
**********/
Project_Label();
Delay(400);
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
void Ph_no_Reg_Message_Sending(void)
{
Disable_UART0_Interrupt();
Lcd_Data_Str(1,1," Misscall ");
Delay(500);
UART0_TX_Str("ATH");
UART0_TX_Chr(0X0D);
UART0_TX_Chr(0X0A);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Delay(100);

UART0_TX_Str("AT+CPBS=\"MC\"");
UART0_TX_Chr(0X0D);
UART0_TX_Chr(0X0A);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Delay(100);

UART0_TX_Str("AT+CPBR=1");
UART0_TX_Chr(0X0D);
UART0_TX_Chr(0X0A);
while(UART0_RX_Chr()!='1');
while(UART0_RX_Chr()!=',');
for(j=0;j<15;j++)
{
misscall_phone[j]=UART0_RX_Chr();
}
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Lcd_Data_Str(2,1,"message sending");
UART0_TX_Str ("AT\r\n");
Delay(250);
UART0_TX_Str ("AT+CMGS=");
UART0_TX_Str(misscall_phone);
UART0_TX_Chr(0x0D);
UART0_TX_Chr(0x0A);
while(UART0_RX_Chr()!='>');
UART0_TX_Str ("Your Phone No:");
UART0_TX_Str(misscall_phone);
UART0_TX_Str("Is Feeded Sucessfully");
UART0_TX_Chr(0x1a);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Lcd_Data_Str(2,1," message sent ");
Delay(500);
Lcd_Data_Chr(0,0,0,LCD_CLEAR);
Enable_UART0_Interrupt();
}
void Misscall(void)

{
Disable_UART0_Interrupt();
Lcd_Data_Str(1,1," Misscall ");
Delay(300);
UART0_TX_Str("ATH");
Delay(100);
UART0_TX_Chr(0X0D);
UART0_TX_Chr(0X0A);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Delay(100);

UART0_TX_Str("AT+CPBS=\"MC\"");
UART0_TX_Chr(0X0D);
UART0_TX_Chr(0X0A);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Delay(100);
UART0_TX_Str("AT+CPBR=1");
UART0_TX_Chr(0X0D);
UART0_TX_Chr(0X0A);
while(UART0_RX_Chr()!='1');
while(UART0_RX_Chr()!=',');
for(j=0;j<15;j++)
{
misscall_phone[j]=UART0_RX_Chr();
}
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Lcd_Data_Str(2,1,"message sending");
Enable_UART0_Interrupt();
//ptr=0;
//for(j=0;j<15;j++)
//{
//Lcd_Data_Chr(1,2,ptr++,misscall_phone[j]);
//}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
void Message_Collect(void)
{
while(UART0_RX_Chr()!='S');
while(UART0_RX_Chr()!='M');
while(UART0_RX_Chr()!=',');
{
for(j=0;j<2;j++)
{
s[j]=UART0_RX_Chr();
}
}
}
void Message_Reading(void)
{

if((s[0]<=0x39)||(s[0]==0x31&&s[1]==0x39))
{
UART0_TX_Str(" AT+CMGR=");
UART0_TX_Chr(s[0]);
UART0_TX_Chr(s[1]);
UART0_TX_Chr(0x0D);
while(UART0_RX_Chr()!='+');
while(UART0_RX_Chr()!='C');
while(UART0_RX_Chr()!='M');
while(UART0_RX_Chr()!='"');
while(UART0_RX_Chr()!='"');
for(j=0;j<16;j++)
{
mess_phone[j]=UART0_RX_Chr();
}
while(UART0_RX_Chr()!='+');
while(UART0_RX_Chr()!=0x0D);// /r
for(k=0;k<15;k++)
{
mess[k]=UART0_RX_Chr();
}
}
if((s[0]==0x31)&&(s[1]==0x30))
{
Message_Deleting();
} // if
Display_Message();
clear_mess_buffer();
Enable_UART0_Interrupt();
//}//,
}//messg_read
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
void clear_mess_buffer(void)
{
for(k=0;k<13;k++) //
{
Message_string[k]=0x00;
}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
void Message_Deleting(void)
{
Lcd_Data_Str(2,1,"Mess over load..");
Lcd_Data_Str(2,1,"Deleting............");
Disable_UART0_Interrupt();
for(l=0X31;l<=0X39;l++)
{
UART0_TX_Str("AT+CMGD=");
UART0_TX_Chr(l);
UART0_TX_Chr(0X0D);
UART0_TX_Chr(0X0A);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
if(l==0x39)
{
UART0_TX_Str("AT+CMGD=");
UART0_TX_Chr(0x31);
UART0_TX_Chr(0x30);
UART0_TX_Chr(0X0D);
UART0_TX_Chr(0X0A);
while(UART0_RX_Chr()!='O');
while(UART0_RX_Chr()!='K');
Lcd_Data_Str(2,1," Deleted ");
Delay(500);
Enable_UART0_Interrupt();
}
}
}
void String_Copy(void)
{
for(cnt=1,cnt1=0;mess[cnt]!='\r';cnt1++,cnt++)
{
Message_string[cnt1]=mess[cnt];
}
cnt1=cnt1+1;
Message_string[cnt1]='\r';
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
void Display_Message(void)
{
ptr1=0;
Lcd_Data_Chr(0,0,0,LCD_CLEAR);
for(k=0;mess[k]!='\r';k++) //
{
Lcd_Data_Chr(1,1,ptr1++,mess[k]);
}
//ptr=0;
//for(j=0;j<16;j++)
//{
//Lcd_Data_Chr(1,2,ptr++,mess_phone[j]);
//}
//ptr1=0;
ptr1=0;
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
void Disable_UART0_Interrupt(void)
{
U0IER = 0x00;
}
void Enable_UART0_Interrupt(void)
{
U0IER = 0x01
