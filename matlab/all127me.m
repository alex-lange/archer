function allDists()
  path = 'c++/127-e/mult';
  limit = 50000000;
  graphs = dir(path);
  [n,m]=size(graphs);
  fid = fopen('127me2ClTable.txt', 'w');
  for i = 1:n
    if ~graphs(i).isdir
      if graphs(i).bytes < limit
	g = strcat(path,'/',graphs(i).name);
	[h,x] = getCloseness(g);
	fprintf(fid, '%s %f\n', graphs(i).name, x);
      end
    end
  end
  fclose(fid);
end