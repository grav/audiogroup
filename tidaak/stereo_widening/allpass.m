function hd = allpass(f,fs)
  c = zeros(1,length(f));

% DAFX eq 2.11
for i = 1:length(f)
    c(i) = (tan(pi*f(i)/fs)-1)/(tan(pi*f(i)/fs)+1);
end

hd = dfilt.cascadeallpass(c(1),c(2),c(3),c(4),c(5),c(6),c(7),c(8));
