//------------------------------------------------------------------------------
// Copyright (c) 2018 Future Design Systems
// http://www.future-ds.com
//------------------------------------------------------------------------------
// trx_axi_api.c
//------------------------------------------------------------------------------
// VERSION = 2018.06.25.
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <sys/types.h>
#if defined(_WIN32)
#	include <windows.h>
#endif
#include "conapi.h"
#include "trx_axi_api.h"

extern con_Handle_t handle;
static unsigned int amba_axi4=16; // 16=AXI3, 256=AXI4

#define GET_CMD(CMD,EI,WR,LK,EX,SZ,BT,PR,CA,ID,BL)\
	CMD	= ((EI)&0x1)<<31\
		| ((WR)&0x1)<<30\
		| ((LK)&0x1)<<29\
		| ((EX)&0x1)<<28\
		| ((SZ)&0x7)<<25\
		| ((BT)&0x3)<<23\
		| ((PR)&0x7)<<20\
		| ((CA)&0xF)<<16\
		| ((ID)&0xF)<<12\
		| ((BL)&0xFFF)

//-------------------------------------------------------------
// It generates 'length' incremental write transactions
// from the address in 'addr' with data in 'data[]'.
// Note that 'data[x]' contains 'size'-bytes in justified fashion.
void BfmWrite( unsigned int  addr
             , unsigned int *data // pointer to the array of justified data
             , unsigned int  size // num of bytes in an item
             , unsigned int  length)
{
#ifdef RIGOR
   if (data==NULL) {
       printf("%s() invalid buffer\n", __FUNCTION__);
       return;
   }
   switch (size) {
   case 1: case 2: case 4: break;
   default: printf("%s() cannot support %d-byte transfer\n", __FUNCTION__, size);
            return;
   }
   if ((length>0x1000)&&(length<0)) {
       printf("%s() can support up to 4096 for length\n", __FUNCTION__);
       return;
   }
   if (addr%size) {
       printf("%s() cannot support mis-aligned access\n", __FUNCTION__);
       return;
   }
#endif
   // to push BFM command for write
   // - control-flit for command
   // - command-flit for bfm write
   // - address-flit for bfm write
   unsigned int cbuf[4];
   cbuf[0] = (2<<16) // command+address
           | ((0b0010&0xF)<<12) // control packet
           | ((0x0&0xF)<<4); // transactor
   GET_CMD(cbuf[1], 0, 1, 0, 0, size>>1, 1, 0, 0, 1, length-1);
           //       EI,WR,LK,EX, SZ     ,BT,PR,CA,ID,   BL
   cbuf[2] = addr;
   // to push BFM data for write
   // - control-flit for data
   cbuf[3] = (length<<16) //  command+data
           | ((0b0100&0xF)<<12) // control packet
           | ((0x0&0xF)<<4); // transactor

   unsigned int done=0;
   if (conStreamWrite(handle, cbuf, 4, &done, 0) || (done!=4)) {
       printf("%s() something went wrong: %d\n", __FUNCTION__, done);
       return;
   }
//conZlpWrite(handle);

   // to push BFM data for write
   unsigned int num;
   unsigned int *pbuf=data;
   for (num=length, done=0; num>0; num -= done, pbuf += done) {
        unsigned int zlp = ((num*4)%handle->usb.bulk_max_pkt_size_out) ? 0 : 1;
//printf("num=%d zlp=%d\n", num, zlp);
        if (conStreamWrite(handle, (void *)pbuf, num, &done, zlp)) {
            printf("%s() something went wrong: %d\n", __FUNCTION__, done);
            return;
        }
if (done<num) printf("num=%d zlp=%d done=%d\n", num, zlp, done);
   }
}

//------------------------------------------------------------------------------
// It generates 'length' incremental read transactions
// from the address in 'addr' with data in 'data[]'.
// Note that 'data[x]' contains 'size'-bytes in justified fashion.
void BfmRead( unsigned int  addr
            , unsigned int *data // pointer to the array of justified data
            , unsigned int  size
            , unsigned int  length)
{
#ifdef RIGOR
   if (data==NULL) {
       printf("%s() invalid buffer\n", __FUNCTION__);
       return;
   }
   switch (size) {
   case 1: case 2: case 4: break;
   default: printf("%s() cannot support %d-byte transfer\n", __FUNCTION__, size);
            return;
   }
   if ((length>0x1000)&&(length<0)) {
       printf("%s() can support up to 4096 for length\n", __FUNCTION__);
       return;
   }
   if (addr%size) {
       printf("%s() cannot support mis-aligned access\n", __FUNCTION__);
       return;
   }
#endif
   // to push BFM command for write
   // - control-flit for command
   // - command-flit for bfm write
   // - address-flit for bfm write
   unsigned int cbuf[4];
   cbuf[0] = (2<<16) // command+address
           | ((0b0010&0xF)<<12) // control packet
           | ((0x0&0xF)<<4); // transactor
   GET_CMD(cbuf[1], 0, 0, 0, 0, size>>1, 1, 0, 0, 1, length-1);
           //       EI,RD,LK,EX, SZ     ,BT,PR,CA,ID,   BL
   cbuf[2] = addr;
   // to push BFM data for write
   // - control-flit for data
   cbuf[3] = (length<<16) // command+data
           | ((0b0101&0xF)<<12) // control packet
           | ((0x0&0xF)<<4); // transactor

   unsigned int done=0;
   if (conStreamWrite(handle, cbuf, 4, &done, 0) || (done!=4)) {
       printf("%s() something went wrong\n", __FUNCTION__);
       return;
   }

   // to pop BFM data for read
   unsigned int num;
   unsigned int *pbuf=data;
   for (num=length, done=0; num>0; num -= done, pbuf += done) {
        if (conStreamRead(handle, (void *)pbuf, num, &done)) {
            printf("%s() something went wrong\n", __FUNCTION__);
            return;
        }
   }
}

