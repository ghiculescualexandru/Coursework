% This function returns a matrix resulted from
% the SVD process.


function A_k = task1(image, k)

  % read matrix   
  image_matrix = double(imread(image));
  [m n] = size(image_matrix);
    
  % find U S and V
  Uk = U(1:m,1:k);
  Sk = S(1:k,1:k);
  Vk = V(1:k,1:n);
  
  % find the A_k result
  A_k = Uk * Sk * Vk;
  
end
