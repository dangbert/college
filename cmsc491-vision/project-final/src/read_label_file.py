#!/usr/bin/env python
"""
Sample script to receive traffic light labels and images
of the Bosch Small Traffic Lights Dataset.

Example usage:
    python read_label_file.py input_yaml
"""
# this file adapted from: https://github.com/bosch-ros-pkg/bstld

import os
import sys
import yaml    # pip install pyyaml


def get_all_labels(input_yaml, riib=False):
    """ Gets all labels within label file

    Note that RGB images are 1280x720 and RIIB images are 1280x736.
    :param input_yaml: Path to yaml file
    :param riib: If True, change path to labeled pictures
    :return: images: Labels for traffic lights
    """
    images = yaml.load(open(input_yaml, 'rb').read())

    count = 0
    for i in range(len(images)):
        images[i]['path'] = os.path.abspath(os.path.join(os.path.dirname(input_yaml), images[i]['path']))
        if riib:
            images[i]['path'] = images[i]['path'].replace('.png', '.pgm')
            images[i]['path'] = images[i]['path'].replace('rgb/train', 'riib/train')
            images[i]['path'] = images[i]['path'].replace('rgb/test', 'riib/test')
            for box in images[i]['boxes']:
                box['y_max'] = box['y_max'] + 8
                box['y_min'] = box['y_min'] + 8

        #for box in images[i]['boxes']:
        #    box['y_max'] = -1 * box['y_max']

        #print(images[i])
        #print(images[i]['path'])
        # change the path to be just the filename (e.g. "1234.jpg")
        count += 1
        fname = images[i]['path'][::-1] # reverse the string
        index = fname.find("/")
        fname = fname[:index][::-1]     # isolate just the file name and reverse back 
        images[i]['path'] = fname
    return images


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(-1)
    images = get_all_labels(sys.argv[1])
    #print(images[:2])
