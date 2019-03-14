#pragma once
#include "pch.h"

extern HSQUIRRELVM sqvm;
extern HSQAPI sqapi;
extern SleepyDiscord::DiscordClient* Bot;

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
				Bot->sendMessage(channelID, std::string((char*)message));
			} catch(std::exception err) {
				return sqapi->throwerror(v, (SQChar*)"Discord_SendMessageToChannel >> Failed to send message");
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

			Bot->updateStatus(std::string((char*)status));
			return 1;
		}
		sqapi->pushbool(v, false);
		return 1;
	}

	void RegisterFunctions(HSQUIRRELVM* v) {
		#define REGISTERSQFUNCTION(name) register_global_func(*v, #name, SquirrelFuncs::name)
		REGISTERSQFUNCTION(Discord_SendMessage);
		REGISTERSQFUNCTION(Discord_SendMessageToChannel);
		REGISTERSQFUNCTION(Discord_SetStatus);
		LOG("Discord Functions registered", "[LOG]");
	}
}