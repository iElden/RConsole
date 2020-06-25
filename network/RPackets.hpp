//
// Created by Gegel85 on 25/06/2020.
//

#ifndef RCONSOLE_RPACKETS_HPP
#define RCONSOLE_RPACKETS_HPP


#define PACKED __attribute((packed))

namespace RC::Network
{
	enum PACKED Opcode {
		HELLO,
		OLLEH,
		PING,
		PONG,
		GOODBYE,
		KICKED,
		JOIN_LOBBY,
		CREATE_LOBBY,
		DELETE_LOBBY,
		LEAVE_LOBBY,
		LOBBY_JOINED,
		LOBBY_CREATED,
		LOBBY_DELETED,
		PLAYER_JOINED,
		OK,
		ERROR,
		LOBBY_LIST_REQUEST,
		LOBBY_LIST,
		LOBBY_STATE_REQUEST,
		LOBBY_STATE,
		PLAYER_READY,
		SET_READY,
		CHOOSE_GAME,
		GAME_START,
		GAME_EVENT
	};

	struct PACKED PacketHello {
		Opcode code;
		char username[64];
		char password[64];
	};

	struct PACKED PackerOlleh {
		Opcode code;
		unsigned id;
	};

	struct PACKED PacketKicked {
		Opcode code;
		char reason[64];
	};

	struct PACKED PacketPing {
		Opcode code;
		unsigned lastPing;
	};

	struct PACKED NPlayer {
		Opcode code;
		unsigned id;
		unsigned ping;
		char username[64];
	};

	struct PACKED PacketLobbyState {
		Opcode code;
		unsigned playerCount;
		NPlayer players[0];
	};

	struct PACKED PacketLobbyJoined {
		Opcode code;
		unsigned playerCount;
		NPlayer players[0];
	};

	struct PACKED NLobby {
		Opcode code;
		unsigned id;
	};

	struct PACKED PacketLobbyCreated {
		Opcode code;
		NLobby lobby;
	};

	struct PACKED PacketLobbyDeleted {
		Opcode code;
		NLobby lobby;
	};

	struct PACKED PacketLobbyList {
		Opcode code;
		unsigned nbLobby;
		NLobby lobbies[0];
	};

	struct PACKED PacketError {
		Opcode code;
		char error[64];
	};

	struct PACKED PacketPlayerJoined {
		Opcode code;
		NPlayer player;
	};

	struct PACKED PacketJoinLobby {
		Opcode code;
		unsigned id;
	};

	struct PACKED PacketCreateLobby {
		Opcode code;
		NLobby lobby;
	};

	struct PACKED PacketChooseGame {
		Opcode code;
		unsigned id;
	};

	struct PACKED PacketGameStart {
		Opcode code;
		unsigned gameSelected;
	};

	struct PacketGameEvent {
		Opcode code;
		char gameData[0];
	};

	union PACKED Packet {
		Opcode code;
		PacketHello hello;
		PackerOlleh olleh;
		PacketKicked kicked;
		PacketPing ping;
		PacketLobbyJoined lobbyJoined;
		PacketLobbyState lobbyState;
		PacketLobbyCreated lobbyCreated;
		PacketLobbyDeleted lobbyDeleted;
		PacketLobbyList lobbyList;
		PacketError error;
		PacketPlayerJoined playerJoined;
		PacketJoinLobby joinLobby;
		PacketCreateLobby createLobby;
		PacketChooseGame chooseGame;
		PacketGameStart gameStart;
		PacketGameEvent gameEvent;
	};
}


#endif //RCONSOLE_RPACKETS_HPP
