function [h,numEdges,numTris,ri,ci] = genSparseH(g)
  n = size(g,1);
  [numEdges,edges] = countEdges(g); 
  %printf("Got edges\n");
  [numTris,t] = countTris2(edges);
  %printf("Got tris\n");
  ri = zeros( numTris*3, 1 );
  ci = ri;
  %ci = ri =[];
  time1=cputime;
  for i = 1:numTris
    ri(i*3-2) = t(i,1);
    ci(i*3-2) = t(i,2);
   
    ri(i*3-1) = t(i,1);
    ci(i*3-1) = t(i,3);
   
    ri(i*3) = t(i,2);
    ci(i*3) = t(i,3);
    
    %ri = [ri; t(i,1)];
    %ci = [ci; t(i,2)];
    %%ri = [ri; t(i,2)];
    %%ci = [ci; t(i,1)];
    %ri = [ri; t(i,1)];
    %ci = [ci; t(i,3)];
    %%ri = [ri; t(i,3)];
    %%ci = [ci; t(i,1)];
    %ri = [ri; t(i,2)];
    %ci = [ci; t(i,3)];
    %%ri = [ri; t(i,3)];
    %%ci = [ci; t(i,2)];
  end
  time2 = cputime;
  %printf("Got skeleton in %f\n",time2-time1);
  v = ones(numTris*6,1);
  temp = ri;
  ri = [ri; ci];
  ci = [ci; temp];
  %printf("Making sparse...\n");
  h = sparse(ri, ci, v, numEdges, numEdges);
end