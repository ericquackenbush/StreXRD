clear;
clc;

fileID = fopen('CeO2_042713_002.mar3450','r');
cimg = fread(fileID, '*uint8');

sd = 3450;
img = int32(unpack345(cimg,sd));
img = int16(img);

% imagesc(img);

% implement new center-finding algorithm
n = 5; % this controls the number of grid lines
x_grid = cell(n,1);
y_grid = cell(n,1);
grid_line = linspace(0, sd, n + 2);
grid_line = grid_line(2:end-1);

% for i = 1:n
%     [cx, cy, c] = improfile(img,[1 grid_line(i)],[sd grid_line(i)]);
%     x_grid{i} = [cx, cy, c];
%     
%     [cx, cy, c] = improfile(img,[grid_line(i) 1],[grid_line(i) sd]);
%     y_grid{i} = [cx, cy, c];
% end

[cx, cy, c] = improfile(img,[1 grid_line(1)],[sd grid_line(1)]);
for i = size(c):-1:1
   if c(i) == 0
       cx(i) = [];
       cy(i) = [];
       c(i) = [];
   end
end

array_of_sum_diff = 1:size(c);
% fold along a pixel value
for i = 1:size(c)
    left = c(1:i-1);
    right = c(i+1:size(c));
    sizeOfLeft = size(left, 1);
    sizeOfRight = size(right, 1);
    if sizeOfLeft > sizeOfRight
      left = left(sizeOfLeft-sizeOfRight+1:end);
    elseif sizeOfRight > sizeOfLeft
      right = right(1: sizeOfLeft);
    end
    right = flip(right,1);
    diff = abs(left - right);
   array_of_sum_diff(i) = sum(diff);
end

[M, I] = min(array_of_sum_diff);
disp(cx(I));
disp(cy(I));

% i = 500;
% left = c(1:i-1);
% right = c(i+1:size(c));
% sizeOfLeft = size(left, 1);
% sizeOfRight = size(right, 1);
% if sizeOfLeft > sizeOfRight
%   left = left(sizeOfLeft-sizeOfRight+1:end);
% elseif sizeOfRight > sizeOfLeft
%   right = right(1: sizeOfLeft);
% end
% right = flip(right,1);
% diff = abs(left - right);
% % plot(1:size(left,1), left, 1:size(right, 1), right);
% sum_diff = sum(diff);