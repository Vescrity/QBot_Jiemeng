#ifndef OUTPUT_SPJFUN
  #define OUTPUT_SPJFUN
  #include"Jie_Meng_Base.h"
  #include<string>
  #include<string.h>
  #include<windows.h>
  using namespace std;
  string MirlKoi(){
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
    for(int i=0;i<int(strlen(ffurl));i++)
      if(ffurl[i]!='\\')furl[j++]=ffurl[i];
    //puts(furl);
    opt=opt+furl+"]";
    return opt;
  }

  string RUA(const char*qid){
    int l,r;
    char bf[2048];
    memset(bf,0,sizeof bf);
    string getimgurls="curl -s \"https://api.wer.plus/api/ruad?url=http://qlogo2.store.qq.com/qzone/";
    getimgurls=getimgurls+qid+"/"+qid+"/100\"";
    execmd(getimgurls.c_str(),bf);
    l=get_st(bf,"\"url\":\"")+7;
    r=get_st(bf+l,"\"}");
    r+=l;
    //puts(bf);
    string opt="[CQ:image,file=";
    char ffurl[512];
    get_copy(l,r,bf,ffurl);
    //puts(ffurl);
    opt=opt+ffurl+"]";
    return opt;
  }
  string hitokoto(){
    char bf[1<<13]={0};
    string toget="curl -s \"https://v2.alapi.cn/api/hitokoto?token="+AL_TOKEN+"\"";
    execmd(toget.c_str(),bf);
    string txxt=json_read(bf,"hitokoto");
    string from=json_read(bf,"from");
    string rt;
    rt=txxt+"\n——"+from;
    return rt;
  }
  string Daily_news(){
    char bf[1<<15]={0};
    string toget="curl -s \"https://v2.alapi.cn/api/zaobao?format=json&token="+AL_TOKEN+"\"";
    execmd(toget.c_str(),bf);
    string txxt=json_read(bf,"image");
    char url[1<<9]={0};
    strcpy(url,txxt.c_str());
    strchg("\\/","/",url);
    //string from=json_read(bf,"from");
    string rt;
    rt=rt+"[CQ:image,file="+url+"]";
    //rt=txxt+"\n——"+from;
    return rt;
  }
#endif
