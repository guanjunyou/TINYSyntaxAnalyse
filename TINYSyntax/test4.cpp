a := (a and 8 or b) or not b;
b:= regex R | c&d;
read x;
if  (0<x) 
  for  fact := x downto 1 do 
    fact := fact * x;
    x:=x+1;
  enddo
  write fact;  
else
 a:=3;
