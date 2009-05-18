function y=sigmoid(x,c)
if nargin == 1 
  c = 1;
end
y=1./(1+exp(1).^(-c*x));