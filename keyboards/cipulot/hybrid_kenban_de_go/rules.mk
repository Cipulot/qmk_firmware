CUSTOM_MATRIX = lite
ANALOG_DRIVER_REQUIRED = yes
QUANTUM_PAINTER_DRIVERS += sh1106_i2c
VIA_INSECURE = yes

SRC += matrix.c
SRC += hybrid_switch_matrix.c
ifeq ($(strip $(QUANTUM_PAINTER_ENABLE)), yes)
    SRC += graphics/display.c
    SRC += graphics/splash_kenban_de_go.qgf.c
    SRC += graphics/splash_cipulot.qgf.c
    SRC += graphics/densha.qgf.c
endif

MCUFLAGS += -march=armv7e-m \
            -mcpu=cortex-m4 \
            -mfloat-abi=hard \
            -mfpu=fpv4-sp-d16 \
            -mabi=aapcs \
            -mcmodel=small \
            -mstrict-align \
            -mthumb

OPT_DEFS += -Ofast -ffast-math -funroll-loops \
            -fno-tree-vectorize -fno-signed-zeros -fno-math-errno \
            -fno-common -fomit-frame-pointer -falign-functions=16 \
            -falign-loops=16 -falign-jumps=16 -fno-exceptions -fno-unwind-tables
