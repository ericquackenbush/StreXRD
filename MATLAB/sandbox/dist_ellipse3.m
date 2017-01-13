% function to find the distance from a point to an ellipse

function [distance] = dist_ellipse3(x_0, y_0, semiMajor, semiMinor, rotation, x, y)

% need to translate and rotate point (x,y) into frame of ellipse
x_p = x - x_0;
y_p = y - y_0;
x = x_p*cos(rotation) + y_p*sin(rotation);
y = -x_p*sin(rotation) + y_p*cos(rotation);

a = semiMajor;
b = semiMinor;

myfun = @(theta, a, b, x, y) (a^2 - b^2)*cos(theta)*sin(theta) - x*a*sin(theta) + y*b*cos(theta);
fun = @(theta) myfun(theta, a, b, x, y);
theta0 = atan2(a*y, b*x);
theta1 = fzero(fun, theta0);

% find point corresponding to theta
x_e = a*cos(theta1);
y_e = b*sin(theta1);

% find distance between (x,y) and (x_e, y_e)
distance = sqrt((x - x_e)^2 + (y - y_e)^2);