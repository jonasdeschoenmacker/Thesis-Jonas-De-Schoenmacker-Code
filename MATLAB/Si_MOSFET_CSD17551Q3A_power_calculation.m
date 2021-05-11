%  KU Leuven - Technology Campus Gent,
%  Gebroeders De Smetstraat 1,
%  B-9000 Gent, Belgium
%
%         File: Si_MOSFET_CSD17551Q3A_power_calculation
%      Created: 2020-04-16
%       Author: Jonas De Schoenmacker
%      Version: 1.0
%
%  Description: Vermogenverlies in MOSFETs CSD17551Q3A, masterproef:
%  snelladen voor robotische applicaties
%% Enkele constanten
% datasheet CSD17551Q3A
% https://www.ti.com/lit/ds/symlink/csd17551q3a.pdf?ts=1597727120181

clear all;
close all;
clc;

f_s = 800e3; %schakelfrequentie [Hz]
% f_s = 1200e3; %schakelfrequentie [Hz], het IC kan ook sneller schakelen
% I_ch = 16.256; % maximale oplaadstroom van de lader [A]
R_dson = 9e-3; % [ohm]
Q_GD = 1.5e-9; % [C]
Q_GS = 2.3e-9; % [C]
Q_G = 6e-9; % [C]
V_plt = 2.7; % [V]
R_on = 1.5; %[ohm]
R_off = 1.5; %[ohm]

V_in = 17:1:20;
V_out = 16.8;

I_DS = 0:0.064:16.256;

%% buck-mode
D = zeros(1,4);
D = V_out./V_in % bij buck


I_on = (6-V_plt)/R_on;
I_off = (V_plt)/R_off;
Q_SW = Q_GD+ Q_GS/2;
t_on = Q_SW/I_on;
t_off = Q_SW/I_off;

P_loss = zeros(8,255);
for nu = 0:3
    P_loss(nu+1,:) = D(nu+1)*I_DS.^2.*R_dson + ((1/2).*V_in(nu+1).*I_DS.*(t_on+t_off).*f_s) + (6*Q_G*f_s);
end
for nu = 0:3
    P_loss(nu+5,:) = (1-D(nu+1))*I_DS.^2.*R_dson + ((1/2).*V_in(nu+1).*I_DS.*(t_on+t_off).*f_s) + (6*Q_G*f_s);
end
figure;
plot(I_DS,P_loss);
grid on;
legend('T5,T8: V_{in} = 17V','T5,T8: V_{in} = 18V','T5,T8: V_{in} = 19V','T5,T8: V_{in} = 20V','T6,T8: V_{in} = 17V','T6,T8: V_{in} = 18V','T6,T8: V_{in} = 19V','T6,T8: V_{in} = 20V','Location','NorthWest');
title('Verliezen in MOSFETs tijdens buck-mode','interpreter','latex');
xlabel('$I_{DS}$ [A]','interpreter','latex');
ylabel('$P_{loss}$ [Watt]','interpreter','latex');

%% boost-mode

f_s = 800e3; %schakelfrequentie [Hz]
% f_s = 1200e3; %schakelfrequentie [Hz], het IC kan ook sneller schakelen
% I_ch = 16.256; % maximale oplaadstroom van de lader [A]
R_dson = 9e-3; % [ohm]
Q_GD = 1.5e-9; % [C]
Q_GS = 2.3e-9; % [C]
Q_G = 6e-9; % [C]
V_plt = 2.7; % [V]
R_on = 1.5; %[ohm]
R_off = 1.5; %[ohm]

V_in = 5:5:15;
V_out = 16.8;

I_DS = 0:0.064:16.256;
D = zeros(1,11);
% D = V_out./V_in % bij buck
D = 1- (V_in/V_out) % bij boost
% D = V_out /(V_out-V_in) % bij buck-boost 
% P_geleiding = D.*I_DS.^2*R_dson;

I_on = (6-V_plt)/R_on;
I_off = (V_plt)/R_off;
Q_SW = Q_GD+ Q_GS/2;
t_on = Q_SW/I_on;
t_off = Q_SW/I_off;

P_loss = zeros(6,255);
for nu = 0:2
    P_loss(nu+1,:) = D(nu+1)*I_DS.^2.*R_dson + ((1/2).*V_in(nu+1).*I_DS.*(t_on+t_off).*f_s) + (6*Q_G*f_s);
end
for nu = 0:2
    P_loss(nu+4,:) = (1-D(nu+1))*I_DS.^2.*R_dson + ((1/2).*V_in(nu+1).*I_DS.*(t_on+t_off).*f_s) + (6*Q_G*f_s);
end
figure;
plot(I_DS,P_loss);
grid on;
legend('T5,T7: V_{in} = 5V','T5,T7: V_{in} = 10V','T5,T7: V_{in} = 15V','T6,T8: V_{in} = 5V','T6,T8: V_{in} = 10V','T6,T8: V_{in} = 15V','Location','NorthWest');
title('Verliezen in MOSFETs tijdens boost-mode','interpreter','latex');
xlabel('$I_{DS}$ [A]','interpreter','latex');
ylabel('$P_{loss}$ [Watt]','interpreter','latex');

