For this project I went through like 3 iterations of different strategies for tackling traffic light detection.  So these notes below are a mess.

But it should be noted that I based a good portion of my project off of:
* [bikz05/object-detector](https://github.com/bikz05/object-detector)


---
## Installation:
* https://github.com/tensorflow/models/tree/master/research/object_detection
* https://www.youtube.com/watch?v=COlbP62-B-U
* Install cuda: 9.0 (9.1 not working with tensorflow) https://developer.nvidia.com/cuda-toolkit
* Install cuDNN https://developer.nvidia.com/rdp/cudnn-download
* Add a windows environment variable named PYTHONPATH and set it to C:\Users\Daniel\Desktop\vision\models\research;C:\Users\Daniel\Desktop\vision\models\research\slim
* (at this point you should be able to run the model_builder_test.py script successfully, and the object_detection_tutorial.ipynb

## Training/Test Data
* Downloaded from https://hci.iwr.uni-heidelberg.de/node/6132
* (Using rgb_train and rgb_test folders)

## run  python setup.py install ??

## Converting .yaml to tfrecords
Need to convert train.yaml and test.yaml to tfrecords
* https://github.com/bosch-ros-pkg/bstld
* https://github.com/datitran/raccoon_dataset/blob/master/generate_tfrecord.py
* http://www.machinelearninguru.com/deep_learning/tensorflow/basics/tfrecord/tfrecord.html
* https://www.skcript.com/svr/why-every-tensorflow-developer-should-know-about-tfrecord/

### Dataset Stats and Labels:
Found using:

````bash
./data_stats.py train.yaml    # this script comes from: https://github.com/bosch-ros-pkg/bstld
````
(NOTE: "occluded" is not a label. it's just a count of the lights that were partially covered in the image set)

#### For train.yaml:
Number of images: 5093, Number of traffic lights: 10756

* Green: 5207
* occluded: 170
* Yellow: 444
* RedLeft: 1092
* Red: 3057
* GreenLeft: 178
* off: 726
* GreenRight: 13
* GreenStraight: 20
* GreenStraightRight: 3
* RedRight: 5
* RedStraight: 9
* RedStraightLeft: 1
* GreenStraightLeft: 1


#### For test.yaml:
Number of images: 8334, Number of traffic lights: 13486

* Green: 7569
* occluded: 2088
* off: 442
* Red: 5321
* Yellow: 154

#### Project Decisions:
* I am removing images that have no labels from the training set.

How to convert the yaml files to tfrecords:
* copy all training images out of their folders (e.g. 2015-10-05-10-52-01_bag) into a single directory first
````bash
./make_tf.py --input_path=`pwd`/test.yaml --img_path="C:/Users/Daniel/Desktop/vision/data/rgb_test/test" --output_path=`pwd`/test.tfrecord
# processed 8334 files (intentionally didn't skip images that contained no labels)

./make_tf.py --input_path=`pwd`/train.yaml --img_path="C:/Users/Daniel/Desktop/vision/data/rgb_train/train" --output_path=`pwd`/train.tfrecord
# processed 3153 files (any files without labels were omitted)

````

### Training a Model (mobilenet_v1):
* [models to choose from](https://github.com/tensorflow/models/blob/master/research/object_detection/g3doc/detection_model_zoo.md)
* click on the model ssd_mobilenet_v1_coco and download and extract it to researc/object_detection (delete existing if present)
* Copy and edit the corresponding configuration file from \research\object_detection\samples\configs
* Instructions: https://www.youtube.com/watch?v=JR8CmWyh2E8
* Set num_classes (12 for this project)
* (lower batch size later if getting a memory error)
* create folder called training in research/object_detection

````bash
# Start Training!
python train.py --logtostderr --train_dir=training/ --pipeline_config_path=../../../traffic-lights/src/ssdlite_mobilenet_v1_coco.config

# open tensorboard:
tensorboard --logdir=training/
````

Troubleshooting:
````bash
# raccoon train command:
python train.py --logtostderr --train_dir=training/ --pipeline_config_path=../../../raccoon_dataset/training/ssd_mobilenet_v1_pets.config
````
* Due to [this](https://github.com/tensorflow/models/issues/3922) bug I had to checkout the models repository from commit 56abd77 to get training to get the training command to actually work.
* When I did start training the first time using my traffic tfrecords, the loss never consistenly decreased it kept increasing (probably an issue with how the tfrecords were made).
* Then I ran the model using the tfrecords and config from the [raccoon dataset](https://github.com/datitran/raccoon_dataset) and it performed much better, but crashed after a while (likely due to running out of vram).
* After training stops, you must kill the tensorboard session before you can start training again (use the same command again to resume training).


### Export and Run Model:
Based on [this](https://github.com/tensorflow/models/blob/master/research/object_detection/g3doc/exporting_models.md):
````bash
# From tensorflow/models/research/
# MUST set the correct checkpoint number below (e.g. model.ckpt-1030)
python object_detection/export_inference_graph.py \
    --input_type image_tensor \
    --pipeline_config_path ../../raccoon_dataset/training/ssd_mobilenet_v1_pets.config \
    --trained_checkpoint_prefix "C:/Users/Daniel/Desktop/vision/models/research/object_detection/training/model.ckpt-1030" \
    --output_directory "C:/Users/Daniel/Desktop/vision/models/research/object_detection/ssd_mobilenet_v1_coco_2017_11_17/raccoon"
````

To get running, just make some tweaks to the "Model preparation" section of object_detection_tutorial.ipynb (in the models repository)
