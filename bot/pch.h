#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>
#include <fstream>
#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>

#include <SQImports.h>
#include <SQModule.h>
#include <squirrel.h>
#include <sqconfig.h>
#include <vcmp.h>

//#include <Settings.h>
//#include <include/Utility.h>
//#include <SquirrelEvents.h>
//#include <SquirrelFunctions.h>
//#include <vcmpEvents.h>

using json = nlohmann::json;

void LOG(const char *, const char * = "[DISCORD]");

#endif //PCH_H
