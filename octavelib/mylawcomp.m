% my-law compressor function
% Usage: mylawcomp(x,my)
% Sets my to 255 if unspecified
function c = mylawcomp(x,my)
  if !exist('my') 
    my=255;
  end
  c = x;
  for i=1:length(x)
    c(i) = max(x) * (log(1 + my * (abs(x(i)/max(x))))/log(1+my)) * sgn(x(i));
   end
end