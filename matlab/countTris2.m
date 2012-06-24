function [count,tris] = countTris2(g)
  count = 0;
  n = size(g,1);
  %tris = zeros(1,3);
  time1 = cputime;
  for i = 1:n-2
    for j = i:n-1
      if( g(i,j) )
	for k = j:n
	  if( g(i,k) && g(j,k) )
	    count = count + 1;
	    %tris = [tris; g(i,j),g(i,k),g(j,k)];
	  end
	end
      end
    end
  end
  tris = zeros(count,3);
  r = 1;
  for i = 1:n-2
    for j = i:n-1
      if( g(i,j) )
	for k = j:n
	  if( g(i,k) && g(j,k) )
	    tris(r,1) = g(i,j);
	    tris(r,2) = g(i,k);
	    tris(r,3) = g(j,k);
	    r = r + 1;
	  end
	end
      end
    end
  end
  %tris = tris(2:count+1,:);
  time2 = cputime;
  time2-time1
end