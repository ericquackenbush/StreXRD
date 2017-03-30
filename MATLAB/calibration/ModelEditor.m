function varargout = ModelEditor(varargin)
% Model editor for use with FitChiTool
%
% To get started, run FitChiToolExample

% Edit the above text to modify the response to help ModelEditor

% Last Modified by GUIDE v2.5 14-Feb-2006 17:48:23

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @ModelEditor_OpeningFcn, ...
                   'gui_OutputFcn',  @ModelEditor_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
               
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before ModelEditor is made visible.
function ModelEditor_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to ModelEditor (see VARARGIN)

% Choose default command line output for ModelEditor
handles.output = hObject;
handles.models = []; % Data structure

drawnow

if nargin>3 && ischar(varargin{1})
    handles = load_file(handles,varargin{1});
end

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes ModelEditor wait for user response (see UIRESUME)
% uiwait(handles.modeleditor_gui);


% --- Outputs from this function are returned to the command line.
function varargout = ModelEditor_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);

% Get default command line output from handles structure
varargout{1} = handles.output;

%=========================================================================

% --- "Add group" button
function pushbutton1_Callback(hObject, eventdata, handles)

i = get(handles.popupmenu1,'Value')-1; % Group number

if ~isempty(handles.models)
    numGroups = length(handles.models);
    groupNames = {handles.models.group};
else
    numGroups = 0;
    groupNames = {};
end

answercell = inputdlg('Enter a new group name:','Add group');
answer = answercell{1};
if ischar(answer)
    if ~any(strcmp(answer,groupNames))
        switch numGroups-i
            case 0
            otherwise
                handles.models(i+2:numGroups+1) = handles.models(i+1:end);
        end
        handles.models(i+1).group = answer;
        handles.models(i+1).children = [];
        handles = fillgroups(handles);
        set(handles.popupmenu1,'Value',i+2);
        popupmenu1_Callback(handles.popupmenu1,eventdata,handles);
        set(handles.file_new_menu,'Enable','on');
    else
        errordlg('Group already exists');
    end
else
    errordlg('Group name must be a character string');
end

guidata(hObject,handles);
        


% --- "Delete group" button
function pushbutton2_Callback(hObject, eventdata, handles)

i = get(handles.popupmenu1,'Value')-1;
str = get(handles.popupmenu1,'String');
nomodels = 0;
switch i
    case 0
    case 1
        if length(handles.models)>1
            handles.models = handles.models(2:end);
        else
            handles.models = [];
            nomodels = 1;
        end
    case length(str)
        handles.models = handles.models(1:end-1);
    otherwise
        handles.models = handles.models([1:i-1 i+1:end]);
end
if i>0
    set(handles.popupmenu1,'Value',i);
end
if ~nomodels
    handles = fillgroups(handles);
    popupmenu1_Callback(handles.popupmenu1,eventdata,handles);
else
    set(handles.file_new_menu,'Enable','off');
    set(handles.popupmenu1,'Value',1);
    set(handles.popupmenu1,'String',{'<open a file or add a group>'});    
    popupmenu1_Callback(handles.popupmenu1,eventdata,handles);    
end
guidata(hObject,handles)


% --- "Groups" pop-up
function popupmenu1_Callback(hObject, eventdata, handles)
handleset = [handles.pushbutton3 handles.pushbutton4 handles.pushbutton6...
    handles.edit1 handles.edit2 handles.edit3];
set(handleset(4:6),'String',''); % Empty edits
set(handleset,'Enable','off');    % Disable model editing tools
set(handles.popupmenu2,'Value',1);  % Set model to select
if get(hObject,'Value') ~= 1        % Fill model pop-up
    handles = fillmodels(handles);  
    set([handles.pushbutton2 handleset(1)],'Enable','on');
else
    set(handles.popupmenu2,'String',{'<select a group>'});
    set(handles.pushbutton2,'Enable','off');
end
guidata(hObject,handles);


% --- "Groups" pop-up
function popupmenu1_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Fills groups menu with group names

