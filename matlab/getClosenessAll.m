function [h, x] = getClosenessAll()
  files = {'c++/sparse1273.txt', 'c++/sparse2814.txt', ...
	   'c++/sparse3134.txt', 'c++/sparse3374.txt', ...
	   'c++/sparse3534.txt', 'c++/sparse4574.txt', ...
	   'c++/sparse5415.txt', 'c++/sparse5715.txt', ...
	   'c++/sparse7015.txt', 'c++/sparse7696.txt', ...
	   'c++/sparse9376.txt', 'c++/sparse9415.txt' }
  for i = 1:12
    filePath = files{i}
    file = load(filePath);
    numEdges = file(1,1);
    numTris = file(1,2);
    r=[file(2:end,1);file(2:end,2)];
    c=[file(2:end,2);file(2:end,1)];
    tic
    o=ones(size(r,1),1);
    h=sparse(r,c,o,numEdges,numEdges);
    alpha=3*numTris/2-min(eigs(h,6,'sa'))*numEdges/4;
    beta = 2*numTris;
    x=(alpha-beta)/alpha;
    toc
    x
  end
end
