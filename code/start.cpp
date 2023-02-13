#include"Jiemeng_Basic.h"
#include<windows.h>
#include<psapi.h>
#include<stdio.h>
#include<tlhelp32.h>
#include<string>
using namespace std;
char txxt[1000];
bool IsProcessRun(char *pName){
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	bool bFind=false;
	hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap==INVALID_HANDLE_VALUE)return 0;
	pe32.dwSize=sizeof(PROCESSENTRY32);
	if(!Process32First(hProcessSnap,&pe32)){
		CloseHandle(hProcessSnap);
		return false;
	}
	do{
		dwPriorityClass=0;
		if (::strstr(pe32.szExeFile,pName)!=NULL){
			bFind=true;
			break;
		}
	}while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return bFind;
}
void thr(string s){
	system(s.c_str());
}
int main(){
	system("chcp 65001");
	info_lable("[Start]");
	info_puts("桔梦守护程序启动中...");
	if(freopen("start.cfg","r",stdin)==NULL){
		error_lable("[Start]");
		error_puts("start.cfg打开失败，请检查后重试。");
		Sleep(10000);
		return 0;
	}
	char go_name[1<<10]={0};
	char show_or_hide[1<<5]={0};
	char symbol_name[1<<10]={0};
	memset(txxt,0,sizeof txxt);
	while(strlen(txxt)==0){
		if(gets(txxt)==NULL)goto errors;
	}
	while(strlen(go_name)==0){
		if(gets(go_name)==NULL)goto errors;
	}
	while(strlen(show_or_hide)==0){
		if(gets(show_or_hide)==NULL)goto errors;
	}
	while(strlen(symbol_name)==0){
		if(gets(symbol_name)==NULL)goto errors;
	}
	if(0){
		errors:error_lable("[Start]");
		error_puts("start.cfg配置文件读取错误，请检查内容后重试。");
		Sleep(10000);
		return 0;
	}
	string start_go(go_name);
  start_go=start_go+" -faststart";

	if(show_or_hide[0]=='h')start_go=start_go+" 2>NUL 1>NUL";
	string stop_cmd(txxt);
	stop_cmd="taskkill /im "+stop_cmd+" /f";
	int cnt=0;
	for(;;){
		if(!IsProcessRun(go_name)){
			//WinExec(start_go.c_str(),SW_SHOWNORMAL);
			//else
			//WinExec(start_go.c_str(),SW_HIDE);
			info_lable("[Start]");
			info_puts("未检测到go-cq程序运行，将执行启动");
			thread athr(thr,start_go);
			athr.detach();
		}
    if(!IsProcessRun(txxt)){
			info_lable("[Start]");
			info_puts("未检测到主程序运行，将执行启动");
			WinExec(txxt,SW_SHOWNORMAL);
		}
		Sleep(5000);
		if(!IsProcessRun(symbol_name)){
			error_lable("[Start]");
			error_puts("未检测到symbol程序运行");
			if(cnt++){
				//WinExec(stop_cmd.c_str(),SW_SHOWNORMAL);
				error_lable("[Start]");
				error_puts("判定为主程序异常，将执行重启任务。");
				system(stop_cmd.c_str());
				cnt=0;
				Sleep(500);
			}
		}

	}
}
