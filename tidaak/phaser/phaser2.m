%% setup
clear

% define lfo frequency
lfo = .5; % Hz

% define filter frequencies
% todo: calculate these?
fcs=[300 1200 3000 6000 10000  12000 14000 18000];

% amplification of LFO
amps =[.0015 .025 .17 .65 1.5 1.6 1.0 .1];

assert(length(fcs)==length(amps));

[x,fs] = wavread('../sound/rhodes2.wav');
x = [x' 0 0];

N = length(x);

y = zeros(1,N);

%% go girl

fclog = zeros(1,N);

y(1) = x(1); y(2) = x(2);
for n = 3:N

  for m=1:length(fcs);
    % calculate filter parameters
    fc = fcs(m);
    fc = fc + fc/10 * sin(2 * pi * n * lfo/fs);
    fb = fc/10;
  
    if m == 4 
      fclog(n)=fc;
    end
    
    % calculate filter coefficients
    [b,a] = allpass2ndorder(fc,fb,fs);
    %fvtool(b,a);
  end
  
  % iir filtering
  for i=1:length(b)
    y(n) = b(i) * x(n-i+1);
  end
  for j=2:length(a)
    y(n) = y(n) - a(i) * y(n-j+1);
  end
  y(n) = y(n)/a(1);
  
  % mix with input
%  y(n) = x(n)/2+y(n)/2;
  
end

