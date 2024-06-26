# coral_in_tree_WS2812B_lib


## Setup

For this build we are using the In-Tree method described [here](https://coral.ai/docs/dev-board-micro/freertos/#freertos-tasks).
Once you have setup the coral micro directory, then you can clone this repo into the `apps` directory.

For precise timing, you'll need to add the following to line`${PROJECT_SOURCE_DIR}/third_party/nxp/rt1176-sdk/devices/MIMXRT1176/drivers/fsl_qtmr.c` to the CMakeLists.txt file in the coral micro directory`libs/nxp/rt1176-sdk/CMakeLists.txt`.


## Build the application

Once your code is setup and you have updated the [CMakeLists.txt](CMakeLists.txt) file, you can build the application by running the following commands:

```bash
bash build.sh
```

## Upload the application

To upload the application to the Coral Dev Board, you can run the following command:

```bash
python3 scripts/flashtool.py --app coral_in_tree_WS2812B_lib
```

## Run the application

The serial output can be viewed by running the following command, where the `dev` path depends on your board:
```bash
screen /dev/serial/by-id/usb-Google_Coral_Dev_Board_Micro_XXX 9600
```

