#ifndef OPSTRING
  #define OPSTRING
  #include<string.h>
  #include<windows.h>
  #include<stdio.h>
  bool pfct(const char *a,const char *b){
    for(int i=0;i<strlen(b);i++){
      if(*(a+i)!=*(b+i))return 0;
    }return 1;
  }
  int get_st(const char *a,const char *sub_str){
    for(int i=0;i<strlen(a);i++){
      if(pfct(a+i,sub_str))return i;
    }return -1;
  }
  /**********区间字符串复制**********/
  inline void get_copy(int l,int r,const char *txt,char *copy){
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
  }inline void strchg_1(const char *oldstr,const char *newstr,char *str){
      char bstr[1<<18];
      bool ifcpy=0;
      memset(bstr,0,sizeof(bstr));
      for(int i = 0;i < strlen(str);i++){
          if((!strncmp(str+i,oldstr,strlen(oldstr)))&&!ifcpy){
              strcat(bstr,newstr);
              i += strlen(oldstr) - 1;
              ifcpy=1;
          }else{
          	strncat(bstr,str + i,1);
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
  #include<string>
  using namespace std;
  string file_path_chg(const char*path){
    string opt_path("file:///");
    char b[1<<10]={0};
    strcpy(b,path);
    strchg("\\","/",b);
    //puts(b);
    opt_path=opt_path+b;
    return opt_path;
  }
  string GBKToUTF8(const char* strGBK)
  {
      int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
      wchar_t* wstr = new wchar_t[len+1];
      memset(wstr, 0, len+1);
      MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
      len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
      char* str = new char[len+1];
      memset(str, 0, len+1);
      WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
      string strTemp = str;
      if(wstr) delete[] wstr;
      if(str) delete[] str;
      return strTemp;
  }

  string UTF8ToGBK(const char* strUTF8)
  {
      int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
      wchar_t* wszGBK = new wchar_t[len+1];
      memset(wszGBK, 0, len*2+2);
      MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
      len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
      char* szGBK = new char[len+1];
      memset(szGBK, 0, len+1);
      WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
      string strTemp(szGBK);
      if(wszGBK) delete[] wszGBK;
      if(szGBK) delete[] szGBK;
      return strTemp;
  }

  int get_16_10(const char str){
    if(str>='0'&&str<='9')return str-'0';
    else return str-'a'+10;
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
  string json_read(const char*msg,const char*lable){
    string rt;
    int l,r;
    l=get_st(msg,lable);
    if(l==-1)return "[NONE]";
    l+=strlen(lable)+3;
    r=get_st(msg+l,"\",")+l;
    if(r-l==-1)r=get_st(msg+l,"\"}")+l;
    for(int i=l;i<r;i++){
      rt=rt+msg[i];
    }
    return rt;
  }
#endif
