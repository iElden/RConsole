#ifndef CLIENT_3DS_HPP
#define CLIENT_3DS_HPP

#include "Keyboard.hpp"
#include "Network.hpp"
#include "Text.hpp"
#include "Touch.hpp"
#include "Vector2.hpp"

#define PACKET_SIZE 16

class Client3DS {

public:
    Client3DS();
    ~Client3DS();

    void run();

private:
    void setStatus(std::string status, int color);
    void displayStrings();

    void scanInputs();
    void fillInputs();

    void testKeyboardsSpawn();

    bool initGamepadAction();
    void runGamepadAction();

private:
    char packet[PACKET_SIZE] = {0};

    Touch touch;

    Text s_title{"RGamepad", {5, 5}};
    Text s_setIP{"Set client ip:", {10, 5}, {VINT{30, 60}, VINT{150, 90}}};
    Text s_setID{"Set client id:", {20, 5}, {VINT{30, 140}, VINT{150, 170}}};
    Text s_connect{"Connect to client", {25, 5}, {VINT{30, 180}, VINT{150, 210}}};
    Text s_help{"Double-tap on connect if needed.", {26, 5}};
    Text s_help2{"Select to change combination.", {27, 5}};
    Text s_help3{"", {28, 5}};
    Text s_ip{"0.0.0.0", {10, 24}};
    Text s_id{"0000", {20, 24}};
    Text s_status{"Not connected", {25, 24}};
    Text s_error{"", {0, 0}};

    Keyboard k_ip{'.'};
    Keyboard k_id{0, 5};

    Network network;

    u32 _keys;

    int idx = 0;
};

#endif // CLIENT_3DS_HPP
