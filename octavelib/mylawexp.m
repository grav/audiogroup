% my-law expander function
% Usage: mylawexp(x,my)
% Sets my to 255 if unspecified
function e = mylawexp(x,my)
  if !exist('my') 
    my=255;
  end
  e = x;
  for i=1:length(x)
    e(i) = (max(x)/my) * ((1+my)^(abs(x(i))/max(x))-1) * sgn(x(i));
   end
end