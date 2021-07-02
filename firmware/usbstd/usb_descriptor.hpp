#pragma once
#include <cstdint>

#include "usb_class.hpp"

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
		InterfaceAssociation = 11, ///< USB Interface Association Descriptor (IAD ECN)
		BinaryObjectStore = 15,
		DeviceCapability = 16,

		// Class Specific Descriptor
		CsDevice = 33,
		CsConfiguration = 34,
		CsString = 35,
		CsInterface = 36,
		CsEndpoint = 37,

#if 0//< @todo?
		Functional = 33,

		Hid = CsDevice,
		HidReport = CsConfiguration,
		HidPhysical = CsString,
#endif
	};

	struct DescriptorHeader
	{
		uint8_t bLength;
		DescriptorType bDescriptorType;
	};

	template<DescriptorType>
	struct DescriptorData;

	template<DescriptorType Type, typename DescriptorData_t = DescriptorData<Type> >
	struct Descriptor
	{
		DescriptorHeader header = { sizeof(Descriptor<Type>), Type };
		DescriptorData_t data;
	};

	/// @{ Class Specific
	template< DescriptorType Type, auto ClassSubType >
	struct SubTypeDescriptorData;

	template< DescriptorType Type, auto ClassSubType, typename DescriptorData_t = SubTypeDescriptorData<Type, ClassSubType> >
	struct SubTypeDescriptor
	{
		DescriptorHeader header = { sizeof(SubTypeDescriptor), Type };
		uint8_t bDescriptorSubType = { ClassSubType }; ///< Descriptor SubType
		DescriptorData_t data;
	};

	template<  auto ClassSubType >
	using CsDeviceDescriptor = SubTypeDescriptor<DescriptorType::CsDevice, ClassSubType>;

	template<  auto ClassSubType >
	using CsConfigurationDescriptor = SubTypeDescriptor<DescriptorType::CsConfiguration, ClassSubType>;

	template<  auto ClassSubType >
	using CsInterfaceDescriptor = SubTypeDescriptor<DescriptorType::CsInterface, ClassSubType>;

	template<  auto ClassSubType >
	using CsEndpointDescriptor = SubTypeDescriptor<DescriptorType::CsEndpoint, ClassSubType>;

	template< auto ClassSubType >
	using CsDeviceDescriptor = SubTypeDescriptor<DescriptorType::CsDevice, ClassSubType>;
	///@}

	using StringDescriptor = Descriptor<DescriptorType::String, char16_t[] >;

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
	static_assert(sizeof(DeviceDescriptor) == 18, "size is not correct");

	template<>
	struct DescriptorData<DescriptorType::Configuration>
	{
		uint16_t wTotalLength; ///< Total length of data returned for this configuration. Includes the combined length of all descriptors (configuration, interface, endpoint, and class- or vendor-specific) returned for this configuration.

		uint8_t  bNumInterfaces; ///< Number of interfaces supported by this configuration
		uint8_t  bConfigurationValue; ///< Value to use as an argument to the SetConfiguration() request to select this configuration.
		uint8_t  iConfiguration; ///< Index of string descriptor describing this configuration
		uint8_t  bmAttributes; /**< Configuration characteristics 
								* - D7: Reserved (set to one)
								* - D6: Self-powered
								* - D5: Remote Wakeup 
								* - D4...0: Reserved (reset to zero)
								* D7 is reserved and must be set to one for historical reasons. 
								* A device configuration that uses power from the bus and a local source reports a non-zero value in bMaxPower to indicate the amount of bus power required and sets D6. 
								* The actual power source at runtime may be determined using the GetStatus(DEVICE) request (see USB 2.0 spec Section 9.4.5). 
								* If a device configuration supports remote wakeup, D5 is set to one.
								*/
		uint8_t  bMaxPower; ///< Maximum power consumption of the USB device from the bus in this specific configuration when the device is fully operational. Expressed in 2 mA units (i.e., 50 = 100 mA).
	};
	using ConfigurationDescriptor = Descriptor<DescriptorType::Configuration>;
	static_assert(sizeof(ConfigurationDescriptor) == 9, "size is not correct");

	template<>
	struct DescriptorData<DescriptorType::Interface>
	{
		uint8_t  bInterfaceNumber; ///< Number of this interface. Zero-based value identifying the index in the array of concurrent interfaces supported by this configuration.
		uint8_t  bAlternateSetting; ///< Value used to select this alternate setting for the interface identified in the prior field
		uint8_t  bNumEndpoints; ///< Number of endpoints used by this interface (excluding endpoint zero). If this value is zero, this interface only uses the Default Control Pipe.
		uint8_t  bInterfaceClass; /**< Class code(assigned by the USB - IF).
									* \li A value of zero is reserved for future standardization. 
									* \li If this field is set to FFH, the interface class is vendor-specific. 
									* \li All other values are reserved for assignment by the USB-IF.
									*/
		uint8_t  bInterfaceSubClass; /**< Subclass code (assigned by the USB-IF).
									* These codes are qualified by the value of the bInterfaceClass field. 
									* \li If the bInterfaceClass field is reset to zero, this field must also be reset to zero.
									* \li If the bInterfaceClass field is not set to FFH, all values are reserved for assignment by the USB-IF.
									*/
		uint8_t  bInterfaceProtocol; /**< Protocol code (assigned by the USB). 
									 * These codes are qualified by the value of the bInterfaceClass and the bInterfaceSubClass fields.
									 * If an interface supports class-specific requests, this code identifies the protocols that the device uses as defined by the specification of the device class. 
									 * \li If this field is reset to zero, the device does not use a class-specific protocol on this interface. 
									 * \li If this field is set to FFH, the device uses a vendor-specific protocol for this interface.
									 */
		uint8_t  iInterface; ///< Index of string descriptor describing this interface
	};
	using InterfaceDescriptor = Descriptor<DescriptorType::Interface>;


	/**
	 * @see Refer to Chapter 5 of USB 2.0 specification for more information.
	*/
	template<>
	struct DescriptorData<DescriptorType::Endpoint>
	{
		uint8_t  bEndpointAddress; ///< The address of the endpoint on the USB device described by this descriptor. The address is encoded as follows: \n Bit 3...0: The endpoint number \n Bit 6...4: Reserved, reset to zero \n Bit 7: Direction, ignored for control endpoints 0 = OUT endpoint 1 = IN endpoint.

		struct Attributes {
			uint8_t xfer : 2; /**<Bits 1..0: Transfer Type 
								*  - 00 = Control 
								*  - 01 = Isochronous 
								*  - 10 = Bulk 
								*  - 11 = Interrupt 
								*/
			uint8_t sync : 2; /**< If isochronous, they are defined as follows: 
								* Bits 3..2: Synchronization Type 
								*  - 00 = No Synchronization 
								*  - 01 = Asynchronous 
								*  - 10 = Adaptive 
								*  - 11 = Synchronous 
							    * @warning If not an isochronous endpoint, bits 5..2 are reserved and must be set to zero.
							    */
			uint8_t usage : 2;/**< If isochronous, they are defined as follows: 
								* Bits 5..4: Usage Type 
								*  - 00 = Data endpoint 
								*  - 01 = Feedback endpoint 
								*  - 10 = Implicit feedback Data endpoint 
								*  - 11 = Reserved 
							    * @warning If not an isochronous endpoint, bits 5..2 are reserved and must be set to zero.
							    */
			uint8_t : 2; //< All other bits are reserved and must be reset to zero. Reserved bits must be ignored by the host.
		} bmAttributes; ///< This field describes the endpoint's attributes when it is configured using the bConfigurationValue. 
		
		struct MaxPacketSize {
			uint16_t size : 11; /**< Maximum packet size this endpoint is capable of sending or receiving when this configuration is selected. 
								* For isochronous endpoints, this value is used to reserve the bus time in the schedule, required for the per-(micro)frame data payloads.
								* The pipe may, on an ongoing basis, actually use less bandwidth than that reserved. 
								* The device reports, if necessary, the actual bandwidth used via its normal, non-USB defined mechanisms. 
								* For all endpoints, bits 10..0 specify the maximum packet size (in bytes).
								*/
			uint16_t hs_period_mult : 2;/**< For high-speed isochronous and interrupt endpoints: \
								*   Bits 12..11 specify the number of additional transaction opportunities per microframe: 
								*  - 00 = None (1 transaction per microframe) 
								* - 01 = 1 additional (2 per microframe) 
								* - 10 = 2 additional (3 per microframe) 
								* - 11 = Reserved 
								*/
			uint16_t : 3; ///< Bits 15..13 are reserved and must be set to zero
		} wMaxPacketSize;

		uint8_t  bInterval; /**< Interval for polling endpoint for data transfers.Expressed in frames or microframes depending on the device operating speed (i.e., either 1 millisecond or 125 us units).
							* - For full-/high-speed isochronous endpoints, this value must be in the range from 1 to 16. 
							*   The bInterval value is used as the exponent for a \f$ 2^(bInterval-1) \f$ value; e.g., a bInterval of 4 means a period of 8 (\f$ 2^(4-1) \f$). 
							* - For full-/low-speed interrupt endpoints, the value of this field may be from 1 to 255. 
							* - For high-speed interrupt endpoints, the bInterval value is used as the exponent for a \f$ 2^(bInterval-1) \f$ value; 
							*   e.g., a bInterval of 4 means a period of 8 (\f$ 2^(4-1) \f$) . This value must be from 1 to 16. 
							* - For high-speed bulk/control OUT endpoints, the bInterval must specify the maximum NAK rate of the endpoint. 
							*   A value of 0 indicates the endpoint never NAKs. Other values indicate at most 1 NAK each bInterval number of microframes. 
							*   This value must be in the range from 0 to 255. 
							*/
	};
	using EndpointDescriptor = Descriptor<DescriptorType::Endpoint>;

	template<>
	struct DescriptorData<DescriptorType::InterfaceAssociation>
	{
		uint8_t bFirstInterface; ///< Index of the first associated interface.
		uint8_t bInterfaceCount; ///< Total number of associated interfaces.

		uint8_t bFunctionClass; ///< Interface class ID.
		uint8_t bFunctionSubClass; ///< Interface subclass ID.
		uint8_t bFunctionProtocol; ///< Interface protocol ID.

		uint8_t iFunction; ///< Index of the string descriptor describing the interface association.
	};
	using InterfaceAssociationDescriptor = Descriptor<DescriptorType::InterfaceAssociation>;

#if 0
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
#endif


#pragma pack(pop)

} //END: usbstd