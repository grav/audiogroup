close all;

dirac=[1 zeros(1,10000)];

mine = phaser2(dirac);

ml = matlabtest;

subplot(2,2,1),plot(angle(fft(mine)));
title 'Mine - phase';
subplot(2,2,2),plot(angle(fft(ml)));
title 'Matlab - phase';
subplot(2,2,3),plot(abs(fft(mine)));
title 'Mine - amplitude';
subplot(2,2,4),plot(abs(fft(ml)));
title 'Matlab - amplitude';
