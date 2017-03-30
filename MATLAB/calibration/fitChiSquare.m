function [params,dParams,gof,stddev]...
    = fitChiSquare(xData,yData,modelFun,guess,dxData,dyData,options)
%FITCHISQUARE Performs a chi-square fit returning parameter uncertainty.
%
%   Usage:
%
%   [params,dParams,gof,stddev] =
%       fitChiSquare(xData,yData,modelFun,guess,dxData,dyData)
%
%       Performs a least-squares fit of the parameters of modelFun,
%       minimizing the chi-square between the model and the data, using the
%       specified uncertainties in x and y data.
%
%   ... = fitChiSquare(xData,yData,modelFun,guess,dxData,dyData,options)
%
%       As above, with the specified options.
%
%   ... = fitChiSquare(xData,yData,modelFun,guess)
%
%       Uses empty data uncertainty arrays, sets ErrorsUnknown to true.
%
%   Author:     N. Brahms
%   Contact:    Contact via Matlab File Exchange website
%   Version:    2.5
%   Updated:    5/26/06
%
%   Inputs:
%       xData   - A m x n matrix of independent variable data, where
%                   columns represent independent variables, and rows
%                   represent successive observations thereof.
%       yData   - A length m vector of dependent variable data.
%       dxData  - A m x n matrix of uncertainties corresponding to each
%                   measurement in xData.  May be [] if
%                   Options.ErrorsUnknown = 1.
%       dyData  - A length m vector of uncertainties corresponding to each
%                   measurement in yData.  May be [] blank if
%                   Options.ErrorsUnknown = 1.
%       modelFun- A pointer to a vectorized modeling function:
%
%           [result] = modelFun(params,xData)
%
%           which returns a length m vector of f(x) using the parameters
%           passed.
%
%       guess   - A length k vector of initial guesses for the parameters.
%       options - An options struct to pass to the fitting routine;
%                 this is the same as described in optimset, but with the
%                 following additional fields.  If any of the following
%                 fields are absent from the struct, the default value is
%                 used.  This parameter is optional.
%
%           Display = 'iter' | {'on'} | 'off'
%               Passed to the fitting routine.  If set to 'on' or 'iter',
%               also causes fitChiSquare to display the fitting results in
%               the command window.
%           Plot = {0} | 1
%               Plots the fit result.  If PlotResiduals is also on, the fit
%               result is plotted in subplot [1 2] and the residuals are
%               plotted in subplot 3.
%           PlotVariable = {1} | ... | k
%               Which independent variable to plot against.
%           PlotResiduals = {0} | 1
%               Plots the weighted fit residuals
%           ErrorsUnknown = {0} | 1
%               Set this option to 1 if data uncertainties (dxData or dyData)
%               are unknown. fitChiSquare produces an error if
%               ErrorsUnknown is 0 and either dxData or dyData is empty.
%               If 0, then
%                   chi2 may be interpretred as a statistic indicating the
%                   likelihood that the model is appropriate for describing
%                   the observed data.  See definitions of chi2 and
%                   probChi2 above.  The delta chi2 = 1 bounds (dParams.dl
%                   and dParams.du) may be interpreted as 68% confidence
%                   intervals for the parameters (given that the parameters
%                   are gaussian-distributed).  stddev is given by
%                       stddev = sqrt(dy^2 + sum( (df/dxi*dxi)^2 ) )
%                   where dy and dxi are the data uncertainties and df/dxi
%                   is the finite-difference derivative of the model versus xi.
%               If 1, then
%                   chi2 is set to 1, and neither chi2 nor probChi2 carry
%                   meaning.  The delta chi2 = 1 bounds may be interpreted
%                   as "68% confidence intervals for the parameters given
%                   the model is correct."
%
%               If 1, assumes the model is correct (that is, chi2 is set to 1)
%               and uses the model to calculate the data standard deviation.
%               Assumes standard deviation is uniform by datum.
%               If using ErrorsUnknown, the delta chi2 = 1 bounds for the
%               parameters can only be interpreted as the "68%
%               confidence interval given the model is correct" (and then
%               only if the parameters are gaussian-distributed).
%               If 0, uses measurement errors to calculate the data
%               standard deviation.  Chi2 is a meaningful estimate of model
%               accuracy.  The delta chi2 = 1 bounds may be interpreted as
%               the "68% confidence interval" (if the parameters are
%               gaussian-distributed).
%           FitUncertainty = 0 | {1} | k-vector
%               If 1 (default), fits uncertainties
%               If 0, does not fit uncertainties (returns empty dparams)
%               If length k boolean vector, fits uncertainties only to 'on'
%                 indices.  Example for a three parameter fit:
%                   FitUncertianty = [0 1 1];
%           FitIndex = {ones(size(guess))} | k-vector
%               fitChiSquare only fits each parameter if the corresponding
%               index in FitIndex is true.
%           LowerBound     = vector | {[-Inf ... -Inf]}
%               a length k row vector of parameter lower bounds
%           UpperBound     = vector | {[Inf ... Inf]}
%               a length k row vector of parameter upper bounds
%           DisplayUncVal  = {0} | 1
%               shows additional information when fitting the uncertainties
%           MaxUncIter = {10}
%               maximum number of iterations in uncertainty solver
%           MaxUncCount = {20}
%               maximum number of function calls in uncertainty solver
%           UncOptions  = optimset options |
%                            {optimset('TolFun',1e-2,'Display','off')}
%               optimset options to pass to uncertainty solver (fzero)
%           Scale = {0} | 1
%               If 1, uses parameter scaling to make optimization radix
%               equally sensitive to each parameter.  This is useful if
%               parameters take on values different by more than an order
%               of magnitude (i.e. p = [1 1e10]).  Note that the scaling
%               factor is determined from the guess, so zero guesses are
%               not scaled.
%
%   Outputs:
%       params  - a length k vector of fit model parameters
%       dParams - a length k cell array of structures with the following
%                 fields:
%           d   - equals (dl+du)/2
%           dl  - absolute lower deviation at dChi^2=1 of the indexed
%                 parameter
%           du  - absolute upper deviation at dChi^2=1 of the indexed
%                 parameter
%           lVal- a length k vector of the value of each parameter at the
%                 dChi^2=1 projection point for the lower deviation of the
%                 indexed paramter
%           uVal- a length k vector of the value of each parameter at the
%                 dChi^2=1 projection point for the upper deviation of the
%                 indexed parameter
%       gof     - a goodness-of-fit struct with the following fields:
%           chi2- the chi-square of the fit
%           dof - degrees-of-freedom of the fit
%           eta2- the eta-square of the fit (a.k.a. correlation index) -
%                 this quantity is analogous to the r-square (correlation
%                 coefficient) of a linear least-squares fitting [3]
%           probChi2 - the probability of obtaining a chi-square equal to or
%                      greater than chi2 given the data is drawn from the
%                      model using the best-fit parameters
%       stddev  - the expected deviation from the fit for each observation
%
%   Example:
%   Run and edit fitChiSquareExample for example use.
%
%   Notes:
%   fitChiSquare is a generalized chi-square fitting routine for any
%   model function when data measurement errors are known; it returns the
%   model parameters and their uncertainties at the delta chi-squared = 1
%   boundary (68% confidence interval).  Alternatively, it can be used to
%   determine measurement errors when the theoretical function is known.
%
%   This function uses the optimization toolbox function lsqnonlin to first
%   perform a nonlinear least squares fit of the data to the model.  It
%   then calculates the standard deviation of each point to the fit value,
%   then calculates the chi squared.  Finally, the function fzero is
%   used on each parameter to find the value at which delta chi-squared is
%   equal to 1 while minimizing chi^2 with respect to the other parameters.
%   In the case that the parameter uncertainties are normally distributed,
%   the delta chi^2 = 1 method gives the 68% confidence limit for the
%   parameters.  Monte Carlo or investigations of many data sets should be
%   used to confirm the parameter uncertainties are normally distributed.
%   This method gives upper and lower bounds for each parameter.[1,2]
%
%   If one encounters the following error message:
%
%       Unexpected termination flag 0 in non-estimating variable
%       minimization during uncertainty estimation
%
%   this is because the non-varying parameter minimization routine has
%   encountered its iteration or evaluation limit.  Raise
%   options.UncOptions.MaxFunEvals or options.UncOptions.MaxIter.
%
%   Note:  If the user can not use lsqnonlin (i.e., the optimization
%   toolbox is not installed), the program will use the built-in function
%   fminsearch instead.  This may reduce the robustness and/or speed of
%   the calculation.
%
%   References:
%   1. W.H. Press, B.P. Flannery, S.A. Teukolsky, W.T. Vetterling.
%      Numerical Recipes; The Art of Scientific Computing. (Cambridge
%      University Press: Cambridge). 1986.
%   2. P.R. Bevington, D.K. Robinson. Data Reduction and Error Analysis
%      for the Physical Sciences. (McGraw-Hill: New York). 1992.
%   3. http://mathworld.wolfram.com

