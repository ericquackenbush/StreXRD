clear;
clc;

% load image
img = imread('test.tiff');

% center coordinates
x_0 = 534;
y_0 = 1138;

bins = 500;

% image resolution
h = size(img,1);
w = size(img,2);

x = 0:w - 1;
y = 0:h - 1;
x = x - x_0;
y = y - y_0;
z = meshgrid(x.*x) + meshgrid(y.*y)';
radius = sqrt(z);

rmax = max(radius(:));
rmin = min(radius(:));
binWidth = (rmax - rmin)/bins;

[u_hist, edges] = histcounts(radius, bins, 'BinWidth',binWidth);

% weighted histogram function
subs = ceil((radius(:)-rmin)/binWidth + 1e-15) ;
w_hist = accumarray(subs,img(:));
w_hist = w_hist';

positions = (edges(1:end-1) + edges(2:end))/2;
integrated = w_hist./u_hist;
integrated(u_hist == 0) = 0;

R = positions;
I = integrated;

plot(R, I);