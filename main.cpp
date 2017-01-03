#undef UNICODE
/* HEADERS */
#include "csgo.h"
#include "settings.h"
/* INCLUDE */
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <thread>

DWORD WINAPI bhp(LPVOID lpParam)
{
	while (true) {
		if (KeyDown(VK_SPACE) && bunnyhopON && me.m_fFlag() == FL_ON_GROUND && (me.m_vecVelocity().x > 0 || me.m_vecVelocity().y > 0))
			me.jump();
		Sleep(10);
	}
}

DWORD WINAPI rcs(LPVOID lpParam) {
	Vector3D myPunch = { 0,0,0 };
	Vector3D previousPunch = { 0, 0, 0 };
	Vector3D delta = { 0,0,0 };

	while (true) {
		if (rcsON && KeyDown(fireKey)) {
			myPunch = me.m_vecPunch();
			myPunch.x *= 2.0f;
			myPunch.y *= 2.0f;

			if (me.m_iShotsFired() > 1) {
				delta = myPunch - previousPunch;

				mouseAimbot(ClampAngles(vecDefault-delta));
			}

			previousPunch = myPunch;
		}
		Sleep(2);
	}
}

DWORD WINAPI esp(LPVOID lpParam)
{
	int myTeam;
	int entityTeam;
	int health;
	GlowObject o;
	Player p;

	while (true) {
		if (wallhackON) {
			myTeam = me.m_iTeamNum();
			for (size_t i = 0; i < playerArray.size(); i++) {
				p = playerArray[i];
				entityTeam = p.m_iTeamNum();

				if (entityTeam != myTeam || seeTeam) {
					o = p.getGlowObject();
					health = p.m_iHealth();

					if (entityTeam == myTeam) { o.Red = 0.0f; o.Green = 0.0f; o.Blue = 0.5f; }
					else if (me.m_iCrosshairID() == p.id) { o.Red = 1.0f; o.Green = 0.0f; o.Blue = 0.0f; }
					else if (health == 100) { o.Red = 0.0f; o.Green = 0.5f; o.Blue = 0.2f; }
					else if (health > 75) { o.Red = 0.1f; o.Green = 0.5f; o.Blue = 0.0f; }
					else if (health > 50) { o.Red = 0.2f; o.Green = 0.5f; o.Blue = 0.0f; }
					else if (health > 25) { o.Red = 0.2f; o.Green = 0.2f; o.Blue = 0.0f; }
					else if (health > 1) { o.Red = 0.5f; o.Green = 0.0f; o.Blue = 0.0f; }
					else if (health == 1) { o.Red = 1.0f; o.Green = 1.0f; o.Blue = 1.0f; }

					o.Alpha = 1.0f;
					o.RenderWhenOccluded = true;
					o.RenderWhenUnoccluded = false;
					p.setGlowObject(o);

					if (radarON && entityTeam != myTeam && health > 0) {
						p.m_bSpotted(1);
					}
				}
			}
		}

		if (noflashON && me.m_flFlashMaxAlpha() > 0.0f && me.m_iHealth() > 0)
			me.m_flFlashMaxAlpha(flashMaxAlpha);

		Sleep(1);
	}
}

DWORD WINAPI aim(LPVOID lpParam)
{
	Vector3D myAngles = { 0,0,0 };
	Vector3D delta = { 0,0,0 };
	Vector3D aimAngles = { 0,0,0 };

	while (true) {
		if (aimbotON && KeyDown(aimKey)) {
			myAngles = me.m_dwViewAngles();
			for (size_t i = 0; i < enemies.size(); i++) {
				aimAngles = CalculateAngle(me.m_vecOrigin(), enemies[i].getBonePos());
				delta = ClampAngles(aimAngles - myAngles);
				if (std::fabsf(delta.x) < aimFOV && std::fabsf(delta.y) < aimFOV && enemies[i].m_iHealth() > 0) {
					mouseAimbot(ClampAngles(aimAngles - me.m_dwViewAngles()));
					break;
				}
			}
		}
		/*else if (aimbotON && KeyDown(fireKey)) {
			int myTeam = me.m_iTeamNum();
			Player e = Player(me.m_iCrosshairID());
			while (me.m_iShotsFired() > 1 && e.m_iTeamNum() == (5 - myTeam) && e.m_iHealth() > 0) {
				mouseAimbot(e, validBones[(int)getRandom<float>(0, validBones.size())]);
			}
		}*/

		Sleep(2);
	}
}

DWORD WINAPI trg(LPVOID lpParam) {
	while (true) {
		if (triggerON && !KeyDown(fireKey) && KeyDown(triggerKey)) {
			int myTeam = me.m_iTeamNum();
			if (getEnemy(me.m_iCrosshairID())) {
				Sleep(triggerDelay);
				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
				Sleep(5);
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
			}
		}
		Sleep(2);
	}
}

int main() {
	SetConsoleTitle("revoltCS");
	setSettings(getSettings());

	Sleep(500);
	hookCSGO();
	Sleep(500);

	getPlayers();

	CreateThread(NULL, NULL, aim, NULL, NULL, NULL);
	CreateThread(NULL, NULL, esp, NULL, NULL, NULL);
	CreateThread(NULL, NULL, bhp, NULL, NULL, NULL);
	CreateThread(NULL, NULL, rcs, NULL, NULL, NULL);
	CreateThread(NULL, NULL, trg, NULL, NULL, NULL);

	printMenu();

	while (true) {
		if (KeyDown(VK_F1)) { toggleTriggerbot(); }
		if (KeyDown(VK_F2)) { toggleAimbot(); }
		if (KeyDown(VK_F3)) { toggleWallhack(); }
		if (KeyDown(VK_F4)) { toggleBunnyhop(); }
		if (KeyDown(VK_F5)) { toggleNoflash(); }
		if (KeyDown(VK_F6)) { toggleRadar(); }
		if (KeyDown(VK_F7)) { toggleRCS(); }
		if (KeyDown(VK_F8)) { changeAimbotTargetBone(); }
		if (KeyDown(VK_F9)) { toggleSeeTeam(); }
		if (KeyDown(VK_HOME)) { scanRanks(); }
		if (KeyDown(VK_INSERT)) { toggleRage(); }
		if (KeyDown(VK_END)) { toggleHideInstructions(); }

		getPlayers();
		Sleep(100);
	}

	int i;
	std::cin >> i;
	return i;
}































