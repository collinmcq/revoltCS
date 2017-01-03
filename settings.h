#ifndef SETTINGS_H
#define SETTINGS_H

#include "process.h"
#include <map>
#include <string>
#include <random>
#include <fstream>

#define KeyDown(vkey) ((GetAsyncKeyState(vkey) && 0x8000) ? true : false)

#pragma region initialize settings vars
// KEY BINDS
int aimKey;
int fireKey;
int triggerKey;

// TOGGLES
bool triggerON;
bool aimbotON;
bool wallhackON;
bool bunnyhopON;
bool rageON;
bool rcsON;
bool noflashON;
bool radarON;

// TRIGGER SETTINGS
int triggerDelay;

// AIMBOT SETTINGS
int aimFOV;
int targetBone;
float pitchMinPunch;
float pitchMaxPunch;
float yawMinPunch;
float yawMaxPunch;
float smoothAmount;

// WALLHACK SETTINGS
bool seeTeam;
float flashMaxAlpha;

// BHOP SETTINGS
int bhopFailChance;

// MENU SETTINGS
bool hideInstructions;

//SYSTEM SETTINGS
int screenResHeight;
int screenResWidth;
#pragma endregion

std::map<std::string, DWORD> getY3offsets() {
	std::map<std::string, DWORD> m;
	std::string src, lambda, name, underscores, offset;
	DWORD off;

	std::ifstream file("OffsetManager.txt");

	std::cout << std::endl << "Loading offsets..." << std::endl;

	while (file >> src >> lambda >> name >> underscores >> offset) {
		size_t end = name.find(':');
		if (name.find('[') != std::string::npos) { end = name.find('['); }
		name = name.substr(0, end);
		off = std::strtoul(offset.c_str(), NULL, 16);

		//std::cout << name << ":0x" << std::hex << off << std::dec << std::endl; //optional: print offsets as loaded
		m[name] = off;
	}

	std::cout << "Offsets loaded." << std::endl;
	return m;
}

std::map < std::string, std::string > getSettings() {
	std::map<std::string, std::string> m;
	std::string name, equals, value;

	std::ifstream file("SettingsManager.txt");

	std::cout << std::endl << "Loading settings..." << std::endl;

	while (file >> name >> equals >> value) {
		if (name.find('#') != std::string::npos) { continue; }
		//std::cout << name << ":" << value << std::endl; //optional: print settings as they are loaded
		m[name] = value;
	}

	std::cout << "Settings loaded." << std::endl;
	return m;
}

void setSettings(std::map<std::string, std::string> m) {
	// KEY BINDS
	aimKey = stoi(m["aimKey"]);
	fireKey = stoi(m["fireKey"]);
	triggerKey = stoi(m["triggerKey"]);

	// TOGGLES
	triggerON = m["triggerON"] == "1";
	aimbotON = m["aimbotON"] == "1";
	wallhackON = m["wallhackON"] == "1";
	bunnyhopON = m["bunnyhopON"] == "1";
	rageON = m["rageON"] == "1";
	rcsON = m["rcsON"] == "1";
	noflashON = m["noflashON"] == "1";
	radarON = m["radarON"] == "1";

	// TRIGGER SETTINGS
	triggerDelay = stoi(m["triggerDelay"]);

	// AIMBOT SETTINGS
	aimFOV = stoi(m["aimFOV"]);
	targetBone = stoi(m["targetBone"]);
	pitchMinPunch = stof(m["pitchMinPunch"]);
	pitchMaxPunch = stof(m["pitchMaxPunch"]);
	yawMinPunch = stof(m["yawMinPunch"]);
	yawMaxPunch = stof(m["yawMaxPunch"]);
	smoothAmount = stof(m["smoothAmount"]);

	// WALLHACK SETTINGS
	seeTeam = m["seeTeam"] == "1";
	flashMaxAlpha = stof(m["flashMaxAlpha"]);

	// BHOP SETTINGS
	bhopFailChance = stoi(m["bhopFailChance"]);

	// MENU SETTINGS
	hideInstructions = m["hideInstructions"] == "1";

	// SYSTEM SETTINGS
	screenResHeight = stoi(m["screenResHeight"]);
	screenResWidth = stoi(m["screenResWidth"]);
}