//-------------------------------------------------------------
// It generates 'length' incremental write transactions
// from the address in 'addr' with data in 'data[]'.
// Note that 'data[x]' contains 'size'-bytes in justified fashion.
void BfmWriteFix( unsigned int  addr
                , unsigned int *data // pointer to the array of justified data
                , unsigned int  size // num of bytes in an item
                , unsigned int  length)
{
#ifdef RIGOR
   if (data==NULL) {
       printf("%s() invalid buffer\n", __FUNCTION__);
       return;
   }
   switch (size) {
   case 1: case 2: case 4: break;
   default: printf("%s() cannot support %d-byte transfer\n", __FUNCTION__, size);
            return;
   }
   if ((length>0x1000)&&(length<0)) {
       printf("%s() can support up to 4096 for length\n", __FUNCTION__);
       return;
   }
#endif
   // to push BFM command for write
   // - control-flit for command
   // - command-flit for bfm write
   // - address-flit for bfm write
   unsigned int cbuf[4];
   cbuf[0] = (2<<16) // command+address
           | ((0b0010&0xF)<<12) // control packet
           | ((0x0&0xF)<<4); // transactor
   GET_CMD(cbuf[1], 0, 1, 0, 0, size>>1, 0, 0, 0, 1, length-1);
           //       EI,WR,LK,EX, SZ     ,BT,PR,CA,ID,   BL
   cbuf[2] = addr;
   // to push BFM data for write
   // - control-flit for data
   cbuf[3] = (length<<16) //  command+data
           | ((0b0100&0xF)<<12) // control packet
           | ((0x0&0xF)<<4); // transactor

   unsigned int done=0;
   if (conStreamWrite(handle, cbuf, 4, &done, 0) || (done!=4)) {
       printf("%s() something went wrong: %d\n", __FUNCTION__, done);
       return;
   }
//conZlpWrite(handle);

   // to push BFM data for write
   unsigned int num;
   unsigned int *pbuf=data;
   for (num=length, done=0; num>0; num -= done, pbuf += done) {
        unsigned int zlp = ((num*4)%handle->usb.bulk_max_pkt_size_out) ? 0 : 1;
//printf("num=%d zlp=%d\n", num, zlp);
        if (conStreamWrite(handle, (void *)pbuf, num, &done, zlp)) {
            printf("%s() something went wrong: %d\n", __FUNCTION__, done);
            return;
        }
if (done<num) printf("num=%d zlp=%d done=%d\n", num, zlp, done);
   }
}

//------------------------------------------------------------------------------
// It generates 'length' incremental read transactions
// from the address in 'addr' with data in 'data[]'.
// Note that 'data[x]' contains 'size'-bytes in justified fashion.
void BfmReadFix( unsigned int  addr
               , unsigned int *data // pointer to the array of justified data
               , unsigned int  size
               , unsigned int  length)
{
#ifdef RIGOR
   if (data==NULL) {
       printf("%s() invalid buffer\n", __FUNCTION__);
       return;
   }
   switch (size) {
   case 1: case 2: case 4: break;
   default: printf("%s() cannot support %d-byte transfer\n", __FUNCTION__, size);
            return;
   }
   if ((length>0x1000)&&(length<0)) {
       printf("%s() can support up to 4096 for length\n", __FUNCTION__);
       return;
   }
#endif
   // to push BFM command for write
   // - control-flit for command
   // - command-flit for bfm write
   // - address-flit for bfm write
   unsigned int cbuf[4];
   cbuf[0] = (2<<16) // command+address
           | ((0b0010&0xF)<<12) // control packet
           | ((0x0&0xF)<<4); // transactor
   GET_CMD(cbuf[1], 0, 0, 0, 0, size>>1, 0, 0, 0, 1, length-1);
           //       EI,RD,LK,EX, SZ     ,BT,PR,CA,ID,   BL
   cbuf[2] = addr;
   // to push BFM data for write
   // - control-flit for data
   cbuf[3] = (length<<16) // command+data
           | ((0b0101&0xF)<<12) // control packet
           | ((0x0&0xF)<<4); // transactor

   unsigned int done=0;
   if (conStreamWrite(handle, cbuf, 4, &done, 0) || (done!=4)) {
       printf("%s() something went wrong\n", __FUNCTION__);
       return;
   }

   // to pop BFM data for read
   unsigned int num;
   unsigned int *pbuf=data;
   for (num=length, done=0; num>0; num -= done, pbuf += done) {
        if (conStreamRead(handle, (void *)pbuf, num, &done)) {
            printf("%s() something went wrong\n", __FUNCTION__);
            return;
        }
   }
}

