% generates random ellipse with outliers
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
t = 0.0:0.1:6.2;
length = size(t);
M(N+1:N+length(2), 1) = a*cos(t);
M(N+1:N+length(2), 2) = b*sin(t);

%scatter(M(:,1), M(:,2))

[x_0, y_0, a, b, phi, bestInIdx] = ransac_ellipse(M, 10, 5, 0.5);