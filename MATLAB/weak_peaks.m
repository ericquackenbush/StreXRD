% Replicates the Fit2D Weak Peaks function

clear;
clc;

% load image
img = imread('test.tiff');

h = size(img, 1);
w = size(img, 2);

% generate random points
numPts = 200;
x=rand(1,numPts)*w;
y=rand(1,numPts)*h;

avg = zeros(numPts,1);
sd = zeros(numPts,1);

for i = 1:numPts
    [xval, yval] = meshgrid(1:w, 1:h);
    mask = ((xval-x(i)).^2 + (yval-y(i)).^2) <= 10;
    values = img(mask);

    avg(i) = mean(values);
    sd(i) = std(double(values));
end

[minI, ind] = min(avg);
minI = (minI - 3*sd(ind))/65535;

[maxI, ind] = max(avg);
maxI = (maxI + 5*sd(ind))/65535;

K = imadjust(img,[minI maxI],[]);
imagesc(K);