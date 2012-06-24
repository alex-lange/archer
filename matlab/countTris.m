function [count,tris] = countTris(g)
  count = 0;
  n = size(g,1);
  tris = zeros(1,3);
  time1=cputime;
  for i = 1:n-2
    for j = i:n-1
      if( g(i,j) )
	for k = j:n
	  if( g(i,k) && g(j,k) )
	    count = count + 1;
	    tris = [tris; g(i,j),g(i,k),g(j,k)];
	  end
	end
      end
    end
  end
  time2 = cputime;
  tris = tris(2:count+1,:);
  time2-time1
end