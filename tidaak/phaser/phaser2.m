%% setup

% sample
[x] = wavread('../sound/guitar1.wav');
x = [x' 0 0];

% white noise
%   x = rand(1,10000)*2-1;

% impulse 
% x = [1 zeros(1,10000)];

% sine
%   x = sin(2*pi*440/fs * linspace(1,20000,20000));

% define lfo frequency
lfo = .5; % Hz

% define filter frequencies
fcs=[300 1200 3000 6000 10000  12000 ];
fcs=[3000];
fs = 44100;

N = length(x);

y = zeros(1,N);

%% go girl

y = zeros(1,N);

for n = 3:N

  for m=1:length(fcs);
    % calculate filter parameters
    fc = fcs(m);
    fc = fc + fc/2 * sin(2 * pi * n * lfo/fs);

    fb = fc/70; % constant

    % calculate filter coefficients
    [b,a] = allpass2ndorder(fc,fb,fs);

    % IIR filtering
    bsum = 0;
    for i=1:length(b)
      bsum = bsum + b(i) * x(n-i+1);
    end

    asum = 0;
    for j=2:length(a)
      asum = asum + a(j) * y(n-j+1);
    end
    y(n) = 1/a(1) * (bsum - asum);
    

  end
  % copy output to input for next filter stage
%   x = y;

end

% sum with input to get phasing
y=x+y;
