function [b,a] = allpass2ndorder(fc,fb,fs)
% return filter coefficients for IIR allpass filter

if nargin == 2 
  fs = 44100;
end

% DAFX eq. 2.19 - 2.20
c = (tan(pi*fb/fs) - 1)  / (tan(pi*fb/fs) + 1);
d = -cos(2 * pi * fc/fs);

% DAFX eq. 2.21
b = [-c d*(1-c) 1];
a = [-1 -d*(1-c) c];
