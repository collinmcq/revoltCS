#ifndef CSGO_H
#define CSGO_H

#define FL_ON_GROUND 257

#include "process.h"
#include "settings.h"
#include "slimstd.h"
#include <map>
#include <vector>

std::map<std::string, DWORD> offsets;

Process csgo;
ProcessModule modClient;
ProcessModule modEngine;
DWORD Client;
DWORD Engine;

void hookCSGO() {
	std::cout << std::endl << "Finding csgo...";
	while (!csgo.Attach("csgo.exe")) {
		std::cout << ".";
		Sleep(1000);
	}
	std::cout << std::endl << "CSGO found." << std::endl;

	modClient = csgo.Module("client.dll");
	modEngine = csgo.Module("engine.dll");
	Client = modClient.dwBase;
	Engine = modEngine.dwBase;

	offsets = getY3offsets();
}

struct GlowObject
{
	DWORD Entity;
	float Red;
	float Green;
	float Blue;
	float Alpha;
	uint8_t UnknownData1[16];
	bool RenderWhenOccluded;
	bool RenderWhenUnoccluded;
	bool FullBloom;
	uint8_t UnkownData2[14];
};

struct Vector3D
{
	float x;
	float y;
	float z;
};

Vector3D vecDefault = { 0.0f,0.0f,0.0f };

std::string printVector(Vector3D v) {
	return "<" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + ">";
}

Vector3D operator-(const Vector3D &v1, const Vector3D &v2) {
	Vector3D r;
	r.x = v1.x - v2.x;
	r.y = v1.y - v2.y;
	r.z = v1.z - v2.z;
	return r;
}

Vector3D operator+(const Vector3D &v1, const Vector3D &v2) {
	Vector3D r;
	r.x = v1.x + v2.x;
	r.y = v1.y + v2.y;
	r.z = v1.z + v2.z;
	return r;
}

