function [count,edges] = countEdges(g)
  count = 0;
  n = size(g,1);
  edges = zeros(n);
  for x = 1:n-1
    for y = x:n
      if( g(x,y) )
	count = count + 1;
	edges(x,y) = count;
	edges(y,x) = count;
      end
    end
  end
end 