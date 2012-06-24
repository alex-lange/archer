function [h,numEdges,numTris] = genH(g)
  n = size(g,1);
  [numEdges,edges] = countEdges(g);
  h = zeros(numEdges);
  [numTris,t] = countTris2(edges);
  for i = 1:numTris
    h(t(i,1),t(i,2))=1;
    h(t(i,2),t(i,1))=1;
    h(t(i,1),t(i,3))=1;
    h(t(i,3),t(i,1))=1;
    h(t(i,2),t(i,3))=1;
    h(t(i,3),t(i,2))=1;
  end
end