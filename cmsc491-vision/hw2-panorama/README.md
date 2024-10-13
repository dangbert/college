[See my report for this assignment here](https://raw.githubusercontent.com/dangbert/college/master/cmsc491-vision/hw2-panorama/paper/report.pdf)

Note usage for custom photos in 2024:

````bash
# this conversion (in place) was necessary to prevent script crashing
cd myfolder
ls *.JPG | xargs -I {} ffmpeg -i {} -vf "format=yuvj422p" -y {}
````
