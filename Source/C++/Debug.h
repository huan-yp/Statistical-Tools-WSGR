#include <windows.h>    
#include <psapi.h>    
#include <direct.h> 
#include <stdio.h>
#pragma comment(lib,"psapi.lib")
namespace Debug {
	void ShowProcessMemoryUsageInfo()
	{
		HANDLE handle = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
		float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
		float memoryUsage_K = pmc.WorkingSetSize / 1024.0;

		std::cout << std::fixed << std::setprecision(2) << "ÄÚ´æÊ¹ÓÃ:" << memoryUsage_K << "K " << memoryUsage_M << "M" << std::endl;
	}
	void PrintWorkPath() {
		system("cd");
	}
}