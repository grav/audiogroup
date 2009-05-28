function [B,A] = mynotch(f,r,fs)
% copied from
% http://groups.google.com/group/comp.soft-sys.matlab/browse_frm/thread/bad
% 839e1a15920bb/5248378bbcea10f8?q=notch&
% author's comment at the bottom

% orig parameters:
% f = 60;   % Location of notch 
% r = 0.95;  % Control parameter. 0 < r < 1. 

% default samplerate
if nargin == 2 
  fs = 44100;
end

A = ones(3,1); 
B = A; 
cW = cos(2*pi*f/fs); 
A(2) = -2*r*cW; 
A(3) = r^2; 
B(2)= -2*cW; 

A=A'; B=B';

% What this filter does, is that it puts a zero exatly on the unit circle 
% at the 60 Hz line. The zero removes the line at 60 Hz, but pulls 
% down the surounding frequencies too. To counter that, a pole 
% is put just inside the unit circle to push the frequency 
% response back up. 
% The notch is put on the nominal location of the interfering line 
% by choosing the ratio f/fs. The width of the notch is controlled 
% by the parameter r. 
% This approach ought to give a little better control and leeway 
% than setting the spectrum coefficient to 0. 
% If the variation turns out to be large, one might cosider to use a 
% band-stop filter or an adaptive filter. It's hard to tell without 
% seeing the data in question. 
% Rune 
