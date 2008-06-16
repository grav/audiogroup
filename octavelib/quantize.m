% Quantize x to n bits  

function y = quantize(x,n)
  y=x*(2^15-1);
  y=int32(y);
  y=y/2^(16-n+1);
%  y=double(y);
%  y=y/2^(n-1)
