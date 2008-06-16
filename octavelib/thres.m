function y = thres(x,r)
  t = r * max(abs(x));
	for i=1:length(x)
		if abs(x(i)) < t 
		  x(i) = 0;
		end
	end
	y=x;