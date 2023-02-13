function [output_name] = get_pic()
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    file_list=dir('background');
    file_list(1:2)=[];
    n=size(file_list);
    rand('seed',sum(100*clock))
    r=randi([1,n(1)]);
    output_name=strcat('background/',file_list(r).name);
end