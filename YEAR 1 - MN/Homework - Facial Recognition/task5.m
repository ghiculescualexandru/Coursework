% This function creates 4 graphics
%    -singular values;
%    -information given by the first 'k' singular values;
%    -the error of approximation;
%    -the compression rate of data.

function task5(image, k)
  
  % read matrix
  [A_k S] = task3(image, k);
  image_matrix = double(imread(image));
  [m n] = size(image_matrix);  
  a = image_matrix;
  b = image_matrix;  
   
  for i = 1 : m
    miu(i) = sum(a(i,:))/n;
  endfor  
  
  for i = 1 : m
    a(i,:) = a(i,:) - miu(i);
  endfor
  
  x = 1/(sqrt(n-1));
  Z = a'*x;
    
  % Graphic 1  
  figure(1);
  plot(diag(S));   
   
  % Graphic 2  
  [U,S,V] = svd(Z);
  V = V';  
  Sn = S(1:min(m,n), 1:min(m,n));
  singular_values = diag(Sn);
  
  sum1 = 0; 
  sum2 = 0; 
  
  for i = 1 : min(m,n)
    sum2 = sum2 + singular_values(i);
  endfor
  
  k_vector = 1:k;
  length = length(k_vector);
 
  for j = 1 : length
    sum1 = 0;
    for i = 1 : k_vector(j)
      sum1 = sum1 + singular_values(i);
    endfor
    
    sum_vec(j) = sum1;
  endfor 
  
  figure(2);  
  plot(sum_vec/sum1); 
    
  %Grafic 3  
  loading = 0; 
  
  for q = 1 : length    
    afis = (loading/length)*100;
    printf("Loading %d%%\n", afis)
    loading = loading + 1;
   
    [A_k unused] = task3(image, q);
    sum_3 = 0;
    for i = 1 : m      
      for j = 1 : n
        x = (image_matrix(i,j) - A_k(i,j));
        sum_3 = sum_3 + x*x;
      endfor
    endfor
   
    sum_vec_3(q) = sum_3;
  endfor
  figure(3)
  plot(sum_vec_3/(m*n));  
  
   %Graphic 4 
   
  for q = 1 : length
    sum_4(q) = (2*q + 1)/(n);
  endfor
  
  figure(4);
  plot(sum_4);
  
 end