%--------------------------------------------------------------------------
%   (c) 2006 N. Brahms
%
%   History
%       1.0     - Initial version
%       1.1     - Changed syntax and added the doError switch
%       1.1.1   - Added more detailed reporting
%       1.2     - Changed parameter uncertainty minimization function from
%                 fminbnd to fminsearch with coded constraints
%       1.3     - Changed non-estimating parameter minimization to occur
%                 inside fminsearch in uncertainty minimization.  Also,
%                 fixed a bug causing improper weighting of initial chi2
%                 minimization.  Removed doError switch.
%       1.3.1   - Minor code rearrangement
%       1.3.2   - Can run without the optimization toolbox
%       1.3.3   - Added UncMaxFunEvals & UncMaxIter options
%       1.3.4   - Fixed a bug whereby the uncertainty estimator would
%                 return bogus values instead of an error if the included
%                 non-varying parameter minimization routine halted
%                 unexpectedly.
%       2.0     - File name change to fitChiSquare.  Added example use
%                 file and included between function in release.  Added
%                 stddev output.
%       2.1     - Added bounds, uncertainty options.  Updated
%                 documentation.
%       2.2     - Added ErrorsUnknown option, cleaned up documentation.
%       2.3     - Added plotting options, changed to allow FitUncertainty
%                 with ErrorsUnknown. Changed dParams to normal array.
%       2.4     - Added gof struct - note syntax change
%       2.4.1   - Fixed bug where struct was misassigned if
%                 FitUncertainty(1) was equal to 0
%       2.4.2   - Added probchi2 field to gof struct
%       2.4.3   - M-Lint optimization
%       2.4.4   - Changed options handling to make code cleaner, added
%                 ability to fit only a variable subspace
%       2.5     - Added Scale option
%       2.5.1   - Help updates, options is now optional :)
%       2.6     - Added 4 parameter syntax
%
%   Known problems / suggested features
%       - Add flags indicating that the fit has stopped at bounds
%
%--------------------------------------------------------------------------

