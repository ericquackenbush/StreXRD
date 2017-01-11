function [length] = robust_length(v_0, v_1)

if (abs(v_0) == max([abs(v_0), abs(v_1)]))
    length = abs(v_0)*sqrt(1 + (v_1/v_0)^2);
elseif (abs(v_1) == max([abs(v_0), abs(v_1)]))
    length = abs(v_1)*sqrt(1 + (v_0/v_1)^2);
end