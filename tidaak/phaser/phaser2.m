%% setup
clear

% define lfo frequency
lfo = .5; % Hz

% define filter frequencies
% todo: calculate these?
fcs=[300 1200 3000 6000 10000  12000 14000 18000];

% fcs = [ 1200 6000 ];

% amplification of LFO
% amps =[.0015 .025 .17 .65 1.5 1.6 1.0 .1];

[x,fs] = wavread('../sound/guitar1.wav');
x = [x' 0 0];

% x = [1 zeros(1,10000)];
% 
% x = sin(2*pi*440/fs * linspace(1,20000,20000));

N = length(x);

y = zeros(1,N);

%% go girl

% fclog = zeros(length(fcs),N);

% y(1) = x(1); y(2) = x(2);
% for n = 3:N
y = x;
  % operate on t and y
%   t = x(n-2:n);
  for m=1:length(fcs);
    % calculate filter parameters
    fc = fcs(m);
%     fc = fc + fc/200 * sin(2 * pi * n * lfo/fs);
    
    fb = fc/1;
  
%     fclog(m,n)=fc;
    
    % calculate filter coefficients
    [b,a] = allpass2ndorder(fc,fb,fs);
    
    y=filter(b,a,y);
%     fvtool(b,a);

%     bsum = 0;
%     for i=1:length(b)
%         bsum = bsum + b(i) * t(3-i+1);
%     end
%     
%     asum = 0;
%     for j=2:length(a)
%         asum = asum + a(j) * y(n-j+1);
%     end
%     y(n) = 1/a(1) * (bsum - asum);
    % update input for next filtering
%     t = y(n-2:n);
    
  end
  
% end

% play(x+y/4);

% figure
% hold on
% for i=1:length(fcs)
%   plot(fclog(i,:));
% end
