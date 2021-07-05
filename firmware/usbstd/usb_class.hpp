#pragma once
#include <cstdint>

namespace usbstd
{
#pragma pack(push, 1)

	//https://www.usb.org/defined-class-codes
	enum class ClassCode : uint8_t
	{
		Unspecified = 0,
		Audio = 1,
		Cdc = 2,
		Hid = 3,
		//Reserved_4 = 4,
		Physical = 5,
		Image = 6,
		Printer = 7,
		Msc = 8,
		Hub = 9,
		CdcData = 10,
		SmartCard = 11,
		//Reserved_12 = 12,
		ContentSecurity = 13,
		Video = 14,
		PersonalHealthcare = 15,
		AudioVideo = 16,

		Diagnostic = 0xdc,
		WirelessController = 0xe0,
		Misc = 0xef,
		ApplicationSpecific = 0xfe,
		VendorSpecific = 0xff
	};

#pragma pack(pop)
} //END: usbstd