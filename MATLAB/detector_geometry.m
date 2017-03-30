clear;
clc;

x_0 = 0;
y_0 = 0;
a = 4;
b = 4;
phi = 0;

N = 1000;
theta = 0:2*pi/N:2*pi;
x = zeros(3, size(theta,2));
f1 = x_0 + a*cos(theta)*cos(phi) - b*sin(theta)*sin(phi);
x(1,:) = f1;
f2 = y_0 + a*cos(theta)*sin(phi) + b*sin(theta)*cos(phi);
x(2,:) = f2;

% let's move to 3d space
sample_to_detector_distance = 1.0; % in meters
x(3,:) = repmat(sample_to_detector_distance, 1, size(theta,2));

% rotate the pixel coordinate in the standard labatory frame
rho_1 = 45;   % rotation around the x_1 axis in degrees
rho_2 = 0;      % rotation around the x_2 axis in degrees
rho_3 = 0;      % rotation around the x_3 axis in degrees

R_1 = [1 0 0; 0 cosd(rho_1) -sind(rho_1); 0 sind(rho_1) cosd(rho_1)];
R_2 = [cosd(rho_2) 0 sind(rho_2); 0 1 0; -sind(rho_2) 0 cosd(rho_2)];
R_3 = [cosd(rho_3) -sind(rho_3) 0; sind(rho_3) cosd(rho_3) 0; 0 0 1];

x_prime = zeros(3, size(x,2));
for i=1:size(x,2)
    x_prime(:,i) = R_1*R_2*R_3*x(:,i);
end

% plot3(x_prime(1,:),x_prime(2,:),x_prime(3,:),'r-','LineWidth',2);

plot(x(1,:),x(2,:),'r-','LineWidth',2);
hold on;
plot(x_prime(1,:),x_prime(2,:),'b-','LineWidth',2);