function [positions, integrated] = integrate(img, x_0, y_0, bins)
    % image resolution
    h = size(img,1);
    w = size(img,2);
    
    x = 0:w - 1;
    y = 0:h - 1;
    x = x - x_0;
    y = y - y_0;
    z = meshgrid(x.*x) + meshgrid(y.*y)';
    radius = sqrt(z);

    rmin = min(radius(:));
    rmax = max(radius(:));
    delta_r = (rmax - rmin)/(2.0 * bins);
    integrated = zeros(1,bins);
    positions = linspace(rmin + delta_r, rmax - delta_r, bins);
    
    for i = 1:length(positions)
       valid = abs(radius - positions(i)) < delta_r;
       pix_pos = find(valid);
       intensities = img(pix_pos);
       integrated(i) = sum(intensities,1)./sum(intensities~=0,1);
    end