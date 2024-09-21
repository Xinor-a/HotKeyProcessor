#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct HOTKEY_DATA {
	bool bCtrl;
	bool bShift;
	bool bAlt;
	char cKey;
	string strCommand;

	bool flag;
};

class clsHotKeyProcessor {
private:
	vector<HOTKEY_DATA> HKDs;
public:
	clsHotKeyProcessor(string filename) {
		LoadHotKeys(filename);
	}

	bool LoadHotKeys(string filename);
	void SetNewShortCut(bool bCtrl, bool bShift, bool bAlt, char cKey, string strCommand);
	bool MonitorShortCut();
	bool WatchKey(bool bCtrl, bool bShift, bool bAlt, char cKey);
};

DWORD WINAPI DoCommand(LPVOID _command) {
	STARTUPINFO si{};
	PROCESS_INFORMATION pi{};

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	LPTSTR command = (LPTSTR)_command;

	CreateProcess(
		nullptr,
		(LPTSTR)command,
		nullptr,
		nullptr,
		false,
		0,
		nullptr,
		nullptr,
		&si,
		&pi
	);

	WaitForSingleObject(pi.hThread, INFINITE);
	CloseHandle(pi.hThread);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	delete[]_command;
	_command = nullptr;

	ExitThread(0);

	return 0;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	clsHotKeyProcessor HotKeyProcessor("HotKey.txt");

	while (1) {
		Sleep(1);
		if (!HotKeyProcessor.MonitorShortCut()) break;
	}

	return 0;
}

bool clsHotKeyProcessor::LoadHotKeys(string filename) {
	SetNewShortCut(true, true, true, 'H', "cmd.exe /c HotKey.txt");

	ifstream fileExists(filename);
	string buff;

	if (!fileExists) {
		ofstream ofs(filename);
		ofs << "CTRL, SHIFT, ALT, KEY, COMMAND" << endl;
		ofs.close();
	}
	else {
		fileExists.close();
	}

	ifstream ifs(filename);

	getline(ifs, buff);	//ignores the first line
	getline(ifs, buff);	//ignores the second line

	while (getline(ifs, buff)) {
		string strTem;

		bool bCtrl;
		bool bShift;
		bool bAlt;
		char cKey;
		string strCommand;

		// Ctrl
		strTem = buff.substr(0, buff.find_first_of(",", 0));
		bCtrl = strTem == "TRUE";

		buff = buff.substr(buff.find_first_of(",", 0) + 1);

		// Shift
		strTem = buff.substr(0, buff.find_first_of(",", 0));
		bShift = strTem == "TRUE";

		buff = buff.substr(buff.find_first_of(",", 0) + 1);

		//Alt
		strTem = buff.substr(0, buff.find_first_of(",", 0));
		bAlt = strTem == "TRUE";

		buff = buff.substr(buff.find_first_of(",", 0) + 1);

		// Key
		cKey = buff[0];
		buff = buff.substr(buff.find_first_of(",", 0) + 1);

		// Command
		strCommand;
		if (buff.find_first_of(",", 0) == string::npos) {
			strCommand = buff;
			strCommand = "cmd.exe /c " + strCommand;
		}
		else return false;

		SetNewShortCut(bCtrl, bShift, bAlt, cKey, strCommand);
	}
	ifs.close();

	return true;
}

void clsHotKeyProcessor::SetNewShortCut(bool bCtrl, bool bShift, bool bAlt, char cKey, string strCommand) {
	HOTKEY_DATA hkd;
	hkd.bCtrl = bCtrl;
	hkd.bShift = bShift;
	hkd.bAlt = bAlt;
	hkd.cKey = cKey;
	hkd.strCommand = strCommand;

	hkd.flag = false;

	this->HKDs.push_back(hkd);

	return;
}

bool clsHotKeyProcessor::MonitorShortCut() {
	// Quit
	if (WatchKey(true, true, true, 'Q')) {
		return false;
	}

	// ReLoad
	if (WatchKey(true, true, true, 'R')) {
		this->HKDs.clear();

		LoadHotKeys("HotKey.txt");

		return true;
	}

	for (HOTKEY_DATA& hkd : this->HKDs) {
		DWORD dwd;
		if (WatchKey(hkd.bCtrl, hkd.bShift, hkd.bAlt, hkd.cKey)) {
			if (!hkd.flag) {
				size_t lengthStr = hkd.strCommand.length() + 1;
				wchar_t* command = new wchar_t[lengthStr];

				size_t lengthConverted;
				mbstowcs_s(&lengthConverted, command, lengthStr, hkd.strCommand.c_str(), _TRUNCATE);

				CreateThread(
					NULL,
					0,
					DoCommand,
					(LPVOID)command,
					0,
					&dwd
				);
			}

			hkd.flag = true;
		}
		else if (hkd.flag) {
			hkd.flag = false;
		}
	}
	return true;
}

bool clsHotKeyProcessor::WatchKey(bool _bCont, bool _bShift, bool _bAlt, char _cKey) {
	SHORT rc;

	rc = GetAsyncKeyState(VK_CONTROL);
	rc = rc & 0x8000;
	if ((_bCont && !rc) || (!_bCont && rc)) {
		return false;
	}

	rc = GetAsyncKeyState(VK_SHIFT);
	rc = rc & 0x8000;
	if ((_bShift && !rc) || (!_bShift && rc)) {
		return false;
	}

	rc = GetAsyncKeyState(VK_MENU);
	rc = rc & 0x8000;
	if ((_bAlt && !rc) || (!_bAlt && rc)) {
		return false;
	}

	rc = GetAsyncKeyState(_cKey);
	rc = rc & 0x8000;
	if (!rc) {
		return false;
	}

	return true;
}
