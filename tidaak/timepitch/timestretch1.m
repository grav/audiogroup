function y=timestretch1(x,grain,a)
% simple and ugly timestretch, which just deletes samples
%
% grain: section size
% alpha: scaling, eg length(y) ~= a * length(x)

N = length(x);
y = zeros(1,ceil(length(x)*a));
i = 1;
j = 1;


while(i<=N)
  y(j) = x(i);
  
  i=i+1;
  j=j+1;
  if mod(i,grain*a) == 0 
    i=i + (1-a) * grain;
  end

end