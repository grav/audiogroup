
clear;
cs=logspace(0,2,20);

z=zeros(length(cs),500);

for i=1:length(cs) 
c=cs(i);
l=linspace(1,1000,1000);
x=sin(2*pi*3000/44100*l);
y=symsigmoid(x,c);
amp=abs(fft(y));
z(i,:)=amp(1:floor(length(amp)/2));

end
waterfall(z);
