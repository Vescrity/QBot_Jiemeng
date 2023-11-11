function [A] = white_to_ground(A,pic_file_name)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    A1=int64(A);
    B=A1(:,:,1)+A1(:,:,2)+A1(:,:,3);
    C=find(B>=220*3);
    ground=imread(pic_file_name);
    S_A=size(A);
    S_G=size(ground);
    spc_A=S_A(1)/S_A(2);
    spc_G=S_G(1)/S_G(2);
    if(spc_A>spc_G)
        rat=S_A(1)/S_G(1);
    else 
        rat=S_A(2)/S_G(2);
    end
    ground=imresize(ground,rat);
    S_G=size(ground);
    if(spc_A<spc_G)
        for(i=1:(S_G(1)-S_A(1))/2)
            ground(1,:,:)=[];
            ground(end,:,:)=[];
        end
    else 
        for(i=1:(S_G(2)-S_A(2))/2)
            ground(:,1,:)=[];
            ground(:,end,:)=[];
        end
    end
    for(i=1:3)
        D=A(:,:,i);
        E=ground(:,:,i)/3;
        D(C)=E(C);
        A(:,:,i)=D;
    end
end