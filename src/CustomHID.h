#pragma once

#include "USB/PluggableUSB.h"
#include <HID.h>

#if defined(USBCON)

#define TRANSFER_RELEASE 0x40

class CustomHID_ : public HID_
{
public:
    CustomHID_();
    int SendReport(const void *data, int len);
};

CustomHID_ &CustomHID();

#endif /* if defined(USBCON) */
