from models import *
from utils import utils
from sort import Sort

import os, sys, time, datetime, random
import cv2
import torch
from torch.utils.data import DataLoader
from torchvision import datasets, transforms
from torch.autograd import Variable

import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image

import argparse

import os
import time
import json

'''
config_path='config/yolov3.cfg'
weights_path='config/yolov3.weights'
class_path='config/coco.names'

# img_size=416
# conf_thres=0.8
# nms_thres=0.4

# model = Darknet(config_path, img_size=img_size)
# tracker = Sort()
# model.load_weights(weights_path)
# model.cuda()
# model.eval()
classes = utils.load_classes(class_path)
Tensor = torch.FloatTensor  # torch.cuda.FloatTensor

def detect_image(img):
    ratio = min(img_size/img.size[0], img_size/img.size[1])
    imw = round(img.size[0] * ratio)
    imh = round(img.size[1] * ratio)
    img_transforms = transforms.Compose([ transforms.Resize((imh, imw)),
         transforms.Pad((max(int((imh-imw)/2),0), max(int((imw-imh)/2),0), max(int((imh-imw)/2),0), max(int((imw-imh)/2),0)),
                        (128,128,128)),
         transforms.ToTensor(),
         ])
    image_tensor = img_transforms(img).float()
    image_tensor = image_tensor.unsqueeze_(0)
    input_img = Variable(image_tensor.type(Tensor))
    with torch.no_grad():
        detections = model(input_img)
        detections = utils.non_max_suppression(detections, 80, conf_thres, nms_thres)
    return detections[0]'''

'''
cmap = plt.get_cmap('tab20b')
colors = [cmap(i)[:3] for i in np.linspace(0, 1, 20)]


for frame in frames:
    if not frame.endswith('.jpg'):
        continue

    frame = cv2.imread(os.path.join(data_dir, frame))
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    pilimg = Image.fromarray(frame)
    detections = detect_image(pilimg)

    img = np.array(pilimg)
    pad_x = max(img.shape[0] - img.shape[1], 0) * (img_size / max(img.shape))
    pad_y = max(img.shape[1] - img.shape[0], 0) * (img_size / max(img.shape))
    unpad_h = img_size - pad_y
    unpad_w = img_size - pad_x
    if detections is not None:

        unique_labels = detections[:, -1].cpu().unique()
        n_cls_preds = len(unique_labels)
        for x1, y1, x2, y2, obj_id, cls_pred in tracked_objects:
            box_h = int(((y2 - y1) / unpad_h) * img.shape[0])
            box_w = int(((x2 - x1) / unpad_w) * img.shape[1])
            y1 = int(((y1 - pad_y // 2) / unpad_h) * img.shape[0])
            x1 = int(((x1 - pad_x // 2) / unpad_w) * img.shape[1])

            color = colors[int(obj_id) % len(colors)]
            color = [i * 255 for i in color]
            cls = classes[int(cls_pred)]
            if 'person'==cls:
                cv2.rectangle(frame, (x1, y1), (x1+box_w, y1+box_h), color, 4)
                cv2.rectangle(frame, (x1, y1-35), (x1+len(cls)*19+60, y1), color, -1)
                cv2.putText(frame, cls + "-" + str(int(obj_id)), (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255), 3)

    fig=plt.figure(figsize=(12, 8))
    plt.title("Video Stream")
    plt.imshow(frame)
    plt.show()
'''

