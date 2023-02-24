#ifndef ANS
  #define ANS
  #include"json.hpp"
  #include"Jiemeng_Basic.h"
  #include"Msg_type.h"
  #ifdef JIE_MENG_BASE
  #include"Spj_Ans.h"
  #endif
  #include<regex>
  using namespace std;
  using json=nlohmann::json;
  bool check_priv(const char*);
  class Ans_grp{
  private:
    string reg_str;
    vector<string>anss;
    vector<string>grps;
    vector<string>user;
    vector<int>lvs;
    bool ifpriv;
    int cnt;
    int pri;

    bool type_check(const Msg_type&type);

    int r_opt(){
      int r=Rands(0,lvs[cnt]-1);
      for(int i=1;i<=cnt;i++){
        if(lvs[i]>r)return i-1;
      }return cnt-1;
    }
  public:
    friend void ans_update();
    json fexport();
    void disp();
    bool operator<(const Ans_grp&a){return pri>a.pri;}
    void get_init(const json&js);
    bool check(const Msg_type&type,const char*msg);
    string opt(){
      return anss[r_opt()];
    }

  };
  vector<Ans_grp>ans_gp;

  #ifdef JIE_MENG_BASE
  string status_msgs(){
    char sttmtxt[200];
    memset(sttmtxt,0,sizeof sttmtxt);
    sprintf(sttmtxt,"自%s至今，统计信息如下：[\\n]"
      "累计启动：%d次[\\n]累计响应消息：%d次[\\n]"
      "应答组：%d组"
      ,STAT_START_TIME.c_str(),start_time,msg_send_time,ans_gp.size());
    return sttmtxt;
  }
  string output_msg_chg(const Msg_type&type,string content);
  string get_output(const Msg_type&type,const char*msg){
    string spjstr=str_spj(type,msg);
    if(spjstr!="[SPJ_CONTINUE]")return spjstr;
    int n=ans_gp.size();
    for(int i=0;i<n;i++){
      if(ans_gp[i].check(type,msg)){
        //return ans_gp[i].opt();
        string str=ans_gp[i].opt();
        if(str=="[IGNORE]")continue;
        return output_msg_chg(type,str);
      }
    }
    return "[FAILED]";
  }
  string output_msg_chg(const Msg_type&type,string content){
    //cout<<content<<endl;
    regex reg(R"(\{%(.*?)\})");
    //string content=ans_gp[i].opt();
    smatch m;
    auto pos=content.cbegin();
    auto end=content.cend();
    int counts=0;
    for(;std::regex_search(pos, end, m, reg);)
    {
      //cout<<m.str(1)<<endl;
      counts++;
       content=string(m.prefix())+get_output(type,m.str(1).c_str())+string(m.suffix());
       pos=content.cbegin();
       end=content.cend();
      if(counts>20)return "[Too many to replace]!";
    }

    reg=regex(R"(\{\[(.*?)\]\})");
    pos=content.cbegin();
    end=content.cend();
    for(;std::regex_search(pos, end, m, reg);)
    {
      //cout<<m.str(1)<<endl;
      counts++;
      //string strs=m.str(1);
      Msg_type mtype=type;
      mtype.msgs=m.str(1);
       content=string(m.prefix())+get_output(mtype,m.str(1).c_str())+string(m.suffix());
       pos=content.cbegin();
       end=content.cend();
      if(counts>20)return "[Too many to replace]!";
    }

    return content;
  }
  bool Ans_grp::check(const Msg_type&type,const char*msg){
    if(!type_check(type))return 0;
    regex reg(reg_str);
    auto ret = regex_search(msg,reg);
    return ret;
  }

  bool Ans_grp::type_check(const Msg_type&type){
    if(user[0]=="-99"||user[0]=="99")ifpriv=1;
    else ifpriv=0;
    int n;
    if(ifpriv){
      if(!CHECK)return 0;
      if(user[0]=="-99"){
        n=user.size();
        for(int i=1;i<n;i++){
          if(user[i]==type.sender_id)return 0;
        }
      }
      //goto black_check;
    }
    if(type.ifgrp){
      n=grps.size();
      if(grps[0]=="0")return 0;
      if(grps[0]=="1"){
        if(user[0]=="1"||user[0]=="0")return 1;
        else goto private_check;
      }
      if(grps[0]=="-1"){
        for(int i=1;i<n;i++){
          if(grps[i]==type.grp_id)return 0;
        }if(user[0]=="1"||user[0]=="0")return 1;
        goto private_check;
      }
      else{
        bool flgs=0;
        for(int i=0;i<n;i++){
          if(grps[i]==type.grp_id){flgs=1;break;}
        }if(!flgs)return 0;
        if(user[0]=="1"||user[0]=="0")return 1;
        goto private_check;
      }

    }
    {

      if(user[0]=="0")return 0;
      if(user[0]=="1")return 1;
      private_check:
      if(ifpriv)return 1;
      bool flgs=user[0]=="-1";
      n=user.size();
      for(int i=0;i<n;i++){
        if(user[i]==type.sender_id)return !flgs;
      }return flgs;
    }
  }

  string chg_resend(Msg_type type,const char*msg){
    string str=get_output(type,msg);
    return (str=="[FAILED]")?"":str;
  }


  #endif
  void Ans_init(const json&js){
    int n=js.size();
    for(int i=0;i<n;i++){
      Ans_grp tmp;
      tmp.get_init(js[i]);
      ans_gp.push_back(tmp);
    }
    sort(ans_gp.begin(),ans_gp.begin()+n);
  }
  void Ans_grp::get_init(const json&js){
    pri     =js["pri"];
    reg_str =js["regex"];
    grps    =js["grps"];
    user    =js["user"];
    anss    =js["anss"];

    cnt=anss.size();
    lvs.resize(cnt+1);
    int t;
    for(int i=0;i<cnt;i++){//cout<<i<<endl;
      t=1;
      if(get_st(anss[i].c_str(),"::")==0){
        sscanf(anss[i].c_str(),"::%d::",&t);
        anss[i]=anss[i].c_str()+(get_st(anss[i].c_str()+2,"::")+4);
      }
      lvs[i+1]=lvs[i]+t;
    }
  }
  json Ans_grp::fexport(){
    json rt;
    rt["pri"]=pri;
    rt["regex"]=reg_str;
    rt["grps"]=grps;
    rt["user"]=user;
    rt["anss"]=anss;
    return rt;
  }
  json Fexport(){
    json ss;
    for(int i=0;i<ans_gp.size();i++){
      ss["Ans_grp"][i]=ans_gp[i].fexport();
    }return ss;
  }

#endif
