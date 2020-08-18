clear
close all

%%% define a Gaussian filter
mu = 0; %%mean
sigma = 3; %%standard deviation
x = [-sigma*3:1:sigma*3]; %% at what point you want to sample the pdf
y = exp(-((x-mu).^2)/(2*sigma^2)); %%calculate the pdf value at those points
y = y/sum(y); %% normalize them to sum to 1.

plot(x, y); %%plot y vs. x

y2 = y'*y; %% multipy transposed version of y with itself to get a 2D pdf 
y2 = y2/sum(y2(:)); %% normalize to sum to 1. 

%%note that y2(:) means reshape y2 to be a long vector rather than a 2D matrix

imshow(y2) % this will be very dark since the values are less than 1.
imshow(y2*1000) %%this may show something
imagesc(y2) %% this command scales the values before showing them to be in the range.



%%% this simple code will define a new image with all zeros and then will
%%% loop over all pixels in the image and add 100 to each pixel value and write into the same location in the new image
im = imread('Moire_small.jpg');
im2 = zeros(size(im, 1), size(im, 2)); %% define an image with all zeros
for i = 1:size(im, 1)
  for j = 1:size(im, 2)
    im2(i, j) = im(i, j, 1)+100;
  end
end
imshow(uint8(im2))


%%% Or shift image in "y" direction by 10 pixels
im2 = zeros(size(im, 1), size(im, 2)); %% define an image with all zeros
for i = 1+10:size(im, 1)
  for j = 1:size(im, 2)
    im2(i-10, j) = im(i, j, 1)+100;
  end
end
imshow(uint8(im2))




