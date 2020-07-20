#include <3ds.h>
#include "Client3ds.hpp"
#include "Utils.hpp"

Client3DS::Client3DS()
{
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, nullptr);

    this->s_status.setColor(Color::RED);
    this->s_error.setColor(Color::RED);
}

Client3DS::~Client3DS()
{
    gfxExit();
}

void Client3DS::setStatus(std::string s, int color)
{
    this->s_status.setColor(color);
    this->s_status.setText(s);
}

void Client3DS::displayStrings()
{
    this->s_error.display();
    this->s_title.display();

    this->s_setIP.display();
    this->s_ip.display();
    this->s_setID.display();
    this->s_id.display();

    this->s_connect.display();
    this->s_status.display();

    this->s_help.display();
    this->s_help2.display();
}

void Client3DS::run()
{
    if (this->network.error()) {
        this->s_error.setText(this->network.getLastError());
    }

    while (aptMainLoop()) {
        this->touch.scan();

        auto keys = hidKeysHeld() | hidKeysDown() | hidKeysDownRepeat();

        if (keys & KEY_SELECT) {
            this->packet[0] = Opcodes::GOODBYE;
            network.send(this->packet);
            network.setConnected(false);
            this->setStatus("Not connected", Color::RED);
        }

        // Set input data.
        this->packet[1] |= ((keys & KEY_UP) != 0) << 0;
        this->packet[1] |= ((keys & KEY_DOWN) != 0) << 1;
        this->packet[1] |= ((keys & KEY_LEFT) != 0) << 2;
        this->packet[1] |= ((keys & KEY_RIGHT) != 0) << 3;
        this->packet[1] |= ((keys & KEY_A) != 0) << 4;
        this->packet[1] |= ((keys & KEY_B) != 0) << 5;
        this->packet[1] |= ((keys & KEY_X) != 0) << 6;
        this->packet[1] |= ((keys & KEY_Y) != 0) << 7;


        this->displayStrings();

        // Set touch actions
        if (this->s_setIP.isTouched(touch.getPosition())) {
            k_ip.spawn([this](std::string buf) {
                this->s_ip.setText(buf);
            });
        }
        if (this->s_setID.isTouched(touch.getPosition())) {
            k_id.spawn([this](std::string buf) {
                this->s_id.setText(buf);
            });
        }
        if (this->s_connect.isTouched(touch.getPosition()) && !network.isConnected()) {
            this->s_error.setText("");
            this->setStatus("Connecting...", Color::YELLOW);
            if (!network.set(this->s_ip.str(), this->s_id.str())) {
                this->s_error.setText(this->network.getLastError());
                this->setStatus("Not connected", Color::RED);
                network.setConnected(false);
                continue;
            }

            packet[0] = Opcodes::HELLO;
            if (!network.send(this->packet)) {
                this->s_error.setText("snd >> " + network.getLastError());
                this->setStatus("Not connected", Color::RED);
                network.setConnected(false);
                continue;
            }

            auto r = network.receive();
            if (r == nullptr) {
                this->s_error.setText("rcv >> " + network.getLastError());
                this->setStatus("Not connected", Color::RED);
                network.setConnected(false);
                continue;
            }

            if (r[0] != Opcodes::OLLEH) {
                this->s_error.setText("invalid response, got (" + std::to_string((int)r[0]) + ") instead of " + std::to_string(Opcodes::OLLEH));
                this->setStatus("Not connected", Color::RED);
                network.setConnected(false);
                continue;
            }

            network.setConnected(true);
            this->setStatus("Connected", Color::GREEN);
        }

        if (this->network.isConnected()) {
            auto r = network.receive();
            if (r == nullptr) {
                this->s_error.setText("rcv nullptr");
                continue;
            } else {
                this->s_error.setText("");
            }

            if (r[0] == Opcodes::GOODBYE) {
                network.setConnected(false);
                this->setStatus("Not connected", Color::RED);
                continue;
            }

            if (r[0] == Opcodes::INPUTS_REQ) {
                packet[0] = Opcodes::INPUTS;
                network.send(this->packet);
                packet[1] = Data::EMPTY;
            }
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    packet[0] = 4;
    network.send(this->packet);
}
