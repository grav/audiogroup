function y = symsigmoid(x,c)
if nargin == 1
  c=1;
end
y = (1 - exp(1).^(-c * x)) ./ (1 + exp(1).^(-c * x));