#pragma region controls
void printMenu() {
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	std::cout << "revoltCS Menu" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), triggerON ? 10 : 12);
	std::cout << "F1: Triggerbot Hold Key: " << triggerKey << " Delay: " << triggerDelay << "ms" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), aimbotON ? 10 : 12);
	std::cout << "F2: Aimbot Target Bone: " << targetBone << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wallhackON ? 10 : 12);
	std::cout << "F3: Wallhack See Team? " << seeTeam << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bunnyhopON ? 10 : 12);
	std::cout << "F4: Bunnyhop" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), noflashON ? 10 : 12);
	std::cout << "F5: NoFlash Max Alpha: " << flashMaxAlpha << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), radarON ? 10 : 12);
	std::cout << "F6: Radar" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rcsON ? 10 : 12);
	std::cout << "F7: Realistic RCS" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	if (rageON) { std::cout << "Rage Mode Enabled" << std::endl; }

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	if (!hideInstructions) {
		std::cout << "F8: Change Aimbot Target Bone" << std::endl;
		std::cout << "F9: Change See Team" << std::endl;
		std::cout << "INS: Toggle Rage Mode" << std::endl;
		std::cout << "END: Hide Instructions" << std::endl;
		std::cout << "HOME: Scan Ranks" << std::endl;
	}
	else {
		std::cout << "END: Show Instructions" << std::endl;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void toggleTriggerbot() {
	triggerON = !triggerON;
	Sleep(200);
	printMenu();
}

void toggleAimbot() {
	aimbotON = !aimbotON;
	Sleep(200);
	printMenu();
}

void toggleWallhack() {
	wallhackON = !wallhackON;
	Sleep(200);
	printMenu();
}

void toggleBunnyhop() {
	bunnyhopON = !bunnyhopON;
	Sleep(200);
	printMenu();
}

void toggleNoflash() {
	system("cls");
	noflashON = !noflashON;
	Sleep(200);
	printMenu();
}

void toggleRadar() {
	system("cls");
	radarON = !radarON;
	Sleep(200);
	printMenu();
}

void toggleRCS() {
	system("cls");
	rcsON = !rcsON;
	Sleep(200);
	printMenu();
}

void changeAimbotTargetBone() {
	system("cls");
	std::cout << "Common Bone Indicies: " << std::endl;
	std::cout << "00. Belt Buckle" << std::endl;
	std::cout << "04. Stomach" << std::endl;
	std::cout << "06. Chest" << std::endl;
	std::cout << "08. Head" << std::endl;
	std::cout << "09. Upper Chest" << std::endl;
	std::cout << "What bone would you like the aimbot to target? ";
	std::cin >> targetBone;
	Sleep(200);
	printMenu();
}

void toggleSeeTeam() {
	seeTeam = !seeTeam;
	Sleep(200);
	printMenu();
}

void toggleHideInstructions() {
	hideInstructions = !hideInstructions;
	Sleep(200);
	printMenu();
}

void toggleRage() {
	rageON = !rageON;
	Sleep(200);
	printMenu();
}
#pragma endregion

std::random_device random;
std::mt19937 randomGen(random());

std::string padLeft(std::string s, size_t width, char padding = ' ') {
	if (width < s.size())
		return "";

	std::string r;

	for (size_t i = 0; i < (width - s.size()); i++)
		r += padding;
	
	r += s;

	return r;
}

std::string padRight(std::string s, size_t width, char padding = ' ') {
	if (width < s.size())
		return "";

	std::string r(s);

	for (size_t i = 0; i < (width - s.size()); i++)
		r += padding;

	return r;
}

template <class T>
T getRandom(T min, T max) {
	std::uniform_real<T> randomDistrib(min, max);
	return randomDistrib(randomGen);
}

void MouseMove(int x, int y)
{
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dx = (LONG)x;
	Input.mi.dy = (LONG)y;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &Input, sizeof(INPUT));
}

#endif
