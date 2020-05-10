#pragma once
#include "pch.h"

namespace Settings {
	bool callSquirrelEvents = false;
	bool defaultMessages = true;
	bool defaultCommands = true;
	bool specialNicks = false;
	bool specialMessages = false;

	std::vector<long long int> channels;

	std::string token;
	std::string prefix = "!";
	std::string status = "VCMP Server";
	const std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-':;\"[]{},./<>?|0123456789 ";

	bool verifyCharacters(const std::string& str) {
		for(const auto& alphabet : str) {
			if(Settings::validChars.find(alphabet) == std::string::npos)
				return false;
		}
		return true;
	}

	void loadSettings(const json& settings) {
		if(settings.find("token") != settings.end())
			Settings::token = settings["token"].get<std::string>();

		if(settings.find("prefix") != settings.end())
			Settings::prefix = settings["prefix"].get<std::string>();

		if(settings.find("status") != settings.end())
			Settings::status = settings["status"].get<std::string>();

		if(settings.find("defaultMessages") != settings.end())
			Settings::defaultMessages = settings["defaultMessages"];

		if(settings.find("defaultCommands") != settings.end())
			Settings::defaultCommands = settings["defaultCommands"];

		if(settings.find("specialNicks") != settings.end())
			Settings::specialNicks = settings["specialNicks"];

		if(settings.find("specialMessages") != settings.end())
			Settings::specialMessages = settings["specialMessages"];

		if(settings.find("channels") != settings.end()) {
			json::const_iterator it;
			for(it = settings["channels"].begin(); it != settings["channels"].end(); it++) {
				Settings::channels.push_back(it.value());
			}
		}
	}
}