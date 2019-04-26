

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
# import json

from models import *
from utils import utils
from sort import Sort

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


class BBox:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height


class Detection:
    def __init__(self, frame_index, bbox, id, confidence):
        self.bbox = bbox
        self.id = id
        self.confidence = confidence
        self.frame_index = frame_index

    def as_string(self):
        return ','.join(map(str, [
            self.frame_index,
            self.id,
            self.bbox.x,
            self.bbox.y,
            self.bbox.width,
            self.bbox.height,
            self.confidence,
            0  # dummy object type
        ]))


def process_video(tracker, detector, imgs_dir,
                  supported_extensions=('.png', '.jpg'),
                  return_classes=['person'],
                  debug_dir=None):

    cmap = plt.get_cmap('tab20b')
    colors = [cmap(i)[:3] for i in np.linspace(0, 1, 20)]

    imgs_list = os.listdir(imgs_dir)
    imgs_list.sort()

    tracked_detections = []

    def valid_extention(img_name):
        return img_name.endswith(supported_extensions)

    imgs_list = filter(valid_extention, imgs_list)

    for frame_index, img_name in enumerate(imgs_list):

        img, shape = load_img(os.path.join(imgs_dir, img_name))
        frame_detections = detector.apply(img)
        tracked_objects = tracker.update(frame_detections.cpu())

        pad_x = max(shape[0] - shape[1], 0) * (detector.img_size / max(shape))
        pad_y = max(shape[1] - shape[0], 0) * (detector.img_size / max(shape))
        unpad_h = detector.img_size - pad_y
        unpad_w = detector.img_size - pad_x

        print("tracked:", tracked_objects.shape)

        if debug_dir is not None:
            img = np.array(img)
            print('shape:', img.shape)
            img = img[:, :, ::-1].copy()  # rgb -> bgr

        for x1, y1, x2, y2, obj_id, cls_pred in tracked_objects:
            box_h = int(((y2 - y1) / unpad_h) * shape[0])
            box_w = int(((x2 - x1) / unpad_w) * shape[1])
            y1 = int(((y1 - pad_y // 2) / unpad_h) * shape[0])
            x1 = int(((x1 - pad_x // 2) / unpad_w) * shape[1])

            if detector.classes[int(cls_pred)] in return_classes:
                detection = Detection(frame_index, BBox(x1, y1, box_w, box_h), obj_id, confidence=1.)
                tracked_detections.append(detection)

                if debug_dir is not None:
                    color = colors[int(obj_id) % len(colors)]
                    color = [int(i * 255) for i in color]

                    cv2.rectangle(img, (x1, y1), (x1 + box_w, y1 + box_h), color=tuple(color), thickness=4)
                    # cv2.rectangle(img, (x1, y1 - 35), (x1 + len(cls) * 19 + 60, y1), color, -1)
                    cv2.putText(img, "id_{}".format(int(obj_id)), (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 1,
                                (255, 255, 255), 3)

        if debug_dir is not None:
            cv2.imwrite(os.path.join(debug_dir, img_name), img)

    return tracked_detections


def write_tracks(filepath, tracked_detections):
    with open(filepath, 'w') as f:
        f.write('\n'.join([detection.as_string() for detection in tracked_detections]))



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--config_dir', type=str, help='Where to find weights?')
    parser.add_argument('--imgs_dir', type=str, help='Where to find the imgs?')
    parser.add_argument('--tracks_filepath', type=str, help='Where to save the tracks?')
    parser.add_argument('--detector_type', nargs='?', default='yolo3', type=str, help='Which model to use?')
    parser.add_argument('--debug_dir', nargs='?', default=None, type=str, help='Store images with detections.')
    args = parser.parse_args()

    '''class Args:
        pass
    args = Args()
    args.config_dir = '/Users/user/SHAD/ML_project/markup_tool/data/models/yolo3'
    args.imgs_dir = '/Users/user/SHAD/ML_project/markup_tool/data/test/MOT16-04/img1'
    args.tracks_filepath = '/Users/user/SHAD/ML_project/build/tracks.txt'
    args.debug_dir = '/Users/user/SHAD/ML_project/build/debug_detections'
    args.detector_type = 'yolo3'
    '''

    '''
    class Args:
        pass
    args = Args()
    base_dir = '/home/user/Desktop/ml_project'
    # args.config_dir = os.path.join(base_dir, '/markup_tool/data/models/yolo3')
    # args.imgs_dir = os.path.join(base_dir, '/markup_tool/data/test/MOT16-04/img1')
    # args.tracks_filepath = os.path.join(base_dir, '/tracks.txt')
    # args.debug_dir = os.path.join(base_dir, 'debug_detections')
    args.config_dir = '/home/user/Desktop/ml_project/markup_tool/data/models/yolo3'
    args.imgs_dir = '/home/user/Desktop/ml_project/markup_tool/data/test/MOT16-04/img1'
    args.tracks_filepath = '/home/user/Desktop/ml_project/tracks.txt'
    args.debug_dir = '/home/user/Desktop/ml_project/debug_detections'
    args.detector_type = 'yolo3'
    '''

    assert os.path.exists(args.config_dir), 'No such config dir: {}'.format(args.config_dir)
    assert os.path.exists(args.imgs_dir), 'No such img dir: {}'.format(args.imgs_dir)
    # assert os.path.exists(args.tracks_filepath), 'No such tracks: {}'.format(args.tracks_filepath)

    non_maximum_suppression = NonMaximumSuppression()

    detector = make_detector(args.detector_type, args.config_dir, non_maximum_suppression)
    tracker = Sort()

    tracked_detections = process_video(tracker, detector, args.imgs_dir, debug_dir=args.debug_dir)
    write_tracks(args.tracks_filepath, tracked_detections)
