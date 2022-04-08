#pragma once
#include <cstdint>

#include "usb_class.hpp"
#include "usb_request.hpp"
#include "usb_descriptor.hpp"

namespace usbstd
{
#pragma pack(push, 1)


	enum
	{
		USB_CDC_SEND_ENCAPSULATED_COMMAND = 0x00,
		USB_CDC_GET_ENCAPSULATED_RESPONSE = 0x01,
		USB_CDC_SET_COMM_FEATURE = 0x02,
		USB_CDC_GET_COMM_FEATURE = 0x03,
		USB_CDC_CLEAR_COMM_FEATURE = 0x04,
		USB_CDC_SET_AUX_LINE_STATE = 0x10,
		USB_CDC_SET_HOOK_STATE = 0x11,
		USB_CDC_PULSE_SETUP = 0x12,
		USB_CDC_SEND_PULSE = 0x13,
		USB_CDC_SET_PULSE_TIME = 0x14,
		USB_CDC_RING_AUX_JACK = 0x15,
		USB_CDC_SET_LINE_CODING = 0x20,
		USB_CDC_GET_LINE_CODING = 0x21,
		USB_CDC_SET_CONTROL_LINE_STATE = 0x22,
		USB_CDC_SEND_BREAK = 0x23,
		USB_CDC_SET_RINGER_PARMS = 0x30,
		USB_CDC_GET_RINGER_PARMS = 0x31,
		USB_CDC_SET_OPERATION_PARMS = 0x32,
		USB_CDC_GET_OPERATION_PARMS = 0x33,
		USB_CDC_SET_LINE_PARMS = 0x34,
		USB_CDC_GET_LINE_PARMS = 0x35,
		USB_CDC_DIAL_DIGITS = 0x36,
		USB_CDC_SET_UNIT_PARAMETER = 0x37,
		USB_CDC_GET_UNIT_PARAMETER = 0x38,
		USB_CDC_CLEAR_UNIT_PARAMETER = 0x39,
		USB_CDC_GET_PROFILE = 0x3a,

		USB_CDC_NOTIFY_RING_DETECT = 0x09,
		USB_CDC_NOTIFY_SERIAL_STATE = 0x20,
		USB_CDC_NOTIFY_CALL_STATE_CHANGE = 0x28,
		USB_CDC_NOTIFY_LINE_STATE_CHANGE = 0x29,
	};

	enum
	{
		USB_CDC_1_STOP_BIT = 0,
		USB_CDC_1_5_STOP_BITS = 1,
		USB_CDC_2_STOP_BITS = 2,
	};

	enum
	{
		USB_CDC_NO_PARITY = 0,
		USB_CDC_ODD_PARITY = 1,
		USB_CDC_EVEN_PARITY = 2,
		USB_CDC_MARK_PARITY = 3,
		USB_CDC_SPACE_PARITY = 4,
	};

	enum
	{
		USB_CDC_5_DATA_BITS = 5,
		USB_CDC_6_DATA_BITS = 6,
		USB_CDC_7_DATA_BITS = 7,
		USB_CDC_8_DATA_BITS = 8,
		USB_CDC_16_DATA_BITS = 16,
	};

	enum
	{
		USB_CDC_DEVICE_CLASS = 2,  // USB Communication Device Class
		USB_CDC_COMM_CLASS = 2,  // CDC Communication Class Interface
		USB_CDC_DATA_CLASS = 10, // CDC Data Class Interface
	};

	enum class CdcSubClass
	{
		None = 0, // No Subclass
		Dlcm = 1, // Direct Line Control Model
		Acm = 2, // Abstract Control Model
		Tcm = 3, // Telephone Control Model
		Mccm = 4, // Multi-Channel Control Model
		Ccm = 5, // CAPI Control Model
		Eth = 6, // Ethernet Networking Control Model
		Atm = 7, // ATM Networking Control Model
	};
	
	enum class CdcProtocol
	{
		None = 0x00, ///< No specific protocol
		At_Itu = 0x01, ///< AT Commands: V.250 etc
		At_Pcca101 = 0x02, ///< AT Commands defined by PCCA-101
		At_Pcca101_A = 0x03, ///< AT Commands defined by PCCA-101 & Annex O
		At_Gsm = 0x04, ///< AT Commands defined by GSM 07.07
		At_3gpp = 0x05, ///< AT Commands defined by 3GPP 27.007
		At_Cs = 0x06, ///< AT Commands defined by TIA for CDMA
		Eem = 0x07,   ///< Ethernet Emulation Model
		Ext = 0xFE,   ///< External protocol
		Vendor = 0xFF   ///< Vendor-specific
	};

	// USB CDC Call Management Capabilities
	enum CdcCallManagmentCapabilities
	{
		Supported = (1 << 0),
		OverDci = (1 << 1),
	};

	// USB CDC ACM Capabilities
	enum CdcAcmRequestCapabilities
	{	
		Feature = (1 << 0),// Device supports the request combination of Set_Comm_Feature,  Clear_Comm_Feature, and Get_Comm_Feature.	
		Line = (1 << 1),// Device supports the request combination of Set_Line_Coding, Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State.	
		SendBreak = (1 << 2),// Device supports the request Send_Break.	
		Notify = (1 << 3),// Device supports the notification Network_Connection.
	};

	enum class CdcCtrlSignal
	{
		DtePresent = (1 << 0), // DTR
		ActivateCarrier = (1 << 1), // RTS
	};

	enum class CdcState
	{
		Dcd = (1 << 0),
		Dsr = (1 << 1),
		Break = (1 << 2),
		Ring = (1 << 3),
		Framing = (1 << 4),
		Parity = (1 << 5),
		Overrun = (1 << 6),
	};

	enum class CdcDescriptorSubType : uint8_t
	{
		Header = 0, // Header Functional Descriptor
		CallManagement = 1, // Call Management
		Acm = 2, // Abstract Control Management
		Union = 6, // Union Functional Descriptor

		/// @todo TinyUsb cdc_func_desc_type_t for full list?
	};

	template<>
	struct SubTypeDescriptorData<DescriptorType::CsInterface, CdcDescriptorSubType::Header>
	{
		uint16_t  bcdCDC;
	};

	template<>
	struct SubTypeDescriptorData<DescriptorType::CsInterface, CdcDescriptorSubType::Acm>
	{
		uint8_t   bmCapabilities;
	};

	template<>
	struct SubTypeDescriptorData<DescriptorType::CsInterface, CdcDescriptorSubType::CallManagement>
	{
		uint8_t   bmCapabilities;
		uint8_t   bDataInterface;
	};

	template<>
	struct SubTypeDescriptorData<DescriptorType::CsInterface, CdcDescriptorSubType::Union>
	{
		uint8_t   bMasterInterface;
		uint8_t   bSlaveInterface0;
	};

	namespace cdc
	{
		using HeaderDescriptor = CsInterfaceDescriptor<CdcDescriptorSubType::Header>;
		using CallDescriptor = CsInterfaceDescriptor<CdcDescriptorSubType::CallManagement>;
		using AcmDescriptor = CsInterfaceDescriptor<CdcDescriptorSubType::Acm>;
		using UnionDescriptor = CsInterfaceDescriptor<CdcDescriptorSubType::Union>;
	} //< END: cdc

	struct usb_cdc_line_coding_t
	{
		uint32_t  dwDTERate;
		uint8_t   bCharFormat;
		uint8_t   bParityType;
		uint8_t   bDataBits;
	} ;

	struct usb_cdc_notify_serial_state_t
	{
		Request request;
		uint16_t value;
	} ;

#pragma pack(pop)
} //END: usbstd