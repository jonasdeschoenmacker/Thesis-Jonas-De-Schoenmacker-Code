%  KU Leuven - Technology Campus Gent,
%  Gebroeders De Smetstraat 1,
%  B-9000 Gent, Belgium
%
%         File: PCB_Thermische_Berekeningen
%      Created: 2020-04-16
%       Author: Jonas De Schoenmacker
%      Version: 1.0
%
%  Description: Thermische berkeneningen volgens axismmetric model met
%  Bessel-functies
%% Thermal resistance of board to ambient of circle region
% https://www.onsemi.com/pub/Collateral/AND9596-D.PDF
clear all;
close all;
clc;
% plot I_0() and I_1()
% plot k_0() and K_1()
z = 0:0.01:5;
IK = zeros(4,501);
for nu = 0:1
    IK(nu+1,:) = besseli(nu,z);
end
for nu = 0:1
    IK(nu+3,:) = besselk(nu,z);
end
figure;
plot(z,IK);
axis([0 5 0 8]);
grid on;
legend('I_0','I_1','K_0','K_1','Location','NorthWest');
title('Bessel functies van de eerste en tweede orde','interpreter','latex');
xlabel('z','interpreter','latex');
ylabel('$I_(z) \& K_(z)$','interpreter','latex');

% plot k thermal conductivity of the PCB W/(mK)
% vanuitgaande dat deze punten een linear verloop hebben zoals hieronder
% weergegeven
% https://www.electronics-cooling.com/1998/05/conduction-heat-transfer-in-a-printed-circuit-board/

oz = 0:1:8;
k = zeros(1,9);
for i = 0:8
    k(i+1) = (5.8871*i)+2.8229;
end
figure;
plot(oz,k);
grid on;
legend('k','Location','NorthWest');
title('Thermischeconductiviteitsparameter k van de PCB $\frac{W}{mK}$','interpreter','latex');
xlabel('$oz/ft^2$','interpreter','latex');
ylabel('$k(oz)$','interpreter','latex');

%% Create formala Axisymmetric model

% parameters
oz = 1;     % copper layers [oz/ft^2]
t = 1.6e-3;    % PCB board thickness [m]
N = 1;      % Number of surfaces coold by h
h = 15;    % Convection film coefficient [W/mK]
a = 1.5e-3;      % Inner radius [m]
b = 6.5e-3;     % Outer radius [m]

% calculated parameters
k = (5.8871*oz)+2.8229; %thermal conductivity of the PCB
alpha = sqrt((N*h)/(k*t))

% circular fin formula
R_ab=(1/(2*pi*a*k*t*alpha))*(((besselk(1,(alpha*b))*besseli(0,alpha*a))+(besselk(0,(alpha*a))*besseli(1,alpha*b)))/((besselk(1,(alpha*a))*besseli(1,alpha*b))-(besselk(1,(alpha*b))*besseli(1,alpha*a))))
%tussen1 = ((besselk(1,(alpha*b))*besseli(0,alpha*a))+(besselk(0,(alpha*a))*besseli(1,alpha*b)))
%tussen2 = ((besselk(1,(alpha*a))*besseli(1,alpha*b))-(besselk(1,(alpha*b))*besseli(1,alpha*a)))
disp("°C/W");
