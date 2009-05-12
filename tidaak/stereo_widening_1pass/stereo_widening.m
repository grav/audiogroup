
  
%% filter
x=wavread('vinduer_mono.wav');
fs=44100;

f = [357 741 1521 2761 4401 7389 9543 11021];

hd = allpass(f,fs);
y1=hd.filter(x)/2;

f = [257 571 1721 2461 4801 7689 9343 12021];

hd = allpass(f,fs);
y2=hd.filter(x)/2;

y = [y1 y2];

%% plot
close;
r = 10;
auto = xcorr(x,x);
auto = decimate(auto,r);
auto = auto/max(auto);

cross = xcorr(x,y);
cross= cross/max(cross);
cross = decimate(cross,r);

hold on;
plot(auto,'b');
plot(cross,'r');


%diffcross = abs(auto)-abs(cross);
%subplot(3,1,1), plot(auto);
%subplot(3,1,2), plot(cross);
%subplot(3,1,3), plot(diffcross);

