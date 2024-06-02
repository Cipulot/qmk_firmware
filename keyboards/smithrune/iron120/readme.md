# IRON120

![1IRON120](https://i.imgur.com/718GRmF.png)

The IRON120 is a numpad designed by [Smith and Rune](https://smithrune.com/).

* Keyboard Maintainer: [cipulot](https://github.com/cipulot)
* Hardware Supported: IRON120
* Hardware Availability: [Smith and Rune](https://smithrune.com/)

Make example for this keyboard (after setting up your build environment):

    make smithrune/iron120:default

Flashing example for this keyboard:

    make smithrune/iron120:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is configured.
* **Physical reset button**: Long press the reset button soldered on the PCB.
* **Bootmagic reset**: Hold down the top left key and plug in the controller.
