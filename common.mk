# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

define PINFO
PINFO DESCRIPTION = SDL
endef

#===== USEFILE - the file containing the usage message for the application.
USEFILE=

TOUCH_OVERLAY_PATH=$(PROJECT_ROOT)/../tco

TOUCH_OVERLAY_INC_PATH=$(TOUCH_OVERLAY_PATH)/public

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+= \
	$(PROJECT_ROOT)/include  \
	$(TOUCH_OVERLAY_INC_PATH)

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/audio  \
	$(PROJECT_ROOT)/src/audio/nto  \
	$(PROJECT_ROOT)/src/audio/playbook  \
	$(PROJECT_ROOT)/src/cdrom  \
	$(PROJECT_ROOT)/src/cdrom/qnx  \
	$(PROJECT_ROOT)/src/cpuinfo  \
	$(PROJECT_ROOT)/src/events  \
	$(PROJECT_ROOT)/src/file  \
	$(PROJECT_ROOT)/src/joystick  \
	$(PROJECT_ROOT)/src/joystick/dummy  \
	$(PROJECT_ROOT)/src/loadso/dlopen  \
	$(PROJECT_ROOT)/src/main  \
	$(PROJECT_ROOT)/src/main/dummy  \
	$(PROJECT_ROOT)/src/stdlib  \
	$(PROJECT_ROOT)/src/thread  \
	$(PROJECT_ROOT)/src/thread/pthread  \
	$(PROJECT_ROOT)/src/timer  \
	$(PROJECT_ROOT)/src/timer/unix  \
	$(PROJECT_ROOT)/src/video  \
	$(PROJECT_ROOT)/src/video/playbook

#===== CCFLAGS - add the flags to the C compiler command line.
CCFLAGS+=-D__BLACKBERRY__=1 -D__QNXNTO__=1

#===== EXTRA_SILENT_VARIANTS - variants that are not appended to the result binary name (like MyBin_g)
EXTRA_SILENT_VARIANTS+=x86

include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))

