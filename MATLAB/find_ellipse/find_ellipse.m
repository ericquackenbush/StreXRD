clear;
clc;

% load image
img = imread('test.tiff');

% image center
xc = 534;
yc = 1138;

% image resolution
xs = 2048;
ys = 2048;

numendpts = 2*(xs-1) + 2*(ys-1);
pts = zeros(numendpts,2);

load('locs.mat');
load('angles.mat');
load('endpts.mat');
load('pks.mat');
load('w.mat');

n = 4;
w(n) = 40; % override width

m = 1;
for k = 1:numendpts
    xe = endpts(k,1);
    ye = endpts(k,2);

    % go through all the rays looking for peaks in region of interest
    rmin = locs(n) - w(n)/2;
    rmax = locs(n) + w(n)/2;

    x = [xc xe];
    y = [yc ye];
    [cx, cy, c] = improfile(img,x,y);
    distance = sqrt((cx - xc).^2 + (cy - yc).^2);


    j = 1;
    temp = 0;
    for i = 1:size(c,1)
        if distance(i) > rmin && distance(i) < rmax
            temp(j,1) = distance(i);
            intensity = c(i);
            temp(j,2) = intensity;
            j = j +1;
        end
    end

    % improfile(img,x,y); hold on;
    % plot(temp(:,1), temp(:,2));

    % find point where max intensity occurs
    % maxI = max(temp(:,2));
    % ind = find(c==maxI);
    % x = cx(ind);
    % y = cy(ind);

    % find distance where max intensity occurs
    if temp ~= 0
        [maxI, ind] = max(temp(:,2));
        dist = temp(ind,1);
        m = m + 1;
    end

    pts(m,1) = cosd(angles(k)) * dist;
    pts(m,2) = sind(angles(k)) * dist;
end

new_pts = pts(any(pts,2),:);
scatter(new_pts(:,1), new_pts(:,2));
