% This function returns A_k, the initial matrix given
% as a parameter, modified through SVD.

function [A_k S] = task3(image, k)
  
  % read matrix
  image_matrix = double(imread(image));
  [m n] = size(image_matrix);  
  a = image_matrix;
  b = image_matrix;
  
  % Step 1 
  % find miu
  for i = 1 : m
    miu(i) = sum(a(i,:))/n;
  endfor  
  
  % Step 2
  % modify a
  for i = 1 : m
    a(i,:) = a(i,:) - miu(i);
  endfor
    
  % Step 3 
  % find Z
  x = 1/(sqrt(n-1));
  Z = a'*x;
  
  % Step 4
  % find U S and V for Z  
  [m1 n1] = size(Z);
  [U,S,V] = svd(Z);  
  
  % Step 5
  W = V(:,1:k);
  
  % Step 6 
  Y = W' * a;
  
  % Step 7  
  A_k = W*Y;
  for i = 1 : m
    A_k(i,:) = A_k(i,:) + miu(i);
  endfor
  
end
