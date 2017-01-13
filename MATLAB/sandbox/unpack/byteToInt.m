function [a] = byteToInt(b1, b2, b3, b4)
    a = b1 + b2*256 + b3*256*256 + b4*256*256*256;
end