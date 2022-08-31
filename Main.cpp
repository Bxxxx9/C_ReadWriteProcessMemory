#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <vector>

using namespace std;

DWORD GetModule(const TCHAR* IpszModuleName, DWORD pID)
{

	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);
	if (handle != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(handle, &entry))
		{
			do
			{
				if (!_strcmpi(IpszModuleName, entry.szModule))
				{
					CloseHandle(handle);
					return (DWORD)entry.modBaseAddr;
				}
			} while (Module32Next(handle, &entry));
		}
	}
	return 0;

}

int main()
{
	HWND gamewindow = FindWindow(NULL, "AssaultCube");
	if (gamewindow == NULL)
	{
		cout << "This game not found !";
		return 0;
	}
	DWORD PID = NULL;
	GetWindowThreadProcessId(gamewindow, &PID);
	
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == INVALID_HANDLE_VALUE || hProcess == NULL)
	{
		cout << "feiled to open Process";
		return 0;
	}
	char modulname[] = "ac_client.exe";
	printf_s("%p\n", GetModule(modulname, PID));
	DWORD gameBaseAddress = GetModule(modulname, PID);

	DWORD localPlayer;
	ReadProcessMemory(hProcess, (LPCVOID)(gameBaseAddress + 0x17E0A8), &localPlayer, sizeof(localPlayer), NULL);
	printf("%p", localPlayer);
	while (true)
	{
		int health;
		ReadProcessMemory(hProcess, (LPCVOID)(localPlayer + 0xEC), &health, sizeof(health), NULL);
		printf("heath = %d\n", health);
		int ammo;
		ReadProcessMemory(hProcess, (LPCVOID)(localPlayer + 0x140), &ammo, sizeof(ammo), NULL);
		printf("ammo = %d\n", ammo);
		float Z;
		ReadProcessMemory(hProcess, (LPCVOID)(localPlayer + 0x30), &Z, sizeof(Z), NULL);
		printf("Z = %f\n", Z);
		float Y;
		ReadProcessMemory(hProcess, (LPCVOID)(localPlayer + 0x2C), &Y, sizeof(Y), NULL);
		printf("Y = %f\n", Y);
		float X;
		ReadProcessMemory(hProcess, (LPCVOID)(localPlayer + 0x28), &X, sizeof(X), NULL);
		printf("X = %f\n", X);

		if (ammo < 150)
		{
			int ammoplus = 200;
			WriteProcessMemory(hProcess, (LPVOID)(localPlayer + 0x140), &ammoplus, sizeof(ammoplus), 0);

		}
		if (health < 150)
		{
			int ammoplus = 200;
			WriteProcessMemory(hProcess, (LPVOID)(localPlayer + 0xEC), &ammoplus, sizeof(ammoplus), 0);
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			float newvalue = Z + 3;
			WriteProcessMemory(hProcess, (LPVOID)(localPlayer + 0x30), &newvalue, sizeof(newvalue), 0);
		}// jump
		Sleep(25);
		system("cls");
	}
}