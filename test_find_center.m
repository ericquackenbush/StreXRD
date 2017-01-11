clear;
clc;

% load image
img = imread('test.tiff');

x_0 = 534;
y_0 = 1138;

bestPeakFound = 0;
[X, Y] = meshgrid(x_0 - 2 : x_0 + 2, y_0 - 2 : y_0 + 2);

maxPeak = zeros(numel(X),1);

for n = 1:numel(X)
    % do azimuthal integration
    [R, I] = integrate2(img, X(n), Y(n), 500);

    [pks, locs] = findpeaks(I,R);

    maxPeak(n) = max(pks);
end

[best, ind] = max(maxPeak);

if ind == 13
    bestPeakFound = 1;
else
    x_0 = X(ind);
    y_0 = Y(ind);
end