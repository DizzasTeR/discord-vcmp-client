#pragma once
#include "pch.h"

#include <include/Utility.h>

extern HSQUIRRELVM sqvm;
extern HSQAPI sqapi;
extern dpp::cluster* bot;

void register_global_func(HSQUIRRELVM vm, const char *name, SQFUNCTION function) {
	sqapi->pushroottable(vm);
	sqapi->pushstring(vm, (const SQChar*)name, -1);
	sqapi->newclosure(vm, function, 0);
	sqapi->newslot(vm, -3, SQFalse);
	sqapi->pop(vm, 1);
}

namespace SquirrelFuncs {
	SQInteger Discord_SendMessage(HSQUIRRELVM v) {
		const SQInteger args = sqapi->gettop(v);
		if(args == 2) {
			const SQChar* message;
			SQRESULT sqresult = sqapi->getstring(v, 2, &message);
			if(SQ_FAILED(sqresult)) {
				return sqresult;
			}

			if(std::string((char*)message).size() == 0) {
				return sqapi->throwerror(v, (SQChar*)"Discord_SendMessage >> Sending an empty/invalid message");
			}

			SendDiscordMessage(std::string((char*)message));
			return 1;
		}
		sqapi->pushbool(v, false);
		return 1;
	}

	SQInteger Discord_SendMessageToChannel(HSQUIRRELVM v)
	{
		const SQInteger args = sqapi->gettop(v);
		if(args == 3) {
			SQInteger channelID;
			SQRESULT sqresult = sqapi->getinteger(v, 2, &channelID);
			if(SQ_FAILED(sqresult)) {
				return sqresult;
			}

			const SQChar* message;
			sqresult = sqapi->getstring(v, 3, &message);
			if(SQ_FAILED(sqresult)) {
					return sqresult;
			}

			if(std::string((char*)message).size() == 0) {
				return sqapi->throwerror(v, (SQChar*)"Discord_SendMessageToChannel >> Sending an empty/invalid message");
			}

			try {
				dpp::message msg(channelID, std::string((char*) message));
				bot->message_create(msg);
			} catch(std::exception err) {
				return sqapi->throwerror(v, (SQChar*)"Discord_SendMessageToChannel >> Failed to send message");
			}
			return 1;
		}
		sqapi->pushbool(v, false);
		return 1;
	}

	SQInteger Discord_SendMessageToChannelName(HSQUIRRELVM v) {
		const SQInteger args = sqapi->gettop(v);
		if(args == 3) {
			const SQChar* channelName;
			SQRESULT sqresult = sqapi->getstring(v, 2, &channelName);
			if(SQ_FAILED(sqresult)) {
				return sqresult;
			}

			const SQChar* message;
			sqresult = sqapi->getstring(v, 3, &message);
			if(SQ_FAILED(sqresult)) {
				return sqresult;
			}

			if(std::string((char*) message).size() == 0) {
				return sqapi->throwerror(v, (SQChar*) "Discord_SendMessageToChannelName >> Sending an empty/invalid message");
			}

			try {
				dpp::cache<dpp::channel>* channelCache = dpp::get_channel_cache();
				std::unordered_map<dpp::snowflake, dpp::channel*>& channels = channelCache->get_container();
				std::shared_lock l(channelCache->get_mutex());

				for(auto [id, channel] : channels) {
					if(channel->name == channelName) {
						bot->message_create(dpp::message(id, message));
					}
				}
			}
			catch(std::exception err) {
				return sqapi->throwerror(v, (SQChar*) "Discord_SendMessageToChannelName >> Failed to send message");
			}
			return 1;
		}
		sqapi->pushbool(v, false);
		return 1;
	}

	SQInteger Discord_SetStatus(HSQUIRRELVM v) {
		const SQInteger args = sqapi->gettop(v);
		if(args == 2) {
			const SQChar* status;
			SQRESULT sqresult = sqapi->getstring(v, 2, &status);
			if(SQ_FAILED(sqresult)) {
				return sqresult;
			}

			if(std::string((char*)status).size() == 0) {
				return sqapi->throwerror(v, (SQChar*)"Discord_SetStatus >> Setting an invalid status");
			}

			dpp::presence presence(dpp::presence_status::ps_online, dpp::activity(dpp::activity_type::at_game, std::string((char*) status), std::string(), std::string()));
			bot->set_presence(presence);

			return 1;
		}
		sqapi->pushbool(v, false);
		return 1;
	}

	void RegisterFunctions(HSQUIRRELVM* v) {
		#define REGISTERSQFUNCTION(name) register_global_func(*v, #name, SquirrelFuncs::name)
		REGISTERSQFUNCTION(Discord_SendMessage);
		REGISTERSQFUNCTION(Discord_SendMessageToChannel);
		REGISTERSQFUNCTION(Discord_SendMessageToChannelName);
		REGISTERSQFUNCTION(Discord_SetStatus);
		LOG("Discord Functions registered", "[LOG]");
	}
}