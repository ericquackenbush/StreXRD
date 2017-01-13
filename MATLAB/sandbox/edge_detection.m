clc;
clear;

% load image
img = imread('test.tiff');

% remove noise with median filter
img = medfilt2(img);

% image properties
h = size(img,1);
w = size(img,2);

% edge detection
BW = edge(img);

% convert to Cartesian coordinates
[y,x] = find(BW);
data = [x(:) - w/2, h/2 - y(:)];

% subtract the points from the outer edge
tol = 10;
for i = length(data):-1:1
    x_1 = data(i,1);
    y_1 = data(i,2);
    distance = sqrt((y_1)^2 + (x_1)^2);
    if distance > w/2 - tol || distance > h/2 - tol
        data(i,:) = [];
    end
end

scatter(data(:,1), data(:,2));