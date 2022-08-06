#include "pch.h"

#include <Settings.h>
#include <include/Utility.h>
#include <SquirrelEvents.h>
#include <SquirrelFunctions.h>
#include <vcmpEvents.h>

PluginFuncs* g_Funcs;
PluginCallbacks* g_Calls;
PluginInfo* g_Info;

HSQUIRRELVM sqvm;
HSQAPI sqapi;

dpp::cluster* bot = nullptr;

extern "C" EXPORT unsigned int VcmpPluginInit(PluginFuncs* pluginFuncs, PluginCallbacks* pluginCalls, PluginInfo* pluginInfo) {
	g_Funcs = pluginFuncs;
	g_Calls = pluginCalls;
	g_Info = pluginInfo;

	pluginInfo->pluginVersion = 0x1005;
	pluginInfo->apiMajorVersion = PLUGIN_API_MAJOR;
	pluginInfo->apiMinorVersion = PLUGIN_API_MINOR;

	pluginCalls->OnServerInitialise = [] () -> uint8_t {
		bot->start(dpp::st_return);

		return 1;
	};

	pluginCalls->OnServerShutdown = OnServerShutdown;
	pluginCalls->OnPluginCommand = OnInternalCommand;

	json settings = read_json_file("settings.json");
	Settings::loadSettings(settings);

	if (Settings::defaultMode) {
		LOG("Default mode is ON");
		pluginCalls->OnPlayerConnect = onPlayerConnect;
		pluginCalls->OnPlayerDisconnect = onPlayerDisconnect;
		pluginCalls->OnPlayerDeath = onPlayerWasted;
		pluginCalls->OnPlayerMessage = onPlayerMessage;
	}

	auto strStartsWith = [] (const std::string& str, std::string startsWith) -> bool {
		return (str.substr(startsWith.size()) == startsWith);
	};
 
	bot = new dpp::cluster(Settings::token, dpp::i_all_intents);

	bot->on_log([] (const dpp::log_t& log) {
		LOG(log.message.c_str(), "[DISCORD LOG]");
	});
 
	bot->on_ready([=] (const dpp::ready_t& event) {
		bot->set_presence(dpp::presence(dpp::presence_status::ps_online, dpp::activity(dpp::activity_type::at_game, Settings::status, std::string(), std::string())));

		if(!Settings::defaultMode)
			SquirrelEvents::SQ_onDiscordConnect(event.raw_event);
    });

	bot->on_message_create([&strStartsWith] (const dpp::message_create_t event) {
		if(!Settings::defaultMode) {
			SquirrelEvents::SQ_onDiscordChannelMessage(std::to_string(event.msg.guild_id), std::to_string(event.msg.channel_id), std::to_string(event.msg.author.id), event.msg.author.username, event.msg.content);
			return;
		}

		if(strStartsWith(event.msg.content, Settings::prefix + "say")) {
			if(!Settings::specialNicks && !Settings::verifyCharacters(event.msg.author.username)) {
				event.reply("* Your username has special characters that the server does not allow!", true);
				return;
			}
			if(!Settings::specialMessages && !Settings::verifyCharacters(event.msg.content)) {
				event.reply("* Your message has special characters that the server does not allow!", true);
				return;
			}
			if(event.msg.content.size() <= 4) {
				event.reply("* Enter a message to send!", true);
				return;
			}
			std::stringstream msg;
			msg << "[Discord] ";
			msg << event.msg.author.username << ": " << event.msg.content.substr(5);
			SendClientMessageToAll(msg.str());
			SendDiscordMessage(msg.str());
		}

		else if(strStartsWith(event.msg.content, Settings::prefix + "players")) {
			std::stringstream msg;
			std::stringstream players;
			unsigned int nPlayers = 0;
			msg << "> Players";
			for(int32_t i = 0; i < 100; i++) {
				if(g_Funcs->IsPlayerConnected(i)) {
					nPlayers++;
					char buffer[25];
					g_Funcs->GetPlayerName(i, buffer, 25);
					if(players.str() != "")
						players << ", ";
					players << buffer;
				}
			}
			msg << " [" << nPlayers << "]: " << (nPlayers > 0 ? players.str() : "No players online");
			event.send(msg.str());
		}
	});

	bot->on_guild_create([] (const dpp::guild_create_t event) {
		if(!Settings::defaultMode) SquirrelEvents::SQ_onDiscordServer(event.raw_event);
	});

	bot->on_guild_member_update([=](const dpp::guild_member_update_t& event) {
		if(!Settings::defaultMode) {
			SquirrelEvents::SQ_onMemberEdit(event.raw_event, std::string());
		}
	});

	bot->on_user_update([=] (const dpp::user_update_t& event) {
		if(!Settings::defaultMode) {
			SquirrelEvents::SQ_onMemberEdit(event.raw_event, std::string());
		}
	});

	return 1;
}

void LOG(const char * msg, const char * tag)
{
	#if defined(WIN32) || defined(_WIN32)
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbBefore;
	GetConsoleScreenBufferInfo(hstdout, &csbBefore);
	SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
	printf("%s ", tag);

	SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%s\n", msg);

	SetConsoleTextAttribute(hstdout, csbBefore.wAttributes);
	#else
	printf("%c[0;32m%s%c[0;37m %s\n", 27, tag, 27, msg);
	#endif
}
