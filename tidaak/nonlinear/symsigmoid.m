function y = symsigmoid(x,c)
if nargin == 1
  c=1;
end
y = (1 - exp(-c * x)) ./ (1 + exp(-c * x));
