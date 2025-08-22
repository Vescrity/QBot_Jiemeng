#include "txt2img_api.hpp"
#include "Jiemeng_Basic.h"
#include <fstream>
using string = std::string;
string txt2img(const string &txt) {
    const int N = 16;
    static int Now = 0;
    char name[16] = {0};
    char txtname[32] = {0};
    char filename[32] = {0};
    char excmd[128] = {0};
    sprintf(name, "text%03d", Now++);
    sprintf(txtname, "tmp/%s.txt", name);
    sprintf(filename, "tmp/%s.png", name);
    sprintf(excmd, "txt2img/txt2img %s %s", txtname, filename);
    if (Now >= N)
        Now = 0;

    std::ofstream out_file(txtname);
    if (out_file) {
        out_file << txt;
        out_file.close();
    }
    return execmd(excmd);
}
