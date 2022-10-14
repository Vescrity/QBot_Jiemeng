#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<windows.h>
#include<psapi.h>
#include<stdio.h>
#include<tlhelp32.h>
#include<string.h>
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
	char txxt[128];
	memset(txxt,0,sizeof txxt);
	while(strlen(txxt)==0){
		gets(txxt);
	}
	for(;;){
		if(!IsProcessRun(txxt))
			WinExec(txxt,SW_SHOWNORMAL);
		Sleep(5000);
	}
}
