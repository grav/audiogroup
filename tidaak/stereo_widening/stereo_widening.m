%% filter
clear;
x=wavread('vinduer_mono.wav');
x = x';
fs=44100;
%x=zeros(1,1000);
%x(1)=1;
F = [357 741 1521 2761 4401 7389 9543 11021];

y1 = x;
for f = F
    y1=allpass(y1,f,fs);
end

F = [267 541 1624 2761 4531 7589 9843 12301];

y2 = x;
for f = F
    y2=allpass(y2,f,fs);
end

y = [y1/2 ;  y2/2]';

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

