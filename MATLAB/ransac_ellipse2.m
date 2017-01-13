function [bestParameter1,bestParameter2, bestParameter3, bestParameter4, bestParameter5, bestInIdx, bestInNum2] = ransac_ellipse(data,iter,threshDist)
% data: a N x 2 dataset with N data points
% iter: the number of iterations
% threshDist: the threshold of the distances between points and the fitting line
% inlierRatio: the threshold of the numer of inliers
num = 5;    % number of points needed to fit ellipse

%% Plot the data points
number = size(data,1);  % Total number of points
bestInNum = 0;          % Best fitting model with largest number of inliers
bestInIdx = 0;
bestInNum2 = 0;
maxEccentricity = 0.2;

% parameters for best ellipse model
bestParameter1=0;bestParameter2=0;bestParameter3=0;bestParameter4=0;bestParameter5=0;
for i=1:iter
    ellipse = 0;
    %% Find an ellipse
    while (ellipse == 0)
        % Randomly select 5 points
            idx = randperm(number,num); sample = data(idx,:);
        % Get coefficients of resulting conic
            [A, B, C, D, E, F] = conic_coeffs(sample);
        % Check if it's non-degenerate and ellipse
        if (is_degenerate(A, B, C, D, E, F) == 0 && is_ellipse(A, B, C) == 1)
            [x_0, y_0, a, b, phi] = ellipse_param(A, B, C, D, E, F);
            e = sqrt(1 - b^2/a^2);
            % make sure it is close to circular
            e = sqrt(1 - b^2/a^2);
            if (e < maxEccentricity)
                ellipse = 1;
            end
        end
    end
    %% Compute the distances between all points and the ellipse
    distance = zeros(1, number);
    parfor k = 1: number
        distance(k) = dist_ellipse2(x_0, y_0, a, b, phi, data(k,1), data(k,2));
    end
    %% Compute the inliers with distances smaller than the threshold
    inlierIdx = find(abs(distance)<=threshDist);
    inlierNum = length(inlierIdx);
    %% Update the number of inliers and fitting model if better model is found     
    if inlierNum > bestInNum
        bestInIdx = inlierIdx;
        bestInNum2 = bestInNum;
        bestInNum = inlierNum;
        bestParameter1 = x_0;
        bestParameter2 = y_0;
        bestParameter3 = a;
        bestParameter4 = b;
        bestParameter5 = phi;
    end
    %% Determine progress
    progress = i/iter * 100;
    fprintf('%1.0f%%\n', progress);
end