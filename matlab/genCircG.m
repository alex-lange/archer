function [g,e] = genCircG(n,r)
  g = zeros(n);
  residues = 1;
  e = 0;
  for i = 1:n-1
    rth = mod(i^r,n);
    if (~any(rth==residues));
      residues = [residues,rth];
    end
  end
  sort(residues);
  for i = 1:n
    for j = i+1:n
      if (any((j-i)==residues))
	g(i,j) = 1;
	g(j,i) = 1;
	e = e + 1;
      end
    end
  end
end