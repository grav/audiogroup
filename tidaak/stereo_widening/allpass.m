function y = allpass(x,fc,fs)
    % calculate fb from fc
    fb = exp(fc/1600);
  
  
    % DAFX eq 2.19 and 2.20
    c = (tan(pi*fb/fs)-1)/(tan(pi*fb/fs)+1);
    d = -cos(2*pi*fc/fs);

    N = length(x);
    y = zeros(1,N);
    y(1) = x(1);
    y(2) = x(2);
    for n=3:N
        % DAFX eq. 2.21
        y(n) = -c*x(n) + d * (1 - c) * ...
            x(n - 1) + x(n - 2) -d * ...
            (1 - c) * y(n - 1) + c * y(n - 2);
    end
