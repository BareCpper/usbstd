cmake_minimum_required(VERSION 3.18)

include(GitVersion)
project ("usbstd"
  VERSION ${PROJECT_SEMANTICVERSION} )
  
add_library(usbstd)
target_compile_features( usbstd PUBLIC cxx_std_17 )
target_include_directories(usbstd PUBLIC ".")

target_sources(usbstd 
    PRIVATE 
        "usbstd.cpp"
    PUBLIC 
        "usbstd.hpp" "usb_bos.hpp"  "usb_descriptor.hpp" "usb_cdc.hpp" "usb_request.hpp" )
        