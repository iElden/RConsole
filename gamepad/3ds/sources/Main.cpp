#include <3ds.h>

#include "Keyboard.hpp"
#include "Socket.hpp"
#include "Touch.hpp"
#include "Text.hpp"
#include "Vector2.hpp"
#include "Utils.hpp"

void setStatus(Text &status, std::string s, int color)
{
    status.setColor(color);
    status.setText(s);
}

int main(int ac, char **av)
{
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, nullptr);

    Touch touch;

    Text s_title("RGamepad", {5, 5});
    Text s_setIP("Set client ip:", {10, 5}, {VINT{30, 60}, VINT{150, 90}});
    Text s_setID("Set client id:", {20, 5}, {VINT{30, 140}, VINT{150, 170}});
    Text s_connect("Connect to client", {25, 5}, {VINT{30, 180}, VINT{150, 210}});
    Text s_help("Double-tap on connect if needed.", {26, 5});
    Text s_help2("Select to change combination.", {27, 5});
    //Text s_help3("", {28, 5});


    Text s_ip("0.0.0.0", {10, 24});
    Text s_id("0000", {20, 24});
    Text s_status("Not connected", {25, 24});
    s_status.setColor(31);

    Keyboard k_ip('.');
    Keyboard k_id(0, 5);

    Socket s;

    Text s_error("", {0, 0});
    s_error.setColor(31);

    char msg[16] = {0};

    if (s.error()) {
        s_error.setText(s.getLastError());
    }

    while (aptMainLoop()) {
        // Scan inputs
        touch.scan();

        auto keys = hidKeysHeld() | hidKeysDown() | hidKeysDownRepeat();

        if (keys & KEY_SELECT) {
            msg[0] = 4;
            s.send(msg);
            s.setConnected(false);
            setStatus(s_status, "Not connected", 31);
        }

        // Set input data.
        msg[1] |= ((keys & KEY_UP) != 0) << 0;
        msg[1] |= ((keys & KEY_DOWN) != 0) << 1;
        msg[1] |= ((keys & KEY_LEFT) != 0) << 2;
        msg[1] |= ((keys & KEY_RIGHT) != 0) << 3;
        msg[1] |= ((keys & KEY_A) != 0) << 4;
        msg[1] |= ((keys & KEY_B) != 0) << 5;
        msg[1] |= ((keys & KEY_X) != 0) << 6;
        msg[1] |= ((keys & KEY_Y) != 0) << 7;


        // Display all strings
        s_title.display();
        s_setIP.display();
        s_setID.display();
        s_connect.display();
        s_help.display();
        s_help2.display();
        //s_help3.display();
        s_ip.display();
        s_id.display();
        s_status.display();

        s_error.display();

        // Set touch actions
        if (s_setIP.isTouched(touch.getPosition())) {
            k_ip.spawn([&s_ip](std::string buf) {
                s_ip.setText(buf);
            });
        }
        if (s_setID.isTouched(touch.getPosition())) {
            k_id.spawn([&s_id](std::string buf) {
                s_id.setText(buf);
            });
        }
        if (s_connect.isTouched(touch.getPosition()) && !s.isConnected()) {
            s_error.setText("");
            setStatus(s_status, "Connecting...", 33);
            if (!s.set(s_ip.str(), s_id.str())) {
                s_error.setText(s.getLastError());
                setStatus(s_status, "Not connected", 31);
                s.setConnected(false);
                continue;
            }

            msg[0] = 0;
            if (!s.send(msg)) {
                s_error.setText("snd >> " + s.getLastError());
                setStatus(s_status, "Not connected", 31);
                s.setConnected(false);
                continue;
            }

            auto r = s.receive();
            if (r == nullptr) {
                s_error.setText("rcv >> " + s.getLastError());
                setStatus(s_status, "Not connected", 31);
                s.setConnected(false);
                continue;
            }

            if (r[0] != 1) {
                s_error.setText("invalid response, got (" + std::to_string((int)r[0]) + ") instead of 1");
                setStatus(s_status, "Not connected", 31);
                s.setConnected(false);
                continue;
            }

            s.setConnected(true);
            setStatus(s_status, "Connected", 32);
        }

        if (s.isConnected()) {
            auto r = s.receive();
            if (r == nullptr) {
                s_error.setText("rcv nullptr");
                continue;
            } else {
                s_error.setText("");
            }

            if (r[0] == 4) {
                s.setConnected(false);
                setStatus(s_status, "Not connected", 31);
                continue;
            }

            if (r[0] == 2) {
                msg[0] = 3;
                s.send(msg);
                msg[1] = 0;
            }
        }


        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    msg[0] = 4;
    s.send(msg);

    gfxExit();
    return 0;
}
