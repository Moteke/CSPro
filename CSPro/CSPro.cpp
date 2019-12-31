#include <iostream>
#include <stdlib.h>
#include "Master.h"
#include "csgo.hpp"

Master master;

struct variables {
	intptr_t gameModule;
	intptr_t localPlayer;
	intptr_t glowObject;
} val;

struct offset {
	intptr_t dwGlowObjectManager = hazedumper::signatures::dwGlowObjectManager;
	intptr_t dwLocalPlayer = hazedumper::signatures::dwLocalPlayer;
	intptr_t dwEntityList = hazedumper::signatures::dwEntityList;
	intptr_t m_iGlowIndex = hazedumper::netvars::m_iGlowIndex;
	intptr_t m_iTeamNum = hazedumper::netvars::m_iTeamNum;
	intptr_t m_iHealth = hazedumper::netvars::m_iHealth;
}offset;

int main()
{
	DWORD pID = master.getPID(L"csgo.exe");
	while (!pID) {
		system("cls");
		std::cout << "Waiting for game";
		for (int i = 0; i < 3; i++) {
			Sleep(200);
			std::cout << ".";
		}
		Sleep(1400);
		pID = master.getPID(L"csgo.exe");
	}
	if (master.openHandle(pID)) {
		system("cls");
		std::cout << "Please wait...";
		Sleep(10000);
		val.gameModule = master.getMAdr(pID, L"client_panorama.dll");
		if (!val.gameModule) {
			std::cout << std::endl << "Can't access the game. Please restart this application. Click enter to exit...";
			getchar();
			return 0;
		}
		// Do cheat logic here:
		system("cls");
		val.localPlayer = master.readMem<intptr_t>(val.gameModule + offset.dwLocalPlayer);
		if(val.localPlayer == NULL)
			while(val.localPlayer == NULL)
				val.localPlayer = master.readMem<intptr_t>(val.gameModule + offset.dwLocalPlayer);

		while (true) {
			val.glowObject = master.readMem<intptr_t>(val.gameModule+offset.dwGlowObjectManager);
			int myTeam = master.readMem<int>(val.localPlayer + offset.m_iTeamNum);

			for (short int i = 0; i < 64; i++) {

				intptr_t entity = master.readMem<intptr_t>(val.gameModule + offset.dwEntityList + i * 0x10);
				if (entity != NULL) {
					int glowIndex = master.readMem<int>(entity + offset.m_iGlowIndex);
					int entityTeam = master.readMem<int>(entity + offset.m_iTeamNum);
					
					if (myTeam == entityTeam)
					{
						master.writeMem<float>((val.glowObject + (glowIndex * 0x38) + 0x4), 0);
						master.writeMem<float>((val.glowObject + (glowIndex * 0x38) + 0x8), 0);
						master.writeMem<float>((val.glowObject + (glowIndex * 0x38) + 0xC), 2);
						master.writeMem<float>((val.glowObject + (glowIndex * 0x38) + 0x10), 1.7);
					}
					else
					{
						master.writeMem<float>((val.glowObject + (glowIndex * 0x38) + 0x4), 2);
						master.writeMem<float>((val.glowObject + (glowIndex * 0x38) + 0x8), 0);
						master.writeMem<float>((val.glowObject + (glowIndex * 0x38) + 0xC), 0);
						master.writeMem<float>((val.glowObject + (glowIndex * 0x38) + 0x10), 1.7);
					}
					master.writeMem<bool>((val.glowObject + (glowIndex * 0x38) + 0x24), true);
					master.writeMem<bool>((val.glowObject + (glowIndex * 0x38) + 0x25), false);
				}

			}

			Sleep(1);
		}

		
		

	}	
}
