clear 

Fs = 44100;

maxF = 20000;
minF = 20;

numBands = 32;

bands = zeros(1,numBands);

subdivision = 'log';

constBW = (maxF-minF)/numBands; % used for linear subdivision
k = (maxF/minF)^(1/numBands); % used for log. subdivision

% destination vector

for i=1:numBands
    if ( subdivision == 'log' )        
        % logarithmic subdivision
        lower = minF * k^(i-1)
        upper = minF * k^i
    else
        % linear subdivision
        lower = constBW * (i-1) + minF
        upper = lower + constBW
    end
    bands(i) = lower;
 
    
    % set up filter
    N = 500; % coefficients
    Wn = [lower,upper]/(Fs/2); % bandwidth

    bpf = fir1(N,Wn,'band');
    
    filename = strcat('fir_',subdivision,'_',num2str(i),'.wav')
    wavwrite(bpf,Fs,filename); 
    
    % modulation (resampling)
    if ( subdivision == 'log' )  
        r = floor(Fs/(upper * 2));
    else
        r = numBands;
    end
end

