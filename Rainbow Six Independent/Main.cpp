// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Globals.h"
#include "Utility.h"
#include "Memory.h"
#include "Overlay.h"
#include <iostream>
#include <time.h>
#include <Psapi.h>

HWND gameWindow = FindWindow(NULL, L"Rainbow Six");

//Defining our globals
namespace Global {
	HANDLE GameHandle = 0x0;

	LPVOID BaseAddress = NULL;

	std::string LocalName = "Name";

	Memoryyy Memory = Memoryyy();

	Overlayyy Over = Overlayyy();

	BOOL Aimbot = TRUE;
	
	BOOL Spread = FALSE;

	BOOL Recoil = TRUE;
}

DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

void OpenHandle() {
	DWORD processID = FindProcessId(L"RainbowSix.exe");
	Global::GameHandle = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, processID);
}

static const char consoleNameAlphanum[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int consoleNameLength = sizeof(consoleNameAlphanum) - 1;

char genRandomConsoleName()
{
	return consoleNameAlphanum[rand() % consoleNameLength];
}

int main()
{
	srand(time(0));
	std::wstring ConsoleNameStr;
	for (unsigned int i = 0; i < 20; ++i)
	{
		ConsoleNameStr += genRandomConsoleName();

	}
	SetConsoleTitle(ConsoleNameStr.c_str());
	std::cout << "Siege External" << std::endl;
	std::cout << "-----" << std::endl;
	if (gameWindow != 0) {
		std::cout << "Game window found" << std::endl;

		OpenHandle();

		std::cout << "Local name: " << std::flush;
		std::cin >> Global::LocalName;

		Sleep(1000);

		if (Global::GameHandle == INVALID_HANDLE_VALUE || Global::GameHandle == NULL || Global::GameHandle == (HANDLE)0x0) {
			std::cout << "Invalid handle to R6" << std::endl;
			system("pause");
			return 1;
		}

		std::string confirmation;

		std::cout << "Confirm [Y]/[N]: " << std::flush;
		std::cin >> confirmation;

		if (confirmation == "Y") {
			Global::Memory.SetBaseAddress();
			std::cout << "Base address: " << std::hex << Global::Memory.GetBaseAddress() << std::dec << std::endl;

			Global::Over.SetupWindow();
			std::cout << "Overlay window set" << std::endl;

			//Start the main loop
			Global::Over.Loop();

			system("pause");
			return EXIT_SUCCESS;

			return 0;
		}
		else {
			std::cout << "Closing..." << std::endl;
			Sleep(2000);
		}
		return 0;
	}
	else {
		std::cout << "Game must be running to continue." << std::endl;
		std::cout << "" << std::endl;
		system("pause");
		return 0;
	}
}
