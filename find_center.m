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

% convert from pixel to cartesian coordinates
x = x - w/2;
y = h/2 - y;
data = [x(:), y(:)];

% find best ellipse
[x_0, y_0, a, b, phi] = ransac_ellipse(data, 200, 10, 0.25);

% convert back to pixel coordinates
x_0 = round(x_0 + w/2, 0);
y_0 = round(h/2 - y_0, 0);

bestPeakFound = 0;

while (bestPeakFound == 0)
    % trial points
    [X, Y] = meshgrid(x_0 - 2 : x_0 + 2, y_0 - 2 : y_0 + 2);

    maxPeak = zeros(numel(X),1);
    
    for n = 1:numel(X)
        % do azimuthal integration
        [R, I] = integrate2(img, X(n), Y(n), 500);

        % calculate sliding average
        A = movmean(I,3);

        % subtract background noise
        I = I - A;
        
        % remove negative values
        for i = 1:size(I,2)
            if I(i) < 0
                I(i) = 0;
            end
        end

        % may want to consider both height and width of peaks
        [pks, locs] = findpeaks(I,R);

        maxPeak(n) = max(pks);
    end
    
    [best, ind] = max(maxPeak);
    
    if ind == 13
        bestPeakFound = 1;
    else
        x_0 = X(ind);
        y_0 = Y(ind);
    end
end