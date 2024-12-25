#pragma once

#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <unordered_set>

#include "../entry/Config.hpp"

#pragma warning(disable : 4996) // Deprecation -- ctime: This function or variable may be unsafe. Consider using ctime_s instead.



namespace Logger {
	inline void MakeConsole() {
		AllocConsole();
		FILE* out;
		freopen_s(&out, "CONOUT$", "w", stdout);
	}

	inline void Log(std::string szMessage, int type = 0) {
		if (Config::SETTINGS.IsDebug) {
			std::string pretext;
			switch (type) {
				case 0:
					// Normal
					pretext = "[+] "; break;
					// Fail
				case -1:
					pretext = "[-] "; break;
					// Warning
				case 1:
					pretext = "[!] "; break;
				default:
				pretext = "[+] "; break;
			}
			MakeConsole();
			std::cout << pretext << szMessage << std::endl;
		}
	};

	// Log to file
	inline void Write(std::string Caller, std::string szMessage) {

		const char* UserProfile = std::getenv("USERPROFILE");
		std::string Path = std::string(UserProfile) + "\\Desktop\\tim_apple_v2.txt";

		std::ofstream LogFile(Path, std::ios::app);
		std::time_t timestamp;
		std::time(&timestamp);
		std::string CurrentTime = ctime(&timestamp);
		CurrentTime.pop_back();

		LogFile << CurrentTime << "  " << Caller << " --> " << szMessage << std::endl;

		LogFile.close();
	};
};