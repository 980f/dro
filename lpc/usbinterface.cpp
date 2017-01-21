#include "usbinterface.h"
#include "nvic.h"

const USBD &UsbDriver(*reinterpret_cast<USBD *>(0x1FFF1FF8));


#define USB_STRING_DESCRIPTOR_TYPE(len) (0x0100+2*(len+1))

/*11.6.4 Example descriptors
Example HID descriptor */
static uint16_t USB_HID_StringDescriptor[] = {
USB_STRING_DESCRIPTOR_TYPE(1),
  0x0409,
/* Index 0x04: Manufacturer */
USB_STRING_DESCRIPTOR_TYPE(13),
'N', 'X', 'P',' ','S', 'E', 'M','I', 'C','O','N', 'D',' ',
/* Index 0x20: Product */
USB_STRING_DESCRIPTOR_TYPE(19),
/* bDescriptorType */
'N', 'X', 'P', ' ', 'L', 'P', 'C', '1', '3', 'X','X', ' ',
'H', 'I', 'D', ' ', ' ', ' ', ' ',
/* Index 0x48: Serial Number */
USB_STRING_DESCRIPTOR_TYPE(12),
/* bDescriptorType */
'U', 'N', 'D', 'U', 'L', 'Y', ' ', 'B', 'L', 'A','N', 'D',
/* Index 0x62: Interface 0, Alternate Setting 0 */
USB_STRING_DESCRIPTOR_TYPE(6),
/* bDescriptorType */
'H', 'I', 'D', ' ', ' ', ' ',
};

static const USB_DEVICE_INFO HidInfo{0,USB_HID_StringDescriptor};

void InitializeHid(void){
  UsbDriver.init_clk_pins();
  UsbDriver.init(&HidInfo);

}

HandleInterrupt(47){
  UsbDriver.isr();
}

//in the table but not mentioned by the driver example:
//HandleInterrupt(48){
//  UsbDriver.isr();
//}
