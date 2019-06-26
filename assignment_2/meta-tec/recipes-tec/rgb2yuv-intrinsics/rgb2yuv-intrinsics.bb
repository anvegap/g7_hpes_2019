DESCRIPTION = "rgb2yuv using intrinsics"
SECTION = "rgb2yuv-intrinsics"
DEPENDS = ""
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=96af5705d6f64a88e035781ef00e98a8"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

SRCREV = "40c6e817eb62f3a82d328dfcec940770aa146c23"
SRC_URI = "file://rgb2yuv-intrinsics.c \
	   file://LICENSE \
	   file://Makefile.am \
	   file://configure.ac \ 
	   file://documentation-example.txt \   
"
S = "${WORKDIR}"

do_install_append() {
	install -d ${D}/rgb2yuv-intrinsics-docs
	cp ${WORKDIR}/documentation-example.txt ${D}/rgb2yuv-intrinsics-docs
}

FILES_${PN} += "/rgb2yuv-intrinsics-docs" 

inherit autotools

# The autotools configuration I am basing this on seems to have a problem with a race condition when parallel make is enabled
PARALLEL_MAKE = ""

