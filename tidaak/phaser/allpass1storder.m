function [b,a] = allpass1storder(fc,fs)
if nargin==1
  fs=44100;
end

c = (tan(pi*fc/fs) - 1)  / (tan(pi*fc/fs) + 1);

b=[c 1]; a = [1 c];

