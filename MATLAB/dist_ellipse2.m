% function to find the distance from a point to an ellipse

function [distance] = dist_ellipse2(x_0, y_0, a, b, rotation, x, y)

% need to translate and rotate point (x,y) into frame of ellipse
x_p = x - x_0;
y_p = y - y_0;
x = x_p*cos(rotation) + y_p*sin(rotation);
y = -x_p*sin(rotation) + y_p*cos(rotation);

% need to transform point to first quadrant
x = abs(x);
y = abs(y);

distance = distance_point_ellipse(a, b, x, y);