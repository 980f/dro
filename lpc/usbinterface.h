#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <stdint.h>


/**
usb driver uses ram at:  0x1000 0050 to 0x1000 0180, will have to reserve that via linker.
since that is the case we can also use linker to locate the jump table which is at 0x1FFF 1FF8


*/
/*
 * 11.5.4 Mass storage device information
The following structure is used to pass the MSC device information: */
struct MSC_DEVICE_INFO {
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint32_t StrDescPtr;

  uint32_t MSCInquiryStr;
  uint32_t BlockCount;
  uint32_t BlockSize;
  uint32_t MemorySize;
  void (*MSC_Write)( uint32_t offset, uint8_t src[], uint32_t length);
  void (*MSC_Read)( uint32_t offset, uint8_t dst[], uint32_t length);
} ;

/* 11.5.5 Human interface device information
The following structure is used to pass the HID device information: */
struct HID_DEVICE_INFO {
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint32_t StrDescPtr;

  uint8_t InReportCount;
  uint8_t OutReportCount;
  uint8_t SampleInterval;
  void (*InReport)( uint8_t src[], uint32_t length);
  void (*OutReport)(uint8_t dst[], uint32_t length);
};




/* 11.5.3 USB device information
The following structure is used to pass the USB device type and information: */
struct USB_DEVICE_INFO {
  uint16_t DevType;
  uint16_t *DevDetailPtr;
};
/*11.5.2 USB driver table
The following structure is used to access the functions exposed by the USB driver: */

struct USBD {
  void (*init_clk_pins)(void);
  void (*isr)(void);
  void (*init)( const USB_DEVICE_INFO * DevInfoPtr );
  void (*connect)(uint32_t con);
};

extern const USBD &UsbDriver;

void InitializeHid();
#endif // USBINTERFACE_H

