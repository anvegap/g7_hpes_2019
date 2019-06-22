SUMMARY = "An image with some C/C++ dev tools that inherits from console image"

require ../../../meta-rpi/images/console-image.bb

#IMAGE_INSTALL += " \
#    ${CORE_OS} \
#    ${DEV_SDK_INSTALL} \
#    ${DEV_EXTRAS} \
#    ${EXTRA_TOOLS_INSTALL} \
#    ${RPI_STUFF} \
#    ${WIFI_SUPPORT} \
#"

export IMAGE_BASENAME = "tec-image"
