#ifndef TRX_AXI_API_H
#define TRX_AXI_API_H
//------------------------------------------------------------------------------
// Copyright (c) 2018 Future Design Systems
//
// http://www.future-ds.com
//------------------------------------------------------------------------------
// trx_axi_api.h
//------------------------------------------------------------------------------
// VERSION = 2018.06.12.
//------------------------------------------------------------------------------
// Note that "data" carries justified data.

#ifdef __cplusplus
extern "C" {
#endif

extern void BfmWrite( unsigned int  addr
                    , unsigned int *data
                    , unsigned int  size
                    , unsigned int  length);
extern void BfmRead ( unsigned int  addr
                    , unsigned int *data
                    , unsigned int  size
                    , unsigned int  length);
extern void BfmWriteFix( unsigned int  addr
                       , unsigned int *data
                       , unsigned int  size
                       , unsigned int  length);
extern void BfmReadFix ( unsigned int  addr
                       , unsigned int *data
                       , unsigned int  size
                       , unsigned int  length);
extern int BfmGpout( unsigned int Value );
extern int BfmGpin ( unsigned int *pValue );
extern int BfmSetAmbaAxi4( void );
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
// Revision History
//
// 2018.06.12: BfmWriteFix/BfmReadFix added
// 2018.05.01: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
#endif
