#pragma once
#include <cstdint>

namespace usbstd
{
#pragma pack(push, 1)

    typedef enum
    {
        MS_OS_20_SET_HEADER_DESCRIPTOR = 0x00,
        MS_OS_20_SUBSET_HEADER_CONFIGURATION = 0x01,
        MS_OS_20_SUBSET_HEADER_FUNCTION = 0x02,
        MS_OS_20_FEATURE_COMPATBLE_ID = 0x03,
        MS_OS_20_FEATURE_REG_PROPERTY = 0x04,
        MS_OS_20_FEATURE_MIN_RESUME_TIME = 0x05,
        MS_OS_20_FEATURE_MODEL_ID = 0x06,
        MS_OS_20_FEATURE_CCGP_DEVICE = 0x07,
        MS_OS_20_FEATURE_VENDOR_REVISION = 0x08
    } microsoft_os_20_type_t;

    // USB Binary Device Object Store (BOS)
    // https://developers.google.com/web/fundamentals/native-hardware/build-for-webusb/
    struct usb_desc_bos_platform_t
    {
        uint8_t bLength; ///< Size of this descriptor
        uint8_t bDescriptorType; ///<  = USB_DEVICE_CAPABILITY_DESCRIPTOR Device capability descriptor
        uint8_t bDevCapabilityType; ///<  = 0x05 Platform capability descriptor
        uint8_t bReserved; ///< = 0x00;
        uint8_t platformCapabilityUUID[16]; ///< WebUSB platform capability descriptor GUID in little-endian format
        //uint8_t capabilityData[];
    };

    struct usb_bos_descriptor_header_t
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t wTotalLength;
        uint8_t bNumDeviceCaps;
    };

    struct usb_bos_ms_os_20_capability_t
    {
        uint32_t dwWindowsVersion; ///< = 0x06030000 Windows version - Minimum Windows version (8.1) (0x06030000)
        uint16_t wMSOSDescriptorSetTotalLength; ///< The length, in bytes of the MS OS 2.0 descriptor set
        uint8_t bMS_VendorCode; ///< Vendor defined code to use to retrieve this version of the MS OS 2.0 descriptor and also to set alternate enumeration behavior on the device.
        uint8_t bAltEnumCode; ///< = 0x00 A non-zero value to send to the device to indicate that the device may return non-default USB descriptors for enumeration.  If the device does not support alternate enumeration, this value shall be 0.
    };

    struct usb_bos_ms_os_20_descriptor_t
    {
        usb_desc_bos_platform_t platform;
        usb_bos_ms_os_20_capability_t capability;
    };

    ///Microsoft OS 2.0 descriptor set header
    struct usb_ms_os_20_descriptor_set_header_t
    {
        uint16_t wLength; ///< The length, in bytes, of this header. Shall be set to 10.
        uint16_t wDescriptorType; ///< MSOS20_SET_HEADER_DESCRIPTOR
        uint32_t dwWindowsVersion; ///< Windows version.
        uint16_t wTotalLength; ////< The size of entire MS OS 2.0 descriptor set. The value shall match the value in the descriptor set information structure.  
    };

    ///Microsoft OS 2.0 configuration subset header
    struct usb_ms_os_20_configuration_subset_header_t
    {
        uint16_t wLength; ///< The length, in bytes, of this subset header. Shall be set to 8.
        uint16_t wDescriptorType; ///< MS_OS_20_SUBSET_HEADER_CONFIGURATION
        uint8_t bConfigurationValue; ///< The configuration value for the USB configuration to which this subset applies
        uint8_t bReserved; ///< Shall be set to 0.
        uint16_t wTotalLength; ////< The size of entire MS OS 2.0 descriptor set. The value shall match the value in the descriptor set information structure.  
    } usb_ms_os_20_configuration_subset_header_t;

    /// Microsoft OS 2.0 function subset header
    struct usb_ms_os_20_function_subset_header_t
    {
        uint16_t wLength; ///< The length, in bytes, of this subset header. Shall be set to 8.
        uint16_t wDescriptorType; ///< MS_OS_20_SUBSET_HEADER_FUNCTION
        uint8_t bFirstInterface; ///< The interface number for the first interface of the function to which this subset applies.
        uint8_t bReserved; ///< Shall be set to 0.
        uint16_t wSubsetLength; ////< The size of entire function subset including this header.
    };

    /// Microsoft OS 2.0 compatible ID descriptor
    struct usb_ms_os_20_compatible_id_descriptor_t
    {
        uint16_t wLength; ///< The length, bytes, of the compatible ID descriptor including value descriptors. Shall be set to 20.
        uint16_t wDescriptorType; ///< MS_OS_FEATURE_COMPATIBLE_ID
        uint8_t compatibleID[8]; ///< Compatible ID String
        uint8_t subCompatibleID[8]; ///< Sub-compatible ID String
    };

    /// MS OS 2.0 Registry property descriptor: length, type
    struct usb_ms_os_20_device_interface_guid_section_t
    {
        uint16_t wLength; ///< The length, in bytes, of this descriptor.
        uint16_t wDescriptorType; ///< MS_OS_20_FEATURE_REG_PROPERTY
        uint16_t  wPropertyDataType; ///< The type of registry property. See Table 15.
        uint16_t  wPropertyNameLength; ///< The length of the property name.
        uint16_t  bPropertyName[20];
        uint16_t dwPropertyDataLength; ///< The length of property data.
        uint16_t  bPropertyData[38];
    };

    struct usb_bos_webusb_capability_t
    {
        uint16_t bcdVersion;/// = 0x0100 WebUSB descriptor version 1.0
        uint8_t bVendorCode;///	= 0x01 bRequest value for WebUSB
        uint8_t iLandingPage;/// = 0x01 URL for landing page
    };

    struct usb_bos_webusb_descriptor_t
    {
        usb_desc_bos_platform_t platform;
        usb_bos_webusb_capability_t capability;
    };

    // USB WebuSB URL Descriptor
    struct usb_desc_webusb_url_t
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bScheme;
        char    url[];
    }; 

#pragma pack(pop)

} //END: usbstd