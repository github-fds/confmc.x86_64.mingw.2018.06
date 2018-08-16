#ifndef CONAPI_INT_H
#define CONAPI_INT_H
//------------------------------------------------------------------------------
// Copyright (c) 2018 by Future Design Systems.
// All right reserved.
// http://www.future-ds.com
//------------------------------------------------------------------------------
/// @file conapi_int.h
/// @brief This header file contains core function prototypes for CON-FMC PCB V1.1.
/// @author Ando Ki
/// @data 15/8/2018
//------------------------------------------------------------------------------
#include <stdint.h>
#include "conapi_error.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
#define CONAPI_VERSION   0x20180815

//------------------------------------------------------------------------------
extern const unsigned int INTERFACE_NUMBER;
extern const unsigned int con_cid_max; // max number of card (CID: 0~7)
extern       unsigned int TIMEOUT_MS; // timeout millisecond

//------------------------------------------------------------------------------
// USB Vandor ID and Product ID
// Refer to 'cyfxslfifousbdscr.c'
enum conapi_vid_pid {
      CON_USB_VID=0x04B4
    , CON_USB_PID=0x00F3
};

// USB ENDPOINT address
// Refer to 'cyfxslfifosync.h
enum conapi_ep_num {
     CON_EP_OUT_DOWN=0x01 // ENDPOINT 1 DownStream (i.e, OUT, write)
   , CON_EP_IN_UP   =0x81 // ENDPOINT 1 UpStream (i.e., IN, read)
};

// USB Vendor Request for bRequest of control packet
// Refer to 'cyfxslfifosync.h
enum conapi_vendor_req {
     CON_CID_READ        =0xC0
   , CON_RST_DRIVE       =0xC1
   , CON_MODE_DRIVE      =0xC2 // wValue[1:0]=Mode[1:0]
   , CON_FX3_INFO_READ   =0xC3
   , CON_LED_DRIVE       =0xC4
   , CON_I2C_WRITE       =0xBA
   , CON_I2C_READ        =0xBB
   , CON_FX3_RESET       =0xE2 // wValue[0]=1 for warm-reset, 0 for cold-reset // CY_FX_RQT_RESET_FX3
   , CON_EP_RESET        =0xE1 // wValue[0]=OUT, wValue[1]=IN, CY_FX_RQT_RESET_EP
};

//------------------------------------------------------------------------------
// CON MODE[1:0] - bitwise
// Refer to 'cyfxslfifosync.h
enum conapi_mode {
     CON_MODE_CMD  =0x0 // command mode
   , CON_MODE_SU2F =0x1 // stream-out (USB-to-FPGA) mode
   , CON_MODE_SF2U =0x2 // stream-in (USB-from-FPGA) mode
   , CON_MODE_SLOOP=0x3 // CON_MODE_SU2F|CON_MODE_SF2U // stream-in/out mode
};

//------------------------------------------------------------------------------
enum conapi_log_level {
     CONAPI_LOG_LEVEL_NONE    = 0
   , CONAPI_LOG_LEVEL_ERROR   = 1
   , CONAPI_LOG_LEVEL_WARNING = 2
   , CONAPI_LOG_LEVEL_INFO    = 3
   , CONAPI_LOG_LEVEL_DEBUG   = 4
};

//------------------------------------------------------------------------------
struct _usb {
       struct libusb_context       *ctx;
       struct libusb_device        *dev;
       struct libusb_device_handle *handle;
       int                          interf; // interface
       int                          kernelDriverDetached;
                                    // 0: initial value
                                    // 1: dettached
       int    speed; // x0.1-Mbps
       int    bulk_max_pkt_size_out; // in bytes
       int    bulk_max_pkt_size_in ; // in bytes
       int    iso_max_pkt_size_out ; // in bytes
       int    iso_max_pkt_size_in  ; // in bytes
};
typedef struct _usb usb_t;

