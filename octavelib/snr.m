% Usage: snr(x,y)
% Calculate Signal-to-noise ratio where x is the signal and y is the noise
% Use 10 * log10(snr(...)) for SNR(dB)

function s = snr(x,y)
  %average squared value
  a = mse(x,zeros(1,length(x)));
  
  %mse
  m = mse(x,y);

  s = a/m;
  
end