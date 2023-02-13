#ifndef SPECIAL_JUDGE
  #define SPECIAL_JUDGE
  #include<string>
  #include"output.h"
  #include"File_Read.h"
  #include"Jie_Meng_Base.h"
  #include"HttpPost.h"
  #include"Game.h"
  #include<map>
  #include<sstream>
  #include<stdlib.h>
  #include"md5.h"
  #include"AI_draw2.h"
  #include"Note.h"
  #include"Weather_Report.h"
  #include"Encrypt.h"
  #define CHECK (string(type.sender_id)=="ADMIN"||check_priv(type.sender_id)||type.ord_lv>99)
  using namespace std;
  void resend(Msg_type type,const char*msg){
    if(spj(type,msg))return;
    for(int i=0;i<grp_num;i++){
      if(grp[i].crt(type,msg)){
        grp[i].print(type);
        break;
      }
    }
  }
  map<string,Connect_Four>game_list;
  void lpscq(Msg_type);
  bool execmd(const char* cmd,char* result);
  void run_cmd(Msg_type);
  void Chem(Msg_type);
  /**********Special Send***************/
  void send_poke(Msg_type type,char *target_id){
    string msg("[CQ:poke,qq=");
    string id(target_id);
    msg=msg+id;
    msg=msg+"]";
    output(type,msg.c_str());
    for(int i=0;i<grp_num;i++){
      if(grp[i].crt(type,"[poke]")){
        grp[i].print(type);
        return;
      }
    }
  }
  string time_cal(const char*);
  string tags(const char*txt,int l,const char c){
    string rt;
    char s[2];
    rt="\0";
    s[1]=0;
    rt+=c;
    rt+="tag=";
    for(int i=l+1;i<strlen(txt);i++){
      if(txt[i]=='#'){
        rt+="&tag=";continue;
      }if(txt[i]==']')break;
      s[0]=txt[i];
      rt+=acl_url_encode(s);
    }
    rt+="\0";
    return rt;
  }
  /**********Special Judge****************/
  bool spj_poke(const char *msg){
    string aa("\"target_id\":");
    aa=aa+Self_ID;
    return get_st(msg,"\"sub_type\":\"poke\"")!=-1&&get_st(msg,aa.c_str())!=-1;
  }bool spj_rand(const char *msg){
    return pfct(msg,".rd");
  }
  bool spj_jrrp(const char*msg){
    return pfct(msg,".jrrp");
  }bool spj_pfct(const char*txxt,const char*msg){
    return pfct(msg,txxt)&&strlen(msg)==strlen(txxt);
  }
  bool spj(Msg_type type/*,const char *buf*/,const char*msg){
      char temp[100],temp2[10];
      memset(temp,0,sizeof temp);
      memset(temp2,0,sizeof temp2);
      if(main_switch){
        /*if(spj_poke(buf)){
          //get_sender_id(buf,temp);
          if(get_st(type.sender_id,Self_ID.c_str())!=-1)return 0;
          for(int i=0;i<grp_num;i++){
            if(grp[i].crt(type,"[poke]")){
              grp[i].print(type);
              break;
            }
          }
          return 1;
        }*/if(get_st(msg,"[plugin:")==0){
          int l=8;
          int r=strlen(msg)-1;
          char pl_name[1<<7]={0};
          get_copy(l,r,msg,pl_name);
          string commd=".\\plugin\\";
          commd=commd+pl_name+"\\"+pl_name+".exe";
          info_lable("[plugin]");
          info_print("调用插件");
          info_puts(pl_name);
          FILE*fp;
          fp=fopen("Current_msg.txt","w");
          fprintf(fp,"%s",type.msgs.c_str());
          fclose(fp);
          char buf[1<<13]={0};
          execmd(commd.c_str(),buf);
          output(type,buf);
          return 1;
        }
        if(spj_pfct(".info",msg)){
          string vinfo="";
          vinfo=vinfo+CURRENT_VERSION+
            "\n更新于"+UPDATE_TIME+"[\\n]"
            "开发by雪と葉[\\n]"
            "项目地址:https://github.com/Vescrity/QBot_Jiemeng[\\n]"
            "Special thanks@AiCorein https://github.com/AiCorein[\\n]"
            "请勿滥用bot酱呐♥";
          output(type,vinfo.c_str());
          return 1;
        }if(get_st(msg,"[msgs:")==0){
          int l,r,sltm,cnt=0;
          l=strlen("[msgs:");
          char buf[1<<15]={0};
          sprintf(buf,"{\"group_id\": %s,\"messages\": [",type.grp_id);
          while(1){
            if(msg[l]=='~'){
              l++;
              char date[16]={0};
              char id[16]={0};
              char name[1<<7]={0};
              char text[1<<13]={0};
              r=get_st(msg+l,"#")+l;
              get_copy(l,r,msg,date);
              l=r+1;
              r=get_st(msg+l,"#")+l;
              get_copy(l,r,msg,id);
              l=r+1;
              r=get_st(msg+l,"#")+l;
              get_copy(l,r,msg,name);
              l=r+1;
              r=(get_st(msg+l,"~")==-1?(strlen(msg+l)-1):get_st(msg+l,"~"))+l;
              get_copy(l,r,msg,text);
              l=r;
              string tmm=time_cal(date);
              {
                if(cnt)sprintf(buf+strlen(buf),",");
                sprintf(buf+strlen(buf),"{\"type\": \"node\",\"data\": {");
                sprintf(buf+strlen(buf),"\"name\": \"%s\",",name);
                sprintf(buf+strlen(buf),"\"uin\": \"%s\",",id);
                sprintf(buf+strlen(buf),"\"content\": \"%s\",",text);
                sprintf(buf+strlen(buf),"\"time\": \"%s\"}}",tmm.c_str());
                cnt++;
              }
            }else{
              sprintf(buf+strlen(buf),"]}");
              break;
            }
          }
          HttpRequest httpReq("127.0.0.1",OUTPORT);
          //printf("%s\n",buf);
          httpReq.HttpPost("/send_group_forward_msg",buf);
          return 1;
        }

        if(spj_pfct("[get_163]",msg)){
          int l,r;
          l=get_st(type.msgs.c_str(),"#");
          r=strlen(type.msgs.c_str());
          if(l==-1)return 0;
          char name[128],sid[20];
          memset(name,0,sizeof name);
          memset(sid,0,sizeof sid);
          get_copy(l+1,r,type.msgs.c_str(),name);
          char*ff=acl_url_encode(name);
          string rt(ff);
          free(ff);
          //puts(rt.c_str());
          string ord="curl -s ";
          string url="\"http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=";
          url=url+rt+"&type=1&offset=0&total=true&limit=1\"";
          char bf[2048];
          memset(bf,0,sizeof bf);
          ord=ord+url;
          execmd(ord.c_str(),bf);
          l=get_st(bf,"[{\"id\":");
          r=get_st(bf+l,",\"");
          r+=l;
          get_copy(l+7,r,bf,sid);
          string opt="[163:";
          opt=opt+sid+",qwq]";
          output(type,opt.c_str());
          return 1;
        }if(spj_pfct("[rec_163]",msg)){
          int l,r;
          l=get_st(type.msgs.c_str(),"#");
          r=strlen(type.msgs.c_str());
          if(l==-1)return 0;
          char name[128],sid[20];
          memset(name,0,sizeof name);
          memset(sid,0,sizeof sid);
          get_copy(l+1,r,type.msgs.c_str(),name);
          char*ff=acl_url_encode(name);
          string rt(ff);
          free(ff);
          //puts(rt.c_str());
          string ord="curl -s ";
          string url="\"http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=";
          url=url+rt+"&type=1&offset=0&total=true&limit=1\"";
          char bf[2048];
          memset(bf,0,sizeof bf);
          ord=ord+url;
          execmd(ord.c_str(),bf);
          l=get_st(bf,"[{\"id\":");
          r=get_st(bf+l,",\"");
          r+=l;
          get_copy(l+7,r,bf,sid);




          string opt="[CQ:record,file=http://music.163.com/song/media/outer/url?id=";
          opt=opt+sid+".mp3]";




          output(type,opt.c_str());
          return 1;
        }if(get_st(msg,"[lolicon_setu")==0){
          int l,r;
          char bf[2048];
          memset(bf,0,sizeof bf);
          string str,str2;
          str="curl -s \"https://api.lolicon.app/setu/v2";
          str2=tags(msg,13,'?');
          str=str+str2+"\"";
          //puts(str.c_str());
          execmd(str.c_str(),bf);
          l=get_st(bf,"{\"original\"");
          r=get_st(bf+l,"\"}");
          if(l==-1){
            resend(type,"[Not Found]");
            return 1;
          }
          r+=l;
          l+=13;
          string opt="[CQ:image,file=";
          char furl[512];
          memset(furl,0,sizeof furl);
          get_copy(l,r,bf,furl);
          //puts(furl);
          opt=opt+furl+"]";
          output(type,opt.c_str());
          return 1;
        }if(get_st(msg,"[r18_setu")==0){
          int l,r;
          char bf[2048];
          string str,str2;
          str="curl -s \"https://api.lolicon.app/setu/v2";
          str2=tags(msg,9,'&');
          str=str+"?r18=1"+str2+"\"";
          //puts(str.c_str());
          memset(bf,0,sizeof bf);
          execmd(str.c_str(),bf);
          //puts(bf);
          l=get_st(bf,"{\"original\"");
          r=get_st(bf+l,"\"}");
          if(l==-1){
            resend(type,"[Not Found]");
            return 1;
          }
          r+=l;
          l+=13;
          string opt="[CQ:image,file=";
          char furl[512];
          memset(furl,0,sizeof furl);
          get_copy(l,r,bf,furl);
          //puts(furl);
          opt=opt+furl+"]";
          output(type,opt.c_str());
          return 1;
        }if(spj_pfct("[lolicon_sotu]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[lolicon_setu]");return 1;}
          int r=strlen(type.msgs.c_str());
          char str[1024];
          get_copy(l,r,type.msgs.c_str(),str);
          string a;
          a="[lolicon_setu";
          a+=str;
          a+="]";
          resend(type,a.c_str());
          return 1;
        }if(spj_pfct("[weather]",msg)){
          if(APP_SECRET.size()<2)return 0;
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[WEATHER_INVALID]");return 1;}
          int r=strlen(type.msgs.c_str());
          char sstr[1024]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          char*str=acl_url_encode(sstr);
          string a;
          a="curl -s \"https://www.mxnzp.com/api/weather/current/";
          a=a+str+"?app_id="+APP_ID+"&app_secret="+APP_SECRET+"\"";
          //puts(a.c_str());
          char bf[1<<13]={0};
          execmd(a.c_str(),bf);
          //puts(bf);
          string temper =json_read(bf,"temp");
          string weather=json_read(bf,"weather");
          string winds  =json_read(bf,"windDirection");
          string wd     =json_read(bf,"windPower");
          winds=winds+wd;
          string humid  =json_read(bf,"humidity");
          string opts=sstr;
          opts=opts+"\n温度："+temper+"\n天气："+weather+"\n风力："+winds+"\n湿度："+humid;
          output(type,opts.c_str());
          return 1;
        }if(spj_pfct("[7d_weather]",msg)){
          if(AL_TOKEN.size()<2)return 0;
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[WEATHER_INVALID]");return 1;}
          int r=strlen(type.msgs.c_str());
          char sstr[1024]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          char*str=acl_url_encode(sstr);
          string a;
          a="curl  --ssl-no-revoke -s \"https://v2.alapi.cn/api/weather/forecast?location=";
          a=a+str+"&token="+AL_TOKEN+"\"";
          //debug_puts(a.c_str());
          char bf[1<<15]={0};
          execmd(a.c_str(),bf);
          //debug_puts(bf);
          //puts(bf);
          City city;
          city.city_7d_init(bf);
          output(type,city._7dprint().c_str());
          if(IF_HAVE_MATLAB){
            city.get_7dplot();
            output(type,city.outplot().c_str());
          }
          return 1;
        }if(spj_pfct("[24h_weather]",msg)){
          if(AL_TOKEN.size()<2)return 0;
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[WEATHER_INVALID]");return 1;}
          int r=strlen(type.msgs.c_str());
          char sstr[1024]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          char*str=acl_url_encode(sstr);
          string a;
          a="curl  --ssl-no-revoke -s \"https://v2.alapi.cn/api/weather/hourly?location=";
          a=a+str+"&token="+AL_TOKEN+"\"";
          puts(a.c_str());
          char bf[1<<15]={0};
          execmd(a.c_str(),bf);
          //puts(bf);
          City city;
          city.city_24h_init(bf);
          output(type,city._24hprint().c_str());
          if(IF_HAVE_MATLAB){
            city.get_24hplot();
            output(type,city.outplot().c_str());
          }
          return 1;
        }if(spj_pfct("[Xinhua]",msg)){
          if(AL_TOKEN.size()<2)return 0;
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[NONE]");return 1;}
          int r=strlen(type.msgs.c_str());
          char sstr[1024]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          char*str=acl_url_encode(sstr);
          string a;
          a="curl  --ssl-no-revoke -s \"https://v2.alapi.cn/api/word?word=";
          a=a+str+"&token="+AL_TOKEN+"\"";
          //puts(a.c_str());
          char bf[1<<15]={0};
          execmd(a.c_str(),bf);
          //puts(bf);
          string py=json_read(bf,"pinyin");
          string exp=json_read(bf,"explanation");
          string op=op+sstr+"\n"+py+"\n"+exp;
          output(type,op.c_str());
          return 1;
        }if(spj_pfct("[MAT_RUN]",msg)){
          type.ord_lv=-1;
          int l=get_st(type.msgs.c_str(),"#");
          int r=strlen(type.msgs.c_str());
          char sstr[1<<13]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          strchg("\\u0026#91;","[",sstr);
          strchg("\\u0026#93;","]",sstr);
          FILE*fin;
          fin=fopen("mat_code.m","w");
          fprintf(fin,"%s",sstr);
          fclose(fin);
          string comm="mat_run.exe";
          //comm=comm+sstr+"\"";
          system(comm.c_str());
          FILE *fp;
          fp=fopen("outtmp.txt","r");
          char strText[1<<12]={0};
          char sstrt[1<<8];
          while(fgets(sstrt,1<<8,fp)!=NULL)
            sprintf(strText+strlen(strText),"%s",sstrt);
          if(strlen(strText)<1)sprintf(strText,"OK!");
          output(type,strText);
          fclose(fp);
          system("del outtmp.txt");
          return 1;
        }if(spj_pfct("[C++_RUN]",msg)){
          type.ord_lv=-1;
          int l=get_st(type.msgs.c_str(),"#");
          int r=strlen(type.msgs.c_str());
          char sstr[1<<13]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          strchg("\\u0026#91;","[",sstr);
          strchg("\\u0026#93;","]",sstr);
          FILE*fi;
          fi=fopen("test.cpp","w");
          fprintf(fi,"#include<stdio.h>\n#include<stdlib.h>\n#include<thread>\n#include<windows.h>\n");
          fprintf(fi,"void calc();\n");
          fprintf(fi,"void tm(){Sleep(200);printf(\"\\nTLE\\n\");exit(1);}\n");
          fprintf(fi,"int main(){std::thread t(tm);t.detach();calc();return 0;}\n");
          fprintf(fi,"%s",sstr);
          fclose(fi);
          char buff[1<<16]={0};

          execmd("g++ 2>&1 test.cpp -o test.exe",buff);
          //char buff[1<<16]={0};
          execmd("test.exe",buff+strlen(buff));
          /*********************
          FILE*fin;
          fin=fopen("mat_code.m","w");
          fprintf(fin,"system('g++ test.cpp -o test.exe');\ndos('test.exe');");
          fclose(fin);
          string comm="mat_run.exe";
          //comm=comm+sstr+"\"";
          system(comm.c_str());
          FILE *fp;
          fp=fopen("outtmp.txt","r");
          char strText[1<<12]={0};
          char sstrt[1<<8];
          while(fgets(sstrt,1<<8,fp)!=NULL)
            sprintf(strText+strlen(strText),"%s",sstrt);
            output(type,strText);
            fclose(fp);
            system("del outtmp.txt");
          ********************/

          output(type,buff);

          return 1;
        }if(spj_pfct("[risk_area]",msg)){
          if(AL_TOKEN.size()<2)return 0;
          int l=get_st(type.msgs.c_str(),"#");
          int r=strlen(type.msgs.c_str());
          char sstr[1024]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          if((get_st(sstr,"city=")==-1)&&(get_st(sstr,"county=")==-1)){output(type,"x");return 1;}
          char*str=acl_url_encode(sstr);
          strchg("%3D","=",str);
          string a;
          a="curl  --ssl-no-revoke -s \"https://v2.alapi.cn/api/springTravel/risk?";
          a=a+str+"&token="+AL_TOKEN+"\"";
          //puts(a.c_str());
          char bf[1<<18]={0};
          execmd(a.c_str(),bf);
          //puts(bf);
          l=0;
          char op[1<<18]="该地区高风险地区如下：\n";
          int l2=0;
          while((l=get_st(bf+l2,"communitys"))!=-1){
            l=l2+l+14;
            r=get_st(bf+l,"]")-1+l;
            get_copy(l,r,bf,op+strlen(op));
            //puts(op);
            l2=r+1;
          }
          //printf("\ngetok!\n");
          //puts(op);
          strchg(",","\n",op);
          strchg("\"","",op);
          //puts(op);
          output(type,op);
          return 1;
        }if(spj_pfct("[baike]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          int r=strlen(type.msgs.c_str());
          char sstr[1024]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          char*str=acl_url_encode(sstr);
          string a;
          a="curl  --ssl-no-revoke -s \"https://api.wer.plus/api/dub?t=";a=a+str+"\"";
          //puts(a.c_str());
          char bf[1<<13]={0};
          //char imgs[1<<10];
          //char txxts[1<<15];
          execmd(a.c_str(),bf);
          //puts(bf);
          string txxts,imgs;
          txxts=json_read(bf,"text");
          imgs=json_read(bf,"img_url");
          string opts=sstr;
          opts=opts+"\n"+txxts+"\n[CQ:image,file="+imgs+"]";
          output(type,opts.c_str());
          return 1;
        }if(spj_pfct("[chat]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          int r=strlen(type.msgs.c_str());
          char sstr[1024]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          strchg("\\u0026#91;","[",sstr);
          strchg("\\u0026#93;","]",sstr);
          char str[1<<12]={0};
          utf8tounicode(sstr,str);
          string commd="curl -s https://api.openai.com/v1/completions -H "
            "\"Content-Type: application/json\" "
            "-H \"Authorization: Bearer "+OPEN_AI_SECRET+"\" "
            "-d \"{\\\"model\\\": \\\""+OPEN_AI_MODEL+"\\\", \\\"prompt\\\": \\\"";
          commd=commd+str;
          commd=commd+"\\\", \\\"user\\\": \\\"";
          string md5_id=md5(string(type.sender_id));
          commd=commd+"Jiemeng"+md5_id;
          commd=commd+"\\\", \\\"temperature\\\": 0.3, \\\"top_p\\\": 1, \\\"max_tokens\\\": 1000}\"";
          char bf[1<<15]={0};
          execmd(commd.c_str(),bf);
          //color_puts(bf);
          string p=json_read(bf,"text");
          p=decode(p);
          output(type,p.c_str());
          return 1;
        }
        if(spj_pfct("[DAILY_NEWS]",msg)){
          string a;
          a="curl  --ssl-no-revoke -s \"https://api.wer.plus/api/ndys\"";
          puts(a.c_str());
          char bf[1<<13]={0};
          char imgs[1<<10];
          execmd(a.c_str(),bf);
          //puts(bf);
          int l,r;
          l=get_st(bf,"\"url\":")+7;
          r=get_st(bf+l,"\"}")+l;
          get_copy(l,r,bf,imgs);
          string opts="";
          opts=opts+"[CQ:image,file="+imgs+"]";
          output(type,opts.c_str());
          return 1;
        }if(spj_pfct("[dict]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[DICT_INVALID]");return 1;}
          int r=strlen(type.msgs.c_str());
          char sstr[1024]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          strchg("\\u0026#91;","[",sstr);
          strchg("\\u0026#93;","]",sstr);
          char*str=acl_url_encode(sstr);
          string a;
          a="curl  --ssl-no-revoke -s \"http://dict.youdao.com/suggest?num=1&doctype=json&q=";
          a=a+str+"\"";
          //puts(a.c_str());
          char bf[1<<13]={0};
          char means[1<<10]={0};

          execmd(a.c_str(),bf);
          //puts(bf);
          if(get_st(bf,"msg\":\"not found")!=-1){output(type,"No Result.");return 1;}
          l=get_st(bf,"\"explain\":")+11;
          r=get_st(bf+l,"\",\"entry")+l;
          get_copy(l,r,bf,means);

          string opts=sstr;
          opts=opts+"\n"+means+"\n来源：http://dict.youdao.com/";
          output(type,opts.c_str());
          return 1;
        }if(spj_pfct("[trans]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[DICT_INVALID]");return 1;}
          int r=strlen(type.msgs.c_str());
          char sstr[1<<15]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          strchg("\\u0026#91;","[",sstr);
          strchg("\\u0026#93;","]",sstr);
          char*str=acl_url_encode(sstr);
          string a;
          a="curl -s \"http://fanyi.youdao.com/translate?&doctype=json&type=AUTO&i=";
          a=a+str+"\"";
          //puts(a.c_str());
          char bf[1<<15]={0};
          char means[1<<15]={0};
          execmd(a.c_str(),bf);
          //puts(bf);
          l=r=0;string opts=sstr;
          opts=opts+"\n";
          while(get_st(bf+r,"\"tgt\":\"")!=-1){
            l=get_st(bf+r,"\"tgt\":\"")+7+r;
            r=get_st(bf+l,"\"}")+l;
            get_copy(l,r,bf,means);
            opts=opts+means;
          }
          opts=opts+"\n来源：http://fanyi.youdao.com/";
          output(type,opts.c_str());
          return 1;
        }if(spj_pfct("[Baidu_trans]",msg)){
          if(BAIDU_APP_SECRET.size()<2)return 0;
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[DICT_INVALID]");return 1;}
          int r=strlen(type.msgs.c_str());
          char sstr[1<<15]={0};
          get_copy(l+1,r,type.msgs.c_str(),sstr);
          strchg("\\u0026#91;","[",sstr);
          strchg("\\u0026#93;","]",sstr);
          string q=sstr;
          string opt_lang;
          string salt="SALTJIEMENG";
          string str_1=BAIDU_APP_ID+q+salt+BAIDU_APP_SECRET;
          str_1=md5(str_1);
          char*str=acl_url_encode(sstr);
          int l2=get_st(type.msgs.c_str(),"~");
          if(l2==-1){opt_lang="en";}
          else{
            r=l;
            get_copy(l2+1,r,type.msgs.c_str(),sstr);
            opt_lang=sstr;
          }
          string a;
          a="curl -s \"https://fanyi-api.baidu.com/api/trans/vip/translate?from=auto&to="+opt_lang+
            "&appid="+BAIDU_APP_ID+"&salt="+salt+"&sign="+str_1+"&q=";
          a=a+str+"\"";
          //puts(a.c_str());
          char bf[1<<15]={0};
          char means[1<<15]={0};
          execmd(a.c_str(),bf);
          //puts(bf);
          l=r=0;string opts=q.c_str();
          opts=opts+"\n";
          while(get_st(bf+r,"\"dst\":\"")!=-1){
            l=get_st(bf+r,"\"dst\":\"")+7+r;
            r=get_st(bf+l,"\"}")+l;
            get_copy(l,r,bf,means);
            char*sss=unicode_chg(means);
            opts=opts+sss;
            delete sss;
          }
          output(type,opts.c_str());
          return 1;
        }if(spj_pfct("[r18_sotu]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){resend(type,"[r18_setu]");return 1;}
          int r=strlen(type.msgs.c_str());
          char str[1024];
          get_copy(l,r,type.msgs.c_str(),str);
          string a;
          a="[r18_setu";
          a+=str;
          a+="]";
          resend(type,a.c_str());
          return 1;
        }if(spj_pfct("[AI]",msg)){
          return AI_Draw(type);
        }if(spj_pfct("[CHG_NOTE]",msg)){
          if(!chg_note(type.msgs.c_str()))resend(type,"[NOTE_ERROR]");
          return 1;
        }if(spj_pfct("[GET_NOTE]",msg)){
          output(type,get_note(type.msgs.c_str()).c_str());
          return 1;
        }if(spj_pfct("[SAVE_NOTE]",msg)){
          if(!write_note())resend(type,"[NOTE_ERROR]");
          resend(type,"[SAVE_SUCCESS]");
          return 1;
        }if(spj_pfct("[CHEM]",msg)){
          Chem(type);return 1;
        }if(spj_pfct("[midigen]",msg)){
          char tempp[1<<15];
          memset(tempp,0,sizeof tempp);
          strcat(tempp,type.msgs.c_str());

          strchg("\\u0026#91;","[",tempp);
          strchg("\\u0026#93;","]",tempp);
          int l=get_st(tempp,"#");
          int r=strlen(tempp);
          char str[1<<15];
          get_copy(l,r,tempp,str);
          HttpRequest httpReq("127.0.0.1",8080);
          string res;
          res=httpReq.HttpPost("/api", HttpRequest::genJsonString("midercode",str));
          //printf("%s\n",res.c_str());
          l=get_st(res.c_str(),"\"stream\" : \"")+12;
          r=get_st(res.c_str()+l,"\"")+l;
          char file_name[1<<9];
          memset(file_name,0,sizeof file_name);
          getcwd(file_name,sizeof file_name);
          get_copy(l,r,res.c_str(),file_name+strlen(file_name));
          //printf("%s\n",file_name);
          for(int i=0;i<strlen(file_name);i++){
            if(file_name[i]=='\\')file_name[i]='/';
          }
          string opt="[CQ:record,file=file:///";
          opt=opt+file_name+"]";
          output(type,opt.c_str());
          return 1;
        }if(spj_pfct("[midigen_info]",msg)){
          string opt="MiraiMidiProduce v0.1.8 By whiter\n";
          opt+="项目地址：https://github.com/whiterasbk/MiraiMidiProduce\n";
          opt+="参考指南：http://smmcat.top/?p=36\n";
          output(type,opt.c_str());
          return 1;
        }if(spj_pfct("[get_screen]",msg)){
          system("nircmd savescreenshot Screen.png");
          string opt="[CQ:image,file=file:///./Screen.png]";
          output(type,opt.c_str());
          return 1;
        }
        if(spj_pfct("[sakura_setu]",msg)){
          int l,r;
          char bf[2048];
          memset(bf,0,sizeof bf);
          execmd("curl -s https://www.dmoe.cc/random.php?return=json",bf);
          l=get_st(bf,"\"imgurl\":\"");
          r=get_st(bf+l+15,"\"");
          r+=l+15;
          l+=10;
          //puts(bf);

          string opt="[CQ:image,file=";
          char ffurl[512],furl[512];
          memset(ffurl,0,sizeof ffurl);memset(furl,0,sizeof furl);
          get_copy(l,r,bf,ffurl);
          //puts(ffurl);
          int j=0;
          for(int i=0;i<strlen(ffurl);i++)
            if(ffurl[i]!='\\')furl[j++]=ffurl[i];
          //puts(furl);
          opt=opt+furl+"]";
          output(type,opt.c_str());
          return 1;
        }if(spj_pfct("[MirlKoi_setu]",msg)){
          int l,r;
          char bf[2048];
          memset(bf,0,sizeof bf);
          execmd("curl -s \"https://iw233.cn/api.php?sort=random&type=json\"",bf);
          l=get_st(bf,"[\"");
          r=get_st(bf+l+2,"\"");
          r+=l+2;
          l+=2;
          //puts(bf);

          string opt="[CQ:image,file=";
          char ffurl[512],furl[512];
          memset(ffurl,0,sizeof ffurl);memset(furl,0,sizeof furl);
          get_copy(l,r,bf,ffurl);
          //puts(ffurl);
          int j=0;
          for(int i=0;i<strlen(ffurl);i++)
            if(ffurl[i]!='\\')furl[j++]=ffurl[i];
          //puts(furl);
          opt=opt+furl+"]";
          output(type,opt.c_str());
          return 1;
        }if(spj_pfct("[tohou_setu]",msg)){
          int l,r;
          char bf[2048];
          memset(bf,0,sizeof bf);
          execmd("curl -s \"https://img.paulzzh.com/touhou/random?type=json\"",bf);
          l=get_st(bf,"/image/");
          r=get_st(bf+l+8,"/");
          r+=l+8;
          l+=7;
          //puts(bf);
          string opt="[CQ:image,file=https://img.paulzzh.com/touhou/konachan/image/";
          char furl[512];
          memset(furl,0,sizeof furl);
          get_copy(l,r,bf,furl);
          //puts(furl);
          opt=opt+furl+".jpg]";
          output(type,opt.c_str());
          return 1;
        }
        /*********************计时器********************/
        if(spj_pfct("[tic]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){output(type,"错误：未指定计时器名称");return 1;}
          int r=strlen(type.msgs.c_str());
          char str[1024];
          get_copy(l+1,r,type.msgs.c_str(),str);
          string a;
          a=str;
          time_list[a]=clock();
          output(type,"计时添加成功");
          return 1;
        }if(spj_pfct("[toc]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){output(type,"错误：未指定计时器名称");return 1;}
          int r=strlen(type.msgs.c_str());
          char str[1024];
          get_copy(l+1,r,type.msgs.c_str(),str);
          string a;
          a=str;
          if(time_list.find(a)==time_list.end()){
            output(type,"错误：不存在该计时器");return 1;
          }
          int dt=clock()-time_list[a];
          int sec,min,hr;
          dt/=1000;
          sec=dt%60;
          dt/=60;
          min=dt%60;
          dt/=60;
          hr=dt;
          char opt[512];
          memset(opt,0,sizeof opt);
          sprintf(opt,"计时器%s用时：%d:%02d:%02d",a.c_str(),hr,min,sec);
          output(type,opt);
          return 1;
        }
        if(spj_pfct("[get_pic]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          //if(l==-1){output(type,"错误：未指定图片名称");return 1;}
          int r=strlen(type.msgs.c_str());
          char str[1024];
          get_copy(l+1,r,type.msgs.c_str(),str);
          string back_x=str+strlen(str)-3;
          if(back_x!="bmp"&&back_x!="png"&&back_x!="jpg"){
            output(type,"不支持的格式");
            return 1;
          }
          char file_path[1<<9];
          memset(file_path,0,sizeof file_path);
          getcwd(file_path,sizeof file_path);
          string fpath="";
          strchg("\\","/",file_path);
          string rt=rt+"[CQ:image,file=file:///"+file_path+"/"+str+"]";
          type.msgs="[OUTPUT_JOIN]";
          output(type,rt.c_str());
          fpath=fpath+"del "+str;
          system(fpath.c_str());
          return 1;

        }if(spj_pfct("[delete_msg]",msg)){
          int l=get_st(type.msgs.c_str(),"[CQ:reply,id=");
          l+=strlen("[CQ:reply,id=")-1;
          //if(l==-1){output(type,"错误：未指定图片名称");return 1;}
          int r=get_st(type.msgs.c_str(),"]");
          char str[19];
          get_copy(l+1,r,type.msgs.c_str(),str);
          char pt[6];
          sprintf(pt,"%d",OUTPORT);
          string commd="curl -s \"";
          commd=commd+IP_ADD+"delete_msg?message_id="+str+"\"";
          //debug_puts(commd.c_str());
          system(commd.c_str());
          return 1;
        }
        if(spj_pfct("[moe_lp]",msg)){
          lpscq(type);
          return 1;
        }
        if(spj_pfct("[Connect_Four_Start]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){output(type,"错误：未指定棋局名称");return 1;}
          int r=strlen(type.msgs.c_str());
          char str[1024];
          get_copy(l+1,r,type.msgs.c_str(),str);
          string a(str);
          Connect_Four aa;
          game_list[a]=aa;
          output(type,game_list[a].opr("start").c_str());
          //resend(type,a.c_str());
          return 1;
        }if(spj_pfct("[Connect_Four]",msg)){
          int l=get_st(type.msgs.c_str(),"#");
          if(l==-1){output(type,"错误：未指定棋局名称");return 1;}
          int r=get_st(type.msgs.c_str()+l+1,"#");
          if(r==-1){output(type,"[错误指令]");return 1;}
          r+=l+1;
          char str[1024];
          get_copy(l+1,r,type.msgs.c_str(),str);
          string a(str);
          if(game_list.find(a)==game_list.end()){
            output(type,"错误：不存在该棋局");return 1;
          }
          l=r;
          r=strlen(type.msgs.c_str());
          get_copy(l+1,r,type.msgs.c_str(),str);
          //string b(str);
          string optt;
          optt="棋局"+a+"\n";
          output(type,(optt+game_list[a].opr(str)).c_str());
          //resend(type,a.c_str());
          return 1;
        }
        if(spj_pfct("[status]",msg)){
          char sttmtxt[200];
          memset(sttmtxt,0,sizeof sttmtxt);
          sprintf(sttmtxt,"自%s至今，统计信息如下：[\\n]"
            "累计启动：%d次[\\n]累计响应消息：%d次[\\n]"
            "[\\n]当前词库共有：[\\n]"
            "应答组：%d组[\\n]"
            "回复：%d条",STAT_START_TIME.c_str(),start_time,msg_send_time,grp_num,all_ans_num);
          output(type,sttmtxt);
          memset(sttmtxt,0,sizeof sttmtxt);
          return 1;
        }if(spj_pfct("[Group_Exit]",msg)&&type.ifgrp){
          char ss[1<<5]={0};
          sprintf(ss,"%d",OUTPORT);
          string coms="curl -s \"http://127.0.0.1:";
          coms=coms+ss+"/set_group_leave?group_id="+type.grp_id+"\"";
          system(coms.c_str());
          return 1;
        }

        if(get_st(msg,".resend")==0&&(CHECK)){
          char tmp[1000];
          memset(tmp,0,sizeof tmp);
          get_copy(7,strlen(msg),msg,tmp);
          //tmp[strlen(tmp)]=0
          strchg("\\u0026#91;","[",tmp);
          strchg("\\u0026#93;","]",tmp);
          //puts(tmp);
          resend(type,tmp);
          return 1;
        }if(get_st(msg,".repost")==0&&(CHECK)){
          char tmp[1000];
          memset(tmp,0,sizeof tmp);
          get_copy(7,strlen(msg),msg,tmp);
          //tmp[strlen(tmp)]=0
          strchg("\\u0026#91;","[",tmp);
          strchg("\\u0026#93;","]",tmp);
          //puts(tmp);
          output(type,tmp);
          return 1;
        }
        /*#ifdef ENCRYPT
          string tmpss=key_repost(Self_ID,type.sender_id);
          //debug_puts(tmpss.c_str());
          if(get_st(msg,tmpss.c_str())==0){
            type.msgs=de_cr(Self_ID,type.sender_id,type.msgs);
            //debug_puts(type.msgs.c_str());
            thread Respjthr=thread(resend,type,type.msgs.c_str());
            Respjthr.detach();
            return 1;
          }

        #endif*/

        if(get_st(msg,".type_repost")==0&&(CHECK)){
          char tmp[1<<13]={0};
          Msg_type ntype;
          //ntype.msgs=type.msgs;
          int l,r;
          l=get_st(msg,"#");
          r=get_st(msg+l+1,"#")+l+1;
          get_copy(l+1,r,msg,tmp);
          if(r-l<3)
            ntype.ifgrp=0;
          else ntype.ifgrp=1;
          get_copy(0,strlen(tmp),tmp,ntype.grp_id);
          l=r+1;
          r=get_st(msg+l+1,"#")+l+1;
          memset(tmp,0,sizeof tmp);
          get_copy(l,r,msg,ntype.sender_id);
          //debug_puts("type_repost");
          //cout<<msg<<endl;
          //ntype.show();
          if(ntype.ifgrp&&string(ntype.grp_id)=="WHITE_LIST"){
            for(int i=1;i<white_list_num;i++){
              Msg_type stype=ntype;
              stype.ord_lv=100;
              strcpy(stype.grp_id,Grp_white_list[i].c_str());
              char nmsg[1<<13];
              strcpy(nmsg,msg);
              strchg("WHITE_LIST",Grp_white_list[i].c_str(),nmsg);
              //debug_puts(nmsg);
              thread Respjthr=thread(spj,stype,nmsg);
              Respjthr.detach();
              Sleep(delay_time);
            }
            return 1;
          }if(string(ntype.sender_id)=="BROAD"){
            for(int i=0;i<broad_pri_num;i++){
              Msg_type stype=ntype;
              stype.ord_lv=100;
              strcpy(stype.sender_id,broad_pri[i].c_str());
              char nmsg[1<<13];
              strchg("BROAD",broad_pri[i].c_str(),nmsg);
              thread Respjthr=thread(spj,stype,nmsg);
              Respjthr.detach();
              Sleep(delay_time);
            }
            return 1;

          }
          //cout<<msg<<endl;
          //char stmps[1<<13]={0};
          //sprintf(tmp,"%s",msg+r+1);
          get_copy(r+1,strlen(msg),msg,tmp);

          strchg("\\u0026#91;","[",tmp);
          strchg("\\u0026#93;","]",tmp);

          output(ntype,tmp);
          return 1;
        }if(get_st(msg,".type_resend")==0&&(CHECK)){

          char tmp[1000];
          Msg_type ntype;

          memset(tmp,0,sizeof tmp);
          int l,r;
          l=get_st(msg,"#");
          r=get_st(msg+l+1,"#")+l+1;
          get_copy(l+1,r,msg,tmp);
          if(r-l<3)
            ntype.ifgrp=0;
          else ntype.ifgrp=1;
          get_copy(0,strlen(tmp),tmp,ntype.grp_id);
          l=r+1;
          r=get_st(msg+l+1,"#")+l+1;
          memset(tmp,0,sizeof tmp);
          get_copy(l,r,msg,ntype.sender_id);
          if(ntype.ifgrp&&string(ntype.grp_id)=="WHITE_LIST"){
            for(int i=1;i<white_list_num;i++){
              Msg_type stype=ntype;
              stype.ord_lv=100;
              strcpy(stype.grp_id,Grp_white_list[i].c_str());
              char nmsg[1<<13];
              strcpy(nmsg,msg);
              strchg("WHITE_LIST",Grp_white_list[i].c_str(),nmsg);
              //debug_puts(nmsg);
              int thr_num=Main_thr_num.get_new_num();
              Main_thread_list[thr_num]=thread(spj,stype,nmsg);
              Main_thread_list[thr_num].detach();

              /*thread Respjthr=thread(spj,stype,nmsg);
              Respjthr.detach();*/

              Sleep(delay_time);
            }
            return 1;
          }if(string(ntype.sender_id)=="BROAD"){
            for(int i=0;i<broad_pri_num;i++){
              Msg_type stype=ntype;
              stype.ord_lv=100;
              strcpy(stype.sender_id,broad_pri[i].c_str());
              char nmsg[1<<13];
              strchg("BROAD",broad_pri[i].c_str(),nmsg);
              int thr_num=Main_thr_num.get_new_num();
              Main_thread_list[thr_num]=thread(spj,stype,nmsg);
              Main_thread_list[thr_num].detach();
              Sleep(delay_time);
            }
            return 1;
          }


          get_copy(r+1,strlen(msg),msg,tmp);
          strchg("\\u0026#91;","[",tmp);
          strchg("\\u0026#93;","]",tmp);
          resend(ntype,tmp);

          return 1;
        }if(get_st(msg,".cmd")==0&&(CHECK)){
          run_cmd(type);
          return 1;
        }if(spj_pfct(".print",msg)&&(CHECK)&&!type.ifgrp){
          freopen(ANS_FILENAME.c_str(),"r",stdin);
          char txt[4000];memset(txt,0,sizeof txt);
          char opt[10000];memset(opt,0,sizeof opt);
          while(txt[0]!='/'){
            memset(opt,0,sizeof opt);
            for(int i=0;i<15&&txt[0]!='/';i++){
              memset(txt,0,sizeof txt);
              gets(txt);
              get_copy(0,strlen(txt),txt,opt+strlen(opt));
              get_copy(0,4,"[\\n]",opt+strlen(opt));
            }
            output(type,opt);
          }//fclose(stdin);
          return 1;
        }
      }

      if(spj_pfct(".switch on",msg)&&(CHECK)){
        main_switch=1;
        return 1;
      }if(spj_pfct(".switch off",msg)&&(CHECK)){
        main_switch=0;
        return 1;
      }/*if(spj_pfct("开启上报",msg)){
        output(type,"已开启上报");
        report=1;
        return 1;
      }*/
      if(spj_rand(msg)){
        get_copy(3,strlen(msg),msg,temp2);
        int rr;
        sscanf(temp2,"%d",&rr);
        memset(temp2,0,sizeof temp2);
        if(rr==0){
          resend(type,"[Divide0]");
          return 1;
        }
        sprintf(temp2,"%d",rand()%rr);
        output(type,temp2);
        return 1;
      }return 0;

    /*catch(...){
      string opts;
      opts="ERROR_in_spj()\n";
      opts=opts+"type.sender_id="+type.sender_id+"\n";
      opts=opts+"type.ifgrp="+(type.ifgrp?"1\n":"0\n");
      opts=opts+"buf="+buf+"\nmsg="+msg+"\ntype.msgs="+type.msgs;
      //error_report(opts);
      return 0;
    }*/
  }
  #define MOE_READ(FILENAME,NAME,NAME_NUM) {\
    freopen(FILENAME,"r",stdin);\
    char tmp[100];\
    tmp[0]=0;\
    gets(tmp);\
    while(tmp[0]!='/'){\
      sscanf(tmp,"%s",NAME[NAME_NUM++]);\
      gets(tmp);\
    }\
  }
  string rand_mbti(){
    char ti[5];
    ti[0]=rand()&1?'I':'E';
    ti[1]=rand()&1?'N':'S';
    ti[2]=rand()&1?'F':'T';
    ti[3]=rand()&1?'P':'J';
    ti[4]=0;
    string rt(ti);
    return rt;
  }
  void lpscq(Msg_type type){
      //printf("调用lpscq\n");
      char optmsg[2000];
      char zokusai[200][20],kami[50][15],kamiclr[50][15],eye[50][15],eyeclr[50][15];
      int zokusai_num=0,kami_num=0,kamiclr_num=0,eye_num=0,eyeclr_num=0;
      int rrd=rand()%100;
      memset(optmsg,0,sizeof optmsg);
      MOE_READ("moe_zokusai.txt",zokusai,zokusai_num)
      MOE_READ("moe_kami.txt",kami,kami_num)
      MOE_READ("moe_kamiclr.txt",kamiclr,kamiclr_num)
      MOE_READ("moe_eye.txt",eye,eye_num)
      MOE_READ("moe_eyeclr.txt",eyeclr,eyeclr_num)
      sprintf(optmsg,"又来啦？少yy些奇怪的东西吧！[\\n][name]YY出的老婆：[\\n]");
      string mbti=rand_mbti();
      int ht,wt,rsttime=0;
      rst:ht=110+rand()%81;
      wt=20+rand()%60;
      double bmi=wt/(ht*ht/10000.0);
      if(bmi>24.5||bmi<16){
        if(rand()%100>5){
          rsttime++;
          goto rst;
        }
      }int midnum[5];
      if(rand()%100<3){bom:sprintf(optmsg+strlen(optmsg),"[\\n]……哎呀，[name]的老婆爆炸了！");goto moe_opt;}
      sprintf(optmsg+strlen(optmsg),"【性别】：");
      if(rrd<3)sprintf(optmsg+strlen(optmsg),"男");
      else if(rrd<7)sprintf(optmsg+strlen(optmsg),"伪郎");
      else if(rrd<15)sprintf(optmsg+strlen(optmsg),"伪娘");
      else if(rrd<18)sprintf(optmsg+strlen(optmsg),"无性别");
      else if(rrd<20)sprintf(optmsg+strlen(optmsg),"扶她");
      else if(rrd<100)sprintf(optmsg+strlen(optmsg),"女");
      sprintf(optmsg+strlen(optmsg),"[\\n]【身高】：%dcm[\\n]【体重】：%dkg[\\n]",ht,wt);
      sprintf(optmsg+strlen(optmsg),"【CUP】：");
      rrd=rand()%6;
      if(rrd==0)sprintf(optmsg+strlen(optmsg),"A-[\\n]");
      else sprintf(optmsg+strlen(optmsg),"%c[\\n]",'A'+rrd-1);
      if(rrd==5){
        sprintf(optmsg+strlen(optmsg),"[\\n]渡劫判定！！");
        midnum[0]=rand()%20;
        midnum[1]=rand()%100;
        sprintf(optmsg+strlen(optmsg),"[\\n]神秘力量的威力值为%d！！[\\n][name]的老婆的抵抗数值为%d！！[\\n]",midnum[1],midnum[0]);
        if(midnum[0]>midnum[1])sprintf(optmsg+strlen(optmsg),"[\\n]渡⭐劫⭐大⭐成⭐功[\\n]");
        else {sprintf(optmsg+strlen(optmsg),"[\\n]渡⭐劫⭐大⭐失⭐败[\\n]");goto bom;}
      }
      sprintf(optmsg+strlen(optmsg),"【发型】：%s[\\n]",kami[rand()%kami_num]);
      sprintf(optmsg+strlen(optmsg),"【发色】：%s[\\n]",kamiclr[rand()%kamiclr_num]);
      sprintf(optmsg+strlen(optmsg),"【眼型】：%s[\\n]",eye[rand()%eye_num]);
      sprintf(optmsg+strlen(optmsg),"【眼色】：%s[\\n]",eyeclr[rand()%eyeclr_num]);
      sprintf(optmsg+strlen(optmsg),"【萌点】：%s[\\n]",zokusai[rand()%zokusai_num]);
      sprintf(optmsg+strlen(optmsg),"【MBTI】：%s",mbti.c_str());
      sprintf(optmsg+strlen(optmsg),"[\\n][name]，你知道吗，你的老婆在诞生过程中经历了%d次重开哦",rsttime);
      moe_opt:output(type,optmsg);
      return;
  }
  typedef long long ll;
  string time_cal(const char*date){
    int y,m,d,h,min,s,deld;
    char yy[5]={0};
    char mm[5]={0};
    char dd[5]={0};
    char hh[5]={0};
    char nn[5]={0};
    char ss[5]={0};
    get_copy(0,4,date,yy);
    get_copy(4,6,date,mm);
    get_copy(6,8,date,dd);
    get_copy(8,10,date,hh);
    get_copy(10,12,date,nn);
    get_copy(12,14,date,ss);
    sscanf(yy,"%d",&y);
    sscanf(mm,"%d",&m);
    sscanf(dd,"%d",&d);
    sscanf(hh,"%d",&h);
    sscanf(nn,"%d",&min);
    sscanf(ss,"%d",&s);
    ll tot_time=0;
    int ddd[13]={0,-1,30,58,89,119,150,180,211,242,272,303,333};
    deld=(y-1970)*365+(y-1968)/4-(y%4==0&&m<=2)+d+ddd[m];
    tot_time=(ll)(deld)*24L*3600L+(ll)(h-8)*3600L+(ll)(min*60)+(ll)(s);
    char opt[1<<5]={0};
    sprintf(opt,"%lld",tot_time);
    string rt(opt);
    return rt;
  }
  void run_cmd(Msg_type type){
    color_lable("[run_cmd]");
    char tmp[1000]={0};
    char buff[1<<16]={0};
    int l,r;
    string msg=type.msgs;
    color_puts(msg.c_str());
    l=get_st(msg.c_str(),"#");
    r=strlen(msg.c_str());
    get_copy(l+1,r,msg.c_str(),tmp);
    strchg("\\u0026#91;","[",tmp);
    strchg("\\u0026#93;","]",tmp);
    string ttp=UTF8ToGBK(tmp);
    //color_puts(ttp.c_str());
    execmd(ttp.c_str(),buff);
    //color_puts(buff);
    output(type,buff);

  }
  #include<map>
  map<string,string>chems;
  bool if_Chem_read;
  void read_chem(){
    freopen("chem.lst","r",stdin);
    char tmp1[50]={0},tmp2[50]={0};
    string a,b;
    char tmptxt[50];
    gets(tmptxt);
    while(tmptxt[0]!='/'){
      sscanf(tmptxt,"%s %s",&tmp1,&tmp2);
      //printf("%s %s\n",tmp1,tmp2);
      a=tmp1;
      b=tmptxt;
      chems[a]=b;
      gets(tmptxt);
    }if_Chem_read=1;
    return;
  }
  void Chem(Msg_type type){
    if(!if_Chem_read)read_chem();
    string msg=type.msgs;
    char tmp[1000]={0};
    int l,r;
    l=get_st(msg.c_str(),"#");
    r=strlen(msg.c_str());
    get_copy(l+1,r,msg.c_str(),tmp);
    string p=tmp;
    if(chems[p].size()==0){
      output(type,"无该元素信息");
      return;
    }
    p=chems[p];
    output(type,p.c_str());
  }
#endif
