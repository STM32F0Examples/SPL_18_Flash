#include "stm32f0xx.h"                  // Device header
#include "retarget_STM32F0.h"
#include "serial_stdio.h"
#include "safeFlash.h"

void setToMaxSpeed(void);
Serial_t UART2_serial = {UART2_getChar, UART2_sendChar};

#define IN_BUFFER_SIZE 80
#define SAVE_SIZE 40
char inputBuffer[IN_BUFFER_SIZE];
char mySavedString[SAVE_SIZE];

int main(void)
{
	setToMaxSpeed();
	UART2_init(9600);
	serial_printf(UART2_serial,"\nSystem ready\n");
	while(1){
		serial_printf(UART2_serial,"BANK0 status: %s\n",(safeFlash_isDataValid(SAVE_SIZE))?"valid":"invalid");
		if(safeFlash_isDataValid(SAVE_SIZE)) safeFlash_loadData(mySavedString,SAVE_SIZE);
		serial_printf(UART2_serial,"Saved data: %s\n",mySavedString);
		serial_printf(UART2_serial,"$ ");
		serial_gets(UART2_serial,inputBuffer,IN_BUFFER_SIZE);
		serial_printf(UART2_serial,"%s\n",inputBuffer);
		safeFlash_saveData(inputBuffer,SAVE_SIZE);
	}
}

void setToMaxSpeed(void){
	int internalClockCounter;
	RCC_PLLCmd(DISABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
	RCC_HSEConfig(RCC_HSE_OFF);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12);
	RCC_PREDIV1Config(RCC_PREDIV1_Div1);
	RCC_PLLCmd(ENABLE);
	while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	for(internalClockCounter=0;internalClockCounter<1024;internalClockCounter++){
		if(RCC_GetSYSCLKSource()==RCC_SYSCLKSource_PLLCLK){
			SystemCoreClockUpdate();
			break;
		}
	}		
}