function handles = fillgroups(handlesIn)

handles = handlesIn;
groupNames = {handles.models.group};
groupOpts = ['<select a group>' groupNames];
set(handles.popupmenu1,'String',groupOpts);




% --- "Models" pop-up
function popupmenu2_Callback(hObject, eventdata, handles)
i = get(handles.popupmenu1,'Value');
j = get(handles.popupmenu2,'Value');

handleset = [handles.pushbutton3 handles.pushbutton4 handles.pushbutton6 ...
        handles.edit1 handles.edit2 handles.edit3];

if j ~= 1
    set(handles.edit1,'String',handles.models(i-1).children(j-1).desc);
    set(handles.edit2,'String',handles.models(i-1).children(j-1).model);
    set(handles.edit3,'String',handles.models(i-1).children(j-1).length);
    set(handleset,'Enable','on');
else
    set(handleset([2:6]),'Enable','off');
    set(handleset(4:6),'String','');
end
guidata(hObject,handles);


% --- Executes during object creation, after setting all properties.
function popupmenu2_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Fills models menu with model names

function handles = fillmodels(handlesIn)

handles = handlesIn;
i = get(handles.popupmenu1,'Value')-1;
if ~isempty(handles.models(i).children)
    modelNames = {handles.models(i).children.name};
    modelOpts = ['<select a model>' modelNames];
else
    modelOpts = {'<add a model>'};
end
set(handles.popupmenu2,'String',modelOpts);
set(handles.popupmenu2,'Value',1);



% --- "Add model" button
function pushbutton3_Callback(hObject, eventdata, handles)

i = get(handles.popupmenu1,'Value')-1;  % group number
j = get(handles.popupmenu2,'Value')-1;  % model number

if i==0
    errordlg('You must choose or create a model first');
else
    numModels = length(handles.models(i).children);
    nomodels = 0;
    if isempty(handles.models(i).children)
        nomodels = 1;
    else
        modelNames = {handles.models(i).children.name};
    end
    
    answercell = inputdlg('Enter a new model name:','Add model');
    answer = answercell{1};
    makemodels=1;
    if ischar(answer)
        if nomodels
        elseif ~any(strcmp(answer,modelNames))
            switch numModels-j
                case 0
                otherwise
                    handles.models(i).children(j+2:numModels+1) =...
                        handles.models(i).children(j+1:end);
            end
        else
            errordlg(sprintf('Model ''%s'' already exists in this group',answer));
            makemodels=0;
        end
    else
        errordlg('Model name must be a character string');
        makemodels=0;
    end
    if makemodels
        handles.models(i).children(j+1).name = answer;
        handles.models(i).children(j+1).desc = 'Type your description here';
        handles.models(i).children(j+1).model = '@(p,x) 0';
        handles.models(i).children(j+1).length = '@(x) 0';
        popupmenu1_Callback(handles.popupmenu1,eventdata,handles);
        set(handles.popupmenu2,'Value',j+2);
        popupmenu2_Callback(handles.popupmenu2,eventdata,handles);
    end
end
guidata(hObject,handles);

% --- "Delete model" button
function pushbutton4_Callback(hObject, eventdata, handles)

i = get(handles.popupmenu1,'Value')-1;
j = get(handles.popupmenu2,'Value')-1;
nummodels = length(handles.models(i).children);
nomodels = 0;
switch j
    case 0
    case 1
        if length(handles.models(i).children)>1
            handles.models(i).children = handles.models(i).children(2:end);
        else
            handles.models(i).children = [];
            nomodels = 1;
        end
    case nummodels
        handles.models(i).children = handles.models(i).children(1:end-1);
    otherwise
        handles.models(i).children = handles.models(i).children([1:i-1 i+1:end]);
end
if i>0
    set(handles.popupmenu2,'Value',i);
end
if ~nomodels
    handles = fillmodels(handles);
else
    set(handles.popupmenu2,'Value',1);
    set(handles.popupmenu2,'String',{'<add a model>'});        
