APPLICATION = sensors

BOARD ?= iotlab-m3

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ztimer_msec
USEMODULE += lps331ap
USEMODULE += lsm303dlhc
USEMODULE +=isl29020
USEMODULE += l3g4200d

# Add the required modules here


RIOTBASE ?= $(CURDIR)/../../RIOT

include $(RIOTBASE)/Makefile.include
