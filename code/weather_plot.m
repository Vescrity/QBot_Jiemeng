function [] = weather_plot(tmp)
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
    axes1 = axes('Parent',gcf);
    hold(axes1,'on');

    % 绘制气温数据
    [ax, h1, h2] = plotyy(1:day_cnt, max_tmp, 1:day_cnt, pop);
    if(stat==7)
        plot(ax(1),(1:day_cnt),min_tmp,'Color',[100 100 255]/255,'LineStyle','-');
        for(i=1:day_cnt)
            text(ax(1),i,min_tmp(i),[num2str(min_tmp(i)),'℃'],'Color',[100 100 255]/255);
        end
    end
    set(h1, 'Color', [255 100 100]/255, 'LineStyle', '-');
    set(h2, 'Color', '#EE82EE', 'LineStyle', '--');

    % 设置坐标轴标签和标题
    xlabel('时间', 'Color', [160,160,160]/255, 'FontWeight', 'bold');
    ylabel(ax(1), '气温', 'Color', [160,160,160]/255, 'FontWeight', 'bold');
    ylabel(ax(2), '本来是降水概率', 'Color', [200,200,200]/255, 'FontWeight', 'bold');
    title('气温趋势图', 'Color', [160,160,160]/255, 'FontWeight', 'bold');

    % 设置y轴范围
    ylim(ax(1), [min_min-2, max_max+2]);
    ylim(ax(2), [0, 100]);
    xlim([0,day_cnt+1]);
    % 添加文本标签
    for i = 1:day_cnt
        text(ax(1),i, max_tmp(i), [num2str(max_tmp(i)),'℃'], 'Color', [255 100 100]/255);
        text(ax(2),i, pop(i), [num2str(pop(i)),'%'], 'Color', [100 255 100]/255);
    end

    %saveas(gcf, 'qwq.jpg');

    print('../tmp/qwq.png','-dpng');
    close all;
    A = imread('../tmp/qwq.png');
    imwrite(white_to_ground(A, get_pic()), '../tmp/qwq.png');
end