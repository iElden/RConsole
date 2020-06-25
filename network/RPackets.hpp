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
		LOBBY_STATE
	};

	struct PACKED PacketHello {
		char username[64];
		char password[64];
	};

	struct PACKED PackerOlleh {
		unsigned id;
	};

	struct PACKED PacketKicked {
		char reason[64];
	};

	struct PACKED PacketPing {
		unsigned lastPing;
	};

	struct PACKED NPlayer {
		unsigned id;
		unsigned ping;
		char username[64];
	};

	struct PACKED PacketLobbyState {
		unsigned playerCount;
		NPlayer players[0];
	};

	struct PACKED PacketLobbyJoined {
		unsigned playerCount;
		NPlayer players[0];
	};

	struct PACKED NLobby {
		unsigned id;
	};

	struct PACKED PacketLobbyCreated {
		NLobby lobby;
	};

	struct PACKED PacketLobbyDeleted {
		NLobby lobby;
	};

	struct PACKED PacketLobbyList {
		unsigned nbLobby;
		NLobby lobbies[0];
	};

	struct PACKED PacketError {
		char error[64];
	};

	struct PACKED PacketPlayerJoined {
		NPlayer player;
	};

	struct PACKED PacketJoinLobby {
		unsigned id;
	};

	struct PACKED PacketCreateLobby {
		NLobby lobby;
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
	};
}


#endif //RCONSOLE_RPACKETS_HPP