//------------------------------------------------------------------------------
struct _con_Handle {
        unsigned int  mode; // GPIF-II operation mode, pseudo-DMA, stream-out/in
        unsigned int  cid;
        struct _usb   usb;
};
typedef struct _con_Handle *con_Handle_t;

//------------------------------------------------------------------------------
struct _con_Fx3Info {
        uint32_t version;
};
typedef struct _con_Fx3Info con_Fx3Info_t;

//------------------------------------------------------------------------------
struct __attribute__((__packed__)) _con_BoardInfo {
        char     MagicID[4]; // null terminated string for "FPI"
        uint32_t FormatVersion; // Little-endian BCD, e.g., 0x2018_04_25
        uint32_t Length; // num of bytes from the beginning to the end (CRC included)
        char     Name[32]; // null terminated string for board name, e.g., "CON-FMC-FX3"
        uint32_t PcbVersion; // Little-endian BCD, e.g., 0x1810_0101
        uint32_t PcbSerial; // Little-endian binary (not BCD)
        char     MajorInfo[32]; // null terminated string for major part, e.g., "Cypress FX3"
        char     MajorPart[32]; // null terminated string for board name, e.g., "CYUSB3014-BZXI
        uint32_t CRC; // formular 0x04C11DB7 (0xED888320/reversed) use not inverted
};
typedef struct _con_BoardInfo con_BoardInfo_t;

//------------------------------------------------------------------------------
struct _con_MasterInfo {
        uint32_t version;
        uint16_t depth_cmd; // num of entries (i.e., words)
        uint16_t depth_u2f; // num of entries (i.e., words)
        uint16_t depth_f2u; // num of entries (i.e., words)
        uint8_t  clk_mhz;
        uint8_t  clk_inv;
};
typedef struct _con_MasterInfo con_MasterInfo_t;

//------------------------------------------------------------------------------
extern struct libusb_device_handle *conOpenDevWithVidPidCid (
                                    struct libusb_context *ctx
                                  , uint16_t               vendor_id
                                  , uint16_t               product_id
                                  , uint8_t                card_id );
extern int conGetUsbInfo ( struct libusb_device_handle *dev_handle
                         , int *pDevSpeed
                         , int *pBulkMaxPktSizeOut
                         , int *pBulkMaxPktSizeIn
                         , int *pIsoMaxPktSizeOut
                         , int *pIsoMaxPktSizeIn );
extern int conUsbControlTransfer( struct libusb_device_handle *dev_handle
                                , uint8_t                      type
                                , uint16_t                     value
                                , uint8_t                      cmd
                                , uint8_t                     *data
                                , uint16_t                     length );
extern int conUsbBulkTransfer( struct libusb_device_handle *dev_handle
                              , uint8_t                      ep_addr
                              , uint8_t                     *pData
                              , int                          toTransfer
                              , int                         *pTransferred );
extern int conFx3I2cTransfer ( struct libusb_device_handle *dev_handle
                             , uint8_t                      type
                             , uint16_t                     byte_addr
                             , uint8_t                     *buffer
                             , uint16_t                     byte_count );
extern int conUsbResetFx3 ( struct libusb_device_handle *dev_handle
                          , unsigned int warm_reset );
extern int conUsbResetEp ( struct libusb_device_handle *dev_handle
                         , unsigned int                 ep_flag );
extern unsigned int conUsbTimeout( int millisecond );
extern int conSetBoardInfo ( struct libusb_device_handle *dev_handle
                           , con_BoardInfo_t *pInfo
                           , unsigned int length
                           , unsigned int crc );
extern uint32_t conCrc32( uint8_t      *pMessage
                        , unsigned int  length );

//------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// Revision history
//
// 2018.08.15: 'interface' to 'interf' of 'struct _usb { }'
//             It causes error on Cygwin64.
// 2018.03.20: Started by Ando Ki (adki@future-ds.com)
//------------------------------------------------------------------------------
#endif //CONAPI_INT_H