% Uncertainty loop constants and defaults
LARGE_NUM = realmax;    % The number used in the minimization routine when
                        % the minimizing variable is out of bounds

if ~exist('options','var')
    options = optimset;
end
options = initializeOptions(options,guess);
opEPar = initializeUncOptions(options);

if nargin==4
    options.ErrorsUnknown = true;
    dxData = [];
    dyData = [];
end

% Check degrees of freedom
if (length(yData) <= length(guess))
    error('Nonpositive degrees of freedom');
end

% Condition input
[xData, dxData, yData, dyData] = conditionInput(xData, dxData,...
    yData, dyData, ~options.ErrorsUnknown);

% Force bound vectors to be row vectors
options.LowerBound = conditionBound(options.LowerBound);
options.UpperBound = conditionBound(options.UpperBound);

% Scale input for unit radix
scale = ones(size(guess));
if options.Scale
    scale(guess~=0) = abs(guess(guess~=0));
    guess = guess./scale;
    if ~isempty(options.OutputFcn)
        unscaledOutputFcn = options.OutputFcn;
        options.OutputFcn = @(p,ov,s) feval(unscaledOutputFcn,p*scale,ov,s);
    end
end

% Check guess is in bounds
for ia=1:length(guess)
    if any(options.LowerBound>options.UpperBound)
        error('Lower bound exceeds upper bound')
    end
    if ~between(guess(ia)*scale(ia),[options.LowerBound(ia) options.UpperBound(ia)],1)
        error('Guess %d is not in bound',ia);
    end
end

% Nested function variable initialization
pVarIndex = 0;
pVar      = 0;
pFix      = 0;
pEVarVal  = zeros(length(guess)-1,1);
pEVarInd  = zeros(length(guess)-1,1);
pVarBnd   = [-Inf, Inf];
pVarMult  = 1;

if ~strcmp(options.Display,'off')
    disp('Fitting function parameters...');
end

% Calculate the degrees of freedom of the problem
dof = length(yData)-length(guess);

