#include "pch.h"

#include "Settings.h"
#include "Utility.h"
#include "SquirrelEvents.h"
#include "SquirrelFunctions.h"
#include "vcmpEvents.h"

std::thread* client_thread;

PluginFuncs* g_Funcs;
PluginCallbacks* g_Calls;
PluginInfo* g_Info;

HSQUIRRELVM sqvm;
HSQAPI sqapi;

class DiscordClient : public SleepyDiscord::DiscordClient {
public:
	using SleepyDiscord::DiscordClient::DiscordClient;
	void onReady(std::string* jsonMessage) {
		if (Settings::callSquirrelEvents)
			SquirrelEvents::SQ_onDiscordConnect(*jsonMessage);
		//updateStatus(Settings::status);
	}

	void onMessage(SleepyDiscord::Message message) {
		if(std::string(message.serverID).size() == 0)
			return;

		if (Settings::callSquirrelEvents)
			SquirrelEvents::SQ_onDiscordChannelMessage(std::string(message.serverID), std::string(message.channelID), std::string(message.author.ID), message.author.username, message.content);
		if(!Settings::defaultCommands)
			return;

		if(message.startsWith(Settings::prefix + "say")) {
			if(!Settings::specialNicks && !Settings::verifyCharacters(message.author.username)) {
				sendMessage(message.channelID, "* Your username has special characters that the server does not allow!", SleepyDiscord::Async);
				return;
			}
			if(!Settings::specialMessages && !Settings::verifyCharacters(message.content)) {
				sendMessage(message.channelID, "* Your message has special characters that the server does not allow!", SleepyDiscord::Async);
				return;
			}
			if(message.content.size() <= 4) {
				sendMessage(message.channelID, "* Enter a message to send!", SleepyDiscord::Async);
				return;
			}
			std::stringstream msg;
			msg << "[Discord] ";
			msg << message.author.username << ": " << message.content.substr(5);
			SendClientMessageToAll(msg.str());
			SendDiscordMessage(msg.str());
		}

		else if(message.startsWith(Settings::prefix + "players")) {
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
			sendMessage(message.channelID, msg.str(), SleepyDiscord::Async);
		}
	}

	void onServer(SleepyDiscord::Server jsonMessage) { SquirrelEvents::SQ_onDiscordServer(jsonMessage.name); }
	void onChannel(std::string* jsonMessage) { SquirrelEvents::SQ_onDiscordChannel(*jsonMessage); }
	
	void onEditMember(SleepyDiscord::Snowflake<SleepyDiscord::Server> serverID, SleepyDiscord::User user, std::vector<SleepyDiscord::Snowflake<SleepyDiscord::Role>> nroles, std::string nnick) {
		if (Settings::callSquirrelEvents)
			SquirrelEvents::SQ_onMemberEdit(user.username, nnick);
	}
	
	void onQuit() { if(Settings::callSquirrelEvents) SquirrelEvents::SQ_onDiscordQuit(); }
	void onDisconnect() { if(Settings::callSquirrelEvents) SquirrelEvents::SQ_onDiscordDisconnect(); }

};
SleepyDiscord::DiscordClient* Bot;

/* --------------- Core Functions --------------- */

void run_client() {
	try {
		Bot = new DiscordClient(Settings::token, 3);
	} catch(std::exception _e) {
		std::cout << _e.what() << std::endl;
	}
}

extern "C" EXPORT unsigned int VcmpPluginInit(PluginFuncs* pluginFuncs, PluginCallbacks* pluginCalls, PluginInfo* pluginInfo) {
	g_Funcs = pluginFuncs;
	g_Calls = pluginCalls;
	g_Info = pluginInfo;

	pluginInfo->pluginVersion = 0x1001;
	pluginInfo->apiMajorVersion = PLUGIN_API_MAJOR;
	pluginInfo->apiMinorVersion = PLUGIN_API_MINOR;

	pluginCalls->OnServerShutdown = onServerShutdown;
	pluginCalls->OnPluginCommand = OnInternalCommand;

	json settings = read_json_file("settings.json");
	Settings::loadSettings(settings);

	if (Settings::defaultMessages) {
		pluginCalls->OnPlayerConnect = onPlayerConnect;
		pluginCalls->OnPlayerDisconnect = onPlayerDisconnect;
		pluginCalls->OnPlayerDeath = onPlayerWasted;
		pluginCalls->OnPlayerMessage = onPlayerMessage;
	}
	else Settings::callSquirrelEvents = true;

	client_thread = new std::thread(run_client);
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
