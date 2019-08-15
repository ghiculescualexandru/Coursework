% 315CB_Ghiculescu_Alexandru
function [m A eigenfaces pr_img] = eigenface_core(database_path)
    
  T = [];
  m = [];
  
  % Step 1 
  % reading each image from dataset
  for i = 1 : 10
    image_path = strcat(database_path,'/',int2str(i),'.jpg');
    M = double(rgb2gray(imread(image_path)));
    v = M(:);
    T = [T v];
  endfor
    
  % Step 2
  % find average 
  m = mean(T');
  m = m';
  
  % Step 3
  A = T - m;
  
  % Step4 
  % eigenvalues
  [vectp valp] = eig(A' * A);
  V = [];  
  [length_valp unused] = size(valp);
  k = 0;
 
  for i = 1 : length_valp
    if valp(i,i) > 1 
      k++;
      V(:,k) = vectp(:,i);
    endif
  endfor
  
  eigenfaces = A * V;
  
  %Pas 5  
  pr_img = eigenfaces' * A; 
  
end
