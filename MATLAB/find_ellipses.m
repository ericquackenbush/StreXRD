% load image
img = imread('untitled.png');

% remove noise with median filter
img = medfilt2(img);

% histogram equalization
% img = histeq(img);

% convert to black and whiteSo 
threshold = 0.7;
BW = im2bw(img, threshold);

% convert to Cartesian coordinates
h = size(img, 1);
w = size(img, 2);
[y,x] = find(BW == 1);
data = [x(:) - w/2, h/2 - y(:)];

[X_0, Y_0, A, B, Phi] = sequential_ransac(data, 100, 10, 0.25);
scatter(data(:,1), data(:,2))
hold on;

%% Plot the best fitting ellipses
N = 1000;
theta = 0:2*pi/N:2*pi;
for i = 1:length(X_0)
    xAxis = X_0(i) + A(i)*cos(theta)*cos(Phi(i)) - B(i)*sin(theta)*sin(Phi(i));
    yAxis = Y_0(i) + A(i)*cos(theta)*sin(Phi(i)) + B(i)*sin(theta)*cos(Phi(i));
    plot(xAxis,yAxis,'r-','LineWidth',2);
end
hold off;