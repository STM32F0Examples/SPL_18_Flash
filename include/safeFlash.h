/* 
 * File:   safeFlash.h
 * Author: eLimones <limonesu.me@gmail.com>
 *
 * Created on August 2, 2015, 1:08 AM
 */

#ifndef SAFEFLASH_H
#define	SAFEFLASH_H
#include "stdint.h"

#define FLASH_SUCCESS 0
#define FLASH_FAIL 1

#ifdef	__cplusplus
extern "C" {
#endif

void flash_writeData(int bankSelect, char * newDataPtr, int dataSize);
int safeFlash_saveData(const char * pData, int dataSize);
int safeFlash_loadData(char * destinyPtr, int dataSize);
int isDataValid(int dataSize);



#ifdef	__cplusplus
}
#endif

#endif	/* SAFEFLASH_H */

