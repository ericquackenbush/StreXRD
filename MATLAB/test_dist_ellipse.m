[x_0, y_0, semiMajor, semiMinor, rotation] = ellipse_param(2, -3, 4, 6, -3, 4);
x = 0;
y = 1.4;


% need to translate and rotate point (x,y) into frame of ellipse
x = x - x_0;
y = y - y_0;
x = x*cos(rotation) + y*sin(rotation);
y = -x*sin(rotation) + y*cos(rotation);

a = semiMajor;
b = semiMinor;

tol = 10^-6;
diff = 1;
theta = atan2(a*y, b*x);
while (diff > tol)
    theta_n = theta - ((a^2 - b^2)*cos(theta)*sin(theta) - ...
        x*a*sin(theta)+y*b*cos(theta))/((a^2-b^2)*(cos(theta)^2 - ...
        sin(theta)^2)- x*a*cos(theta)-y*b*sin(theta));
    diff = abs(theta_n - theta);
    theta = theta_n;
end

% find point corresponding to theta
x_e = a*cos(theta);
y_e = b*cos(theta);

% find distance between (x,y) and (x_e, y_e)
distance = sqrt((x - x_e)^2 + (y - y_e)^2);
disp(distance)
    