class NonMaximumSuppression:
    def __init__(self, num_classes=80,
                 nms_conf_thres=0.4,
                 conf_thres=0.8):
        self.nms_conf_thres = nms_conf_thres
        self.conf_thres = conf_thres
        self.num_classes = num_classes
    pass

    def apply(self, prediction):
        """
        Removes detections with lower object confidence score than 'conf_thres' and performs
        Non-Maximum Suppression to further filter detections.
        Returns detections with shape:
            (x1, y1, x2, y2, object_conf, class_score, class_pred)
        """

        # From (center x, center y, width, height) to (x1, y1, x2, y2)
        box_corner = prediction.new(prediction.shape)
        box_corner[:, :, 0] = prediction[:, :, 0] - prediction[:, :, 2] / 2
        box_corner[:, :, 1] = prediction[:, :, 1] - prediction[:, :, 3] / 2
        box_corner[:, :, 2] = prediction[:, :, 0] + prediction[:, :, 2] / 2
        box_corner[:, :, 3] = prediction[:, :, 1] + prediction[:, :, 3] / 2
        prediction[:, :, :4] = box_corner[:, :, :4]

        output = [None for _ in range(len(prediction))]
        for image_i, image_pred in enumerate(prediction):
            # Filter out confidence scores below threshold
            conf_mask = (image_pred[:, 4] >= self.conf_thres).squeeze()
            image_pred = image_pred[conf_mask]
            # If none are remaining => process next image
            if not image_pred.size(0):
                continue
            # Get score and class with highest confidence
            class_conf, class_pred = torch.max(image_pred[:, 5 : 5 + self.num_classes], 1, keepdim=True)
            # Detections ordered as (x1, y1, x2, y2, obj_conf, class_conf, class_pred)
            detections = torch.cat((image_pred[:, :5], class_conf.float(), class_pred.float()), 1)
            # Iterate through all predicted classes
            unique_labels = detections[:, -1].cpu().unique()
            if prediction.is_cuda:
                unique_labels = unique_labels.cuda()
            for c in unique_labels:
                # Get the detections with the particular class
                detections_class = detections[detections[:, -1] == c]
                # Sort the detections by maximum objectness confidence
                _, conf_sort_index = torch.sort(detections_class[:, 4], descending=True)
                detections_class = detections_class[conf_sort_index]
                # Perform non-maximum suppression
                max_detections = []
                while detections_class.size(0):
                    # Get detection with highest confidence and save as max detection
                    max_detections.append(detections_class[0].unsqueeze(0))
                    # Stop if we're at the last detection
                    if len(detections_class) == 1:
                        break
                    # Get the IOUs for all boxes with lower confidence
                    ious = utils.bbox_iou(max_detections[-1], detections_class[1:])
                    # Remove detections with IoU >= NMS threshold
                    detections_class = detections_class[1:][ious < self.nms_conf_thres]

                max_detections = torch.cat(max_detections).data
                # Add max detections to outputs
                output[image_i] = (
                    max_detections if output[image_i] is None else torch.cat((output[image_i], max_detections))
                )

        return output

class Yolo3:
    def __init__(self, model_path,
                 img_size=416,
                 non_maximum_suppression=None):
        self.img_size = img_size

        self.config_path = os.path.join(model_path, 'yolov3.cfg')
        self.weights_path = os.path.join(model_path, 'yolov3.weights')
        self.class_path = os.path.join(model_path, 'coco.names')

        self.non_maximum_suppression = non_maximum_suppression

        self.model = Darknet(self.config_path, img_size=img_size)
        self.model.load_weights(self.weights_path)
        self.model.eval()

        self.classes = utils.load_classes(self.class_path)



    def prepreprocess(self, img):
        ratio = min(self.img_size / img.size[0], self.img_size / img.size[1])

        imw = round(img.size[0] * ratio)
        imh = round(img.size[1] * ratio)

        img_transforms = transforms.Compose([
            transforms.Resize((imh, imw)),
            transforms.Pad((max(int((imh - imw) / 2), 0),
                            max(int((imw - imh) / 2), 0),
                            max(int((imh - imw) / 2), 0),
                            max(int((imw - imh) / 2), 0)),
                            (128, 128, 128)),
            transforms.ToTensor(),
        ])

        image_tensor = img_transforms(img).float()
        image_tensor = image_tensor.unsqueeze_(0)
        input_img = Variable(image_tensor.type(torch.FloatTensor))

        return input_img

    def apply(self, img):
        preped_img = self.prepreprocess(img)

        with torch.no_grad():
            detections = self.model(preped_img)
            if self.non_maximum_suppression is not None:
                detections = self.non_maximum_suppression.apply(detections)

            detections = detections[0]

        return detections


