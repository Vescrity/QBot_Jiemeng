function [] = mat_run(input_exp)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
    %p=eval(input_exp);
    diary('outtmp.txt')
    diary on
    eval(input_exp)
    diary off
    %save outtmp.txt -ascii ans
    %dlmwrite('outtmp.txt',ans,'\t');
    %fp=fopen('outtmp.txt','w');
    %fprintf(fp,'%0.6g ',p);
end