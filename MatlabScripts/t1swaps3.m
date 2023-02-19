gamma20 = [3 3 3 3.5 3.5 3.5 4 4 4 4.5 4.5 4.5 5 5 5 5.5 5.5 5.5 6 6 6 6.5 6.5 6.5];
maxt1swaps = [59	45	53 49	42	48 47	42	49 50	45	47 55	44	43 48	47	46 39	45	38 42	41	40];
time2half = [0.245	0.28	0.28 0.315	0.315	0.245 0.28	0.35	0.385 0.3325	0.385	0.28 0.3675	0.56	0.42 0.3675	0.42	0.49 0.8225	0.5775	0.7 0.7175	0.525	0.7875];

close all
figure;
scatter(gamma20, maxt1swaps, 'diamond', 'LineWidth', 1.5, 'MarkerFaceColor', 'flat');
title('Total T1 Swaps v. \gamma', 'FontSize', 16, 'FontWeight', 'bold');
xlabel('\gamma', 'FontSize', 24, 'FontWeight', 'bold');
ylabel('Total T1 Swaps','FontSize', 14, 'FontWeight', 'bold');

figure;
scatter(gamma20, time2half, 'diamond', 'LineWidth', 1.5, 'MarkerFaceColor', 'flat');
title('1/2 Time to Max T1 v. \gamma', 'FontSize', 16, 'FontWeight', 'bold');
xlabel('\gamma', 'FontSize', 24, 'FontWeight', 'bold');
ylabel('1/2 Time to Max T1','FontSize', 14, 'FontWeight', 'bold');