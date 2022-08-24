#pragma once
#include <cstdint>

namespace usbstd
{
#pragma pack(push, 1)

	struct Request
	{
		uint8_t   bmRequestType;
		uint8_t   bRequest;
		uint16_t  wValue;
		uint16_t  wIndex;
		uint16_t  wLength;
	};


#pragma pack(pop)

} //END: usbstd