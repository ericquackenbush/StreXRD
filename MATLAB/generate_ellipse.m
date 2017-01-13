% generates random ellipse with outliers
% make sure ransac_ellipse has a sufficient maxEccentricity value!

clc;
clear; 
a = -100;   % min
b = 100;    % max
N = 50;     % Number of points
% generate random matrix
M = round((b-a)*rand(N,2)+a);
% add points from an ellipse
a = 100;     % radius along semi-major axis
b = 50;      % radius along semi-minor axis
t = 0.0:0.01:2*pi;
length = size(t);
M(N+1:N+length(2), 1) = a*cos(t);
M(N+1:N+length(2), 2) = b*sin(t);

[x_0, y_0, a, b, phi] = ransac_ellipse(M, 100, 2, 0.8);

scatter(M(:,1), M(:,2)); hold on;

N = 1000;
theta = 0:2*pi/N:2*pi;
xAxis = x_0 + a*cos(theta)*cos(phi) - b*sin(theta)*sin(phi);
yAxis = y_0 + a*cos(theta)*sin(phi) + b*sin(theta)*cos(phi);
plot(xAxis,yAxis,'r-','LineWidth',2);
hold off;