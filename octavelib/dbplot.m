function dbplot(y,Fs)
    % Number of fft points
    p = 10000;

    xaxis = linspace(1,Fs/2,p/2);
    %xaxis = logspace(0,5,p/2);
    Yamp = abs(fft(y,p));
    Ydb = 20*log10(Yamp);
    YdbMax = max(Ydb);
    YdbNorm = Ydb - YdbMax;
    dBmin = -100;
    YdbP = max(YdbNorm,dBmin);

    figure
    semilogx(xaxis,YdbP(1:p/2));
    ylabel 'Amplitude (dB)';
    xlabel 'Frequency (Hz)';
 
