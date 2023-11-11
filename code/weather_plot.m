function [] = weather_plot(tmp)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    tm=sscanf(tmp,"%d,");
    day_cnt=tm(1);
    stat=tm(2);
    l=3;
    r=l+day_cnt-1;
    max_tmp=tm(l:r);
    max_max=max(max_tmp);
    min_min=min(max_tmp);
    if(stat==7)
        l=r+1;
        r=l+day_cnt-1;
        min_tmp=tm(l:r);
        min_min=min(min_tmp);
    end
    l=r+1;
    r=l+day_cnt-1;
    pop=tm(l:r);
    
    figure('Visible','off');
    %set(gcf,'Color',[0,0,0]);
    hold on;
    yyaxis left;
    plot((1:day_cnt),max_tmp,'Color',[255 100 100]/255,'LineStyle','-');
    for(i=1:day_cnt)
        text(i,max_tmp(i),[num2str(max_tmp(i)),'℃'],'Color',[255 100 100]/255);
    end
    if(stat==7)
        plot((1:day_cnt),min_tmp,'Color',[100 100 255]/255,'LineStyle','-');
        for(i=1:day_cnt)
            text(i,min_tmp(i),[num2str(min_tmp(i)),'℃'],'Color',[100 100 255]/255);
        end
    end
    title('气温-降水概率趋势图','Color',[160,160,160]/255,'FontWeight','bold');
    xlabel('时间','Color',[160,160,160]/255,'FontWeight','bold');
    ylabel('气温','Color',[160,160,160]/255,'FontWeight','bold');
    ylim([min_min-2,max_max+2]);
    yyaxis right;
    plot(1:day_cnt,pop,'Color','#EE82EE','LineStyle','--');
    xlim([0,day_cnt+1]);
    ylim([0,100]);
    for(i=1:day_cnt)
            text(i,pop(i),[num2str(pop(i)),'%'],'Color',[100 255 100]/255);
        end
    ylabel('降水概率','Color',[200,200,200]/255,'FontWeight','bold');
    %fname='C:\Users\Vescr\Desktop\pics';
    saveas(gcf,'qwq.jpg');
    close all;
    A=imread('qwq.jpg');
    imwrite(white_to_ground(A,get_pic()),'qwq.jpg')
end