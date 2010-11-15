function y=symclip(x)

N=length(x);
y = zeros(1,N);

for i=1:N
  if abs(x(i)) >= 0 && abs(x(i)) < 1/3
    y(i) = 2 * x(i);
  elseif abs(x(i)) >= 1/3 && abs(x(i)) < 2/3
    y(i) = sign(x(i)) * (3-(2-3 * abs(x(i)))^2) / 3;
  else
    y(i) = 1 * sign(x(i));
  end
end