bool operator==(const Vector3D &v1, const Vector3D &v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

bool operator!=(const Vector3D &v1, const Vector3D &v2) {
	return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}

Vector3D ClampAngles(Vector3D v) {
	if (v.x > 89.0f && v.x <= 180.0f) v.x = 89.0f;
	if (v.x > 180.0f) v.x -= 360.0f;
	if (v.x < -89.0f) v.x = -89.0f;
	if (v.y > 180.0f) v.y -= 360.0f;
	if (v.y < -180.0f) v.y += 360.0f;
	v.z = 0.0f;

	return v;
}

static const char *Ranks[] =
{
	"No Rank",
	"Silver I",
	"Silver II",
	"Silver III",
	"Silver IV",
	"Silver Elite",
	"Silver Elite Master",

	"Gold Nova I",
	"Gold Nova II",
	"Gold Nova III",
	"Gold Nova Master",
	"Master Guardian I",
	"Master Guardian II",

	"Master Guardian Elite",
	"Distinguished Master Guardian",
	"Legendary Eagle",
	"Legendary Eagle Master",
	"Supreme Master First Class",
	"The Global Elite"
};

static class LocalPlayer {
public:
	LocalPlayer() = default;

	DWORD getBase() {
		return csgo.Read<DWORD>(Client + offsets["m_dwLocalPlayer"]);
	}

	DWORD getEngine() {
		return csgo.Read<DWORD>(Engine + offsets["m_dwClientState"]);
	}

	DWORD m_dwGlowObject() {
		return csgo.Read<DWORD>(Client + offsets["m_dwGlowObject"]);
	}

	Vector3D m_vecViewOffset() {
		return csgo.Read<Vector3D>(getBase() + offsets["m_vecViewOffset"]);
	}

	Vector3D m_vecPunch() {
		return csgo.Read<Vector3D>(getBase() + offsets["m_vecPunch"]);
	}

	Vector3D m_dwViewAngles() {
		return csgo.Read<Vector3D>(getEngine() + offsets["m_dwViewAngles"]);
	}

	void m_dwViewAngles(Vector3D v) {
		v = ClampAngles(v);
		//std::cout << "setting view: " << printVector(v) << std::endl;
		if (v != vecDefault)
			csgo.Write<Vector3D>(getEngine() + offsets["m_dwViewAngles"], v);
	}

	int m_iCrosshairID() {
		return csgo.Read<int>(getBase() + offsets["m_iCrossHairID"]);
	}

	int m_fFlag() {
		return csgo.Read<int>(getBase() + offsets["m_fFlags"]);
	}

	DWORD m_dwBoneMatrix() {
		csgo.Read<DWORD>(getBase() + offsets["m_dwBoneMatrix"]);
	}

	int m_iFlags() {
		return csgo.Read<int>(getBase() + offsets["m_iFlags"]);
	}

	int m_iShotsFired() {
		return csgo.Read<int>(getBase() + offsets["m_iShotsFired"]);
	}

	int m_hActiveWeapon() {
		return csgo.Read<int>(getBase() + offsets["m_hActiveWeapon"]);
	}

	Vector3D m_vecVelocity() {
		return csgo.Read<Vector3D>(getBase() + offsets["m_vecVelocity"]);
	}

	int m_iGlowIndex() {
		return csgo.Read<int>(getBase() + offsets["m_iGlowIndex"]);
	}

	int m_iHealth() {
		return csgo.Read<int>(getBase() + offsets["m_iHealth"]);
	}

	int m_iVirtualTable() {
		return csgo.Read<int>(getBase() + offsets["m_iVirtualTable"]);
	}

	int m_iID() {
		return csgo.Read<int>(getBase() + offsets["m_iID"]);
	}

	BYTE m_iDormant() {
		return csgo.Read<BYTE>(getBase() + offsets["m_iDormant"]);
	}

	int m_hOwnerEntity() {
		return csgo.Read<int>(getBase() + offsets["m_hOwnerEntity"]);
	}

	int m_iTeamNum() {
		return csgo.Read<int>(getBase() + offsets["m_iTeamNum"]);
	}

	int m_bSpotted() {
		return csgo.Read<int>(getBase() + offsets["m_bSpotted"]);
	}

	void m_bSpotted(int spotted) {
		csgo.Write<int>(getBase() + offsets["m_bSpotted"], spotted);
	}

	void jump() {
		csgo.Write<int>(Client + offsets["m_dwForceJump"], 6);
	}

	Vector3D m_vecOrigin() {
		return csgo.Read<Vector3D>(getBase() + offsets["m_vecOrigin"]);
	}

	Vector3D m_angRotation() {
		return csgo.Read<Vector3D>(getBase() + offsets["m_angRotation"]);
	}

	float m_flFlashMaxAlpha() {
		return csgo.Read<float>(getBase() + offsets["m_flFlashMaxAlpha"]);
	}

	void m_flFlashMaxAlpha(float maxAlpha) {
		csgo.Write<float>(getBase() + offsets["m_flFlashMaxAlpha"], maxAlpha);
	}
};

LocalPlayer me;

class Player {
public:
	int id;

	Player() = default;
	Player(int i) { id = i; }

	DWORD getBase() {
		return csgo.Read<DWORD>(Client + offsets["m_dwEntityList"] + ((id - 1) * 16));
	}

	int m_iHealth() {
		return csgo.Read<int>(getBase() + offsets["m_iHealth"]);
	}

	int m_iVirtualTable() {
		return csgo.Read<int>(getBase() + offsets["m_iVirtualTable"]);
	}

	int m_iID() {
		return csgo.Read<int>(getBase() + offsets["m_iID"]);
	}

	BYTE m_iDormant() {
		return csgo.Read<BYTE>(getBase() + offsets["m_iDormant"]);
	}

	int m_hOwnerEntity() {
		return csgo.Read<int>(getBase() + offsets["m_hOwnerEntity"]);
	}

	int m_iTeamNum() {
		return csgo.Read<int>(getBase() + offsets["m_iTeamNum"]);
	}

	int m_bSpotted() {
		return csgo.Read<int>(getBase() + offsets["m_bSpotted"]);
	}

	void m_bSpotted(int spotted) {
		csgo.Write<int>(getBase() + offsets["m_bSpotted"], spotted);
	}

	Vector3D m_vecOrigin() {
		return csgo.Read<Vector3D>(getBase() + offsets["m_vecOrigin"]);
	}

	Vector3D m_angRotation() {
		return csgo.Read<Vector3D>(getBase() + offsets["m_angRotation"]);
	}

	DWORD m_dwBoneMatrix() {
		return csgo.Read<DWORD>(getBase() + offsets["m_dwBoneMatrix"]);
	}

	Vector3D getBonePos(int bone = targetBone) {
		Vector3D pos;
		DWORD boneMatrix = m_dwBoneMatrix();

		pos.x = csgo.Read<float>(boneMatrix + 0x30 * bone + 0x0C);
		pos.y = csgo.Read<float>(boneMatrix + 0x30 * bone + 0x1C);
		pos.z = csgo.Read<float>(boneMatrix + 0x30 * bone + 0x2C);

		return pos;
	}

	int m_iFlags() {
		return csgo.Read<int>(getBase() + offsets["m_iFlags"]);
	}

	int m_iShotsFired() {
		return csgo.Read<int>(getBase() + offsets["m_iShotsFired"]);
	}

	int m_hActiveWeapon() {
		return csgo.Read<int>(getBase() + offsets["m_hActiveWeapon"]);
	}

	Vector3D m_vecVelocity() {
		return csgo.Read<Vector3D>(getBase() + offsets["m_vecVelocity"]);
	}

	int m_iGlowIndex() {
		return csgo.Read<int>(getBase() + offsets["m_iGlowIndex"]);
	}

	GlowObject getGlowObject() {
		return csgo.Read<GlowObject>(me.m_dwGlowObject() + m_iGlowIndex() * sizeof(GlowObject));
	}

	void setGlowObject(GlowObject o) {
		csgo.Write<GlowObject>(me.m_dwGlowObject() + m_iGlowIndex() * sizeof(GlowObject), o);
	}

	void setID(int newID) {
		id = newID;
	}
};

std::vector<Player> playerArray;
std::vector<Player> enemies;
std::vector<int> validBones = { 0, 3, 6, 7, 8, 9, 10 };

bool getEnemy(int id) {
	for (size_t i = 0; i < enemies.size(); i++) {
		if (enemies[i].id == id)
			return true;
	}

	return false;
}

Player getPlayer(int id) {
	for (size_t i = 0; i < playerArray.size(); i++) {
		if (playerArray[i].id == id)
			return playerArray[i];
	}

	return NULL;
}

Vector3D CalculateAngle(Vector3D myPos, Vector3D enemyPos) {
	Vector3D myPunch = me.m_vecPunch();
	Vector3D vecView = me.m_vecViewOffset();
	float viewOrigin = vecView.z;

	float pitchReduction = getRandom<float>(pitchMinPunch, pitchMaxPunch);
	float yawReduction = getRandom<float>(yawMinPunch, yawMaxPunch);

	Vector3D delta = myPos - enemyPos;
	delta.z += viewOrigin;

	float hypotenuse = std::sqrtf(delta.x * delta.x + delta.y * delta.y);

	Vector3D angles;
	angles.x = std::atanf(delta.z / hypotenuse) * 57.295779513082f - myPunch.x * pitchReduction;
	angles.y = std::atanf(delta.y / delta.x) * 57.295779513082f - myPunch.y * yawReduction;
	angles.z = 0.0f;

	if (delta.x >= 0.0f) angles.y += 180.0f;

	return angles;
}

Vector3D CompensateVelocity(Vector3D enemyPos, Vector3D enemyVelocity) {
	enemyPos.x += enemyVelocity.x / 100.0f * 40.0f / smoothAmount;
	enemyPos.y += enemyVelocity.y / 100.0f * 40.0f / smoothAmount;
	enemyPos.z += enemyVelocity.z / 100.0f * 40.0f / smoothAmount;

	Vector3D myVelocity = me.m_vecVelocity();
	enemyPos.x -= myVelocity.x / 100.0f * 40.0f / smoothAmount;
	enemyPos.y -= myVelocity.y / 100.0f * 40.0f / smoothAmount;
	enemyPos.z -= myVelocity.z / 100.0f * 40.0f / smoothAmount;

	return enemyPos;
}

float distance2D(Vector3D v1, Vector3D v2) {
	return qsqrt(sqr(v2.x - v1.x) + sqr(v2.y - v1.y));
}

float distance3D(Vector3D v1, Vector3D v2) {
	return qsqrt(sqr(v2.x - v1.x) + sqr(v2.y - v1.y) + sqr(v2.z - v1.z));
}

void getPlayers() {
	Player current;
	int team;
	int mTeam = me.m_iTeamNum();

	std::vector<Player> p;
	std::vector<Player> e;

	for (int i = 0; i < 64; i++) {
		current = Player(i);
		team = current.m_iTeamNum();
		if (team == 2 || team == 3) {
			p.push_back(current);
			if (team != mTeam)
				e.push_back(current);
		}
	}

	enemies = e;
	playerArray = p;
}

void mouseAimbot(Player p, int bone = targetBone) {
	int xPixelsPerDegree = screenResWidth / 90;
	int yPixelsPerDegree = screenResHeight / 90;
	Vector3D pPos = CompensateVelocity(p.getBonePos(bone), p.m_vecVelocity());

	Vector3D myAngle = me.m_dwViewAngles();
	Vector3D aimAngle = ClampAngles(CalculateAngle(me.m_vecOrigin(), pPos));
	Vector3D delta = ClampAngles(aimAngle - myAngle);

	int mouseY = delta.x * xPixelsPerDegree;
	int mouseX = delta.y * yPixelsPerDegree;

	if (std::abs(mouseX) < 60 && std::abs(mouseY) < 60)
		MouseMove(-mouseX, mouseY);

	Sleep(1);
}

void mouseAimbot(Vector3D angle) {
	int xPixelsPerDegree = screenResWidth / 90;
	int yPixelsPerDegree = screenResHeight / 90;

	int mouseY = angle.x * xPixelsPerDegree;
	int mouseX = angle.y * yPixelsPerDegree;

	if (std::abs(mouseX) < 60 && std::abs(mouseY) < 60)
		MouseMove(-mouseX*2, mouseY);

	Sleep(1);
}

std::string getPlayerName(int id) {
	DWORD radarBase = csgo.Read<DWORD>(Client + offsets["m_dwRadarBase"]); // radar base: 0x04EFCD2C NOW CUSTOM: 0x4EFCD6C ptrScan 3.0
	DWORD radarPointer = csgo.Read<DWORD>(radarBase + offsets["m_dwRadarBasePointer"]); // name offset 0x50

	//std::cout << "radarBase          0x" << std::hex << radarBase << std::dec << std::endl;
	//std::cout << "radarPointer       0x" << std::hex << radarPointer << std::dec << std::endl;

	DWORD dwRadarBase = 0x0CA72558; // radar base should be ptr to this address?
	DWORD m_dwRadarName = 0x54;
	DWORD m_dwRadarSize = 0x1EC;

	std::string name = csgo.ReadUnicodeString(radarPointer + (id - 1) * m_dwRadarSize, 256);

	return name.length() > 0 ? name : "#undef";
}

void scanRanks() {
	if (playerArray.size() < 1) { return; }

	std::cout << "id  " << padRight("Name", 32) << " " << "Wins Rank" << std::endl;
	DWORD resource = csgo.Read<DWORD>(Client + offsets["CSPlayerResource"]);
	int id, rank, wins;

	for (size_t i = 0; i < playerArray.size(); i++) {
		id = playerArray[i].id;
		rank = csgo.Read<int>(resource + (offsets["m_iCompetitiveRanking"] + id * 4));
		wins = csgo.Read<int>(resource + (offsets["m_iCompetitiveWins"] + id * 4));

		std::cout << padLeft(std::to_string(id), 2, '0') << ": " << padRight(getPlayerName(id), 32) << " " << padRight(std::to_string(wins), 4) << " " << Ranks[rank] << std::endl;
	}
}









#endif