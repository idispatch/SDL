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

USEFILE=

TOUCH_OVERLAY_PATH = $(PROJECT_ROOT)/../tco
TOUCH_OVERLAY_INC_PATH = $(TOUCH_OVERLAY_PATH)/public

EXTRA_INCVPATH +=$(PROJECT_ROOT)/include $(TOUCH_OVERLAY_INC_PATH)

EXTRA_SRCVPATH += $(PROJECT_ROOT)/src
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/audio
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/audio/nto
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/audio/playbook
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/cdrom
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/cdrom/qnx
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/cpuinfo
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/events
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/file
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/joystick
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/joystick/dummy
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/loadso/dlopen
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/main
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/main/dummy
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/stdlib
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/thread
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/thread/pthread
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/timer
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/timer/unix
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/video
EXTRA_SRCVPATH += $(PROJECT_ROOT)/src/video/playbook

CCFLAGS += -D__BLACKBERRY__ -D__PLAYBOOK__ -D__QNXNTO__

include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)

include $(MKFILES_ROOT)/qtargets.mk
OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
