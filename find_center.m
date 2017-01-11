clear;
clc;

% load image
img = imread('test.tiff');

% image resolution
w = size(img,1);
h = size(img,2);

% convert to black and white 
threshold = 0.01;
BW = im2bw(img, threshold);

% convert to Cartesian coordinates
[y,x] = find(BW == 1);

% convert from raster to cartesian
x = x - w/2;
y = h/2 - y;
data = [x(:), y(:)];

% % find best ellipse
% [x_0, y_0, a, b, phi] = ransac_ellipse2(data, 200, 10, 0.25);

% % plot ellipse
% N = 1000;
% theta = 0:2*pi/N:2*pi;
% xAxis = x_0 + a*cos(theta)*cos(phi) - b*sin(theta)*sin(phi);
% yAxis = y_0 + b*cos(theta)*sin(phi) + b*sin(theta)*cos(phi);
% scatter(x, y); hold on;
% plot(xAxis,yAxis,'r-','LineWidth',2);

% center
x_0 = 534;
y_0 = 1137;

bestPeakFound = 0;

while (bestPeakFound == 0)
    % trial points
    pts = [x_0, y_0; x_0 - 1, y_0; x_0 + 1, y_0; x_0 - 1, y_0 - 1; ...
        x_0, y_0 - 1; x_0 + 1, y_0 - 1; x_0 + 1, y_0 + 1; x_0, y_0 + 1; ... 
        x_0 + 1, y_0 + 1];

    maxPeak = zeros(size(pts,1),1);
    
    for n = 1:size(pts,1)
        % do azimuthal integration
        [R, I] = integrate(img, pts(n, 1), pts(n, 2), 500);

        % plot(R,I);

        % calculate sliding average
        e = movmean(I,3);

        % remove negative values
        e = I - e;
        for i = 1:size(e,2)
            if e(i) < 2
                e(i) = 0;
            end
        end

        % plot(R, I); hold on;
        % plot(R,e);

        % may want to consider both height and width of peaks
        [pks, locs] = findpeaks(e,R);

        maxPeak(n) = max(pks);
        
    end
    
    [best, ind] = max(maxPeak);
    
    if ind == 1
        bestPeakFound = 1;
    else
        x_0 = pts(ind, 1);
        y_0 = pts(ind, 2);
    end
end