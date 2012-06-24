function p = genTableLast3()
  p = zeros(3,3);
  p(1,1)=769;
  p(1,2)=6;
  p(2,1)=937;
  p(2,2)=6;
  p(3,1)=941;
  p(3,2)=5;
  for i=1:3
    g=genCircG(p(i,1), p(i,2));
    [h,numEdges,numTris] = genSparseH(g);
    alpha=3*numTris/2-min(eigs(h,6,'sa'))*numEdges/4;
    beta = 2*numTris;
    p(i,3)=(alpha-beta)/alpha
  end
end