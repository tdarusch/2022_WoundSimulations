gamma = 3:0.5:6.5;

duration = [0.316666667	0.376666667	0.42	0.42	0.583333333	0.61	1.086666667	1.03];
errduration = [0.035118846	0.051316014	0.03	0.03	0.106926766	0.017320508	0.385010822	0.072111026];

close all
figure;
e = errorbar(gamma, duration, errduration, "vertical", "o");
e.MarkerSize = 4;
e.CapSize = 4;
e.MarkerFaceColor = "auto";
hold on
plot(gamma, y_est2, 'r--', 'LineWidth', 0.5, 'Color', 'black');
title('Wound Healing Duration v. \gamma (Intercellular)')
xlabel('\gamma (Intercellular)');
ylabel('Wound Healing Duration (h)');
xlim([3 6.5]);
ylim([0 1.5]);

c1 = polyfit(gamma,duration,1);
y_est2 = polyval(c1,gamma);
dresid = duration - y_est2;
SSresid = sum(dresid.^2);
SStotal = ((length(duration) - 1) * var(duration));
rsq = (1 - (SSresid/SStotal));
slopetxt = ['y = ' num2str(c1(1)) '*x + ' num2str(c1(2))];
rsq2 = num2str(rsq)
rsqtxt = append('R^2 = ', rsq2);
linetxt2 = {slopetxt, rsqtxt};
text(5, 1.3, linetxt2, "FontWeight", "bold", "EdgeColor", "black", "HorizontalAlignment", "Center");
hold off