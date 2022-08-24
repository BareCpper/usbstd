#pragma once

#include <string_view> //< std::u16string_view

#include "usb_descriptor.hpp" //< usbstd::DescriptorHeader

namespace usbstd {
namespace helper {

    /** Represents table of default (const) String values for USB-Descriptors 
     * @note Dynamic string should define `updateFns`, 
     * @tparam   StringView_t   Type used to store default string values internally 
     * @code
     *   constexpr usbstd::helper::StringTable<2> usbStringTable = { 
     *        0x0409 //< TODO: Document language codes!?
     *       ,{
     *          [USB_STR_MANUFACTURER] = u"Fixed_String"
     *         ,[USB_STR_PRODUCT] = u"Dynamic_String_xxxx"
     *       }
     *       ,{
     *          [USB_STR_MANUFACTURER] = nullptr
     *         ,[USB_STR_PRODUCT] = readProductName
     *       }
     *   };
     * @endcode
    */
    template<size_t Count, typename StringView_t = std::u16string_view>
    struct StringTable
    {
        uint16_t langId;

        /** User-provided string
        */
        StringView_t strings[Count];

        /** [optiona] User-provided string-update function used `StringDescriptorGenerator::generate()`
        */
        uint16_t(*updateFns[Count])(char16_t* const, const uint16_t) = {};
    };

    /** Provides runtime generation of String-descriptors to satify 
    * @code 
    *   static usbstd::helper::StringDescriptorGenerator<usbStringTable> usbStringGenerator = {};
    * 
    *   extern "C" uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
    *   {
    *       return usbStringGenerator.generate(index, langid);
    *   }
    * @endcode
    */
    template< auto& stringTable >
    class StringDescriptorGenerator
    {
    public:
        /** Internal buffer Capacity is based on longest string in the `stringTable`
        */
        static constexpr size_t Capacity = std::max_element(std::begin(stringTable.strings), std::end(stringTable.strings)
            , [](const auto& lhs, const auto& rhs) { return lhs.length() < rhs.length(); })->length();

        /** Runtime call to retrieve a string-scecriptor for the given index and langid
         * @param index 
         * @param langid 
         * @return Pointer to String-descriptor for requested string, or `nullptr` when the Index of language is not supported
         * @warning Generated string-descriptor is tied to the lifetime of resources allocated by StringDescriptorGenerator
         * @todo Only a single Language is currently supported for the `stringTable`, should support alternatives
        */
        const uint16_t* generate(const uint8_t index, const uint16_t langid)
        {
            if (((index != 0) && (langid != stringTable.langId))
                || (index > std::size(stringTable.strings)))
            {
                return nullptr;
            }

            if (index == 0) //< Special case index==0 returns the supported `langId`
            {
                descriptorBuffer_.header.bLength = sizeof(usbstd::DescriptorHeader) + sizeof(stringTable.langId);
                descriptorBuffer_.string[0] = stringTable.langId;
            }
            else
            {
                const auto& string = stringTable.strings[index - 1]; //< @note Strings are 1-base indexed (0 reserved for language-Id)
                const auto defaultLength = string.copy(descriptorBuffer_.string, std::size(descriptorBuffer_.string));
                const auto updateFn = stringTable.updateFns[index - 1];
                const auto updateLength = updateFn ? updateFn(descriptorBuffer_.string, defaultLength) : defaultLength;
                descriptorBuffer_.header.bLength = sizeof(usbstd::DescriptorHeader) + (updateLength * sizeof(string[0]));
            }

            return reinterpret_cast<const uint16_t*>(&descriptorBuffer_);
        }

    private:

        /** Internal string-descirptor buffer sized to store the longest compile-time default string in `stringTable`
        * @note This buffer is updated on each call to `generate(...)`
        */
        struct StringDescriptor
        {
            usbstd::DescriptorHeader header = { sizeof(usbstd::DescriptorHeader), usbstd::DescriptorType::String };
            char16_t string[Capacity] = {};
        } descriptorBuffer_ = {};
    };

} //END: helper
} //END: usbstd