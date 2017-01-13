function out = removerows(in1, in2)

A = in1;
b = in2;
A(b,:) = [];
out = A;