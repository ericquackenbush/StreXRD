clear;
clc;

% load image
img = imread('test.tiff');

K = weak_peaks(img);

mesh(K);