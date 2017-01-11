clear;
clc;

shape = [1000, 1000];   % shape of image
% position of center
x_c = 500;
y_c = 500;
rings = 0:50:450;
x = 0:shape(1) - 1;
y = 0:shape(2) - 1;
x = x - x_c;
y = y - y_c;
z = meshgrid(x.*x) + meshgrid(y.*y)';
r = sqrt(z);
img = 0;
for i = 1:length(rings)
    img = img + exp(-(r - rings(i)).^2);   % gaussian profile
end

bins = 500;
radius = r;

maxValue = max(radius(:));
minValue = min(radius(:));
binWidth = (maxValue - minValue)/bins;

[u_hist, edges] = histcounts(radius, bins, 'BinWidth',binWidth);

% weighted histogram function
subs = ceil((r(:)-minValue)/binWidth + 1e-15) ;
w_hist = accumarray(subs,img(:),[bins, 1]);
w_hist = w_hist';

% % slow (but sure) weighted histogram function
% w_hist = zeros(1, bins);
% for i = 1:length(r(:))
%     ind = find(edges <= r(i), 1, 'last' );
%     if ~isempty(ind)
%         if ind == length(edges)
%             ind = length(edges) - 1;
%         end
%         w_hist(ind) = w_hist(ind) + img(i);
%     end
% end

positions = (edges(1:end-1) + edges(2:end))/2;
integrated = w_hist./u_hist;
integrated(u_hist == 0) = 0;

plot(positions,integrated);