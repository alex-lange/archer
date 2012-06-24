function [h, x] = getCloseness(filePath)
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
