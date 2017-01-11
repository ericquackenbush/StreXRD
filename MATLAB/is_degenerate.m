% Tests whether conic is degenerate or not
% Returns 1 if the conic is degenerate, 0 if the conic is non-degenerate
function [output] = is_degenerate(A, B, C, D, E, F)

M = [A, B/2, D/2; B/2, C, E/2; D/2, E/2, F];
tol = 10^-3;    % tolerance
% non-zero determinant means non-degenerate
if (abs(det(M)) > tol)
    output = 0;
else
    output = 1;
end