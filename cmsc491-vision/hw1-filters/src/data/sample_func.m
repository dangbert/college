function [im2 im3] = sample_func(im1, x, y)
im2 = im1(y:end, x:end, :);
im3 = im1(1:y, 1:x, :);
