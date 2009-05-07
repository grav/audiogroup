
%% setup stuff

% speed of sound
c = 340;
% samplerate
fs = 44100;

% distance between the  ears in spatial samples,
% i.e., the distance in meters multiplied by fs/c
delta = round(0.18 * fs/c);

%  the angle (degrees) subtended by a loudspeaker  with the median  plane
theta = 30;
theta_rad = 2 * pi * theta/360;

% the distance in meters between a loudspeaker and  the  nearest  ear,
Dm = 6.0;
% the distance in samples between a loudspeaker and  the  nearest  ear,
D = Dm * fs/c;

h = hlow;
h=[1];

d= round(1.5 * delta * sin(theta_rad));

% app. attenuation
g = D/(delta * sin(theta_rad) + D);

%g = g * 0.5;
%% read input
x=wavread('feel.wav');
x=x';
l1=x(1,:);
l2=x(2,:);


%% filter
l1_delayed = horzcat(zeros(1,d),l1);
l2_delayed = horzcat(zeros(1,d),l2);

l2_low = conv(h,l2_delayed);
l1_low = conv(h,l1_delayed);

pad = zeros(1,length(l1_low)-length(l1));

e1 = horzcat(l1,pad) + g * conv(h,l2_delayed);
e2 = horzcat(l2,pad) + g * conv(h,l1_delayed);

y = [e1;e2];

