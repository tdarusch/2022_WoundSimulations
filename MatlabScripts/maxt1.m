maxt1s = [59 45 53 49 42 48 47 42 49 50 45 47 56 45 43 48 47 46 39 45 38 42 41 40];

close all
figure;
scatter(gammavals, maxt1s, 'x');
title('Maximum T1 Swaps in Wound v. \gamma', 'FontSize', 16, 'FontWeight', 'bold');
xlabel('\gamma', 'FontSize', 24, 'FontWeight', 'bold');
ylabel('Maximum T1 Swaps','FontSize', 14, 'FontWeight', 'bold');