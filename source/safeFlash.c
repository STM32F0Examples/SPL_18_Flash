#include "safeFlash.h"
#include "stm32f0xx.h"
#include <string.h>

/* |Checksum | NOT USED | DATA ...  */
const unsigned char __attribute__((aligned (1024))) flashDataBank0[1024]={0x00,0x00};

uint8_t getChecksum(const uint8_t * data, uint8_t nData);

int safeFlash_saveData(const char * pData, int dataSize){
	const unsigned char * saveBankAddress=flashDataBank0;
	int32_t tempData;
	int8_t * pTempData=(int8_t *) &tempData;
	
	FLASH_Unlock();
	FLASH_WaitForLastOperation(100000);
	FLASH_ErasePage((uint32_t)saveBankAddress);
	FLASH_WaitForLastOperation(100000);
	
	//write data
	int nIterations =(dataSize/2)+(dataSize%2);//Add extra iteration if not multiple of 2 
	for(int i=0; i<nIterations; i++){
		for(int j = 0; j<2 ; j++){
			if(((i*2)+j) < dataSize) pTempData[j]=pData[(i*2)+j];
			else pTempData[j]=0xFF;
		}
		FLASH_ProgramHalfWord((int)((saveBankAddress+2)+(i*2)),tempData);
		FLASH_WaitForLastOperation(100000);
	}
	//write checksum
	pTempData[0]=getChecksum((uint8_t *)pData,dataSize);
	pTempData[1]=0xFF;
	
	FLASH_ProgramWord((int)saveBankAddress,tempData);
	FLASH_WaitForLastOperation(100000);
	FLASH_Lock();
	return safeFlash_isDataValid(dataSize);
}

void safeFlash_rawWriteData(int bankSelect, char * newDataPtr, int dataSize){
	int32_t newData;
	int8_t * pNewData=(int8_t *) &newData;
	int flashBankAddress=(int)flashDataBank0;
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

int safeFlash_loadData(char * destinyPtr, int dataSize){
	if(safeFlash_isDataValid(dataSize)){
		memcpy(destinyPtr,(flashDataBank0+2),dataSize);
		return FLASH_SUCCESS;
	}else{
		return FLASH_FAIL;
	}
}

uint8_t get8BitAddWithCarry(const uint8_t * data, uint8_t nData) {
	uint16_t sum = 0;
	for (int i = 0; i < nData; i++) {
		sum += data[i];
		if (sum >= 256) {
			sum -= 255;
		}
	}
	return sum;
}

uint8_t getChecksum(const uint8_t * data, uint8_t nData){
	uint8_t checksum;
	checksum = get8BitAddWithCarry(data, nData);
	checksum = ~checksum;
	return checksum;
}

int safeFlash_isDataValid(int dataSize){
	const unsigned char * pBank = flashDataBank0;
	const unsigned char * pData= pBank+2;
	unsigned char bankChecksum= pBank[0];
	unsigned char checksum = getChecksum(pData,dataSize); 
	return  bankChecksum == checksum;
}

