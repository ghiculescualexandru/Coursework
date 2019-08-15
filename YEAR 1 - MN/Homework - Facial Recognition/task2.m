% This function creates 4 graphics
%    -singular values;
%    -information given by the first 'k' singular values;
%    -the error of approximation;
%    -the compression rate of data.

function task2(image, k)
    
  % Graphic  1
  % read matrix  
  image_matrix = double(imread(image));
  [m n] = size(image_matrix); 
  % find U S and V
  [U,S,V] = svd(image_matrix);
  V = V';
  % find Sn
  Sn = S(1:min(m,n), 1:min(m,n));
  % create an array with elements from diagonal
  singular_values = diag(Sn);
  
  figure(1);
  plot(singular_values);
     
  % Graphic 2  
  sum1 = 0; 
  sum2 = 0; 
  
  for i = 1 : min(m,n)
    sum2 = sum2 + singular_values(i);
  endfor
  
  % array for different 'k' values
  % array length
  k_vector = 1:k;
  length = length(k_vector);
 
  for j = 1 : length
    sum1 = 0;
    for i = 1 : k_vector(j)
      sum1 = sum1 + singular_values(i);
    endfor
    % array for sums
    sum_vec(j) = sum1;
  endfor 
  
  figure(2);  
  plot(sum_vec/sum2);
   
  % Graphic 3  
  loading = 0; 
  
  for q = 1 : length    
    afis = (loading/length)*100;
    printf("Loading %d%%\n", afis)
    loading = loading + 1;
    
    % find Uk Sk si Vk for each K
    Uk_3 = U(1:m,1:q);
    Sk_3 = S(1:q,1:q);
    Vk_3 = V(1:q,1:n);  
    % find A_k for each K
    A_k_3 = Uk_3 * Sk_3 * Vk_3;    
    sum_3 = 0;
    for i = 1 : m      
      for j = 1 : n
        x = (image_matrix(i,j) - A_k_3(i,j));
        sum_3 = sum_3 + x*x;
      endfor
    endfor
    % array for sums
    sum_vec_3(q) = sum_3;
  endfor
  
  figure(3)
  plot(sum_vec_3/(m*n));
      
  % Graphic 4    
  for q = 1 : length
    % array for sums
    sum_4(q) = (m*q + n*q + q)/(m*n);
  endfor
  
  figure(4);
  plot(sum_4);
  
end
