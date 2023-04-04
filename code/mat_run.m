function [] = mat_run()
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
    %p=eval(input_exp);
    mat_run_s=fopen('mat_code.m','r');
    mat_run_p=fscanf(mat_run_s,'%c');
    t=timer('TimerFcn','fprintf(''[ERROR]运行超时，请检查死循环等问题。\n'');system(''taskkill /im mat_run.exe /f'');','StartDelay',5);
    start(t);
    diary('outtmp.txt')
    diary on
    eval(mat_run_p)
    diary off
    %save outtmp.txt -ascii ans
    %dlmwrite('outtmp.txt',ans,'\t');
    %fp=fopen('outtmp.txt','w');
    %fprintf(fp,'%0.6g ',p);
end