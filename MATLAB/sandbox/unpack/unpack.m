clear;
clc;

fileID = fopen('CeO2_042713_002.mar3450','r');
cimg = fread(fileID, 'uchar');

header = cimg(1:4096);

% create array for 16 32-bit integers
parameter = int32(zeros(16,1));

% read experimental parameters
n = 0;
for i = 1:16
    parameter(i) = byteToInt(header(n + 1), header(n + 2), ...
        header(n + 3), header(n + 4));
    n = n + 4;
end

% check file
if parameter(1) == 1234
    disp('File OK!');
end

% get size of image
xs = parameter(2);
ys = xs;

% high intensity pixels
numHighIntensity = parameter(3);

% read keywords
keywords = char(header(129:end));

% get high intensity pixels
if numHighIntensity > 0
    disp('High intensity pixels found!');
    highIntensityPixels = size(numHighIntensity, 2);
    n = 0;
    for i = 4097:(4097 + 8*numHighIntensity)
        highIntensityPixels(n, 1) = byteToInt(cimg(i), cimg(i + 1), ...
            cimg(i + 2) + cimg(i + 3));     % locations of pixels
        highIntensityPixels(n, 2) = byteToInt(cimg(i + 4), cimg(i + 5), ...
            cimg(i + 6), cimg(i + 7));      % intensity values
        n = n + 8;
    end
end

img = zeros(xs*ys, 1);

% note this is a hack! making sure cimg starts after CCP4 identifier
cimg = cimg(4134:end);

% unpack i2
valids = 0;
spillbits = 0;
total = xs * ys;
window = 0;
pixel = 0;
x = 3450;
y = 3450;
bitdecode = [0, 4, 5, 6, 7, 8, 16, 32];
setbits = hex2dec(['00000000'; '00000001'; '00000003'; '00000007'; ...
    '0000000F'; '0000001F'; '0000003F'; '0000007F'; '000000FF'; ...
    '000001FF'; '000003FF'; '000007FF'; '00000FFF'; '00001FFF'; ...
    '00003FFF'; '00007FFF'; '0000FFFF'; '0001FFFF'; '0003FFFF'; ...
    '0007FFFF'; '000FFFFF'; '001FFFFF'; '003FFFFF'; '007FFFFF'; ...
    '00FFFFFF'; '01FFFFFF'; '03FFFFFF'; '07FFFFFF'; '0FFFFFFF'; ...
    '1FFFFFFF'; '3FFFFFFF'; '7FFFFFFF'; 'FFFFFFFF']);

i = 1;
progress = 0;
while pixel < total
    
    new_progress = round( i/length(cimg) * 100);
    if new_progress > progress
        progress = new_progress;
        str = sprintf('Progress: %1d%%', progress);
        disp(str);
    end
    
    if valids < 6
        if spillbits > 0
            window = bitor(window, shift_left(spill, valids));
            valids = valids + spillbits;
            spillbits = 0;
        else
            spill = cimg(i);
            i = i + 1;
            spillbits = 8;
        end
    else
        pixnum = bitshift(1, bitand(window, setbits(4)));
        window = shift_right(window, 3);
        bitnum = bitdecode(bitand(window, setbits(4)) + 1);
        window = shift_right(window, 3);
        valids = valids - 6;
        while (pixnum > 0) && (pixel < total)
            if valids < bitnum
                if spillbits > 0
                    window = bitor(window, shift_left(spill, valids));
                    if (32 - valids) > spillbits
                        valids = valids + spillbits;
                        spillbits = 0;
                    else
                        usedbits = 32 - valids;
                        spill = shift_right(spill, usedbits);
                        spillbits = spillbits - usedbits;
                        valids = 32;
                    end
                else
                    spill = cimg(i);
                    i = i + 1;
                    spillbits = 8;
                end
            else
                pixnum = pixnum - 1;
                
                if bitnum == 0
                    nextint = 0;
                else
                    nextint = bitand(window, setbits(bitnum + 1));
                    valids = valids - bitnum;
                    window = shift_right(window, bitnum);
            
                    if bitand(nextint, bitshift(1, bitnum - 1)) ~= 0
                        nextint = bitor(nextint, bitcmp(setbits(bitnum + 1), 'int32'), 'int32');
                    end
                end
                        
                    if pixel > x
                        img(pixel + 1) = nextint + (img(pixel) + ...
                            img(pixel - x + 2) + img(pixel - x + 1) + ...
                            img(pixel - x) + 2) / 4;
                        pixel = pixel + 1;
                    elseif pixel ~= 0 
                        img(pixel) = img(pixel) + nextint;
                        pixel = pixel + 1;
                    else
                        img(pixel + 1) = nextint;
                        pixel = pixel + 1;
                    end
            end
        end
    end
end

img = int32(reshape(img, [3450 3450]));