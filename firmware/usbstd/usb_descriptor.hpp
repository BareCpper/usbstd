#pragma once
#include <cstdint>

namespace usbstd
{
#pragma pack(push, 1)

	enum class DescriptorType : uint8_t
	{
		Device = 1,
		Configuration = 2,
		String = 3,
		Interface = 4,
		Endpoint = 5,
		DeviceQualifier = 6,
		OtherSpeedConfiguration = 7,
		InterfacePower = 8,
		Otg = 9,
		Debug = 10,
		InterfaceAssociation = 11,
		BinaryObjectStore = 15,
		DeviceCapability = 16,
		Hid = 33,
		HidReport = 34,
		HidPhysical = 35,
		CsInterface = 36,
	};

	struct DescriptorHeader
	{
		uint8_t bLength;
		DescriptorType bDescriptorType;
	};

	template<DescriptorType>
	struct DescriptorData;

	template<DescriptorType Type>
	struct Descriptor
	{
		DescriptorHeader header = { sizeof(Descriptor<Type>), Type };
		DescriptorData<Type> data;
	};

	template<>
	struct DescriptorData<DescriptorType::String>
	{
		int tempppppp;
		char16_t  bString[];
	};
	using StringDescriptor = Descriptor<DescriptorType::String>;

	template<>
	struct DescriptorData<DescriptorType::Device>
	{
		uint16_t  bcdUSB;
		uint8_t   bDeviceClass;
		uint8_t   bDeviceSubClass;
		uint8_t   bDeviceProtocol;
		uint8_t   bMaxPacketSize0;
		uint16_t  idVendor;
		uint16_t  idProduct;
		uint16_t  bcdDevice;
		uint8_t   iManufacturer;
		uint8_t   iProduct;
		uint8_t   iSerialNumber;
		uint8_t   bNumConfigurations;
	};
	using DeviceDescriptor = Descriptor<DescriptorType::Device>;

	template<>
	struct DescriptorData<DescriptorType::Configuration>
	{
		uint16_t  wTotalLength;
		uint8_t   bNumInterfaces;
		uint8_t   bConfigurationValue;
		uint8_t   iConfiguration;
		uint8_t   bmAttributes;
		uint8_t   bMaxPower;
	};
	using ConfigurationDescriptor = Descriptor<DescriptorType::Configuration>;

	template<>
	struct DescriptorData<DescriptorType::Interface>
	{
		uint8_t   bInterfaceNumber;
		uint8_t   bAlternateSetting;
		uint8_t   bNumEndpoints;
		uint8_t   bInterfaceClass;
		uint8_t   bInterfaceSubClass;
		uint8_t   bInterfaceProtocol;
		uint8_t   iInterface;
	};
	using InterfaceDescriptor = Descriptor<DescriptorType::Interface>;


	template<>
	struct DescriptorData<DescriptorType::Hid>
	{
		uint16_t  bcdHID;
		uint8_t   bCountryCode;
		uint8_t   bNumDescriptors;
		uint8_t   bDescriptorType1;
		uint16_t  wDescriptorLength;
	};
	using HidDescriptor = Descriptor<DescriptorType::Hid>;



#pragma pack(pop)

} //END: usbstd