clear;
clc;

fileID = fopen('test.img','r');
img = fread(fileID, 'int32');
imgr = reshape(img, [3450 3450]);

imagesc(imgr');
% imagesc(weak_peaks(img));