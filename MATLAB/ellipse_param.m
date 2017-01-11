% Determines ellipse parameters from coefficients of ellipse conic section

function [x_0, y_0, semiMajor, semiMinor, rotation] = ellipse_param(A, B, C, D, E, F)

B = B/2;
D = D/2;
G = F;
F = E/2;

% determine center
x_0 = (C*D - B*F)/(B^2 - A*C);
y_0 = (A*F - B*D)/(B^2 - A*C);

semiMajor = sqrt((2*(A*F^2 + C*D^2 + G*B^2 - 2*B*D*F - A*C*G))...
    / ((B^2 - A*C)*(sqrt((A - C)^2 + 4*B^2) - (A + C))));

semiMinor = sqrt((2*(A*F^2 + C*D^2 + G*B^2 - 2*B*D*F - A*C*G))...
    / ((B^2 - A*C)*(-sqrt((A - C)^2 + 4*B^2) - (A + C))));

rotation = 0;

if (B == 0 && A < C)
    rotation = 0;
elseif (B == 0 && A > C)
    rotation = (1.0/2.0)*pi;
elseif (B ~= 0 && A < C)
    rotation = (1.0/2.0)*acot((A - C)/(2*B));
elseif (B ~= 0 && A > C)
    rotation = pi/2.0 + (1.0/2.0)*acot((A - C)/(2.0*B));
end

if (semiMinor > semiMajor)
    temp = semiMajor;
    semiMajor = semiMinor;
    semiMinor = temp;
    rotation = rotation + pi/2;
end