x_0 = 0;
y_0 = 0;
a = 4;
b = 2;
phi = 0;

N = 1000;
theta = 0:2*pi/N:2*pi;
xAxis = x_0 + a*cos(theta)*cos(phi) - b*sin(theta)*sin(phi);
yAxis = y_0 + a*cos(theta)*sin(phi) + b*sin(theta)*cos(phi);
plot(xAxis,yAxis,'r-','LineWidth',2);
hold on;

% Given point
x_p = 0;
y_p = 4;
plot(x_p, y_p, 'ob');

% need to translate and rotate point (x,y) into frame of ellipse
x_p = x_p - x_0;
y_p = y_p - y_0;
x = x_p*cos(phi) + y_p*sin(phi);
y = -x_p*sin(phi) + y_p*cos(phi);

tol = 10^-6;
diff = 1;
theta = atan2(a*y, b*x);
while (diff > tol)
    theta_n = theta - ((a^2 - b^2)*cos(theta)*sin(theta) - ...
        x*a*sin(theta)+y*b*cos(theta))/((a^2 - b^2)*(cos(theta)^2 - ...
        sin(theta)^2)- x*a*cos(theta)-y*b*sin(theta));
    diff = abs(theta_n - theta);
    theta = theta_n;
end

% find point corresponding to theta
x_e = a*cos(theta);
y_e = b*sin(theta);
plot(x_e, y_e, 'og');

% find distance between (x,y) and (x_e, y_e)
distance = sqrt((x - x_e)^2 + (y - y_e)^2);
disp(distance)
