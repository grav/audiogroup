%% load sound
[x,fs]=wavread('../sound/guitar1.wav');
x=x';

%% impulse 
% x = [1 zeros(1,1000)];

%% filter while changing 2nd coefficient of a and b
N = length(x);

% can't get global vars to work :-(
fs = 44100;

% frequences to notch
notchFreqs=[300 1200 3000 6000 10000 12000];
% notchFreqs=[6000];

% lfo amplifications
notchAmps =[.0015 .025  .17   .65   1.5  1.6];
% notchAmps = [.65];

BW     = 1200;  % Bandwidth
Apass  = 1;     % Bandwidth Attenuation

% filter banks
bs = zeros(1,length(notchFreqs));
as = zeros(1,length(notchFreqs));

% generate notch filters
for i=1:length(notchFreqs)
  fc = notchFreqs(i);
  [b,a] = iirnotch(fc/(fs/2), BW/(fs/2), Apass);
  bs(i) = b(2); as(i) = a(2);
end

phase = 0;
lfoFreq = .5; % Hz

y = zeros(1,N);

for m=1:length(notchFreqs)

  for n=3:N

    A = notchAmps(m);
    % changing second a- and b-coeff. changes notch freq.
    % thus we use our lfo controlled pShift to change it
    % slightly up and down as a function of n
    pShift = A * sin(2 * pi * n * lfoFreq/fs + phase);

    b(2) = bs(m)+pShift;
    a(2) = as(m)+pShift;
    
    % make iir-filtering of current sample
    % ref: http://en.wikipedia.org/wiki/IIR
    bsum = 0;
    for i=1:length(b)
        bsum = bsum + b(i) * x(n-i+1);
    end
    
    asum = 0;
    for j=2:length(a)
        asum = asum + a(j) * y(n-j+1);
    end
    y(n)= 1/a(1) * (bsum - asum);
    
  end
  % copy output to input for next filtering
  x = y;

end

play(y);

