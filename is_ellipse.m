% determines whether the conic is an ellipse
function [output] = is_ellipse(A, B, C)

M = [A, B/2; B/2, C];
tol = 10^-3; % tolerance
if (det(M) > tol)
    output = 1;
else
    output = 0;
end