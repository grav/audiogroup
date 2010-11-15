function y=matlabtest(x)

fcs=setfcs;

% impluse
x=[1 zeros(1,10000)];

y=x;
for m=1:length(fcs)
  fc=fcs(m)
  fb=fcs(m)/10;
  [b,a]=allpass2ndorder(fc,fb);
%   [b,a] =allpass1storder(fc);
  y = filter(b,a,y);
end

% plot(angle(fft(y)));