% load image
img = imread('untitled.png');

% remove noise with median filter
img = medfilt2(img);

% histogram equalization
% img = histeq(img);

% convert to black and white
threshold = 0.7;
BW = im2bw(img, threshold);

% BW = edge(img);

% convert to Cartesian coordinates
[y,x] = find(BW == 1);
data = [x(:), -y(:) + length(img)];

scatter(data(:,1), data(:,2))
hold on;
%imshow(BW);

a = 565;
b = a;
x_c = 531.0276;
y_c = 909.5;
phi = 0;

N = 1000;
theta = 0:2*pi/N:2*pi;
xAxis = x_c + a*cos(theta)*cos(phi) - b*sin(theta)*sin(phi);
yAxis = y_c + a*cos(theta)*sin(phi) + b*sin(theta)*cos(phi);
plot(xAxis,yAxis,'r-','LineWidth',2);
hold off;