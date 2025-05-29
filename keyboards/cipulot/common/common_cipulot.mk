CUSTOM_MATRIX = lite
ANALOG_DRIVER_REQUIRED = yes
VPATH += keyboards/cipulot/common
SRC += matrix.c ec_board.c ec_switch_matrix.c

ifeq ($(strip $(BETA)), yes)
    SRC += beta_ec_switch_matrix.c
else
	SRC += ec_switch_matrix.c
endif

ifeq ($(strip $(VIA_ENABLE)), yes)
    SRC += via_ec.c
endif
