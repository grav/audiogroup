function b = lowpass
%LOWPASS Returns a discrete-time filter object.

%
% M-File generated by MATLAB(R) 7.6 and the Signal Processing Toolbox 6.9.
%
% Generated on: 02-Jun-2009 13:13:06
%

% Equiripple Lowpass filter designed using the FIRPM function.

% All frequency values are in Hz.
Fs = 44100;  % Sampling Frequency

Fpass = 10000;           % Passband Frequency
Fstop = 11000;           % Stopband Frequency
Dpass = 0.057501127785;  % Passband Ripple
Dstop = 0.0001;          % Stopband Attenuation
dens  = 20;              % Density Factor

% Calculate the order from the parameters using FIRPMORD.
[N, Fo, Ao, W] = firpmord([Fpass, Fstop]/(Fs/2), [1 0], [Dpass, Dstop]);

% Calculate the coefficients using the FIRPM function.
b  = firpm(N, Fo, Ao, W, {dens});
%Hd = dfilt.dffir(b);

% [EOF]