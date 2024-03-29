#ifndef CODE_RUNNER
#define CODE_RUNNER
#include <string>
#include "Jiemeng_Basic.h"
#include "Func_Timeout.h"
using namespace std;

/// @brief Run command
/// @param msg
/// @return
string String_Cmd_Run(const char *msg)
{
  return execmd(msg);
}

string fortran_runner(const string &code)
{
  static int tmpn = 0;
  tmpn++;
  if (tmpn >= 8)
    tmpn = 0;
  FILE *fi;
  char fname[32] = {0};
  char codename[32] = {0};
  char gforcmd[128] = {0};
  char exname[32] = {0};
  char stopcmd[128] = {0};
  char excmd[32] = {0};
  sprintf(fname, "Fortrantest%03d", tmpn);
  sprintf(codename, "tmp/%s.f90", fname);
#ifdef _WIN32
  sprintf(exname, "%s.exe", fname);
  sprintf(stopcmd, "taskkill /im %s /f", exname);
  sprintf(excmd, "tmp\\%s", exname);
#else
  sprintf(exname, "./%s", fname);
  sprintf(stopcmd, "killall -9 %s", fname);
  sprintf(excmd, "./tmp/%s", exname);
#endif

  sprintf(gforcmd, "gfortran 2>&1 %s -o tmp/%s", codename, exname);
  fi = fopen(codename, "w");
  fprintf(fi, "%s", code.c_str());
  fclose(fi);
  string rt = String_Cmd_Run(gforcmd);
  auto Tl = [stopcmd]()
  {
    string str = stopcmd;
    minisleep(500);
    system(str.c_str());
  };
  thread tl = thread(Tl);
  tl.detach();
  rt += String_Cmd_Run(excmd);
  return rt;
}

/// @brief Try to run the given Matlab code with a 5s time limit
/// @param code the Matlab code
/// @return the result
string Matlab_runner(const string &code)
{
  FILE *fi, *fp;
  /*
  fp = fopen("outtmp.txt", "w");
  fprintf(fp, "");
  fclose(fp);*/
  fi = fopen("tmp/mat_code.m", "w");
  fprintf(fi, "%s", code.c_str());
  fclose(fi);
  char strText[1 << 16] = {0};
#ifdef _WIN32
  /*string comm = "mat_run.exe";

  system(comm.c_str());
  fp = fopen("outtmp.txt", "r");
  char sstrt[1 << 8];
  while (fgets(sstrt, 1 << 8, fp) != NULL)
    sprintf(strText + strlen(strText), "%s", sstrt);
  if (strlen(strText) < 1)
    sprintf(strText, "OK!");
  fclose(fp);
  fp = fopen("outtmp.txt", "w");
  fprintf(fp, "");
  fclose(fp);
  return strText;*/
  return "Unsupport.";
#else
  string comm = "./jm_script/octave_launch";
  return String_Cmd_Run(comm.c_str());
#endif
}

/// @brief Try to run the given c++ code with a 200ms time limit
/// @param code the c++ code
/// @return the result
string cpp_runner(const string &code)
{
  FILE *fi;
  fi = fopen("tmp/test.cpp", "w");
#ifdef _WIN32
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
#ifdef _WIN32
  return execmd("g++ 2>&1 test.cpp -o test.exe") + execmd("test.exe");
#else
  string aa = execmd("g++ 2>&1 tmp/test.cpp -o tmp/test");
  aa = aa + execmd("tmp/test");
  return aa;
#endif
}
/// @brief Try to run the given python code with a 3s time limit
/// @param code the python code
/// @return the result
string py_runner(const string &code)
{
  FILE *fi;
  string scode = str_strchg("\n", "\n  ", code.c_str());
  fi = fopen("tmp/test.py", "w");
  fprintf(fi, "import time\nfrom func_timeout import func_set_timeout\n\n");
  fprintf(fi, "import sys, io\nsys.stdout = io.TextIOWrapper(sys.stdout.buffer,encoding='utf8')\n");
  fprintf(fi, "@func_set_timeout(3)\ndef task():\n");
  fprintf(fi, "%s\n", scode.c_str());
  fprintf(fi, "if __name__ == '__main__':\n  task()");
  fclose(fi);
  string buf;
  buf = execmd("python 2>&1 tmp/test.py");
  if ((buf.length()) == 0)
    return "OK!";
  return buf;
}
/// @brief Try to run the given R code with a 3s time limit
/// @param code the R code
/// @return the result
string R_runner(const string &code)
{
  FILE *fi;
  string scode = str_strchg("\r", "", code.c_str());
  fi = fopen("tmp/test.r", "w");
  fprintf(fi, "require('R.oo',quietly=TRUE,warn.conflicts=FALSE)\nrequire('R.utils',quietly=TRUE,warn.conflicts=FALSE)\n");
  fprintf(fi, "fexe <- function() {");
  fprintf(fi, "%s\n", scode.c_str());
  fprintf(fi, "}\nres <- withTimeout({\n  fexe()\n}, timeout = 3, onTimeout = \"silent\")");
  fclose(fi);
  string buf;
  buf = execmd("Rscript 2>&1 tmp/test.r");
  if ((buf.length()) == 0)
    return "OK!";
  return buf;
}

#endif