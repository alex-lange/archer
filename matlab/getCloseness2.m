function A = getCloseness2( fileName )

  file = fopen(fileName, 'r');

  A = textscan(file, '%d %d');
  %file

  fclose(file);

end