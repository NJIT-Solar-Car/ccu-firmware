#include "Usb/usb_cdc.h"
#include "usbd_cdc_if.h"

void usb_cdc_write(void *data, size_t len)
{
    while (CDC_Transmit_FS(data, len) == USBD_BUSY) { /* wait */ }

    return;
}
