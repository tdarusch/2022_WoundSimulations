gamma1 = [3 3 3 3.5 3.5 3.5 4 4 4 4.5 4.5 4.5 5 5 5 5.5 5.5 5.5 6 6 6 6.5 6.5 6.5];
maxsp = [4.726621538	4.64988	4.607375 4.373658235	4.42266125	4.524843571 4.487024286	4.463051818	4.469230667 4.412042	4.4351075	4.38832 4.384386875	4.360152308	4.517976 4.193799091	4.349360526	4.426948333 4.271498125	4.335055833	4.177822143 4.231512667	4.101320476	4.279813333];

close all
figure;
scatter(gamma1, maxsp, 'diamond', 'LineWidth', 1.5, 'MarkerFaceColor', 'flat');
title('Maximum Shape Parameter v. \gamma', 'FontSize', 16, 'FontWeight', 'bold');
xlabel('\gamma', 'FontSize', 24, 'FontWeight', 'bold');
ylabel('Maximum Shape Parameter','FontSize', 14, 'FontWeight', 'bold');