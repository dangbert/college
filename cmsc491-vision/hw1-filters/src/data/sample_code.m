clear
close all
clc

im1 = imread('Yosemite.png');
size(im1)

im2 = double(im1);
im3 = uint8(im2);

figure(1)
imshow(im3)

figure(2)
imshow(uint8(im2+50))

[im4 im5] = sample_func(im2, 100, 110);

figure(3)
subplot(1,2,1), 
imshow(uint8(im4))

subplot(1,2,2), 
imshow(uint8(im5))

