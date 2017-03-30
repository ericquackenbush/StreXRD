clear;
clc;

d_spacing = 3.1241;     % in Angstroms
E = 52e3;               % energy in eV
L = 1.1;                % sample to detector distance in meters
x_0 = 0;                % x-coordinate of beam center                
y_0 = 0;                % y-coordinate of beam center

h = 4.135667662e-15;                    % Plank's constant
lambda = h* physconst('LightSpeed')/E;  % in meters
lambda_A = lambda/1e-10;                % in Angstroms

two_theta = 2*asind(lambda_A/(2*d_spacing));

num_pts = 100;
theta = 0:2*pi/num_pts:2*pi;
pts = zeros(3, size(theta, 2));
n_pts = pts;

pts(3,:) = L;
for i = 1:size(d_spacing, 1)
    r = L*tand(two_theta(i));
    
    x = x_0 + r*cos(theta);
    y = y_0 + r*sin(theta);
    
    pts(1,:) = x;
    pts(2,:) = y;
end

% rotate the pixel coordinate in the standard labatory frame
rho_1 = 0.0;       % rotation around the x_1 axis in degrees
rho_2 = 0.0;       % rotation around the x_2 axis in degrees
rho_3 = 100.0;        % rotation around the x_3 axis in degrees

R_1 = [1 0 0; 0 cosd(rho_1) -sind(rho_1); 0 sind(rho_1) cosd(rho_1)];
R_2 = [cosd(rho_2) 0 sind(rho_2); 0 1 0; -sind(rho_2) 0 cosd(rho_2)];
R_3 = [cosd(rho_3) -sind(rho_3) 0; sind(rho_3) cosd(rho_3) 0; 0 0 1];

for i=1:size(pts, 2)
    n_pts(:,i) = R_1*R_2*R_3*pts(:,i);
end

plot3(pts(1,:), pts(2,:), pts(3,:), 'b-','LineWidth',2); hold on;
plot3(n_pts(1,:), n_pts(2,:), n_pts(3,:), 'r-','LineWidth',2);
% scatter(n_pts(1,:), n_pts(2,:),'r');

ellipse_t = fit_ellipse( n_pts(1,:), n_pts(2,:));