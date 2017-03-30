function varargout = fitLibrary(name,file)
%
% model = fitLibrary(name)
%
% Returns a model function handle from the fit library.  "name" is the name
% of a library function.  Run fitLibrary with no inputs to display a list
% of all library functions, or a group name to display all functions within
% a group.
%
% model - a struct with the following fields
%   modelFun - a string of the anonymous model function
%   lengthFun - a string of an anonymous parameter length function
%   desc  - model description
%
% Groups:
%   poly
%   exp
%   thermal

LIBRARY_NAME = 'defaultChiSquareModels.mat';

if ~exist('file','var');
    file = LIBRARY_NAME;
end
load(file);
groupNames = {models.group};

for i=1:length(models)
    for j=1:length(models(i).children)
        childNames{j,i}=models(i).children(j).name;
    end
end

% Display all models if nothing entered
if ~exist('name','var')
    disp(sprintf('Note: all models are vectorized\n'));
    for i=1:length(models)
        dispGroup(groupNames{i},models)
        disp(sprintf(' '));
    end
% Display all groups if 'groups' is entered
elseif strcmp(name,'groups')
    dispstr = '';
    for i=1:length(models)
        dispstr = [dispstr sprintf('''%s''\n',groupNames{i})];
    end
    disp([sprintf('Groups in library are:\n-----------------------\n')...
        dispstr]);
% Display the model or group
else
    ind = find(strcmp(childNames,name));
    groupind = find(strcmp(groupNames,name));

    % Model name is not found
    if isempty(ind)
        % Name is not a group
        if isempty(groupind)
            error('Model or group ''%s'' not found in library',...
                name);
            % If name is a group, print the models in the group
        else
            dispGroup(name,models)
        end
    else
        [i, j] = ind2sub(size(childNames),ind);
        model = models(j).children(i);
    end
end

if nargout>0
    if exist('model')
        varargout{1} = model;
    else
        error('fitLibrary produces no output when called on groups or on no input');
    end
end

end

%======= Internal functions ======================================== 

%------- dispGroup -------------------------------------------------

function dispGroup(groupName,models)
disp(sprintf(['Group ''%s'' contains the following models:\n'...
    '-------------------------------------------------------'],...
    groupName));
groups = {models.group};
groupind = find(strcmp(groups,groupName));
for i=1:length(models(groupind).children)
    disp([sprintf('''%s'': ',models(groupind).children(i).name)...
        models(groupind).children(i).desc])
end
end
