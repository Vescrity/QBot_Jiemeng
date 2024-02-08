#include "Jiemeng_Basic.h"
#ifdef _WIN32
#include <psapi.h>
#include <tlhelp32.h>
#endif
#include <stdio.h>

#include <string>
#include <libgen.h>
#include <stdlib.h>
using namespace std;
char txxt[1000];
bool IsProcessRun(char *pName)
{

#ifndef _WIN32
	{
		FILE *fp = NULL;
		char command[150];
		char buf[300];
		int count = 1;
		snprintf(command, sizeof(command), "ps -ef | grep -c %s", pName);
		fp = popen(command, "r");
		if (!fp)
		{
			printf("popen errno\n");
			return 0;
		}

		if ((fgets(buf, sizeof(buf), fp)) != NULL)
		{
			count = atoi(buf);
			// printf("%d\n", count);
		}

		pclose(fp);
		fp = NULL;
		// printf("%s=%d\n",pName,count);
		return (count > 2);
	}
#else
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	bool bFind = false;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return 0;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return false;
	}
	do
	{
		dwPriorityClass = 0;
		if (::strstr(pe32.szExeFile, pName) != NULL)
		{
			bFind = true;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return bFind;

#endif
}
void thr(string s) { system(s.c_str()); }
int main()
{
#ifdef _WIN32
	system("chcp 65001");
#endif
	info_lable("[Start]");
	info_puts("桔梦守护程序启动中...");
	if (freopen("start.cfg", "r", stdin) == NULL)
	{
		error_lable("[Start]");
		error_puts("start.cfg打开失败，请检查后重试。");
		minisleep(10000);
		return 0;
	}
	char go_name[1 << 10] = {0};
	char show_or_hide[1 << 5] = {0};
	char symbol_name[1 << 10] = {0};
	memset(txxt, 0, sizeof txxt);
	while (strlen(txxt) == 0)
	{
		if (gets(txxt) == NULL)
			goto errors;
	}
	while (strlen(go_name) == 0)
	{
		if (gets(go_name) == NULL)
			goto errors;
	}
	while (strlen(show_or_hide) == 0)
	{
		if (gets(show_or_hide) == NULL)
			goto errors;
	}
	while (strlen(symbol_name) == 0)
	{
		if (gets(symbol_name) == NULL)
			goto errors;
	}
	if (0)
	{
	errors:
		error_lable("[Start]");
		error_puts("start.cfg配置文件读取错误，请检查内容后重试。");
		minisleep(10000);
		return 0;
	}
	string start_go(go_name);

	start_go = start_go + " -faststart";
#ifndef _WIN32
	start_go = "./" + start_go;
#endif

#ifdef _WIN32
	if (show_or_hide[0] == 'h')
		start_go = start_go + " 2>NUL 1>NUL";
#else
	if (show_or_hide[0] == 'h')
		start_go = start_go + " 1>/dev/null 2>/dev/null";
#endif

	string stop_cmd(txxt);
#ifndef _WIN32
	stop_cmd = "killall -9 " + stop_cmd;
#else
	stop_cmd = "taskkill /im " + stop_cmd + " /f";
#endif
	int cnt = 0;
	for (;;)
	{
		if (!IsProcessRun(go_name))
		{
			info_lable("[Start]");
			info_puts("未检测到go-cq程序运行，将执行启动");
			thread athr(thr, start_go);
			athr.detach();
		}
		if (!IsProcessRun(txxt))
		{
			info_lable("[Start]");
			info_puts("未检测到主程序运行，将执行启动");
			string main_name(txxt);
#ifndef _WIN32
			main_name = "./" + main_name;
#endif
			thread athr(thr, main_name);
			athr.detach();
		}
		minisleep(1000);
		if (!IsProcessRun(symbol_name))
		{
			error_lable("[Start]");
			error_puts("未检测到symbol程序运行");
			if (cnt++)
			{
				error_lable("[Start]");
				error_puts("判定为主程序异常，将执行重启任务。");
				system(stop_cmd.c_str());
				cnt = 0;
				minisleep(1000);
			}
		}
		minisleep(4000);
	}
}