% Calculate the fit with uniform uncertainty
stddev = 1;
% Average deviation from the guess
stddev = sqrt(sum(model(guess).^2)/dof);

if ~options.ErrorsUnknown
    startOptions = options; startOptions.TolFun = 1e-2; startOptions.TolX = 1e-8;
    params = trylsq(@model,guess,[],[],startOptions);
    % Calculate the standard deviation for each datum
    stddev = calcStdDev;
    % Calculate the least-squares fit
    params = trylsq(@model,params,[],[],options);
else
    % Calculate the least-squares fit
    params = trylsq(@model,guess,[],[],options);
    stddev = 1;
    stddev = sqrt(sum(model(params).^2)/dof)*ones(length(yData),1);
end

% Calculate the chi-square function
chiSquare = sum( model(params).^2 );

% Fill goodness-of-fit struct
gof.chi2 = chiSquare;
gof.dof  = dof;
gof.eta2 = (std( (yData-model(params).*stddev) ) / std( yData) )^2;
gof.probChi2 = chi2prob(chiSquare,dof);

% Fits parameter uncertainties
% Be careful not to use ia in any nested functions!
dParams = initializeDParams;
dParams(1:length(params)) = dParams;
for ia=1:length(params)
    if options.FitUncertainty(ia)
        dParams(ia)=delta(ia);
    end
end

% Rescale data back from unity
params = params.*scale;
for ia=1:length(dParams)
    dParams(ia).du = dParams(ia).du*scale(ia);
    dParams(ia).dl = dParams(ia).dl*scale(ia);
    dParams(ia).d  = dParams(ia).d*scale(ia);
    dParams(ia).lVal = dParams(ia).lVal*scale(ia);
    dParams(ia).uVal = dParams(ia).uVal*scale(ia);
end

% If a verbose mode is on, display the answer and reduced chi-squared
if ~strcmp(options.Display,'off')
    for id=1:length(params)
        disp(sprintf('Parameter %d = %g + %g - %g',...
            id,params(id),dParams(id).du,dParams(id).dl));
    end
    disp(sprintf('Reduced chi-square = %f',chiSquare/dof));
end

% Plot the fit
if options.Plot
    % Subplot if also plotting residuals
    if options.PlotResiduals
        subplot(3,1,[1 2]);
    end
    holdstate = ishold;
    errorbar(xData(:,options.PlotVariable),yData,stddev,'.');
    plotHandle = gca;
    hold on
    plot(xData(:,options.PlotVariable),...
        yData - model(params./scale).*stddev,'-r');
    switch holdstate
        case 0
            hold off
    end
    xlabel('x1');
    ylabel('y');
    title('Fit plot');
    lstring = sprintf('Fit: ~\\chi^2 = %0.2g\n',chiSquare/dof);
    for ia = 1:length(params);
        lstring = [lstring ...
            sprintf('Par. %d = %0.2g + %0.1g - %0.1g\n',...
            ia,params(ia),dParams(ia).du,dParams(ia).dl)];
    end
    legend({sprintf('Data \\pm \\sigma'),lstring(1:end-1)});
        
end

% Plots the weighted fit residual
if options.PlotResiduals
    if options.Plot
        subplot(3,1,3);
    end
    holdstate = ishold;
    plot(xData(:,options.PlotVariable),model(params./scale),'.');
    hold on
    line([min(xData(:,options.PlotVariable)) max(xData(:,options.PlotVariable))],...
        [0 0],'Color',[1 0 0]);
    switch holdstate
        case 0
            hold off
    end
    xlabel('x1');
    ylabel('Residual');
    title('Weighted residual plot');
    if options.Plot
        set(gca,'XLim',get(plotHandle,'XLim'));
    end
end

if options.Plot || options.PlotResiduals
    drawnow
end
% Last executing line of fitChiSquare    

