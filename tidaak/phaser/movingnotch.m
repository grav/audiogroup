function y = notch(x,notchFreq,A)

N = length(x);

% can't get global vars to work :-(
fs = 44100;

y = zeros(1,N);

%% calculate initial a and b
BW     = 1200;  % Bandwidth
Apass  = 1;     % Bandwidth Attenuation
[b, a] = iirnotch(notchFreq/(fs/2), BW/(fs/2), Apass);
%[b, a] = mynotch(notchFreq,0.98);

% just copy two first samples since we're using 3 coeffs.
y(1)=x(1);
y(2)=x(2);

for n=3:N

    % changing second a- and b-coeff. changes notch freq.
    % thus we use our lfo controlled pShift to change it
    % slightly up and down as a function of n
    phase = 0;
    lfoFreq = 2; % Hz
    pShift = A * sin(2 * pi * n * lfoFreq/fs + phase);

    b(2) = b(2)+pShift;
    a(2) = a(2)+pShift;
    
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
    
    % change coeff back
    b(2) = b(2)-pShift;
    a(2) = a(2)-pShift;
    
end


