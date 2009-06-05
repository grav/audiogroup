%% setup

  % sample
  [x] = wavread('../sound/guitar1.wav');
  x = [x' 0 0];

  % white noise
%   x = rand(1,10000)*2-1;

  % sine
%   x = sin(2*pi*440/fs * linspace(1,20000,20000));

% define lfo frequency
lfo = .5; % Hz

% define filter frequencies
fcs = setfcs;

fs = 44100;

N = length(x);

y = zeros(1,N);

%% go girl


y = zeros(1,N);

for m=1:length(fcs);
  for n = 3:N

    % calculate filter parameters
    fc = fcs(m);
    fc = fc + fc/2 * sin(2 * pi * n * lfo/fs);

    fb = fc/30; % constant

    %     fclog(m,n)=fc;

    % calculate filter coefficients
    [b,a] = allpass2ndorder(fc,fb,fs);
%   [b,a]=allpass1storder(fc);

    %     y=filter(b,a,y);
    %     fvtool(b,a);

    bsum = 0;
    for i=1:length(b)
      bsum = bsum + b(i) * x(n-i+1);
    end

    asum = 0;
    for j=2:length(a)
      asum = asum + a(j) * y(n-j+1);
    end
    y(n) = 1/a(1) * (bsum - asum);
%     y(n) = 1/a(1)*(b(1)*x(n) + b(2)*x(n-1) + b(3)*x(n-2) - a(2)*y(n-1) - a(3)*y(n-2));
    

  end
  % copy output to input for next filter stage
  x = y;

end

% sum with input to get phasing
y=x+y;

play(y/(max(abs(y))));