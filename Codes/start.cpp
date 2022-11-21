#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
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
int main(){
	freopen("start.cfg","r",stdin);
	char go_name[1<<10]={0};
	char symbol_name[1<<10]={0};
	memset(txxt,0,sizeof txxt);
	while(strlen(txxt)==0)gets(txxt);
	while(strlen(go_name)==0)gets(go_name);
	while(strlen(symbol_name)==0)gets(symbol_name);
	string start_go(go_name);
  start_go=start_go+" -faststart";
	string stop_cmd(txxt);
	stop_cmd="taskkill /im "+stop_cmd+" /f";
	int cnt=0;
	for(;;){
    if(!IsProcessRun(txxt))
			WinExec(txxt,SW_SHOWNORMAL);
    if(!IsProcessRun(go_name))
  		WinExec(start_go.c_str(),SW_SHOWNORMAL);
		Sleep(5000);
		if(!IsProcessRun(symbol_name)){
			if(++cnt==2){
				WinExec(stop_cmd.c_str(),SW_SHOWNORMAL);
				cnt=0;
			}
		}

	}
}
