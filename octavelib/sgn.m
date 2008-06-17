% Usage: sgn(r)
% Returns the sign of the number r, 
% 1 if r is positive, -1 if negative, 0 if r = 0
function s=sgn(r)
  if (r > 0) s = 1;
  elseif (r < 0) s =-1;
  else s = 0;
end