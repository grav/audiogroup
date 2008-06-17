% Quantize x to n bits  
% A bit hacky for now ... 
% Expects input x to be between 0 and 1, and outputs
% values quantized to n bits between 0 and 1
% Note that the function also always quantizes to 16 bits(!)

function y = quantize(x,n)
  % normalize to min=0, max=1
  %y=x+abs(min(x));
  %y=y/max(y);
  
  y = x;
  
  % convert to 16 bit unsigned integer
  y=y*(2^16);
  %y=floor(y);
  y=uint16(y);

  % quantize
  y = y/2^(16-n);
  
  for i=1:length(y)
    if y(i)==2^n
      y(i) = y(i)-1;
    end
  end
  
  %convert to double precision once again
  y = double(y);
  
%  y=double(y);
%  y=y/2^(n-1)
