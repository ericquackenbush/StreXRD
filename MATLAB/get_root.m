function [s] = get_root(r_0, z_0, z_1, g)

maxIterations = 1074;
n_0 = r_0 * z_0;
s_0 = z_1 - 1;
if (g < 0)
    s_1 = 0;
else
    s_1 = robust_length(n_0, z_1) - 1;
end
s = 0;

i = 0;
while (i < maxIterations)
    s = (s_0 + s_1) / 2;
    if (s == s_0 || s == s_1)
        break
    end
    ratio_0 = n_0/(s + r_0);
    ratio_1 = z_1/(s + 1);
    g = ratio_0^2 + ratio_1^2 - 1;
    if (g > 0)
        s_0 = s;
    elseif (g < 0)
        s_1 = s;
    else
        break
    end
    i = i + 1;
end