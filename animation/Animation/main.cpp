#include "main.h"
#include "AppDelegate.h"

#include <windows.h>
#include <shellapi.h>

#include "JSON/CustomAnimation.h"
#include "HelperFunc.h"


USING_NS_CC;

// uncomment below line, open debug console
#define USE_WIN32_CONSOLE

void mergePngs(wchar_t* path)
{
	SHELLEXECUTEINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SEE_MASK_NOCLOSEPROCESS;
	si.lpVerb = _TEXT("open");
	si.lpFile = path;
	si.nShow = SW_SHOWNORMAL;

	ShellExecuteEx(&si);

	DWORD dwExitCode;
	GetExitCodeProcess(si.hProcess, &dwExitCode);

	while (dwExitCode == STILL_ACTIVE)
	{
		Sleep((DWORD)5);
		GetExitCodeProcess(si.hProcess, &dwExitCode);
	}
	CloseHandle(si.hProcess);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	/*UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);*/

#ifdef USE_WIN32_CONSOLE
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	int argc = 0;
	LPWSTR *lpParam = NULL;
	lpParam = CommandLineToArgvW(lpCmdLine, &argc);
	if (5 <= argc)
	{
		std::string resPath, roleName, savePath, plistPath, frameRate;
		resPath = Tool::HelperFunc::UnicodeToANSI(lpParam[0]);
		roleName = Tool::HelperFunc::UnicodeToANSI(lpParam[1]);
		plistPath = Tool::HelperFunc::UnicodeToANSI(lpParam[2]);
		savePath = Tool::HelperFunc::UnicodeToANSI(lpParam[3]);
		frameRate = Tool::HelperFunc::UnicodeToANSI(lpParam[4]);
		int iRate = atoi(frameRate.c_str());

		if (argc == 6)
		{
			mergePngs(lpParam[5]);
		}
	
		if (roleName == "all")
		{
			std::vector<std::string> roles = Tool::HelperFunc::GetSubDirFiles(resPath);
			for (int i = 0; i < roles.size();++i)
			{
				Tool::CustomAnimation::exportToJson(resPath, roles[i], plistPath, savePath, iRate);
			}
		}
		else if (!roleName.empty() && roleName != "none")
		{
			Tool::CustomAnimation::exportToJson(resPath, roleName, plistPath, savePath, iRate);
		}
	}
	else
	{
		printf("cmdline parameters is wrong!\n");
	}
	
	for (int i = 0; i < argc; ++i)
	{
		std::string argv = Tool::HelperFunc::UnicodeToANSI(lpParam[i]);
		printf("param [%d] : %s\n", i, argv.c_str());
	}
	

	// create the application instance
	AppDelegate app;
	int ret = Application::getInstance()->run();

#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

	return ret;
}
