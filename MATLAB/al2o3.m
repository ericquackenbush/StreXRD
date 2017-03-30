clear;
clc;

% d-spacings for Al2O3 in descending order (in Angstroms)
d_spacing = [2.379; 2.165; 1.961; 1.740; 1.375; 1.147];
E = 50e3;           % energy in eV
L = 2.0;            % sample to detector distance in meters
x_0 = 0;         % x-coordinate of beam center                
y_0 = 0;         % y-coordinate of beam center

h = 4.135667662e-15;                    % Plank's constant
lambda = h* physconst('LightSpeed')/E;  % in meters
lambda_A = lambda/1e-10;                % in Angstroms

two_theta = 2*asind(lambda_A./(2*d_spacing));

num_pts = 1000;
theta = 0:2*pi/num_pts:2*pi;
rings = cell(size(d_spacing, 1),1);
for i = 1:size(d_spacing, 1)
    r = L*tand(two_theta(i));
    
    x = x_0 + r*cos(theta);
    y = y_0 + r*sin(theta);
    
    rings{i} = [x; y];
    rings{i}(3,:) = L;
end
rings_prime = rings;

% rotate the pixel coordinate in the standard labatory frame
rho_1 = -0.1;     % rotation around the x_1 axis in degrees
rho_2 = -0.5;      % rotation around the x_2 axis in degrees
rho_3 = 0;      % rotation around the x_3 axis in degrees

R_1 = [1 0 0; 0 cosd(rho_1) -sind(rho_1); 0 sind(rho_1) cosd(rho_1)];
R_2 = [cosd(rho_2) 0 sind(rho_2); 0 1 0; -sind(rho_2) 0 cosd(rho_2)];
R_3 = [cosd(rho_3) -sind(rho_3) 0; sind(rho_3) cosd(rho_3) 0; 0 0 1];

for i=1:size(rings, 1)  
    for j=1:size(rings{i}, 2)
        rings_prime{i}(:,j) = R_1*R_2*R_3*rings{i}(:,j);
    end
end

fileID = fopen('CeO2_042713_002.mar3450','r');
cimg = fread(fileID, '*uint8');

sd = 3450;
img = int32(unpack345(cimg,sd));

min_x = -0.45;
max_x = 0.45;
min_y = -0.45;
max_y = 0.45;

% imagesc([min_x max_x], [min_y max_y], flipud(img));
hold on;
for i = 1:size(rings_prime, 1)
    plot(rings_prime{i}(1,:),rings_prime{i}(2,:),'r-');
end
hold off;