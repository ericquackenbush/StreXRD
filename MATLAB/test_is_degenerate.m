% Test is_degenerate

a = -100;
b = 100;
value = 0;

while (value == 0)
    M = (b-a)*rand(5,2) + a;
    [A, B, C, D, E, F] = conic_coeffs(M);
    [value, detM] = is_degenerate(A, B, C, D, E, F);
end