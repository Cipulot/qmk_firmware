# EC TKL F13

![EC TKL PCB](https://i.imgur.com/jQTHGzBh.png)

Universal TKL Electrostatic Capacitive PCB, with multi-layout support (F13 version).

* Keyboard Maintainer: [cipulot](https://github.com/cipulot)
* Hardware Supported: EC TKL F13 PCB Rev 1.0
* Hardware Availability: TBD

Make example for this keyboard (after setting up your build environment):

    make cipulot/ec_tkl_f13:default

Flashing example for this keyboard:

    make cipulot/ec_tkl_f13:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Physical reset**: Long short the exposed pins on the PCB
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
