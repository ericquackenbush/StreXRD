function [zeta,eta]=xy2el(x,y,a,b)
%-------------------------------------------------------------------------
% Conversion from Cartesian mesh (x,y) to elliptic coordinates (zeta,eta)
% zeta (>=0), eta [0 2pi)
% a -- semi-major axis
% b -- semi-minor axis
% c -- elliptic eccentricity
% (x,y,zeta,eta) same-size matrix
% Author: Che Sun (CAS, 2016)
% Source: http://eprint.las.ac.cn/abs/201611.00721
%-------------------------------------------------------------------------
c2=(a^2-b^2);c=2*c2;
x2=x.^2; y2=y.^2;
B=x2+y2-c2;
del2=B.^2+2*c*y2; del=sqrt(del2);
q=(-B+del)/c; q=sqrt(q); et0=asin(q);
eta=x;
i=find(x>=0&y>=0); eta(i)=et0(i);
i=find(x<0&y>=0); eta(i)=pi-et0(i);
i=find(x<=0&y<0); eta(i)=pi+et0(i);
i=find(x>0&y<0); eta(i)=2*pi-et0(i);
p=(-B-del)/c;
del2=p.^2-p; del=sqrt(del2);
zeta=log(1-2*p+2*del)/2;% only keep the positive root
function [x,y]=el2xy(zeta,eta,a,b)
%--------------------------------------------------------------------------
% Conversion from elliptic coordiantes (zeta,eta) to Cartesian coordinates
% zeta (>=0), eta [0 2pi)
% a -- semi-major axis
% b -- semi-minor axis
% c -- elliptic eccentricity
% (x,y,zeta,eta) same-size matrix
% Author: Che Sun (CAS, 2016)
% Source: http://eprint.las.ac.cn/abs/201611.00721
%-------------------------------------------------------------------------- 
c2=a^2-b^2; c=sqrt(c2);
x=c*cosh(zeta).*cos(eta);
y=c*sinh(zeta).*sin(eta); 