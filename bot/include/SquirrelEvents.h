#pragma once

extern HSQUIRRELVM sqvm;
extern HSQAPI sqapi;

namespace SquirrelEvents {
	void SQ_onDiscordConnect(const std::string& data) {
		if(sqvm == NULL) return;
		int top = (int)sqapi->gettop(sqvm);
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)"SQ_onDiscordConnect", -1);
		if(SQ_FAILED(sqapi->get(sqvm, -2)))
			return;
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)data.c_str(), -1);
		if(SQ_FAILED(sqapi->call(sqvm, 2, SQFalse, SQFalse)))
			return;
		sqapi->settop(sqvm, top);
	}

	void SQ_onDiscordServer(const std::string& data) {
		if(sqvm == NULL) return;
		int top = (int)sqapi->gettop(sqvm);
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)"SQ_onDiscordServer", -1);
		if(SQ_FAILED(sqapi->get(sqvm, -2)))
			return;
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)data.c_str(), -1);
		if(SQ_FAILED(sqapi->call(sqvm, 2, SQFalse, SQFalse)))
			return;
		sqapi->settop(sqvm, top);
	}

	void SQ_onDiscordChannel(const std::string& data) {
		if(sqvm == NULL) return;
		int top = (int)sqapi->gettop(sqvm);
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)"SQ_onDiscordChannel", -1);
		if(SQ_FAILED(sqapi->get(sqvm, -2)))
			return;
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)data.c_str(), -1);
		if(SQ_FAILED(sqapi->call(sqvm, 2, SQFalse, SQFalse)))
			return;
		sqapi->settop(sqvm, top);
	}

	void SQ_onDiscordChannelMessage(const std::string& _serverID, const std::string& _channelID, const std::string& _userID, const std::string& userName, const std::string& message) {
		// Attempt to convert IDs
		SQInteger serverID, channelID, userID;
		try {
			serverID = std::stoll(_serverID);
			channelID = std::stoll(_channelID);
			userID = std::stoll(_userID);
		} catch(std::exception error) {
			LOG("Failed to convert message IDs", "[ERROR]");
			return;
		}
		//
		if(sqvm == NULL) return;
		int top = (int)sqapi->gettop(sqvm);
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)"SQ_onDiscordChannelMessage", -1);
		if(SQ_FAILED(sqapi->get(sqvm, -2)))
			return;
		sqapi->pushroottable(sqvm);
		sqapi->pushinteger(sqvm, serverID);
		sqapi->pushinteger(sqvm, channelID);
		sqapi->pushinteger(sqvm, userID);
		sqapi->pushstring(sqvm, (const SQChar*)userName.c_str(), -1);
		sqapi->pushstring(sqvm, (const SQChar*)message.c_str(), -1);
		if(SQ_FAILED(sqapi->call(sqvm, 6, SQFalse, SQFalse)))
			return;
		sqapi->settop(sqvm, top);
	}

	void SQ_onMemberEdit(const std::string& username, const std::string& nick) {
		if(sqvm == NULL) return;
		int top = (int)sqapi->gettop(sqvm);
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)"SQ_onMemberEdit", -1);
		if(SQ_FAILED(sqapi->get(sqvm, -2)))
			return;
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)username.c_str(), -1);
		sqapi->pushstring(sqvm, (const SQChar*)nick.c_str(), -1);
		if(SQ_FAILED(sqapi->call(sqvm, 3, SQFalse, SQFalse)))
			return;
		sqapi->settop(sqvm, top);
	}

	void SQ_onDiscordQuit() {
		if(sqvm == NULL) return;
		int top = (int)sqapi->gettop(sqvm);
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)"SQ_onDiscordQuit", -1);
		if(SQ_FAILED(sqapi->get(sqvm, -2)))
			return;
		sqapi->pushroottable(sqvm);
		if(SQ_FAILED(sqapi->call(sqvm, 1, SQFalse, SQFalse)))
			return;
		sqapi->settop(sqvm, top);
	}

	void SQ_onDiscordDisconnect() {
		if(sqvm == NULL) return;
		int top = (int)sqapi->gettop(sqvm);
		sqapi->pushroottable(sqvm);
		sqapi->pushstring(sqvm, (const SQChar*)"SQ_onDiscordDisconnect", -1);
		if(SQ_FAILED(sqapi->get(sqvm, -2)))
			return;
		sqapi->pushroottable(sqvm);
		if(SQ_FAILED(sqapi->call(sqvm, 1, SQFalse, SQFalse)))
			return;
		sqapi->settop(sqvm, top);
	}
}