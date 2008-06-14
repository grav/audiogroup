% Make n-pole FIR bandpass filter 
% Usage: h = bandpass(n,fs,low,high)

function h = bandpass(n,fs,low,high)
  wn = [low,high]/(fs/2);
  h = fir1(n,wn);
end
