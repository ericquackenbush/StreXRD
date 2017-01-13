clear;
clc;

% delete(gcp('nocreate'));
% poolobj = parpool('local', 2);

% load image
img = imread('test.tiff');

shape = [2048, 2048];   % shape of image

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
% rays = cell(numendpts, 1);

% generate initial ray
x_orig = [x0 xs-1];
y_orig = [y0 0];
% improfile(img,x,y), grid on;
[cx, cy, c] = improfile(img,x_orig,y_orig);
distance = sqrt((cx - x0).^2 + (cy - y0).^2);
% rays{1,1} = [distance, c];

% Integrate the whole image with rays
% 
% so cast rays always from (x0,y0) to (xs-1,1),(xs-1,2)...(xs-1,ys-1) then
% (xs-2,ys-1),...(0,ys-1) etc to cover whole circumference of image. Find out the
% scale ratio to original stored line (most of peaks must match) and add rescaled
% image content along rays into to stored line buffer.

% generate an array containing all the endpoints of the square

endpts = zeros(numendpts,2);
endpts(1:xs-1,1) = sort(1:xs-1,'descend');
endpts(1:xs-1,2) = 0;
endpts(xs:xs + ys-1,1) = 0;
endpts(xs:xs + ys-1,2) = 0:ys-1;
endpts(xs + ys: 2*xs + ys-3,1) = 1:xs-2;
endpts(xs + ys: 2*xs + ys-3,2) = ys-1;
endpts(2*xs + ys - 2: 2*xs + 2*ys - 4,1) = xs - 1;
endpts(2*xs + ys - 2: 2*xs + 2*ys - 4,2) = sort(1:ys-1,'descend');

% find angle of each endpoint
angles = zeros(numendpts,1);

% find angle of original ray
a = atan2(y0,xs-1 - x0);
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
% for i = 2:numendpts
%     disp(['Generating rays: ', num2str(i/numendpts*100,2), '%']);
%     x = [x0 endpts(i,1)];
%     y = [y0 endpts(i,2)];
%     [cx, cy, c] = improfile(img,x,y);
%     distance = sqrt((cx - x0).^2 + (cy - y0).^2);
%     rays{i,1} = [distance, c];
% end

load('rays.mat');

% a = 6000;
% b = 6000;
% 
% load('pks.mat');
% load('locs.mat');

% calculate sliding average
% e = movmean(rays{a,1}(:,2),4);

% remove negative values
% e = rays{a,1}(:,2) - e;
% for i = 1:size(e,1)
%     if e(i) < 20
%         e(i) = 0;
%     end
% end

% [pks1, locs1] = findpeaks(rays{a,1}(:,2), rays{a,1}(:,1), 'MinPeakProminence', 100);
% 
% plot(rays{a,1}(:,1), rays{a,1}(:,2)); hold on;
% scatter(locs1,pks1);

% plot(rays{a,1}(:,1), e);
% scatter(locs, pks);
% plot(rays{b,1}(:,1), rays{b,1}(:,2));

% Remember the scale for each angle used.
% 
% After covering whole image divide line buffer by the count of used rays so you
% got average cut through the oval rings with much much lover noise. Now just
% find the peaks in it. Each peak represents separate oval and its position gives
% you radius at the initial ray position angle.

% for i = 1:numendpts
    

% The scales gives you the oval/elliptical properties. So find min and max scale
% that gives you angles where major and minor axises are ... The scale itself with
% peak position gives you the size of axis ... So no need for RANSAC (but you can
% still use it for the scale computation).

% [Notes]
% 
% To avoid seems in the average line you can add a count for each pixel in it holding
% how many rays was added to each pixel and use that for the final division.