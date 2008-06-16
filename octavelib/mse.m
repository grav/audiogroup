% Mean Square Error

function m = mse(x,y)
  N = length(x);
  m = 0;
  for i=1:N
    m = m+(x(i)-y(i))^2;
  end
  m = m/N;
end
    