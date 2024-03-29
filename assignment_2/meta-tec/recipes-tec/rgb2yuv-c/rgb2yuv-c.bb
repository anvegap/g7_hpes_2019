DESCRIPTION = "rgb2yuv using C"
SECTION = "rgb2yuv-c"
DEPENDS = ""
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=96af5705d6f64a88e035781ef00e98a8"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

SRCREV = "40c6e817eb62f3a82d328dfcec940770aa146c23"
SRC_URI = "file://rgb2yuv-c.c \
	   file://LICENSE \
	   file://Makefile.am \
	   file://configure.ac \
	   file://input.rgb \ 
	   file://output_c.yuv \   
	   file://c_results.pdf \  
"
S = "${WORKDIR}"

do_install_append() {
	install -d ${D}/rgb2yuv-c-docs
	cp ${WORKDIR}/rgb2yuv-c.c ${D}/rgb2yuv-c-docs
	cp ${WORKDIR}/input.rgb ${D}/rgb2yuv-c-docs
	cp ${WORKDIR}/output_c.yuv ${D}/rgb2yuv-c-docs
	cp ${WORKDIR}/c_results.pdf ${D}/rgb2yuv-c-docs
}

FILES_${PN} += "/rgb2yuv-c-docs" 

inherit autotools

# The autotools configuration I am basing this on seems to have a problem with a race condition when parallel make is enabled
PARALLEL_MAKE = ""

