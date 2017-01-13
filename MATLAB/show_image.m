clear;
clc;

fileID = fopen('CeO2_042713_002.mar3450','r');
cimg = fread(fileID, '*uint8');

sd = 3450;
img = int32(unpack345(cimg,sd));

imagesc(img);
% imagesc(weak_peaks(img));