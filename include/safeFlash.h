/* 
 * File:   safeFlash.h
 * Author: eLimones <limonesu.me@gmail.com>
 *
 * Created on August 2, 2015, 1:08 AM
 */

#ifndef SAFEFLASH_H
#define	SAFEFLASH_H

#ifdef	__cplusplus
extern "C" {
#endif

void flash_writeData(int bankSelect, char * newDataPtr, int dataSize);
void flash_loadData(int bankSelect,char * destinyPtr, int dataSize);



#ifdef	__cplusplus
}
#endif

#endif	/* SAFEFLASH_H */

