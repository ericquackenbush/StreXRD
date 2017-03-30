clear;
clc;

fileID = fopen('CeO2_042713_002.mar3450','r');
cimg = fread(fileID, '*uint8');

sd = 3450;
img = int32(unpack345(cimg,sd));
indices = find(abs(img)<1000);
img(indices) = 0;
imagesc(img);
% BW = edge(int16(img),'canny');