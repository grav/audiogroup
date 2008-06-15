% Usage: y=remove(x,n)
% remove every n'th element from x and return it as y

function y = remove(x,n)
  for i = 1:length(x)
    if mod(i,n)==0
      x(i)=0;
    end
  end
  y=x;