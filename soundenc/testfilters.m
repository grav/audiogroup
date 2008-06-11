clear

maxF = 20000;
minF = 20;

numBands = 32;

[x,Fs] = wavread('sounds/warm_mono_short.wav');

y = zeros(length(x),1);

bands = zeros(1,numBands);

subdivision = 'log';

% destination vector

for i=1:numBands
    
    % set up filter
    filename = strcat('filters/fir_',subdivision,'_',num2str(i),'.wav')
    bpf = wavread(filename);
    
    y = y + filter(bpf,1,x);
    
end
y = y/max(abs(y));

