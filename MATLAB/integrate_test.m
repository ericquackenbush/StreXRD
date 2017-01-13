clear;
clc;

shape = [1000, 1000];   % shape of image
% position of center
x_c = 500;
y_c = 500;
rings = 0:50:450;
x = 0:shape(1) - 1;
y = 0:shape(2) - 1;
x = x - x_c;
y = y - y_c;
z = meshgrid(x.*x) + meshgrid(y.*y)';
r = sqrt(z);
img = 0;
for i = 1:length(rings)
    img = img + exp(-(r - rings(i)).^2);   % gaussian profile
end

bins = 500;

rmin = min(r(:));
rmax = max(r(:));
delta_r = (rmax - rmin)/(2.0 * bins);
integrated = zeros(1,bins);
positions = linspace(rmin + delta_r, rmax - delta_r, bins);
for i = 1:length(positions)
   valid = abs(r - positions(i)) < delta_r;
   pix_pos = find(valid);
   intensities = img(pix_pos);
   integrated(i) = mean(intensities);
end

R = positions;
I = integrated;

plot(R, I)
