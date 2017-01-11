function [distance] = distance_point_ellipse(e_0, e_1, y_0, y_1)

distance = 0;
if (y_1 > 0)
    if (y_1 > 0)
        z_0 = y_0/e_0;
        z_1 = y_1/e_1;
        g = z_0^2 + z_1^2 - 1;
        if (g ~= 0)
            r_0 = (e_0/e_1)^2;
            s_bar = get_root(r_0, z_0, z_1, g);
            x_0 = r_0*y_0/(s_bar + r_0);
            x_1 = y_1/(s_bar + 1);
            distance = sqrt((x_0 - y_0)^2 + (x_1 - y_1)^2);
        else
            x_0 = y_0;
            x_1 = y_1;
            distance = 0;
        end
    else
        x_0 = 0;
        x_1 = e_1;
        distance = abs(y_1 - e_1);
    end
else
    numer_0 = e_0*y_0;
    denom_0 = e_0^2 - e_1^2;
    if (numer_0 < denom_0)
        xde_0 = numer_0/denom_0;
        x_0 = e_0 * xde_0;
        x_1 = e_1*sqrt(1 - xde_0^2);
        distance = sqrt((x_0 - y_0)^2 + x_1^2);
    else
        x_0 = e_0;
        x_1 = 0;
        distance = abs(y_0 - e_0);
    end
end
        