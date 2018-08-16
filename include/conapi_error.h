#ifndef CONAPI_ERROR_H
#define CONAPI_ERROR_H
//------------------------------------------------------------------------------
// Copyright (c) 2018 by Future Design Systems.
// All right reserved.
// http://www.future-ds.com
//------------------------------------------------------------------------------
/// @file conapi_error.c
/// @brief This file contains error related code.
/// @author Ando Ki
/// @data 20/3/2018
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
#define CONAPI_ERROR_NO                0
#define CONAPI_ERROR_MALLOC            1
#define CONAPI_ERROR_INVALID_PARAM     2
#define CONAPI_ERROR_CID               3
#define CONAPI_ERROR_MODE              4
#define CONAPI_ERROR_CON_HANDLE        5
#define CONAPI_ERROR_USB_HANDLE        6
#define CONAPI_ERROR_USB_INIT          7
#define CONAPI_ERROR_USB_OPTION        8
#define CONAPI_ERROR_USB_OPEN          9
#define CONAPI_ERROR_USB_DETACH        10
#define CONAPI_ERROR_USB_ATTACH        11
#define CONAPI_ERROR_USB_CLAIM         12
#define CONAPI_ERROR_USB_RELEASE       13
#define CONAPI_ERROR_USB_CONTROL_OUT   14
#define CONAPI_ERROR_USB_CONTROL_IN    15
#define CONAPI_ERROR_USB_BULK_OUT      16
#define CONAPI_ERROR_USB_BULK_IN       17
#define CONAPI_ERROR_USB_RESET         18
#define CONAPI_ERROR_CID_NOT_FOUND     19
#define CONAPI_ERROR_INVALID_MODE      20
#define CONAPI_ERROR_CRC               21
#define CONAPI_ERROR_UNKNOWN           22
#define CONAPI_ERROR_MAX               23

//------------------------------------------------------------------------------
// It keeps LIBUSB_ERROR code if any.
extern int libusb_error;

//------------------------------------------------------------------------------
// It keeps CONAPI_ERROR code if any.
extern int conapi_error;

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// Revision history
//
// 2018.03.20: Started by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
#endif //CONAPI_ERROR_H
