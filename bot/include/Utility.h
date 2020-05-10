#pragma once
#include "pch.h"

extern PluginFuncs* g_Funcs;
extern SleepyDiscord::DiscordClient* Bot;

json read_json_file(std::string file) {
	std::ifstream ifs(file, std::ifstream::in);
	json j;

	if(ifs.is_open())
		ifs >> j;
	return j;
}

void SendClientMessageToAll(const std::string& message) {
	for(int32_t i = 0; i < 100; i++) {
		if(g_Funcs->IsPlayerConnected(i))
			g_Funcs->SendClientMessage(i, 0x8198E3FF, message.c_str());
	}
}

void SendDiscordMessage(const std::string& message) {
	for(std::vector<long long int>::const_iterator it = Settings::channels.begin(); it != Settings::channels.end(); it++) {
		Bot->sendMessage(*it, message, SleepyDiscord::Async);
	}
}

const std::vector<std::string> vcmp_WeaponNames = {
	/* 0  */ "Unarmed",                     /* 1  */ "Brass Knuckles",
	/* 2  */ "Screwdriver",                 /* 3  */ "Golf Club",
	/* 4  */ "Nightstick",                  /* 5  */ "Knife",
	/* 6  */ "Baseball Bat",                /* 7  */ "Hammer",
	/* 8  */ "Meat Cleaver",                /* 9  */ "Machete",
	/* 10 */ "Katana",                      /* 11 */ "Chainsaw",
	/* 12 */ "Grenade",                     /* 13 */ "Remote Detonation Grenade",
	/* 14 */ "Tear Gas",                    /* 15 */ "Molotov Cocktails",
	/* 16 */ "Missile",                     /* 17 */ "Colt .45",
	/* 18 */ "Python",                      /* 19 */ "Pump-Action Shotgun",
	/* 20 */ "SPAS-12 Shotgun",             /* 21 */ "Stubby Shotgun",
	/* 22 */ "TEC-9",                       /* 23 */ "Uzi",
	/* 24 */ "Silenced Ingram",             /* 25 */ "MP5",
	/* 26 */ "M4",                          /* 27 */ "Ruger",
	/* 28 */ "Sniper Rifle",                /* 29 */ "Laserscope Sniper Rifle",
	/* 30 */ "Rocket Launcher",             /* 31 */ "Flamethrower",
	/* 32 */ "M60",                         /* 33 */ "Minigun",
	/* 34 */ "Bomb",                        /* 35 */ "Helicanon",
	/* 36 */ "Camera",                      /* 37 */ "",
	/* 38 */ "",                            /* 39 */ "Vehicle",
	/* 40 */ "Heliblade",                   /* 41 */ "Explosion",
	/* 42 */ "Driveby",                     /* 43 */ "Drowned",
	/* 44 */ "Fall",                        /* 45 */ "",
	/* 46 */ "",                            /* 47 */ "",
	/* 48 */ "",                            /* 49 */ "",
	/* 50 */ "",                            /* 51 */ "Explosion",
	/* 52 */ "",                            /* 53 */ "",
	/* 54 */ "",                            /* 55 */ "",
	/* 56 */ "",                            /* 57 */ "",
	/* 58 */ "",                            /* 59 */ "",
	/* 60 */ "Heliblade",                   /* 61 */ "",
	/* 62 */ "",                            /* 63 */ "",
	/* 64 */ "",                            /* 65 */ "",
	/* 66 */ "",                            /* 67 */ "",
	/* 68 */ "",                            /* 69 */ "",
	/* 70 */ "Suicide",                     /* 71 */ ""
};

const std::vector<std::string> vcmp_BodyParts = {
	/* 0  */ "Body",						/* 1  */ "Torso",
	/* 2  */ "Left Arm",					/* 3  */ "Right Arm",
	/* 4  */ "Left Leg",					/* 5  */ "Right Leg",
	/* 6  */ "Head",						/* 7  */ "In Vehicle"
};

const std::vector<std::string> vcmp_PartReasons = {
	/* 0  */ "Timeout",						/* 1  */ "Quit",
	/* 2  */ "Kick/Ban",					/* 3  */ "Crash",
	/* 4  */ "Anti-Cheat"
};