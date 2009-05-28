% oevelse 2.3
% phaser
% x: input signal 
% y: processed signal
% 

%% init
clear
close all

%% load sound
fs=44100; % must also be changed in notch.m since I have no clue about global variables
x=wavread('../sound/rhodes2.wav');
x=x';

%% impulse response
%x = zeros(1,1000); x(1)=1;

%% filter while changing 2nd coefficient of a and b
y=x;

% frequences to notch
notchFreqs=[300    1200 3000 6000 10000  12000 14000 18000];

% lfo amplifications
notchAmps =[.0015 .025  .17   .65   1.5  1.6     1.0  .1];

for i=1:length(notchFreqs)
    y=notch(y,notchFreqs(i),notchAmps(i));
   % imp = notch(imp,notchFreqs(i),notchAmps(i));
end

play(y);

