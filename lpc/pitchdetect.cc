function [AMDF,min] = pitchdetect (f)
[y,fs]=wavread(f);
N=length(y);
k=0;

% lowest and highest period of human speech
l_s = 0.0025;
h_s = 0.0195;

% same in samples
l_P = round(fs * l_s);
h_P = round(fs * h_s);

AMDF = zeros(1,h_P);

for P=l_P:h_P
   sum = 0;
   for i = P+1:N
      sum = sum + abs(y(i)-y(i-P));
   end
   AMDF(P) = 1/N * sum;
end

min = min(AMDF);

