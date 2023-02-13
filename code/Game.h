#ifndef GAME
#define GAME
#define MM (100)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<io.h>
#include<windows.h>
#include<string>
using namespace std;

struct pr{
  int n,x;
  int value(){
    //printf("%d %d\n",n,x);
    if(n>=4)return n;
    else if(x==2)return 0;
    else return n-x;

    //else return 0;
  }
};

struct Connect_Four{
  int stat,crt_player;
  static int Mx,My,step;
  Connect_Four(){Mx=My=step=stat=step=0;}
  struct player{
    char name[1<<5];
    int id,game_mode;
  }ply[5];
  struct Mp{
    int mp[MM][MM],tl[MM];
    void inst(int x,int t){
      mp[tl[x]++][x]=t;
    }
    void init(){
      memset(mp,0,sizeof mp);
      for(int i=0;i<Mx+2;i++){
        mp[0][i]=mp[My+1][i]=mp[i][Mx+1]=mp[i][0]=3;
        tl[i]=1;
      }
    }
    string prt(){
      char opt[1<<12];
      memset(opt,0,sizeof(opt));
      sprintf(opt+strlen(opt),"step=%d\n",step);
      for(int i=1;i<Mx+1;i++)sprintf(opt+strlen(opt),"| %d ",i);
      sprintf(opt+strlen(opt),"|\n");
      for(int i=My;i>0;i--){
        for(int j=1;j<=Mx;j++){
          sprintf(opt+strlen(opt),"|%s",(mp[i][j]==0?"　":(mp[i][j]==1?" x ":" 0 ")));
          //putchar('|');
          //putchar(mp[i][j]==0?' ':(mp[i][j]==1?'X':'0'));
        }
        sprintf(opt+strlen(opt),"|\n");
      }sprintf(opt+strlen(opt),"\n");
      //printf("mxcnt1=%d\n",mxcnt(1));
      //printf("mxcnt1=%d\n",mxcnt(2));
      return string(opt);
    }
    int cnt(int x,int y,int t,int w){
      int dx,dy,ct,ii;
      ct=0;ii=1;
      if(w==0){dx=-1;dy=1;}
      if(w==1){dx=0;dy=1;}
      if(w==2){dx=1;dy=1;}
      if(w==3){dx=1;dy=0;}
      if(w==4){dx=1;dy=-1;}
      if(w==5){dx=0;dy=-1;}
      if(w==6){dx=-1;dy=-1;}
      if(w==7){dx=-1;dy=0;}
      if(mp[y-dy][x-dx])ct++;
      while(mp[y+ii*dy][x+ii*dx]==t)ii++;
      if(mp[y+ii*dy][x+ii*dx]&&(ii<4))ct++;
      pr rt;
      rt.n=ii;
      rt.x=ct;
      return rt.value();
    }
    int mxcnt(int t){
      int mxv,td;
      mxv=-1;
      for(int x=1;x<=Mx;x++){
        for(int y=1;y<=My;y++){
          for(int k=0;k<8;k++){
            if(mp[y][x]==t)td=cnt(x,y,t,k);
            if(td>mxv)mxv=td;
          }
        }
      }
      return mxv;
    }
    pr aly(int t,player py){
      Mp tmp[Mx+1];
      int mxv,opt,mxpv,mxx,mxpx,td,mxpts=-100;
      opt=(t==2)?1:2;
      mxx=mxpx=mxpv=mxv=-1;
      //int pts=-100;
      //printf("mxcnt%d=%d\n",t,mxcnt(t));
      for(int i=1;i<=Mx;i++){
        int mxtt,mxopt;
        int pts=0,mnpts=100;
        tmp[i]=*this;
        if(tmp[i].tl[i]<=My)tmp[i].inst(i,t);
        else continue;
        mxtt=tmp[i].mxcnt(t);
        if(mxtt>=4)pts+=100;
        else pts+=mxtt;
        Mp ttp;
        ttp=tmp[i];
        for(int j=1;j<=Mx;j++){
          int ppts=pts;
          if(tmp[i].tl[j]<=My)ttp.inst(j,opt);
          else continue;
          mxopt=ttp.mxcnt(opt);
          if(mxopt>=4)ppts-=50;
          else ppts-=mxopt;
          if(ppts<=mnpts)mnpts=ppts;
        }if(mnpts>mxpts){
          mxpts=mnpts;
          mxx=i;
        }
      }pr rt;
      rt.x=mxx;
      return rt;

    }
  }mmp;
  void m_insert(int t,player x,int n){
    //char str[1<<10];
    step++;
    //sprintf(str,"%s: insert%d\n",x.name,t);
    //txxt(str);
    mmp.inst(t,n);
  }
  string opr(const char*command){
    puts(command);
    bool ifr=0;                                     //本次调用是否已读取数据
    char opt[1<<12];
    memset(opt,0,sizeof(opt));
    string cm(command);
    if(cm=="start"){
      sprintf(opt,"即将开始游戏，请输入游戏参数[\\n]");
      sprintf(opt+strlen(opt),"2.pvc[\\n]3.cvp[\\n]");
      sprintf(opt+strlen(opt),"第二、第三参数为棋盘大小[\\n]输入格式：【m x y】");
      string rt(opt);return rt;
    }
    if(stat==0){
      int ppp;
      if(strlen(command)<5){
        sprintf(opt+strlen(opt),"错误：参数缺失！");string rt(opt);return rt;
      }
      sscanf(command,"%d%d%d",&ppp,&Mx,&My);
      if(Mx<5||My<5||(ppp<1||ppp>3)||Mx>20||My>100){
        sprintf(opt+strlen(opt),"错误：参数不合理！");string rt(opt);return rt;
      }
      //printf("%d %d %d\n",ppp,Mx,My);
      ifr=1;
      if(ppp==1){
        ply[1].game_mode=1;
        ply[2].game_mode=1;
      }if(ppp==2){
        ply[1].game_mode=1;
        ply[2].game_mode=2;
      }if(ppp==3){
        ply[1].game_mode=2;
        ply[2].game_mode=1;
      }
      strcpy(ply[1].name,"A");
      strcpy(ply[2].name,"B");
      step=0;
      mmp.init();
      crt_player=1;
      stat=1;
    }
    if(stat==1){
      CFst:int tt;
      //printf("stat=1\nifr=%d\ncrt_player=%d",ifr,crt_player);
      if(ply[crt_player].game_mode==2){
        tt=mmp.aly(crt_player,ply[crt_player]).x;
        m_insert(tt,ply[crt_player],crt_player);
        sprintf(opt+strlen(opt),"%s",mmp.prt().c_str());
        if(step>=Mx*My-1){
          sprintf(opt+strlen(opt),"平局力");
          stat=2;
          string rt(opt);
          return rt;
        }
        if(!(mmp.mxcnt(1)<4&&mmp.mxcnt(2)<4)){
          sprintf(opt+strlen(opt),"是我赢了呢w\n");stat=2;
          string rt(opt);return rt;
        }else{crt_player++;if(crt_player==3)crt_player-=2;string rt(opt);return rt;}
      }else if(!ifr){
        //printf("try getin\n");
        sscanf(command,"%d",&tt);
        if(tt>Mx||tt<1){
          sprintf(opt+strlen(opt),"错误：参数不合理！");string rt(opt);return rt;
        }ifr=1;
        m_insert(tt,ply[crt_player],crt_player);
        //sprintf(opt+strlen(opt),"%s",mmp.prt().c_str());
        if(!((mmp.mxcnt(1)<4)&&(mmp.mxcnt(2)<4))){
          sprintf(opt+strlen(opt),"%s",mmp.prt().c_str());
          sprintf(opt+strlen(opt),"mx1=%d,mx2=%d",mmp.mxcnt(1),mmp.mxcnt(2));
          sprintf(opt+strlen(opt),"[name] win!\n");stat=2;

          string rt(opt);return rt;
        }
        else{crt_player++;if(crt_player==3)crt_player-=2;goto CFst;}
      }else{
        sprintf(opt+strlen(opt),"%s",mmp.prt().c_str());
        //crt_player++;if(crt_player==3)crt_player-=2;
        string rt(opt);return rt;
      }
    }
    if(stat==2){
      sprintf(opt+strlen(opt),"已经结束了捏\n");
      string rt(opt);return rt;
    }
    return string("GAME_ERROR");
  }
};
int Connect_Four::step=0;
int Connect_Four::Mx=8;
int Connect_Four::My=8;
#endif
