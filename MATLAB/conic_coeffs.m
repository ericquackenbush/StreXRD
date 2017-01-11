% Input: 5 x 2 data array

% Output: Coefficients of conic equation: 
% Ax^2 + By^2 + Cxy + Dx + Ey + F = 0

function [A, B, C, D, E, F] = conic_coeffs(data)

% determine A
M(:,1) = data(:,1).*data(:,2);  % element-wise multiplication
M(:,2) = data(:,2).^2;          % square each entry
M(:,3) = data(:,1);
M(:,4) = data(:,2);
M(:,5) = ones(5,1);
A = det(M);

% determine B
M(:,1) = data(:,1).^2;          % square each entry
M(:,2) = data(:,2).^2;          % square each entry
M(:,3) = data(:,1);
M(:,4) = data(:,2);
M(:,5) = ones(5,1);
B = -det(M);

% determine C
M(:,1) = data(:,1).^2;          % square each entry
M(:,2) = data(:,1).*data(:,2);  % element-wise multiplication
M(:,3) = data(:,1);
M(:,4) = data(:,2);
M(:,5) = ones(5,1);
C = det(M);

% determine D
M(:,1) = data(:,1).^2;          % square each entry
M(:,2) = data(:,1).*data(:,2);  % element-wise multiplication
M(:,3) = data(:,2).^2;          % square each entry
M(:,4) = data(:,2);
M(:,5) = ones(5,1);
D = -det(M);

% determine E
M(:,1) = data(:,1).^2;          % square each entry
M(:,2) = data(:,1).*data(:,2);  % element-wise multiplication
M(:,3) = data(:,2).^2;          % square each entry
M(:,4) = data(:,1);
M(:,5) = ones(5,1);
E = det(M);

% determine F
M(:,1) = data(:,1).^2;          % square each entry
M(:,2) = data(:,1).*data(:,2);  % element-wise multiplication
M(:,3) = data(:,2).^2;          % square each entry
M(:,4) = data(:,1);
M(:,5) = data(:,2);
F = -det(M);