end
popupmenu2_Callback(handles.popupmenu2,eventdata,handles);
guidata(hObject,handles)

% --- "Description" edit
function edit1_Callback(hObject, eventdata, handles)

function edit1_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- "Model" edit
function edit2_Callback(hObject, eventdata, handles)

function edit2_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- "Update model" button
function pushbutton6_Callback(hObject, eventdata, handles)

i = get(handles.popupmenu1,'Value');
j = get(handles.popupmenu2,'Value');

descstr = get(handles.edit1,'String');
modelstr = get(handles.edit2,'String');
lengthstr = get(handles.edit3,'String');

try
    eval(['model = ' modelstr ';']);
    eval(['lmodel = ' lengthstr ';']);
    descstr = convertDescription(descstr);
    handles.models(i-1).children(j-1).desc = descstr;
    handles.models(i-1).children(j-1).model = modelstr;
    handles.models(i-1).children(j-1).length = lengthstr;
catch
    str = lasterr;
    errordlg(str);
end
guidata(hObject,handles);


% Converts the description edit 2-dimensional character array to a single
% string.
function string = convertDescription(stringIn)
string = stringIn; sis = size(string);
if ~(sis(1)==1 && sis(2)==1)
    string = '';
    for i=1:sis(1)
        % Write line i to the output
        string = [string deblank(stringIn(i,:)) sprintf('\n')];
    end
    % Shave last newline
    string = string(1:end-1);
end
        

% --------------------------------------------------------------------
function file_menu_Callback(hObject, eventdata, handles)


% --------------------------------------------------------------------
function file_open_menu_Callback(hObject, eventdata, handles)

[file,path] = uigetfile('*.mat','Choose library file...');

if any(file~=0)
    handles.path = path; handles.file = file;
    handles = load_file(handles,[handles.path handles.file]);
    guidata(hObject,handles);
end

function handles = load_file(handlesIn, path)

handles = handlesIn;

data = load(path,'models');

handles.models = data.models;
handles = fillgroups(handles);
set(handles.popupmenu1,'Value',1);
popupmenu1_Callback(handles.popupmenu1,[],handles);
set(handles.file_save_menu,'Enable','on');
set(handles.file_save_as_menu,'Enable','on');
set(handles.file_new_menu,'Enable','on');
set(handles.pushbutton1,'Enable','on');
set(handles.pushbutton2,'Enable','on');


% --------------------------------------------------------------------
function file_save_menu_Callback(hObject, eventdata, handles)
try
    models = handles.models;
    try
        save([handles.path handles.file],'models');
    catch
        models = handles.models;
        [file path] = uiputfile('*.mat','Choose save location...');
        
        if any(file~=0)
            handles.file = file; handles.path = path;
            save([path file],'models');
            guidata(hObject,handles);
        end
    end
catch
end

% --------------------------------------------------------------------
function file_save_as_menu_Callback(hObject, eventdata, handles)
try
    models = handles.models;
    [file path] = uiputfile('*.mat','Choose save location...');

    if any(file~=0)
        handles.file = file; handles.path = path;
        save([path file],'models');
        guidata(hObject,handles);
    end
catch
end



% --------------------------------------------------------------------
function edit_menu_Callback(hObject, eventdata, handles)


% --------------------------------------------------------------------
function edit_cut_menu_Callback(hObject, eventdata, handles)


% --------------------------------------------------------------------
function edit_copy_menu_Callback(hObject, eventdata, handles)


% --------------------------------------------------------------------
function edit_paste_menu_Callback(hObject, eventdata, handles)


% --------------------------------------------------------------------
function file_new_menu_Callback(hObject, eventdata, handles)

handles.models = [];
handleset = [handles.file_save_menu handles.file_save_as_menu];
set(handleset,'Enable','off');
popupmenu1_Callback(handles.popupmenu1,eventdata,handles);
pushbutton1_Callback(handles.pushbutton1,eventdata,handles);


% --- "Guess length" edit -----------

function edit3_Callback(hObject, eventdata, handles)


% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

