l=linspace(-1,1,500);

fid = fopen('data.txt', 'wt');
fprintf(fid, '%ff, ', l);
fclose(fid)