def make_detector(detector_type, model_path, non_maximum_suppression):
    if detector_type == 'yolo3':
        return Yolo3(model_path, non_maximum_suppression=non_maximum_suppression)
    else:
        raise ValueError("Unknown detector dype: {}".format(detector))


def load_img(filepath):
    frame = cv2.imread(filepath)
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    pil_img = Image.fromarray(frame)
    return pil_img, frame.shape

class Detection:
    def __init__(self):
        self.bbox_x = None
        self.x = None

def process_video(tracker, detector, imgs_dir,
                  supported_extensions=('.png', '.jpg'),
                  return_classes=['person']):
    imgs_list = os.listdir(imgs_dir)
    imgs_list.sort()

    tracked_detections = {}
    tracked_detections['tracks'] = []
    for img_name in imgs_list:
        if not img_name.endswith(supported_extensions):
            continue

        img, shape = load_img(os.path.join(imgs_dir, img_name))
        frame_detections = detector.apply(img)
        tracked_objects = tracker.update(frame_detections.cpu())

        pad_x = max(shape[0] - shape[1], 0) * (detector.img_size / max(shape))
        pad_y = max(shape[1] - shape[0], 0) * (detector.img_size / max(shape))
        unpad_h = detector.img_size - pad_y
        unpad_w = detector.img_size - pad_x

        print("tracked:", tracked_objects.shape)

        for x1, y1, x2, y2, obj_id, cls_pred in tracked_objects:
            box_h = int(((y2 - y1) / unpad_h) * img.shape[0])
            box_w = int(((x2 - x1) / unpad_w) * img.shape[1])
            y1 = int(((y1 - pad_y // 2) / unpad_h) * img.shape[0])
            x1 = int(((x1 - pad_x // 2) / unpad_w) * img.shape[1])



            color = colors[int(obj_id) % len(colors)]
            color = [i * 255 for i in color]
            cls = classes[int(cls_pred)]
            if detector.classes[int(cls_pred)] in return_classes:
                tracked_detections['tracks'].append({'frame' : img_name.split('.')[0], 'id': obj_id,
                                           'class' : cls_pred, 'x' : x1, 'y' : y1, 'width' : box_w, 'height' : box_h,
                                           'confidence' : 1.})

        return tracked_detections


def write_tracks(tracked_detections, fname):
    with open(fname + '.json', 'w') as outfile:
        json.dump(tracked_detections, outfile)


if __name__ == '__main__':
    # parser = argparse.ArgumentParser(description='Process some integers.')
    # parser.add_argument('--model_dir', default='', type=str, help='Where to find model files?')
    # parser.add_argument('--imgs_dir', type=str, help='Where to find the imgs?')
    # parser.add_argument('--detector_type', nargs='?', default='yolo3', type=str, help='Which model to use?')
    # parser.add_argument('--debug_dir', nargs='?', default=None, type=str, help='Store images with detections.')
    # args = parser.parse_args()
    class Args:
        pass
    args = Args()
    args.model_dir = '/Users/user/SHAD/ML_project/markup_tool/data/models/yolo3'
    args.imgs_dir = '/Users/user/SHAD/ML_project/markup_tool/data/test/MOT16-04/img1'
    args.detector_type = 'yolo3'

    non_maximum_suppression = NonMaximumSuppression()

    detector = make_detector(args.detector_type, args.model_dir, non_maximum_suppression)
    tracker = Sort()

    tracked_detections = process_video(tracker, detector, args.imgs_dir)
    write_tracks(tracked_detections, args.imgs_dir.split('/')[-1])
