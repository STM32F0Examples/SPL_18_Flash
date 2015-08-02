#include "safeFlash.h"
#include "stm32f0xx.h"
#include <string.h>

const unsigned char __attribute__((aligned (1024))) flashDataBank1[1024]={'H','o','l','a','\0'};
const unsigned char __attribute__((aligned (1024))) flashDataBank2[1024]={'M','u','n','d','o','\0'};


void flash_writeData(int bankSelect, char * newDataPtr, int dataSize){
	int32_t newData;
	int8_t * pNewData=(int8_t *) &newData;
	int flashBankAddress=(int)((bankSelect==0)?(&flashDataBank1):(&flashDataBank2));
	FLASH_Unlock();
	FLASH_WaitForLastOperation(100000);
	FLASH_ErasePage(flashBankAddress);
	FLASH_WaitForLastOperation(100000);
	for(int i=0; i<(dataSize/4); i++){
		for(int j = 0; j<4 ; j++){
			pNewData[j]=newDataPtr[(i*4)+j];
		}
		FLASH_ProgramWord((flashBankAddress+(i*4)),newData);
		FLASH_WaitForLastOperation(100000);
	}
	FLASH_Lock();
}

void flash_loadData(int bankSelect,char * destinyPtr, int dataSize){
	memcpy(destinyPtr,flashDataBank1,dataSize);
}
