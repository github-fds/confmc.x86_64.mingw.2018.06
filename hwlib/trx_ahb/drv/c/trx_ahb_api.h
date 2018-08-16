#ifndef TRX_AHB_API_H
#define TRX_AHB_API_H
//------------------------------------------------------------------------------
// Copyright (c) 2018 Future Design Systems
//
// http://www.future-ds.com
//------------------------------------------------------------------------------
// trx_ahb_api.h
//------------------------------------------------------------------------------
// VERSION = 2018.04.27.
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
extern int BfmGpout( unsigned int Value );
extern int BfmGpin ( unsigned int *pValue );

#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
// Revision History
//
// 2018.04.27: Start by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
#endif
