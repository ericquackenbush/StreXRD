% load image
img = imread('untitled.png');

% remove noise with median filter
img = medfilt2(img);

% histogram equalization
% img = histeq(img);

% convert to black and white
threshold = 0.7;
BW = im2bw(img, threshold);

% convert to Cartesian coordinates
h = size(img, 1);
w = size(img, 2);

[y,x] = find(BW == 1);
data = [x(:) - w/2, h/2 - y(:)];

scatter(data(:,1), data(:,2))
hold on;

a = 565;
b = a;
x_c = 534 - w/2;
y_c = h/2 - 1138;
phi = 0;

N = 1000;
theta = 0:2*pi/N:2*pi;
xAxis = x_c + a*cos(theta)*cos(phi) - b*sin(theta)*sin(phi);
yAxis = y_c + a*cos(theta)*sin(phi) + b*sin(theta)*cos(phi);
plot(xAxis,yAxis,'r-','LineWidth',2);
hold off;