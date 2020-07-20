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
    this->s_help3.display();
}

void Client3DS::testKeyboardsSpawn()
{
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
}

bool Client3DS::initGamepadAction()
{
    /*if (this->network.isConnected()) {
        return true;
    }*/
    this->s_error.setText("");
    this->setStatus("Connecting...", Color::YELLOW);

    if (!this->network.set(this->s_ip.str(), this->s_id.str())) {
        this->s_error.setText(this->network.getLastError());
        this->setStatus("Not connected", Color::RED);
        this->network.setConnected(false);
        return false;
    }

    if (this->idx == 0) {
        this->packet[0] = Opcodes::HELLO;
        if (!this->network.send(this->packet)) {
            this->s_error.setText("snd >> " + this->network.getLastError());
            this->setStatus("Not connected", Color::RED);
            this->network.setConnected(false);
            return false;
        }
        idx += 1;
    }

    auto r = this->network.receive();
    if (r == nullptr) {
        this->s_error.setText("rcv >> " + this->network.getLastError());
        this->setStatus("Not connected", Color::RED);
        this->network.setConnected(false);
        return false;
    }

    if (r[0] != Opcodes::OLLEH) {
        this->s_error.setText("invalid response, got (" + std::to_string((int)r[0]) + ") instead of " + std::to_string(Opcodes::OLLEH));
        this->setStatus("Not connected", Color::RED);
        this->network.setConnected(false);
        return false;
    }

    this->network.setConnected(true);
    this->setStatus("Connected", Color::GREEN);
    this->idx = 0;
    return true;
}

void Client3DS::runGamepadAction()
{
    auto r = this->network.receive();
    if (r == nullptr) {
        this->s_error.setText("rcv nullptr");
        return;
    } else {
        this->s_error.setText("");
    }

    if (r[0] == Opcodes::GOODBYE) {
        this->network.setConnected(false);
        this->setStatus("Not connected", Color::RED);
        return;
    }

    if (r[0] == Opcodes::INPUTS_REQ) {
        //this->s_help3.setText("AAAHHHH");
        this->packet[0] = Opcodes::INPUTS;
        this->network.send(this->packet);
        this->packet[1] = Data::EMPTY;
    } else {
        this->s_help3.setText("Nope. {" + std::to_string((int)r[0]) + "}");
    }
    idx = 0;
}

void Client3DS::scanInputs()
{
    this->touch.scan();
    this->_keys = hidKeysHeld() | hidKeysDown() | hidKeysDownRepeat();

    if (this->_keys & KEY_SELECT) {
        this->packet[0] = Opcodes::GOODBYE;
        this->network.send(this->packet);
        this->network.setConnected(false);
        this->setStatus("Not connected", Color::RED);
    }
}

void Client3DS::fillInputs()
{
    this->packet[1] |= ((this->_keys & KEY_UP) != 0) << 0;
    this->packet[1] |= ((this->_keys & KEY_DOWN) != 0) << 1;
    this->packet[1] |= ((this->_keys & KEY_LEFT) != 0) << 2;
    this->packet[1] |= ((this->_keys & KEY_RIGHT) != 0) << 3;
    this->packet[1] |= ((this->_keys & KEY_A) != 0) << 4;
    this->packet[1] |= ((this->_keys & KEY_B) != 0) << 5;
    this->packet[1] |= ((this->_keys & KEY_X) != 0) << 6;
    this->packet[1] |= ((this->_keys & KEY_Y) != 0) << 7;
}

void Client3DS::run()
{
    if (this->network.error()) {
        this->s_error.setText(this->network.getLastError());
    }

    bool init = false;

    while (aptMainLoop()) {
        this->displayStrings();

        this->scanInputs();
        this->fillInputs();

        this->testKeyboardsSpawn();

        if (this->network.isConnected()) {
            this->runGamepadAction();
            init = false;
        } else if (this->s_connect.isTouched(touch.getPosition())) {
            this->s_connect.setTouchArea();
            init = true;
        }

        if (init) {
            init = false;
            if (this->initGamepadAction()) {
                this->s_connect.setTouchArea({VINT{30, 180}, VINT{150, 210}});
            }
        }

        this->s_help3.setText(std::to_string(this->idx));

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    this->packet[0] = Opcodes::GOODBYE;
    this->network.send(this->packet);
}
