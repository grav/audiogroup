function wavplay(y,Fs)
  if !exist('Fs') 
    Fs = 44100; % not like std. function
  end
  tmp = '/tmp/octave_temp_sound.wav';
  player = 'mplayer -really-quiet > /dev/null';
  wavwrite(y,Fs,tmp);
  system(strcat(player,' ',tmp));
  system(strcat('rm ',tmp));
end
