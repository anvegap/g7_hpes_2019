SUMMARY = "An image with some C/C++ dev tools that inherits from console image"

require ../../../meta-rpi/images/console-image.bb

IMAGE_INSTALL += "rgb2yuv-c rgb2yuv-intrinsics"

export IMAGE_BASENAME = "tec-image"