% --- Internal functions follow ------------------------------------------

    % ----------- model --------------------------------------------------

    % This calls the model function.  It returns the residual at each
    % datum.
    function F = model(x)
        
        % If we scaled to unit radix, we must scale back before calling the
        % model.  Since scale was initialized to ones, we just multiply in
        % any case.
        x = x.*scale;
        
        % Handle boundary
        testInBound = between(x,[options.LowerBound; options.UpperBound],1);
        
        if ~all(testInBound)
            % Return ridiculously large number
            F = sqrt(LARGE_NUM)/(length(x)+1)*ones(length(xData),1);
        else
            % Return the chi-value
            F = (yData - feval(modelFun,x,xData))./(stddev+eps); 
            %eps to avoid problems with zero weights
        end
    end

    % ----------- modelError ---------------------------------------------

    % Used in finding the point-by-point standard deviation
    function F = modelError(x,xbndData)   
        % See model
        x = x.*scale;
        
        F = feval(modelFun,x,xbndData);
    end


    % ---------- calcStdDev ------------------------------------------------

    % Calculates the std. dev. of (f(x_i)-y_i) using finite methods

    function sigma = calcStdDev

        dim = size(xData);
        dm2 = zeros(dim(1),1);
        for ib=1:dim(2)
            xtestu = xData;
            xtestl = xData;
            xtestu(:,ib) = xtestu(:,ib)+dxData(:,ib);
            xtestl(:,ib) = xtestl(:,ib)-dxData(:,ib);
            dmodelu = modelError(params,xtestu);
            dmodell = modelError(params,xtestl);
            dm2 = dm2 + (dmodelu-dmodell).^2/4;
        end
        sigma = sqrt(dyData.^2+dm2);

    end

    %-------------------- fzeroOutput ---------------------------
    % Used with fzero to stop when UncIter is exceeded
    function stop = fzeroOutput(x,o)
        
        if o.iteration > options.MaxUncIter || o.funccount > options.MaxUncCount
            stop = 1;
        else
            stop = 0;
        end
            
        % Display iterative function information
        if options.DisplayUncVal
            str1 = sprintf('Iteration = %d\tVarA = %6.4g\tVarB = %6.4g\tFValA = %6.4g\tFValB = %6.4g\tEVal = ',...
                o.iteration,o.intervala,o.intervalb,o.fvala,o.fvalb);
            str2 = sprintf('%g\t',pEVarVal);
            disp([str1 str2]);
        end
        
    end


    %----------------------- modelFix ---------------------------

    % This function is used to fit only one variable to delta chi2 = 1.  It
    % is used with fzero, not lsqnonlin.
    function F = modelFix(var)
        
        if between(var,pVarBnd,1)
            
            pVar = var;
            if length(params)>1
                [pEVarVal,eflag] = trylsq(@modelNFix,pEVarVal,[],[],opEPar);
                if(eflag<=0)
                    disp(sprintf(['\tUnexpected termination flag %d in'...
                        ' non-estimating variable minimization\n\tduring'...
                        ' uncertainty estimation'],eflag));
                    F=NaN;
                else
                    x = reconstruct(pVar);
                    F = (chiSquare+1) - sum( model(x).^2 );
                end
            else
                F = (chiSquare+1) - sum( model(var).^2 );
            end
            
        % If past the variable-side bound, make F large.  If past the
        % extent-side bound, make F infinitely negative.
        elseif var<min(pVarBnd)
            F = pVarMult*LARGE_NUM;
            eflag = 1;            
        elseif var>max(pVarBnd)
            F = -pVarMult*LARGE_NUM;
            eflag = 1;
        end
    end % modelFix

    % ------------ reconstruct -------------------------------------------
    % Builds the parameter array from pEVar and pVar

    function x = reconstruct(in)
        x = zeros(size(params));
        for ic=1:length(pEVarInd)
           x(pEVarInd(ic)) = pEVarVal(ic);
        end
        x(pVarIndex) = in;
    end

    % ------------ modelNFix ---------------------------------------------
    % This function is used to fit all but one variable
    function F = modelNFix(x)
        xu = zeros(size(params));
        for ig=1:length(pEVarInd)
            xu(pEVarInd(ig)) = x(ig);
        end
        xu(pVarIndex) = pVar;
        F = model(xu);
    end

    % ------------ delta -------------------------------------------------
    % This function finds the variation to delta chi-squared = 1
    % One variable minimizes chi-square+1, while the other variables minimize
    % chi-squared
    function bnd = delta(varyingIndex)
        
        bnd = initializeDParams;
        
        % These are the shared variables for the fitting functions
        pVarIndex = varyingIndex;   % The index of the variable being solved
        
        j=1;                        % Load all the other variables in an array
        for ie=1:length(params)
            if ie~=varyingIndex
                pEVarVal(j)=params(ie);
                pEVarInd(j)=ie;
                j=j+1;
            end
        end
        pEVarValStatic = pEVarVal;  % The non-varying initial array value
        
        pVar = params(pVarIndex);   % The solution is stored here, but must
                                    % start at the minimized value
        
        % This is the original variable value
        pFix = params(pVarIndex);
        
        if ~strcmp(options.Display,'off')
            disp(sprintf('Finding parameter %d lower bound',varyingIndex));
        end

        % Calculate the lower bound
        [bnd.dl,bnd.lVal] = uncMin('lb');
        bnd.dl = pFix-bnd.dl;
        
        % Here we have to reinitiate the variables
        pEVarVal = pEVarValStatic;
        pVar = params(pVarIndex);
        
        % Calculate the upper bound
        if ~strcmp(options.Display,'off')
            disp(sprintf('Finding parameter %d upper bound',varyingIndex));
        end
        
        [bnd.du,bnd.uVal] = uncMin('ub');
        bnd.du = bnd.du-pFix;
        
        bnd.d = mean([bnd.du bnd.dl]);
        
        % This function finds the parameter deviation for delta chi-squared
        % = 1, and then reminimizes the other variables.
        function [bnd,val] = uncMin(lbub)
            
            % Set the search bounds on the uncertainty
            switch(lbub)
                case 'ub'
                    pVarBnd = [pFix, options.UpperBound(varyingIndex)]; A=1;
                case 'lb'
                    pVarBnd = [options.LowerBound(varyingIndex), pFix]; A=-1;
            end
            pVarMult = A;
            
            opParDev = initializeUncOptions(options);
            opParDev.OutputFcn = @(x,o,s) fzeroOutput(x,o);
            % Find the parameter deviation to get delta chi2 = 1
