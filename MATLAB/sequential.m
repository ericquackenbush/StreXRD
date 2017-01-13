function [X_0, Y_0, A, B, Phi] = sequential(data, iter, threshDist, minInlierRatio)

delete(gcp('nocreate'));
poolobj = parpool('local', 2);

i = 1;
inlierRatio = 1.0;
while (inlierRatio > minInlierRatio)
   [x_0, y_0, a, b, phi, bestInIdx, bestInNum2] = ransac_ellipse(data, iter, threshDist);
   X_0(i) = x_0;
   Y_0(i) = y_0;
   A(i) = a;
   B(i) = b;
   Phi(i) = phi;
   data = removerows(data, bestInIdx);
   number = size(data,1);
   disp(number);
   inlierRatio = bestInNum2/number;
   disp(inlierRatio);
   i = i + 1;
end

delete(poolobj)