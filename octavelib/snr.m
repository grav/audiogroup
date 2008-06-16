% SNR (not logarithmic)

function s = snr(x,y)
  %average squared value
  a = mse(x,zeros(1,length(x)));
  
  %mse
  m = mse(x,y);

  s = a/m;
  
end