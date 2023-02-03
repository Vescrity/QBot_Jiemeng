#include"File_Read.h"
#include<iostream>
string SAVE_ANS_FILENAME;

void guide();
/************************MAIN*************************/
int main(){
  system("chcp 936");
  system("cls");
  guide();

}
/*****************************************************/
bool insert_group(){
  Group grp_tmp;
  printf("--请指定该应答组生效范围(所有聊天0/指定人1/指定群聊2/仅限bot管理员3)：");
  {
    int t;
    cin>>t;
    if(t==1||t==2){
      t==1?(grp_tmp.sp=1):(grp_tmp.sg=1);
      printf("请输入号码：");
      cin>>grp_tmp.spid;
    }
    else if(t==3)grp_tmp.priv=1;
  }
  bool flg=1;
  do{
    Keys key_tmp;
    printf("--将设定该应答组的第%d组关键词\n",grp_tmp.key_num+1);
    {
      bool flgs=1;
      do{
        printf("----请输入该关键词组的第%d个关键词，输入\"//end\"结束\n    ",key_tmp.word_num+1);
        {
          string ss;
          do{getline(cin,ss);}
          while(ss.length()==0);
          if(ss=="//end")flgs=0;
          else{
            Words temp;
            strcpy(temp.txt,GBKToUTF8(ss.c_str()).c_str());
            key_tmp.insert_word(temp);
          }
        }
      }while(flgs);
    }
    if(key_tmp.word_num==1){
      printf("--请指定该关键词组的判断类型(仅需包含关键词0/完全匹配1)\n  ");
      cin>>key_tmp.allcrt;
    }else key_tmp.allcrt=0;
    grp_tmp.insert_key(key_tmp);
    printf("--是否继续下一个关键词组的录入？(是1/否0)\n  ");
    cin>>flg;
  }while(flg);

  flg=1;
  do{
    Keys key_tmp;
    printf("--将设定该应答组的第%d种回复\n",grp_tmp.ans_num+1);
    {
      bool flgs=1;
      do{
        printf("----请输入该回复的第%d条消息，输入\"//end\"结束\n    ",key_tmp.word_num+1);
        {
          string ss;
          do{getline(cin,ss);}
          while(ss.length()==0);
          if(ss=="//end")flgs=0;
          else{
            Words temp;
            strcpy(temp.txt,GBKToUTF8(ss.c_str()).c_str());
            key_tmp.insert_word(temp);
          }
        }
      }while(flgs);
    }
    grp_tmp.insert_ans(key_tmp);
    printf("--是否继续下一种回复的录入？(是1/否0)\n  ");
    cin>>flg;
  }while(flg);
  printf("请输入该应答组的优先级：");
  cin>>grp_tmp.pri;
  grp.push_back(grp_tmp);
  grp_num++;
  printf("是否继续词库录入？(是1/否0)：");
  {
    bool t;
    cin>>t;
    return t;
  }
}
void Words::save_to_file(FILE*fp){
  fprintf(fp,"%s\n",txt);
}
void Keys::save_to_file(FILE*fp,bool if_key){
  for(int i=0;i<word_num;i++)word[i].save_to_file(fp);
  if(if_key)
    fprintf(fp,allcrt?"Key_1\n":"Key_0\n");
}
void Group::save_to_file(FILE*fp){
  if(sp)        fprintf(fp,"spid=%s\n",spid.c_str());
  else if(sg)   fprintf(fp,"sgid=%s\n",spid.c_str());
  else if(priv) fprintf(fp,"Priv_key\n");
  else          fprintf(fp,"Key_st\n");
  for(int i=0;i<key_num;i++)key[i].save_to_file(fp,1);

  fprintf(fp,"Key_ed\n");

  for(int i=0;i<ans_num;i++){
    ans[i].save_to_file(fp,0);
    if(i!=(ans_num-1))
    fprintf(fp,"Ans_or\n");
  }

  fprintf(fp,"Pri\n%d\n\n",pri);
}
void save_ans_to_file(FILE*fp){
  for(int i=0;i<grp_num;i++)grp[i].save_to_file(fp);
  fprintf(fp,"//end\n");
}
void guide(){
  printf("是否导入已有词库？(是1/否0)：");
  {
    bool t;
    cin>>t;
    if(t){
      printf("请输入导入词库文件名：");
      cin>>ANS_FILENAME;
      read_ans();
      freopen("CON","r",stdin);
    }
  }
  do{
    printf("下面将进行第%d个应答组的录入。\n",grp_num+1);
  }while(insert_group());
  printf("请输入保存的文件名：");
  cin>>SAVE_ANS_FILENAME;
  FILE*fp;
  fp=fopen(SAVE_ANS_FILENAME.c_str(),"w");
  save_ans_to_file(fp);
  printf("程序运行结束，请检查是否保存成功。\n");
  system("pause");
}