%             [bnd,fval,exitflag] = fzero(@modelFix,pFix,opParDev);
            bnd = fzero(@modelFix,pFix,opParDev);
            
            % Assign the result
            val = reconstruct(bnd);
                
            if options.DisplayUncVal
                disp(sprintf('Fix = %g\tVar = %g\tChi2 = %g',...
                    pFix,pVar,chiSquare));
            end
        end % uncMin

    end % delta

end % fitChiSquare

% ----- initializeUncOptions ------------------------------------------

function uncOp = initializeUncOptions(options)

% Set a relatively high finishing tolerance
uncOp = options.UncOptions;

end %initializeUncOptions
    
% ----- initializeOptions ---------------------------------------------

function options = initializeOptions(optionsIn,guess)

% Manufacture the fit options defaults
options = optimset;
options.Plot = false;
options.PlotResiduals = false;
options.PlotVariable = true;
options.FitUncertainty = true(1,length(guess));
options.FitIndex = true(1,length(guess));
options.ErrorsUnknown = false(1,length(guess));
options.UncOptions=optimset('TolFun',1e-2,'Display','off');
options.DisplayUncVal=false;
options.LowerBound=-Inf*ones(1,length(guess));
options.UpperBound=Inf*ones(1,length(guess));
options.Scale = false;
options.MaxUncIter = 10;
options.MaxUncCount = 20;

names = fieldnames(options);

for i = 1:length(names)
    if isfield(optionsIn,names{i})
        if length(optionsIn.(names{i})) == 1 &&...
                length(options.(names{i})) == length(guess) &&...
                length(guess)>1
            optionsIn.(names{i}) = optionsIn.(names{i})*ones(1,length(guess));
        end
        if islogical(options.(names{i}))
            options.(names{i}) = convertBoolean(optionsIn.(names{i}));
        else
            options.(names{i}) = optionsIn.(names{i});
        end %if islogical
    end %if isfield
end %for

end
    
% ----- chi2fun ----------------------------------------------------------

% Returns the probability of chi-square equal to or greater than x, with r
% degrees of freedom.  This is 1-D(x,r), where D is the cumulative
% distribution function.  See
% http://mathworld.wolfram.com/Chi-SquaredDistribution.html
% Note that MATLAB's gammainc is the "regularized gamma function" used in
% MathWorld.
function p = chi2prob(x,r)

p = gammainc(r/2,x/2);

end

% ----- trylsq ------------------------------------------------------------

% Uses lsqnonlin if it exists
function [sol,exitflag] = trylsq(fun,x0,lb,ub,options)

    if(exist('lsqnonlin','file'))
        [sol,trash,trash,exitflag] = lsqnonlin(fun,x0,lb,ub,options);
    else
        [sol,trash,exitflag] = fminsearch(@modelChi2,x0,options);
    end 

    % For use when substituting fminsearch for lsqnonlin
    function F = modelChi2(x)
        F = sum(feval(fun,x).^2);
    end

