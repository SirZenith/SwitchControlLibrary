#include "CustomHID.h"

#if defined(USBCON)

CustomHID_::CustomHID_()
{
}

int CustomHID_::SendReport(const void *data, int len)
{
    auto ret = USBDevice.send(pluggedEndpoint | TRANSFER_RELEASE, data, len);
    return ret;
}

CustomHID_ &CustomHID()
{
    static CustomHID_ obj;
    return obj;
}

#endif /* if defined(USBCON) */
