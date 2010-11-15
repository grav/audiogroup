function y = fuzzexp(x,c)

  % DAFX eq. 5.9

  y = c * sign(x).*(1-exp(-c * sign(x) .* x))/c;