end

% ----- between -----------------------------------------------------------

% Returns the portion of vector in and out of the range ( as determined by
% the inclusive tag )
function [inner,outer] = between(vector,range,inclusive)

    if ~exist('inclusive','var')
        inclusive = 0;
    end

    if inclusive
        inner = vector<=max(range) & vector>=min(range);
        outer = vector<=min(range) & vector>=max(range);
    else
        inner = vector<max(range) & vector>min(range);
        outer = vector<min(range) & vector>max(range);
    end
    
end

% ----- conditionInput ----------------------------------------------------

function [xData, dxData, yData, dyData] = conditionInput(xin, dxin, yin, dyin,...
    doError)

xData = xin; dxData = dxin; yData = yin; dyData = dyin;

% Force yData and dyData to be column vectors
siy = size(yData);
if siy(1)>1 && siy(2)>1
    error('Y is not a vector');
end
yData = yData(:);
siy = size(yData);

if doError
    sidy = size(dyData);
    if sidy(1)>1 && sidy(2)>1
        error('dY is not a vector');
    end
    dyData = dyData(:);
    sidy = size(yData);
end

% Transpose xData if it is n x m, not m x n
six = size(xData);
if (siy(1) == six(2) && siy(1) ~= six(1))
    xData = xData';
    six = size(xData);
end
if doError
    sidx = size(dxData);
    if (siy(1) == sidx(2) && siy(1) ~= sidx(1))
        dxData = dxData';
        sidx = size(dxData);
    end
end
% Check that vectors match
if (siy(1) ~= six(1))
    error('X and Y data vectors do not match.')
end
if doError
    % Grow error vectors if the user passed scalar errors
    if sidx(1) == 1 && sidx(2) == six(2)
        for i=1:six(2)
            dxData(2:length(xData(:,1)),i)=dxData(1,i);
        end
        sidx = size(dxData);
    end
    if sidy(1) == 1 && sidy(2) == 1
        dyData(2:length(yData(:,1)),1)=dyData(1);
        sidy = size(dyData);
    end
    if any((sidx ~= six) | (sidy ~= siy))
        error('Data vectors do not match error vectors');
    end
end
end

% ----- initializeDParams -------------------------------------------------

function dParamsEmpty = initializeDParams()

dParamsEmpty = struct('dl', NaN, 'lVal', [], 'du', NaN, 'uVal', [],...
    'd',NaN);

end

% ----- conditionBound ----------------------------------------------------

function boundVecOut = conditionBound(boundVecIn)

sib = size(boundVecIn);
if (sib(1)~=1 && sib(2)==1)
    boundVecOut = boundVecIn';
elseif (sib(1)~=1)
    error('Badly formed bound vector');
else
    boundVecOut = boundVecIn;
end
end


% ----- convertBoolean ----------------------------------------------------

% Converts vectors of {>0, <=0} or {'on', 'off'} to
% vectors of {1, 0}

function value = convertBoolean(in)

warning off MATLAB:sprintf:InputForPercentSIsNotOfClassChar;
errstr = ['Input vector' sprintf(' %s',in)...
            ' can not be converted to a boolean value'];
warning on MATLAB:sprintf:InputForPercentSIsNotOfClassChar;

% Handles cell arrays
if iscell(in)
    % All cells are numbers
    if all(isnumeric(cell2mat(in)))
        value = cell2mat(in) > 0;
    elseif islogical(cell2mat(in))
        value = cell2mat(in);
    % All cells are 'on' or not 'on'
    elseif iscellstr('in');
        in = lower(in);
        for i=1:length(in)
            switch in{i}
                case 'on'
                    value(i)=true;
                case 'off'
                    value(i)=false;
                otherwise
                    error(errstr);
            end %switch
        end %for
    else
        error(errstr);
    end %if number or string
% Array of numbers or booleans, or single string
else
    if all(isnumeric(in))
        value = in > 0;
    elseif all(islogical(in))
        value = in;
    elseif isa(in,'char') && all(isstrprop(in,'alpha'));
        switch lower(in)
            case 'on'
                value=true;
            case 'off'
                value=false;
            case 'true'
                value=true;
            case 'false'
                value=false;
            otherwise
                error(errstr);
        end %switch
    else
        error(errstr);
    end %if number or string
end %if cell

end %convertBoolean