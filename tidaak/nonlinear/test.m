%% init
clear;
close all;

% waterfall plot setup
N = 30; % number of input levels

fftN = 2000; % number of fft points
r = 0.5; % ratio of frequency spectrum to plot (1=fs)

amps = logspace(-1.5,0,N);
w = zeros(N,fftN*r);

%% load
l_in = linspace(-1,1,1000);
[x,fs] = wavread('../sound/guitar1.wav');
h = lowpass;
f = 1000;
s_in = sin(2*pi*f/fs*linspace(1,10000,10000));


%% process and play

% parameters
c = 1;

% for tube
q = -0.01;
dist = 80;

% Fuzz Exp
name = sprintf('Fuzz Exp (c=%d)',c)
l_out = fuzzexp(l_in,c); y = fuzzexp(x,c); 
for i=1:N
  s_out = fuzzexp(amps(i)*s_in,c);
  spectrum = abs(fft(s_out,fftN));
  w(i,:) = 20*log10(spectrum(1:fftN*r));
end

% Sym Sigmoid
% name = sprintf('Sym Sigmoid (c=%d)',c)
% l_out = symsigmoid(l_in,c); y = symsigmoid(x,c);
% for i=1:N
%   amps(i)
%   s_out = symsigmoid(amps(i)*s_in,c);
%   spectrum = abs(fft(s_out,fftN));
%   w(i,:) = 20*log10(spectrum(1:fftN*r));
% end


% Tube
% name = sprintf('Tube (dist=%d, q=%f)',dist,q);
% l_out = tube(l_in,q,dist); y = tube(x,q,dist); 
% for i=1:N
%   s_out = tube(amps(i)*s_in,q,dist);
%   spectrum = abs(fft(s_out,fftN));
%   w(i,:) = 20*log10(spectrum(1:fftN*r));
% end

% Symclip
% name = 'Symclip'
% l_out = symclip(l_in); y = symclip(x);
% for i=1:N
%   s_out = symclip(amps(i)*s_in);
%   spectrum = abs(fft(s_out,fftN));
%   w(i,:) = 20*log10(spectrum(1:fftN*r));
% end

%% plot
% characteristic of function
subplot(3,1,1),plot(l_in,l_out);
title(sprintf('Characteristics of %s', name));
xlabel 'Input amplitude';
ylabel 'Output amplitude';
% a sine tone
subplot(3,1,2),plot(s_out(1:100));
title 'Processing of a sine';
xlabel 'Time';
ylabel 'Amplitude';
spectrum = (abs(fft(s_out)));
% subplot(3,1,3),plot(linspace(1,fs/2,5000),spectrum(1:end/2));
subplot(3,1,3),mesh(linspace(1,fs*r,length(w(1,:))),amps,w);
title 'Output Spectrum';
xlabel 'frequency (Hz)';
ylabel 'Input level'
zlabel 'Amplitude (dB)';

%% play
% play(y_nofilter);

