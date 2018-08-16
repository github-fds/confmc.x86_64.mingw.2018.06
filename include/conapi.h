#ifndef CONAPI_H
#define CONAPI_H
//------------------------------------------------------------------------------
// Copyright (c) 2018 by Future Design Systems.
// All right reserved.
// http://www.future-ds.com
//------------------------------------------------------------------------------
/// @file conapi.h
/// @brief This header file contains function prototypes for CON-FMC PCB V1.1.
/// @author Ando Ki
/// @data 20/3/2018
//------------------------------------------------------------------------------
#include "conapi_int.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
extern con_Handle_t conInit ( unsigned int con_cid
                            , unsigned int con_mode
                            , unsigned int conapi_log_level );
extern int conRelease ( con_Handle_t con_handle );
extern int conCmdWrite ( con_Handle_t  con_handle
                       , void         *pBuffer
                       , unsigned int  nNumberOfItemsToWrite
                       , unsigned int *pNumberOfItemsWritten
                       , unsigned int  transactor );
extern int conDataWrite( con_Handle_t  con_handle
                       , void         *pBuffer
                       , unsigned int  nNumberOfItemsToWrite
                       , unsigned int *pNumberOfItemsWritten
                       , unsigned int  transactor );
extern int conDataRead ( con_Handle_t  con_handle
                       , void         *pBuffer
                       , unsigned int  nNumberOfItemsToRead 
                       , unsigned int *pNumberOfItemsRead
                       , unsigned int  transactor );
extern int conStreamWrite( con_Handle_t  con_handle
                         , void         *pBuffer
                         , unsigned int  nNumberOfItemsToWrite
                         , unsigned int *pNumberOfItemsWritten
                         , unsigned int  zlp );
extern int conStreamRead ( con_Handle_t  con_handle
                         , void         *pBuffer
                         , unsigned int  nNumberOfItemsToRead 
                         , unsigned int *pNumberOfItemsRead );
extern int conZlpWrite ( con_Handle_t  con_handle );
extern int conGetBoardInfo ( con_Handle_t     con_handle
                           , con_BoardInfo_t *pInfo
                           , unsigned int     length
                           , unsigned int     crc_check );
extern int conGetMasterInfo ( con_Handle_t      con_handle
                            , con_MasterInfo_t *pInfo );
extern int conGetFx3Info ( con_Handle_t   con_handle
                         , con_Fx3Info_t *pVersion );
extern int conReset   ( con_Handle_t con_handle
                      , unsigned int duration );
extern int conSetMode ( con_Handle_t con_handle
                      , unsigned int con_mode );
extern int conGetCid  ( con_Handle_t con_handle );
extern unsigned int conGetVersionApi( void );
extern unsigned int conGetVersionLibusb( void );
extern const int conGetErrorLibusb( void );
extern const int conGetErrorConapi( void );
extern const char *conErrorMsgConapi( int error );
extern const char *conErrorMsgLibusb( int error );

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// Revision history
//
// 2018.05.18: conZlpWrite() added
// 2018.03.20: Started by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
#endif //CONAPI_H
