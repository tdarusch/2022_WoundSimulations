gammavals = [3 3 3 3.5 3.5 3.5 4 4 4 4.5 4.5 4.5 5 5 5 5.5 5.5 5.5 6 6 6 6.5 6.5 6.5];
durationvals = [.28 .35 .32 .42 .39 .32 .39 .42 .45 .39 .42 .45 .49 .7 .56 .6 .6 .63 1.47 .7 1.09 1.09 .95 1.05];

close all
figure;
scatter(gammavals, durationvals, 'x');
title('Wound Closure Time v. \gamma', 'FontSize', 16, 'FontWeight', 'bold');
xlabel('\gamma', 'FontSize', 14, 'FontWeight', 'bold');
ylabel('Closure Time (h)','FontSize', 14, 'FontWeight', 'bold');

