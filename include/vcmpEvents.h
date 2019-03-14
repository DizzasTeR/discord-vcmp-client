#pragma once
#include "pch.h"
extern SleepyDiscord::DiscordClient* Bot;

void OnSquirrelScriptLoad() {
	size_t size;
	int32_t sqID = g_Funcs->FindPlugin("SQHost2");
	const void** sqExports = g_Funcs->GetPluginExports(sqID, &size);
	if(sqExports != NULL && size > 0) {
		SquirrelImports* sqFuncs = (SquirrelImports*)(*sqExports);
		if(sqFuncs) {
			sqvm = *sqFuncs->GetSquirrelVM();
			sqapi = *sqFuncs->GetSquirrelAPI();
			SquirrelFuncs::RegisterFunctions(&sqvm);
		}
	}
}

uint8_t OnInternalCommand(uint32_t uCmdType, const char* pszText) {
	switch(uCmdType) {
		case 0x7D6E22D8:
			OnSquirrelScriptLoad();
			break;

		default:
			break;
	}
	return 1;
}

void onServerShutdown() {
	Bot->quit();
	delete Bot;
}

/* General events */

void onPlayerConnect(int32_t playerId) {
	char pName[25];
	g_Funcs->GetPlayerName(playerId, pName, 25);
	std::string strName = pName;
	SendDiscordMessage(":heavy_plus_sign: " + strName + " has joined the server");
}

void onPlayerDisconnect(int32_t playerId, vcmpDisconnectReason reason) {
	char pName[25];
	g_Funcs->GetPlayerName(playerId, pName, 25);
	std::string strName = pName;
	SendDiscordMessage(":heavy_minus_sign: " + strName + " has left the server [" + vcmp_PartReasons[reason] + "]");
}

void onPlayerDeath(int32_t playerId, int32_t reason) {
	char pName[25];
	g_Funcs->GetPlayerName(playerId, pName, 25);
	std::string strName = pName;
	SendDiscordMessage(":dagger: " + strName + " died! [" + vcmp_WeaponNames[reason] + "]");
}

void onPlayerWasted(int32_t playerId, int32_t killerId, int32_t reason, vcmpBodyPart bodyPart) {
	if(killerId == -1) {
		onPlayerDeath(playerId, reason);
		return;
	}
	std::string msg;
	char pName[25], kName[25];
	g_Funcs->GetPlayerName(killerId, kName, 25);
	g_Funcs->GetPlayerName(playerId, pName, 25);
	std::string killerName(kName), playerName(pName);
	msg.append("> " + killerName + " killed " + playerName + " ");
	msg.append("(" + (vcmp_WeaponNames[reason] == "" ? "Unknown" : vcmp_WeaponNames[reason]) + ") ");
	msg.append("(" + vcmp_BodyParts[bodyPart] + ")");
	SendDiscordMessage(":dagger: " + msg);
}

uint8_t onPlayerMessage(int32_t playerId, const char* text) {
	if(text[0] == '\\') {
		return 1;
	}
	char pName[25];
	g_Funcs->GetPlayerName(playerId, pName, 25);
	std::string strName = pName;
	SendDiscordMessage(":exclamation: " + strName + ": " + text);
	return 1;
}
