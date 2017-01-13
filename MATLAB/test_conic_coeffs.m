% Unit test for conic_coeffs function
% Generate random 5 x 2 matrices
% Look for 5 x 1 numeric matrix as output

N = 10000;   % number of iterations
a = -100;
b = 100;
error_flag = 0;

for i = 1:N
    M = (b-a)*rand(5,2) + a;
    [A, B, C, D, E, F] = conic_coeffs(M);
    if (~isreal(A) || ~isreal(B) || ~isreal(C) || ~isreal(D) || ... 
            ~isreal(E) || ~isreal(F))
        error_flag = 1;
    elseif (isnan(A) || isnan(B) || isnan(C) || isnan(D) || isnan(E) || ...
            isnan(F))
        error_flag = 1;
    end
end

if (error_flag == 0)
    disp('Unit test succeeded')
else
    disp('Unit test failed')
end