function y = tube(x,Q,dist)
  assert(not(Q==0), 'Q cannot be 0');
  y = (x - Q) ./ (1 - exp(-dist * (x - Q)));
  
  y = y + Q ./ (1 - exp(dist * Q));
  
  %TODO: normalize?