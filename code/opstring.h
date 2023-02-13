#ifndef OPSTRING
  #define OPSTRING
  #include<string.h>
  #include<windows.h>
  #include<stdio.h>
  #include<string>

  using namespace std;

  inline int get_st(const char *a,const char *sub_str){
    char*d=strstr(a,sub_str);
    if(d==NULL)return -1;
    else return d-a;
    /*for(int i=0;i<strlen(a);i++){
      if(pfct(a+i,sub_str))return i;
    }return -1;*/
  }
  inline bool pfct(const char *a,const char *b){
    /*for(int i=0;i<strlen(b);i++){
      if(*(a+i)!=*(b+i))return 0;
    }return 1;*/
    return get_st(a,b)==0;
  }
  /**********区间字符串复制**********/
  inline void get_copy(int l,int r,const char *txt,char *copy){
    memset(copy,0,sizeof copy);
    strncpy(copy,txt+l,size_t(r-l));
    copy[r-l]=0;
  }
  inline void strchg(const char *oldstr,const char *newstr,char *str){
    char bstr[1<<18];
    memset(bstr,0,sizeof(bstr));
    for(int i=0;i<strlen(str);i++){
      if(!strncmp(str+i,oldstr,strlen(oldstr))){
        strcat(bstr,newstr);
        i+=strlen(oldstr)-1;
      }else{
        strncat(bstr,str+i,1);
  	  }
    }
    strcpy(str,bstr);
    return;
  }inline string str_strchg(const char *oldstr,const char *newstr,const char *str){
      char bstr[1<<18];
      memset(bstr,0,sizeof(bstr));
      for(int i=0;i<strlen(str);i++){
          if(!strncmp(str+i,oldstr,strlen(oldstr))){
              strcat(bstr,newstr);
              i+=strlen(oldstr)-1;
          }else{
          	strncat(bstr,str+i,1);
  	    }
      }
      return bstr;
  }inline void strchg_1(const char *oldstr,const char *newstr,char *str){
      char bstr[1<<18];
      bool ifcpy=0;
      memset(bstr,0,sizeof(bstr));
      for(int i = 0;i < strlen(str);i++){
          if((!strncmp(str+i,oldstr,strlen(oldstr)))&&!ifcpy){
              strcat(bstr,newstr);
              i+=strlen(oldstr) - 1;
              ifcpy=1;
          }else{
          	strncat(bstr,str+i,1);
  	    }
      }
      strcpy(str,bstr);
      return;
  }
  static unsigned char enc_tab[]="0123456789ABCDEF";
  inline char*acl_url_encode(const char*str){
    int len=(int)strlen(str);
    int tmp_len=len;
    unsigned char*tmp=(unsigned char*)malloc(len+1);
    int i,j;
    for(i=0,j=0;i<len;i++,j++){
      tmp[j]=(unsigned char)str[i];
      if(!isalnum(tmp[j])&&strchr("_-.",tmp[j])==NULL){
        tmp_len+=3;
        tmp=(unsigned char*)realloc(tmp,tmp_len);
        tmp[j++]='%';
        tmp[j++]=enc_tab[(unsigned char)str[i]>>4];
        tmp[j]=enc_tab[(unsigned char)str[i]&0x0F];
      }
    }tmp[j]='\0';
    return (char*)tmp;
  }

  string file_path_chg(const char*path){
    string opt_path("file:///");
    char b[1<<10]={0};
    strcpy(b,path);
    strchg("\\","/",b);
    //puts(b);
    opt_path=opt_path+b;
    return opt_path;
  }
  string GBKToUTF8(const char* strGBK){
      int len = MultiByteToWideChar(CP_ACP,0,strGBK,-1,NULL,0);
      wchar_t* wstr = new wchar_t[len+1];
      memset(wstr,0,len+1);
      MultiByteToWideChar(CP_ACP,0,strGBK,-1,wstr,len);
      len = WideCharToMultiByte(CP_UTF8,0,wstr,-1,NULL,0,NULL,NULL);
      char* str = new char[len+1];
      memset(str,0,len+1);
      WideCharToMultiByte(CP_UTF8,0,wstr,-1,str,len,NULL,NULL);
      string strTemp = str;
      if(wstr) delete[] wstr;
      if(str) delete[] str;
      return strTemp;
  }

  string UTF8ToGBK(const char* strUTF8){
      int len = MultiByteToWideChar(CP_UTF8,0,strUTF8,-1,NULL,0);
      wchar_t* wszGBK = new wchar_t[len+1];
      memset(wszGBK,0,len*2+2);
      MultiByteToWideChar(CP_UTF8,0,strUTF8,-1,wszGBK,len);
      len = WideCharToMultiByte(CP_ACP,0,wszGBK,-1,NULL,0,NULL,NULL);
      char* szGBK = new char[len+1];
      memset(szGBK,0,len+1);
      WideCharToMultiByte(CP_ACP,0,wszGBK,-1,szGBK,len,NULL,NULL);
      string strTemp(szGBK);
      if(wszGBK) delete[] wszGBK;
      if(szGBK) delete[] szGBK;
      return strTemp;
  }

  int get_16_10(const char str){
    if(str>='0'&&str<='9')return str-'0';
    else return str-'a'+10;
  }
  char*utf8tounicode(const char* input,char*output)
  {
    int len = strlen(input);
    int unicode_len = 0;
    unsigned char ch;
    unsigned char ch1, ch2, ch3, ch4;

    for (int i = 0; i < len; i++)
    {
      ch = (unsigned char)input[i];

      if (ch < 0x80){
        sprintf(output + unicode_len, "\\u%04x", ch);
        unicode_len += 6;
      }
      else if (ch < 0xE0){
        ch1 = ((unsigned char)ch& (0x1C))>>2;
        ch2 = ((unsigned char)ch& (0x03))<<2;
        ch  = ((unsigned char)input[++i]);
        ch2|= ((unsigned char)ch& (0x30))>>4;
        ch3 = ((unsigned char)ch& (0x0F));
        sprintf(output + unicode_len, "\\u%02x%x%x", ch1, ch2, ch3);
        unicode_len += 6;
      }
      else if (ch < 0xF0){
        ch1 = ((unsigned char)ch& (0x0F));
        ch  = ((unsigned char)input[++i]);
        ch2 = ((unsigned char)ch& (0x3C))>>2;
        ch3 = ((unsigned char)ch& (0x03))<<2;
        ch  = ((unsigned char)input[++i]);
        ch3|= ((unsigned char)ch& (0x30))>>4;
        ch4 = ((unsigned char)ch& (0x0F));
        sprintf(output + unicode_len, "\\u%x%x%x%x", ch1, ch2, ch3, ch4);
        unicode_len += 6;
      }
      else{
        sprintf(output + unicode_len, "\\u%04x", ch);
        unicode_len += 6;
      }
    }return output;
  }
  char*unicode_chg(char*str){
    int l=0,r=0;
    int cnt=0;
    char*opt=(char*)(malloc(1<<15 * sizeof(char)));

    strchg("\\u0026#91;","[",str);
    strchg("\\u0026#93;","]",str);
    strchg("\\\"","\"",str);
    while(str[l]!='\\'&&l<strlen(str)){
      *(opt+cnt)=str[l++];
      cnt++;
    }
    while(get_st(str+l,"\\u")!=-1){
      l=get_st(str+l,"\\u")+2+l;
      *(opt+cnt)=(0xe)<<4;
      *(opt+cnt)|=get_16_10(str[l]);
      cnt++;
      l++;
      *(opt+cnt)=(0x8)<<4;
      *(opt+cnt)|=get_16_10(str[l])<<2;
      l++;
      *(opt+cnt)|=get_16_10(str[l])>>2;
      cnt++;
      *(opt+cnt)=(0x8)<<4;
      *(opt+cnt)|=(get_16_10(str[l])&3)<<4;
      l++;
      *(opt+cnt)|=(get_16_10(str[l]));
      l++;
      cnt++;
      while(str[l]!='\\'&&l<strlen(str)){
        *(opt+cnt)=str[l++];
        cnt++;
      }
    }
    *(opt+cnt)=0;
    return opt;
  }
  void color_puts(const char*);
  string json_read(const char*msg,const char*lable){
    string rt;
    int l,r,r2;
    string _lable="\"";
    bool ifstr=0;
    _lable=_lable+lable+"\":";
    l=get_st(msg,_lable.c_str());
    if(l==-1)return "[NONE]";
    l+=strlen(lable)+3;
    if(msg[l]=='\"')ifstr=1;
    else if(msg[l+1]=='\"'){ifstr=1;l++;}
    if(ifstr){
      r=l++;
      do{
        r++;
        r=get_st(msg+r,"\"")+r;
      }while(msg[r-1]=='\\');
    }else{
      r=get_st(msg+l,",")+l;
      r2=get_st(msg+l,"}")+l;
      r=(r<r2&&(r-l)!=-1)?r:r2;
    }
    for(int i=l;i<r;i++){
      rt=rt+msg[i];
    }
    return rt;
  }
  /*string fjson_read(const char*msg,const char*lable){
    string rt;
    int l,r,r2;
    string _lable="\"";
    _lable=_lable+lable+"\":";
    l=get_st(msg,_lable.c_str());
    if(l==-1)return "[NONE]";
    l+=strlen(lable)+3;
    r=get_st(msg+l,",\"")+l;
    r2=get_st(msg+l,"},\"")+l;
    if(r2-l==-1)r2=get_st(msg+l,"\"}")+l+1;
    if(r2-l-1==-1)r2=get_st(msg+l,"}")+l;
    if(r-l==-1||(r2<r&&(r2-l)!=-1))r=r2;
    if(msg[l]=='\"'){l++;r--;}
    for(int i=l;i<r;i++){
      rt=rt+msg[i];
    }
    //color_puts(msg);
    //color_puts(rt.c_str());
    return rt;
  }*/
  string decode(string msg){
    msg=str_strchg("\\\\n"   ,"[text_n]", msg.c_str());
    msg=str_strchg("\\\\r"   ,"[text_r]", msg.c_str());
    msg=str_strchg("\\r"          , "\r", msg.c_str());
    msg=str_strchg("\\n"          , "\n", msg.c_str());
    msg=str_strchg("\\\""         , "\"", msg.c_str());
    msg=str_strchg("\\\\"         , "\\", msg.c_str());
    msg=str_strchg("\\u003e"      , ">" , msg.c_str());
    msg=str_strchg("\\u003c"      , "<" , msg.c_str());
    msg=str_strchg("\\u0026amp;"  , "&" , msg.c_str());
    msg=str_strchg("[text_n]"     ,"\\n", msg.c_str());
    msg=str_strchg("[text_r]"     ,"\\r", msg.c_str());
    return msg;
  }string msg_decode(string msg){
    msg=str_strchg("\\u0026#91;","[", msg.c_str());
    msg=str_strchg("\\u0026#93;","]", msg.c_str());
    return msg;
  }
#endif
