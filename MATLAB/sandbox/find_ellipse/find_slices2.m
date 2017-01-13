clear;
clc;

% load image
img = imread('test.tiff');

% Find the center
% 
% or approximate of it. Let call it x0,y0 and let the image resolution be xs,ys.
% Either you already know its position (due to knowledge of your image properties)
% or estimate it (scan one oval and find 2 lines that goes through whole oval their
% intersection is your center). If the center is un-precise then the result will be
% more like ovals then ellipses but that does not matter you can reconstruct original
% de-noised image latter and or recompute center from latter data...

% position of center
x0 = 534;
y0 = 1138;

% image resolution
xs = 2048;
ys = 2048;

% Cast ray from (x0,y0) to (xs-1,0)
% 
% and copy image content along it to some cleared line buffer of size xs+ys just to
% be sure to cover any center position with reserve...

% create cell containing all the rays
numendpts = 2*(xs-1) + 2*(ys-1);
rays = cell(numendpts, 1);

% Integrate the whole image with rays
% 
% so cast rays always from (x0,y0) to (xs-1,1),(xs-1,2)...(xs-1,ys-1) then
% (xs-2,ys-1),...(0,ys-1) etc to cover whole circumference of image. Find out the
% scale ratio to original stored line (most of peaks must match) and add rescaled
% image content along rays into to stored line buffer.

% generate an array containing all the endpoints of the square

endpts = zeros(numendpts,2);
endpts(1:xs,1) = sort(1:xs,'descend');
endpts(1:xs,2) = 1;
endpts(xs+1:xs + ys-1,1) = 1;
endpts(xs+1:xs + ys-1,2) = 2:ys;
endpts(xs + ys: 2*xs + ys-2,1) = 2:xs;
endpts(xs + ys: 2*xs + ys-2,2) = ys;
endpts(2*xs + ys-1: 2*xs + 2*ys - 4,1) = xs;
endpts(2*xs + ys-1: 2*xs + 2*ys - 4,2) = sort(2:ys-1,'descend');

% find angle of each endpoint
angles = zeros(numendpts,1);

% find angle of original ray
a = atan2(y0 - 1,xs - x0);
tem = (a < 0);
a(tem) = a(tem) + 2.*pi;
angle_orig = radtodeg(a);

for i = 1:size(angles)
    a = atan2(y0 - endpts(i,2),endpts(i,1) - x0);
    tem = (a < 0);
    a(tem) = a(tem) + 2.*pi;
    angle = radtodeg(a);
    if (angle >= angle_orig)
        angle = angle - angle_orig;
    else
        angle = 360 - (angle_orig - angle);
    end
    angles(i) = angle;
end

% generate rays
for i = 1:numendpts
    [x, y] = bresenham(x0, y0, endpts(i,1), endpts(i,2));
    distance = sqrt((x - x0).^2 + (y - y0).^2);
    intensity = 1:size(x,1);
    intensity = intensity';
    for j = 1:size(x,1)
        intensity(j) = img(x(j), y(j));
    end
    rays{i,1} = [x, y, intensity, distance];
end