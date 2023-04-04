#ifndef CODE_RUNNER
#define CODE_RUNNER
#include <string>
#include "Jiemeng_Basic.h"
using namespace std;

string cpp_runner(const string &code)
{
  FILE *fi;
  fi = fopen("test.cpp", "w");
#ifndef LINUX_V
  fprintf(fi, "#include<stdio.h>\n#include<stdlib.h>\n#include<thread>\n#include<windows.h>\n");
  fprintf(fi, "void calc();\n");
  fprintf(fi, "void tm(){Sleep(200);printf(\"\\nTLE\\n\");exit(1);}\n");
#else
  fprintf(fi, "#include<stdio.h>\n#include<stdlib.h>\n#include<thread>\n#include <unistd.h>\n");
  fprintf(fi, "void calc();\n");
  fprintf(fi, "void tm(){usleep(200000);printf(\"\\nTLE\\n\");exit(1);}\n");
#endif
  fprintf(fi, "int main(){std::thread t(tm);t.detach();calc();return 0;}\n");
  fprintf(fi, "%s", code.c_str());
  fclose(fi);
  char buf[1 << 16] = {0};
  memset(buf, 0, sizeof buf);
  execmd("g++ 2>&1 test.cpp -o test.exe", buf);
  execmd("test.exe", buf + strlen(buf));
  return buf;
}
string py_runner(const string &code)
{
  FILE *fi;
  string scode = str_strchg("\n", "\n  ", code.c_str());
  fi = fopen("test.py", "w");
  fprintf(fi, "import time\nfrom func_timeout import func_set_timeout\n\n");
  fprintf(fi, "import sys, io\nsys.stdout = io.TextIOWrapper(sys.stdout.buffer,encoding='utf8')\n");
  fprintf(fi, "@func_set_timeout(3)\ndef task():\n");
  fprintf(fi, "%s\n", scode.c_str());
  fprintf(fi, "if __name__ == '__main__':\n  task()");
  fclose(fi);
  char buf[1 << 16] = {0};
  execmd("python 2>&1 test.py", buf);
  if (strlen(buf) == 0)
    return "OK!";
  return buf;
}
string R_runner(const string &code)
{
  FILE *fi;
  string scode = str_strchg("\r", "", code.c_str());
  fi = fopen("test.r", "w");
  fprintf(fi, "require('R.oo',quietly=TRUE,warn.conflicts=FALSE)\nrequire('R.utils',quietly=TRUE,warn.conflicts=FALSE)\n");
  fprintf(fi, "fexe <- function() {");
  fprintf(fi, "%s\n", scode.c_str());
  fprintf(fi, "}\nres <- withTimeout({\n  fexe()\n}, timeout = 3, onTimeout = \"silent\")");
  fclose(fi);
  char buf[1 << 16] = {0};
  execmd("Rscript 2>&1 test.r", buf);
  if (strlen(buf) == 0)
    return "OK!";
  return buf;
}

#endif