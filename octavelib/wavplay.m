function wavplay(y,Fs)
  tmp = '/tmp/octave_temp_sound.wav';
  player = 'mplayer > /dev/null';
  wavwrite(y,Fs,tmp);
  system(strcat(player,' ',tmp));
  system(strcat('rm ',tmp));
end
