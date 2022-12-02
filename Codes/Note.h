#ifndef NOTE
#define NOTE
#include<stdio.h>
#include<string>
#include<map>
#include"opstring.h"
#include"Jie_Meng_Base.h"
using namespace std;
map<string,string>note_notes;
bool note_read;
bool chg_note(const char*msg);
bool read_note(){
  note_read=1;
  freopen(NOTE_FILENAME.c_str(),"r",stdin);
  char ss[1<<15]={0};
  gets(ss);
  puts(ss);
  while(ss[0]!='/'){
    if(!chg_note(ss))return 0;
    gets(ss);
    puts(ss);
  }return 1;
}
bool chg_note(const char*msg){
  char mssg[1<<15]={0};
  strcpy(mssg,msg);
  strchg("\\u0026#91;","[",mssg);
  strchg("\\u0026#93;","]",mssg);
  strchg("\n","[\\n]",mssg);
  if(!note_read)if(!read_note())return 0;
  int l=get_st(mssg,"#");
  string s=mssg+l+1;
  char ss[1<<10]={0};
  int r=get_st(mssg,"~");
  if(r==-1)return 0;
  get_copy(l+1,r,mssg,ss);
  note_notes[ss]=mssg+r+1;
  puts(note_notes[ss].c_str());
  return 1;
}string get_note(const char*msg){
  if(!note_read)if(!read_note())return "ERROR_IN_NOTEFILE";
  int l=get_st(msg,"#");
  string s=msg+l+1;
  if(note_notes.find(s)==note_notes.end()){
    return "[NONE]";
  }
  return note_notes[s];
}

bool write_note(){
  if(!note_read)if(!read_note())return 0;
  FILE* fopt;
  fopt=fopen(NOTE_FILENAME.c_str(),"w");
  for(auto it:note_notes){
    fprintf(fopt,"#%s~%s\n",it.first.c_str(),it.second.c_str());
  }
  fprintf(fopt,"//end\n");
  fclose(fopt);
  return 1;
}
#endif