//------------------------------------------------------------------------------
// [command fifo for internal write access]
//  31 30 29 28 27-25 24-23 22-20 19-16 15 - 0
// |EI|WR|                             | DA   |
//
//  EI      : Mode (0:external, 1:internal)
//  WR      : Write/Read (0:read, 1:write)
//  DA[15:0]: Data
//------------------------------------------------------------------------------
// It writes GPIO pins.
//
// Return <0 on failure, 0 on success.
int BfmGpout( unsigned int value ) {
   unsigned int cbuf[1], pbuf[1];
   unsigned int done;
   cbuf[0] = 1<<31
           | 1<<30
           | (value&0xFFFF);
   if (conCmdWrite(handle, (void *)cbuf, 1, &done, 0)) {
       printf("%s() something went wrong\n", __FUNCTION__);
       return -1;
   }
   return 0;
}

//------------------------------------------------------------------------------
// [command fifo for internal read access]
//  31 30 29 28 27-25 24-23 22-20 19-16 15 - 0
// |EI|WR|                                    |
//
//  EI      : Mode (0:external, 1:internal)
//  WR      : Write/Read (0:read, 1:write)
//
// [data fifo for internal read access]
//  31 30 29 28 27-24 23-20 19 18-16 15 - 0
// |FI|IT|ST   |MSTID|RSV  |A4|WIDTH| DA   |
//
//  FI        : FIQ active-high
//  IT        : IRQ active-high
//  ST[1:0]   : status
//  MstID[3:0]: Master ID
//  A4        : AMBA AXI4 (1), AMBA AXI3 (0)
//  WIDTH[2:0]: width of data bus, 0=1-byte,1=2-byte,2=4-byte,3=8-byte,4=16-byte
//  DA[15:0]: Data
//------------------------------------------------------------------------------
// It reads GPIO pins.
//
// Return <0 on failure, 0 on success.
int BfmGpin( unsigned int *pValue ) {
   unsigned int cbuf[1], pbuf[1];
   unsigned int done;
   if (pValue==NULL) {
       return -1;
   }
   cbuf[0] = 1<<31;
   if (conCmdWrite(handle, (void *)cbuf, 1, &done, 0)) {
       printf("%s() something went wrong\n", __FUNCTION__);
       return -1;
   }
   if (conDataRead(handle, (void *)pbuf, 1, &done, 0)) {
       printf("%se() something went wrong\n", __FUNCTION__);
       return -1;
   }
   *pValue = pbuf[0];
   return 0;
}

//------------------------------------------------------------------------------
// It set 'amba_axi4' after reading GPIO pins.
//
// Return <0 on failure, vlaue>=0 on success.
int BfmSetAmbaAxi4(void)
{
    unsigned int value;
    if (BfmGpin(&value)) return -1;
    if ((value>>19)&0x1) {
        amba_axi4 = 256;
    } else {
        amba_axi4 = 16;
    }
    return amba_axi4;
}

//------------------------------------------------------------------------------
// [command fifo for external access]
//  31 30 29 28 27-25 24-23 22-20 19-16 15-12 11-10 9-0
// |EI|WR|LK|EX| SZ  | BT  | PR  | CA  | ID  |resev| BL|
// |                     start address                 |
// [read fifo for write]
// |                     justified data (when write)   |
// [write fifo for read]
// |                     justified data (when write)   |
//
//  EI      : Mode (0:external, 1:internal)
//  WR      : Write/Read (0:read, 1:write)
//  LK      : Lock (1:lock)
//  EX      : Exclusive (1:exclusive)
//  SZ[2:0] : Size (1<<SZ byte)
//  BT[1:0] : Burst type (0:fixed, 1:inc, 2:wrap, 3:reserved)
//  PR[2:0] : Protection
//  CA[3:0] : Cache
//  ID[3:0] : Transaction ID
//  BL[9:0] : Burst length (BL+1 beats); 0 means 1-beat
//------------------------------------------------------------------------------
// Revision History
//
// 2018.06.25: Error handling for mis-aligned case
// 2018.06.12: BfmWriteFix/BfmReadFix added
// 2018.05.01: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
