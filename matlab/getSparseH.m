function [h, numEdges, numTris] = getSparseH(filePath)
  file = load(filePath);
  numEdges = file(1,1);
  numTris = file(1,2);
  r=[file(2:end,1);file(2:end,2)];
  c=[file(2:end,2);file(2:end,1)];
  tic
  o=ones(size(r,1),1);
  h=sparse(r,c,o,numEdges,numEdges);
end
