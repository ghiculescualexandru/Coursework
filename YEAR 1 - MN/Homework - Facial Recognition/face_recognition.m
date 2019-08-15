% 315CB_Ghiculescu_Alexandru
function [min_dist output_img_index] = face_recognition(image_path, m, A, eigenfaces, pr_img)
    
  min_dist = inf;
  output_img_index = 0;    
  T = [];
  V = [];
  
  % Step 6
  % read image
  M = double(rgb2gray(imread(image_path)));
  T = M(:);
  T = T - m;    
    
  % Step 7 
  PrTestImg = eigenfaces' * T;
    
  % Step 8   
  [n m] = size(pr_img);    
  % find the minimum distance and the image index
  for i = 1 : n 
    x = norm(PrTestImg - pr_img(:,i));
    if x < min_dist
      min_dist = x;
      output_img_index = i;
    endif
  endfor
  
end