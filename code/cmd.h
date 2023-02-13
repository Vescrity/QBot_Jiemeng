// created by (c) AiCorein https://github.com/AiCorein

#ifndef __CMD_H__
#define __CMD_H__
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tchar.h>
#include <string.h>
#include <sys/stat.h>
#include <cstdio>
#include <random>
using namespace std;


class Cmd {
public:
    string cmd;
    string retText = "[NONE]";
    int runDelay;
    int id;
    string tempName1 = "_write.tmp";
    string tempName2 = "_res.tmp";
    random_device rd;

    Cmd(const string& cmdStr, int delay = 5) {
        cmd = cmdStr;
        runDelay = delay;
        id = (rd() % 10000) + 0;
        string pre = to_string(id);
        tempName1 = pre + tempName1;
        tempName2 = pre + tempName2;
    }

    inline string getRes() {
        cmdExec();
        while (isFileExist(tempName2) || isFileExist(tempName1)) {
            remove(tempName1.c_str());
            remove(tempName2.c_str());
            Sleep(100);
        }
        return retText;
    }

private:
    inline bool isFileExist(const string& name) {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }

    inline void cmdExec() {
        string ArgsText = "";
        ArgsText.append("/c "); ArgsText += cmd;
        ArgsText.append(" > "); ArgsText += tempName1;
        ArgsText.append(" & copy "); ArgsText += tempName1;
        ArgsText.append(" "); ArgsText += tempName2;

        ShellExecute(NULL, _T("open"), _T("cmd"), ArgsText.c_str(), NULL, SW_HIDE);

        int i = 0;
        while (true) {
            if (isFileExist(tempName2)) break;
            else {
                Sleep(100);
                i += 100;
                if (i > runDelay * 1000) {
                    return;
                }
            }
        }
        fstream fs(tempName2);
        stringstream ss;
        ss << fs.rdbuf();
        retText = ss.str();
        fs.close();
    }
};

void createCmdTask(string cmdText, int delay, string& result) {
    result = Cmd(cmdText, delay).getRes();
}

#endif
