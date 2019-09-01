#include <lvgl/lvgl.h>
#include <switch.h>

#include "drivers.h"
#include "log.h"
#include "assets.h"
#include "decoder.h"
#include "gui.h"
#include "apps.h"

int main(int argc, char **argv) {
    appletLockExit();

    logInitialize("/hbc.log");

    lv_init();

    Result rc = assetsInit();
    if (R_FAILED(rc)) return rc;

    driversInitialize();
    
    decoderInitialize();

    setup_screen();
    setup_menu();
    setup_misc();

    while (appletMainLoop()) {
        consoleUpdate(NULL);

        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break;

        lv_task_handler();
        svcSleepThread(1e+6L); // Sleep for 1 millisecond
        lv_tick_inc(1);
    }

    assetsExit();
    driversExit();
    logExit();

    appletUnlockExit();

    return 0;
}