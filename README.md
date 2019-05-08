# Automatic Video Markup Tool

## Installation

### Linux

- Install building dependencies
```Shell
sudo apt-get update
sudo apt-get install g++ cmake git
```

- Install app dependencies
```Shell
sudo pip3 install torchvision torch numba filterpy opencv-python
sudo apt-get install libboost-all-dev libopencv-dev qt5-default
```

- Build app
```Shell
git clone https://github.com/stasysp/markup_tool
mkdir build
cd build
cmake ..
make
```
- Download weights
```Shell
wget https://pjreddie.com/media/files/yolov3.weights data/models/yolo3/yolov3.weights
```

- From ../build-dir/ run app
```Shell
./markup_tool
```
