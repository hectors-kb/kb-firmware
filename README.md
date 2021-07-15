First, one needs to be acquainted with QMK - [this is a good place to start](https://beta.docs.qmk.fm/tutorial/newbs_getting_started)

Once you have a local copy of the repository, put the `...left` and `...right` subfolders into your `crkbd/keymaps/` folder. then compile the  with:

In order to edit the keymap, the `keymap.c` file can be edited as per any other keymap. After making the desired modifications, before compiling, ensure that all the files in the subfolders are identical **except** `rules.mk`. The side that has the trackball must have `PIMORONI_TRACKBALL_ENABLE = yes`, and similarly the other side must have `PIMORONI_TRACKBALL_ENABLE = no`.

The firmware has to be compiled separately for each half with these commands:
```
qmk compile -kb crkbd/rev1/common -km <name-of-subfolder_left>
qmk compile -kb crkbd/rev1/common -km <name-of-subfolder_right>
```
(For more details, see this page from the original creator: 
[https://github.com/greyhatmiddleman/crkbd-pimoroni-trackball](https://github.com/greyhatmiddleman/crkbd-pimoroni-trackball))

The output `*.hex` files can then be flashed using the QMK Toolbox GUI when the controllers are in bootloader mode. Disconnect the keyboard from the PC, then separate the halves. For each half, connect it to the PC and then press the reset button twice quickly to enter bootloader mode. You will then have a few seconds to click "Flash" in QMK Toolbox. (If the timing poses any difficulty, you can try ticking the `Auto-Flash` box instead.)

The `combos.def` file governs the combos on the FrogPad layers. For more details, the [creator's page is here](https://github.com/konomu/qmk_firmware/tree/frogpad/keyboards/crkbd/keymaps/frogpad).
