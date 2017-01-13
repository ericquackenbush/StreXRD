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
[x,y] = find(BW == 1);
data = [x(:), y(:)];

%scatter(data(:,1), data(:,2))
%imshow(BW);

[X_0, Y_0, A, B, Phi] = sequential(data, 100, 10, 0.25);
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