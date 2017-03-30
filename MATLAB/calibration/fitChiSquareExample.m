function fit = fitChiSquareExample
%FITCHISQUAREEXAMPLE Example for use with fitChiSquare
%
% Usage:
%   fitChiSquareExample
%
% This file gives example usage of the fitChiSquare function.  It generates
% a set of x and y data using the modelling function at the bottom of the
% file.  Uniformly distributed noise is added to both the x and y
% variables, then fitChiSquare is called to determine the starting model
% parameters.

% Author:   N. Brahms
% Date:     Jan 25, 2006
% Version:  2.1
%
% The example model is the general low-temperature thermal conduction
% function,
%       y = p(1) * ( x_1^p(2) - x_2^p(2) )
% where y is conducted power, x_1 and x_2 are temperatures at either end of
% the material, p(1) is a material-dependent prefactor, and p(2) is a
% material-type-dependent power law (2 for metals, 3 for amorphous
% insulators, 4 for crystalline insulators)

% ------------------- SETUP ------------------------------------------

% The model to use
m=fitLibrary('cryotherm');
disp(sprintf('Using the following model:\n%s',m.desc));

model = eval(m.model);

% Set up the data and model parameters here
x1 = 0:0.5:10;  % Independent variable 1
x2 = 0:0.1:2;   % Independent variable 2

p(1) = 1;       % Model parameter 1
p(2) = 3;       % Model parameter 2

dx1 = 0.1;      % Measurement error (std. dev.) on ind. variable 1
dx2 = 0.05;     % Measurement error (std. dev.) on ind. variable 2
dy = 5;         % Measurement error (std. dev.) on the dep. variable

% Set up the optimization options here.  See the optimset help file for
% more information.
op = optimset;          % Default optimization options
op.DisplayUncVal = 0;   % Do not display iterative uncertainty information
op.Display = 'on';      % Display fit output and plot result
op.LowerBound = [0.5 2.5];
op.UpperBound = [1.5 3.5];
% op.LowerBound = [1e4 -5];
% op.UpperBound = [1e6 -2];
op.ErrorsUnknown = 0;
op.FitUncertainty = [0 1];
op.Plot = 1;
op.PlotResiduals = 1;
op.Scale = 1;

% ------------------- PROCEDURE --------------------------------------

% Used to time the fitting
tic;

% Load independent variable array
x(:,1) = x1;
x(:,2) = x2;

% Calculate dependent variable
y  = feval(model,p,x);

% Load independent variable error array
dx(:,1) = dx1;
dx(:,2) = dx2;

% Add uniformly distributed noise
x(:,1) = x(:,1) + sqrt(12)*dx1*(rand(length(x(:,1)),1)-0.5);
x(:,2) = x(:,2) + sqrt(12)*dx2*(rand(length(x(:,2)),1)-0.5);
y = y + sqrt(12)*dy*(rand(length(y),1)-0.5);

% Perform the fit
[fit.params,fit.dParams,fit.gof,fit.stddev] = ...
    fitChiSquare(x,y,model,p,dx,dy,op);

% Display elapsed time
disp(sprintf('Time = %fs', toc));

% Add plot information
%     if op.Plot
%         if op.PlotResiduals
%             subplot(3,1,[1 2]);
%         end
%         legend({...
%             sprintf('Data \\pm \\sigma'), ...
%             sprintf(['Fit: y = %0.3g (x_1^{%0.3g} - x_2^{%0.3g})\n'...
%             '~\\chi^2 = %0.2g'],...
%             fit.params(1), fit.params(2), fit.params(2),...
%             fit.chiSquare/fit.dof)},...
%             'Location','NorthWest');
%     end

end

%{
% Modeling function
function result = model(p,x)

% Take the real value to prevent complex output for negative x values.
result = real(p(1)*(x(:,1).^p(2)-x(:,2).^p(2)));

end
%}