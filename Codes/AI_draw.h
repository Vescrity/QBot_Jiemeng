#ifndef AI_DRAW
  #define AI_DRAW
  #include<string>
  #include"Jie_Meng_Base.h"
  using namespace std;
  int reads(const char*buf,char*rt){
    int nxt;
    for(int i=0;;i++){
      if(buf[i]=='\n'){
        nxt=i+1;
        rt[i]=0;
        return nxt;
      }else if(buf[i]==0){
        rt[i]=0;
        return -1;
      }
      rt[i]=buf[i];
    }
  }
  string get_file(){
    char buf[1<<15]={0};
    string DIR_cmd("dir ");
    DIR_cmd=DIR_cmd+AI_PATH;
    //puts(DIR_cmd.c_str());
    execmd(DIR_cmd.c_str(),buf);
    char readline[1<<6][1<<13]={0};
    //printf("buf=");
    //puts(buf);
    int sts=0,prs;
    int cnt=0;
    for(int i=0;i<100;i++){
      prs=reads(buf+sts,readline[i]);
      sts+=prs;
      //puts(readline[i]);
      cnt++;
      if(prs==-1)break;
    }
    string str=readline[cnt-5];
    int l,r;
    for(int i=0;i<str.length();i++){
      if(str[i]=='-'){
        l=i-5;
        break;
      }
    }r=str.length();
    char filename[1<<13]={0};
    get_copy(l,r,str.c_str(),filename);
    //puts(filename);
    string final_name,flname;
    flname="rename \""+AI_PATH+"\\";
    flname=flname+filename+"\" \"qwq.png\"";
    /***/
    string ttp=UTF8ToGBK(flname.c_str());
    /***/
    execmd(ttp.c_str(),filename);
    final_name=file_path_chg(AI_PATH.c_str());
    //puts(final_name.c_str());
    final_name="[CQ:image,file="+final_name+"/qwq.png]";
    return final_name;
  }
  string gener(string a,string b,string Sample_step,string prompt){
    //HttpRequest httpReq("127.0.0.1",7860);
    string req;
    string res;
    string Del_cmd("del \"");
    Del_cmd=Del_cmd+AI_PATH+"\\qwq.png\"";
    system(Del_cmd.c_str());
    req="{\"fn_index\":11,\"data\":[\""+prompt+
      "\",\"lowers, bad anatomy, bad hands, text, error, missing fingers, extra digits ,"
      "fewer digits, cropped, wort quality ,low quality, normal quality, jpeg artifacts, "
      "signature, watermark, username, blurry, bad feet\","
      "\"None\",\"None\","+Sample_step+",\"Euler a\",false,false,1,1,7,-1,-1,0,0,0,false,"+
      a+","+b+
      ",false,false,0.7,\"None\",false,false,null,\"\",\"Seed\",\"\",\"Steps\",\"\",true,false,null,\"\",\"\"],\"session_hash\":\"bc24xhhghcd\"}";
    WSADATA wData;
    ::WSAStartup(MAKEWORD(2, 2), &wData);
    SOCKET clientSocket = socket(AF_INET, 1, 0);
    struct sockaddr_in ServerAddr = {0};
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerAddr.sin_port = htons(7860);
    ServerAddr.sin_family = AF_INET;
    int errNo = connect(clientSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));

        // 格式化data长度
        char len[10] = {0};
        sprintf(len, "%d", req.length());
        std::string strLen = len;
        char pot[10]={0};
        sprintf(pot,"%d",7860);
        std::string strSend = "POST /api/predict/ HTTP/1.1\r\n"
        "Host: 127.0.0.1:7860\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length:";
        strSend=strSend+strLen+"\r\n";
        strSend=strSend+"sec-ch-ua: \"Microsoft Edge\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\r\n"
                        "sec-ch-ua-platform: \"Windows\"\r\n"
                        "sec-ch-ua-mobile: ?0\r\n"
                        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36 Edg/107.0.1418.26\r\n"
                        "Content-Type: application/json\r\n"
                        "Accept: */*\r\n"
                        "Origin: http://127.0.0.1:7860\r\n"
                        "Sec-Fetch-Site: same-origin\r\n"
                        "Sec-Fetch-Mode: cors\r\n"
                        "Sec-Fetch-Dest: empty\r\n"
                        "Referer: http://127.0.0.1:7860/\r\n"
                        "Accept-Encoding: gzip, deflate, br\r\n"
                        "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n\r\n"+req;
        // 发送
        //puts(strSend.c_str());
        send(clientSocket, strSend.c_str(), strSend.length(), 0);
        char bufRecv[1<<10] = {0};
        recv(clientSocket, bufRecv, 1<<10, 0);
        //puts(bufRecv);
        return get_file();
  }
  bool AI_Draw(Msg_type type){

      if(if_AI_run){resend(type,"[AI_Running]");return 1;}
      int l,r;
      string l_a,l_b,l_step;

      //memset(ttemp,0,sizeof ttemp);
      l=get_st(Current_Msg.c_str(),"%");

      if(l!=-1){
        int aa,bb;
        char ttemp[8]={0};
        aa=get_st(Current_Msg.c_str(),"#");
        bb=get_st(Current_Msg.c_str(),"~");
        r=bb==-1?aa:bb;
        get_copy(l+1,r,Current_Msg.c_str(),ttemp);
        l_step=ttemp;
        l=r+1;
      }else l_step="10";

      l=get_st(Current_Msg.c_str(),"~");

      if(l==-1){l_a=l_b="512";}
      else{
        char ttemp[8]={0};
        int la,lb,ls;

        l++;
        r=get_st(Current_Msg.c_str()+l,"~")+l;
        if(r==-1){
          resend(type,"[AI_INVALID]");
          return 1;
        }
        get_copy(l,r,Current_Msg.c_str(),ttemp);
        l_a=ttemp;
        l=r+1;
        r=get_st(Current_Msg.c_str()+l,"#")+l;
        get_copy(l,r,Current_Msg.c_str(),ttemp);
        l_b=ttemp;
        sscanf(l_a.c_str(),"%d",&la);
        sscanf(l_b.c_str(),"%d",&lb);
        sscanf(l_step.c_str(),"%d",&ls);
        //printf("%d,%d\n",la,lb);
        if(la>1920||la<=0||lb>1920||lb<=0||la*lb>1920*1088||(la&63)||(lb&63)||(ls>64)){
          resend(type,"[AI_OUT_OF_RANGE]");
          return 1;
        }
      }
      l=get_st(Current_Msg.c_str(),"#");
      if(l==-1){resend(type,"[AI_INVALID]");return 1;}
      r=strlen(Current_Msg.c_str());
      char str[1<<12];
      get_copy(l+1,r,Current_Msg.c_str(),str);
      strchg("\\u0026#91;","[",str);
      strchg("\\u0026#93;","]",str);
      strchg("，",",",str);
      strchg("（","(",str);
      strchg("）",")",str);
      strchg("\r\n"," ",str);
      strchg("\n"," ",str);
      /*for(int i=0;i<strlen(str);i++)if(str[i]<0){
        string err_tips="【警告】存在无效字符，以下是提示：\n";
        char tips[1<<12];
        get_copy(0,i,str,tips);
        err_tips+=tips;
        output(type,err_tips.c_str());
        break;
        //return 1;
      }*/
      string ss=str;
      if_AI_run=1;
      output(type,gener(l_a,l_b,l_step,ss).c_str());
      //Sleep(5000);
      if_AI_run=0;
      return 1;
  }
#endif
