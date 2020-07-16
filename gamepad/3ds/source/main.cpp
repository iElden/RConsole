#include "main.h"

void display_helper()
{
    Output::printAt("RGamepad", 5, 5);
    Output::printAt("Set client ip: ", 10, 5);
    Output::printAt("Set client id: ", 20, 5);
    Output::printAt("Connect to client", 25, 5);
}

int main(int argc, char **argv) {

    Info info;
    Keyboard kb;
    Socket s;
    touchPosition touch;

    gfxInitDefault();
    //consoleInit(GFX_TOP, NULL);
    consoleInit(GFX_BOTTOM, NULL);
    char buf[16]{'0'};

    atexit(gfxExit);

    while(aptMainLoop()) {

        display_helper();
        info.display();

        hidScanInput();
        auto v = hidKeysDown();

        hidTouchRead(&touch);

        if (touch.px >= 30 && touch.py <= 150 && touch.py >= 60 && touch.py <= 90) {
            kb.spawn(buf, 16, '.', [&info](std::string buf) {
                if (buf.empty())
                    info.setIp("0.0.0.0");
                else
                    info.setIp(buf);
            });
        }

        if (touch.px >= 30 && touch.px <= 150 && touch.py >= 140 && touch.py <= 170) {
            kb.spawn(buf, 5, 0, [&info](std::string buf) {
                if (buf.empty())
                    info.setId("00000");
                else
                    info.setId(buf);
            });
        }

        if (touch.px >= 30 && touch.px <= 150 && touch.py >= 180 && touch.py <= 210) {
            s.tryConnection(info);

            /*info.setStatus(CONNECTED);
        } else {
            info.setStatus(NOT_CONNECTED);*/
        }

        // if (v & KEY_A) {
        //     info.setIp("255.255.255.255");
        // }
        // else if (v & KEY_B) {
        //     info.setId("6500");
        // }
        // else if (v & KEY_Y)
        // {
        //     info.setIp("0.0.0.0");
        // }
        // else if (v & KEY_X)
        // {
        //     info.setId("00000");
        // }

        // if (v & KEY_UP) {
        //     info.setStatus(NOT_CONNECTED);
        // } else if (v & KEY_DOWN) {
        //     info.setStatus(CONNECTING);
        // } else if (v & KEY_LEFT) {
        //     info.setStatus(CONNECTED);
        // }


//        Output::printAt(std::to_string(v), 20, 20);
        if(v & KEY_START) {
            break;
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    return 0;
}