% %% buck-boost
% 
% f_s = 800e3; %schakelfrequentie [Hz]
% % f_s = 1200e3; %schakelfrequentie [Hz], het IC kan ook sneller schakelen
% % I_ch = 16.256; % maximale oplaadstroom van de lader [A]
% R_dson = 9e-3; % [ohm]
% Q_GD = 1.5e-9; % [C]
% Q_GS = 2.3e-9; % [C]
% Q_G = 6e-9; % [C]
% V_plt = 2.7; % [V]
% R_on = 1.5; %[ohm]
% R_off = 1.5; %[ohm]
% 
% V_in = 15:1:17;
% V_out = 16.8;
% 
% I_DS = 0:0.064:16.256;
% D = zeros(1,11);
% % D = V_out./V_in % bij buck
% % D = 1- (V_in/V_out) % bij boost
% D = V_out./(V_out-V_in) % bij buck-boost 
% % P_geleiding = D.*I_DS.^2*R_dson;
% 
% I_on = (6-V_plt)/R_on;
% I_off = (V_plt)/R_off;
% Q_SW = Q_GD+ Q_GS/2;
% t_on = Q_SW/I_on;
% t_off = Q_SW/I_off;
% 
% P_loss = zeros(6,255);
% for nu = 0:2
%     P_loss(nu+1,:) = D(nu+1)*I_DS.^2.*R_dson + ((1/2).*V_in(nu+1).*I_DS.*(t_on+t_off).*f_s) + (6*Q_G*f_s);
% end
% for nu = 0:2
%     P_loss(nu+4,:) = (1-D(nu+1))*I_DS.^2.*R_dson + ((1/2).*V_in(nu+1).*I_DS.*(t_on+t_off).*f_s) + (6*Q_G*f_s);
% end
% figure;
% plot(I_DS,P_loss);
% grid on;
% legend('High-side: V_{in} = 15V','High-side: V_{in} = 16V','High-side: V_{in} = 17V','Low-side: V_{in} = 15V','Low-side: V_{in} = 16V','Low-side: V_{in} = 17V','Location','NorthWest');
% title('Verliezen in high- en low-side MOSFET tijdens boost-mode','interpreter','latex');
% xlabel('$I_{DS}$ [A]','interpreter','latex');
% ylabel('$P_{loss}$ [Watt]','interpreter','latex');

%% Gemeten effecientie

Eff_V_in = 12; %[V]
Eff_I_in = [0.05 0.23 0.36 0.62 1.26 2.46 5.16]; %[A]

Eff_V_out = [13.9884 14.0081 14.0312 14.0669 14.1653 14.3478 14.6772]; %[V]
Eff_I_out = [-0.0023 0.1211 0.2172 0.4305 0.9969 1.9719 3.9227]; %[I]

Eff_P_in = Eff_V_in.*Eff_I_in; %[W]
Eff_P_out = Eff_V_out.*Eff_I_out; %[W]

Eff = Eff_P_out./Eff_P_in;

figure;
plot(Eff_I_out,Eff);
grid on;
%legend('Effecientie lader','Location','NorthWest');
set(gca,'YLim',[0 1])
set(gca,'YTick',(0:0.05:1))
title('Effecientie lader','interpreter','latex');
xlabel('$I_{oplaad}$ [A]','interpreter','latex');
ylabel('$\eta_{lader}$','interpreter','latex');

%% temperaturen componenten lader

raw_data = csvread('datalog_temperaturen_lader.csv',1,0);

nauwkeurigheid = 5/1024

TH_T2 = raw_data(:,2);
TH_T2 = (-8.05585e-3.*((5-(TH_T2.*nauwkeurigheid))./((TH_T2.*nauwkeurigheid)./26860)))+110.55853;
TH_T5 = raw_data(:,3);
TH_T5 = (-8.05585e-3.*((5-(TH_T5.*nauwkeurigheid))./((TH_T5.*nauwkeurigheid)./26670)))+110.55853;
TH_L1 = raw_data(:,4);
TH_L1 = (-8.05585e-3.*((5-(TH_L1.*nauwkeurigheid))./((TH_L1.*nauwkeurigheid)./26760)))+110.55853;
TH_T7 = raw_data(:,5);
TH_T7 = (-8.05585e-3.*((5-(TH_T7.*nauwkeurigheid))./((TH_T7.*nauwkeurigheid)./26660)))+110.55853;
TH_RSR = raw_data(:,6);
TH_RSR = (-8.05585e-3.*((5-(TH_RSR.*nauwkeurigheid))./((TH_RSR.*nauwkeurigheid)./26810)))+110.55853;


TH = [smoothdata(TH_T2.','gaussian',15);smoothdata(TH_T5.','gaussian',15);smoothdata(TH_L1.','gaussian',15);smoothdata(TH_T7.','gaussian',15);smoothdata(TH_RSR.','gaussian',15)];

% TH = [TH_T2.';TH_T5.';TH_L1.'];
time_as = raw_data(:,1).*5./60;
figure;
plot(time_as,TH);
grid on;
legend('T2','T5','L1','T7','RSR','Location','NorthWest');
set(gca,'XLim',[0 35])
set(gca,'XTick',(0:5:35))
title('Temperaturen verschillende componenten','interpreter','latex');
xlabel('Tijd [min.]');
ylabel('Temperatuur [°C]');


