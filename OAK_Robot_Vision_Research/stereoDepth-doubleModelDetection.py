#!/usr/bin/env python3


from pathlib import Path
import sys
import cv2
import depthai as dai
import numpy as np
import time

nnPathMONO = str((Path(__file__) / Path('../mobilenet-ssd_openvino_2021.4_6shave.blob')).resolve().absolute())
if not Path(nnPathMONO).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

labelMapMONO = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", 
            "cow","diningtable", "dog", "horse", "motorbike", "person","pottedplant", "sheep", "sofa", "train", "tvmonitor"]

# get the input width and length of the yolov7 model
model = dai.OpenVINO.Blob("yolov7.blob")
dim = next(iter(model.networkInputs.values())).dims
W, H = dim[:2] # here dim is [320,320,3,1], dim[:2] means [320,320], and W takes the dim[0], H takes the dim[1]

# Get argument first
nnPath = str((Path(__file__) / Path('../yolov7.blob')).resolve().absolute())

if not Path(nnPath).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

# tiny yolo v7 label texts
labelMap = [
    "person",         "bicycle",    "car",           "motorbike",     "aeroplane",   "bus",           "train",
    "truck",          "boat",       "traffic light", "fire hydrant",  "stop sign",   "parking meter", "bench",
    "bird",           "cat",        "dog",           "horse",         "sheep",       "cow",           "elephant",
    "bear",           "zebra",      "giraffe",       "backpack",      "umbrella",    "handbag",       "tie",
    "suitcase",       "frisbee",    "skis",          "snowboard",     "sports ball", "kite",          "baseball bat",
    "baseball glove", "skateboard", "surfboard",     "tennis racket", "bottle",      "wine glass",    "cup",
    "fork",           "knife",      "spoon",         "bowl",          "banana",      "apple",         "sandwich",
    "orange",         "broccoli",   "carrot",        "hot dog",       "pizza",       "donut",         "cake",
    "chair",          "sofa",       "pottedplant",   "bed",           "diningtable", "toilet",        "tvmonitor",
    "laptop",         "mouse",      "remote",        "keyboard",      "cell phone",  "microwave",     "oven",
    "toaster",        "sink",       "refrigerator",  "book",          "clock",       "vase",          "scissors",
    "teddy bear",     "hair drier", "toothbrush"
]

syncNN = True

# Create pipeline
pipeline = dai.Pipeline()

# Define sources and outputs
camRgb = pipeline.create(dai.node.ColorCamera)
detectionNetwork = pipeline.create(dai.node.YoloDetectionNetwork)
monoRight = pipeline.createMonoCamera()
monoLeft = pipeline.createMonoCamera()
stereo = pipeline.createStereoDepth()
manip = pipeline.createImageManip()
nn = pipeline.createMobileNetDetectionNetwork()


xoutRgb = pipeline.create(dai.node.XLinkOut)
nnOutRGB = pipeline.create(dai.node.XLinkOut)
nnOutMONO = pipeline.createXLinkOut()
disparityOut = pipeline.createXLinkOut()
xoutRight = pipeline.createXLinkOut()



xoutRgb.setStreamName("rgb")
nnOutRGB.setStreamName("nnRGB")
disparityOut.setStreamName("disparity")
xoutRight.setStreamName("rectifiedRight")
nnOutMONO.setStreamName("nnMONO")

# Properties
monoRight.setBoardSocket(dai.CameraBoardSocket.RIGHT)
monoRight.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)

# must to make sure the output of the color cam (input of the detection NN) are the same as wxh of yolov7
camRgb.setPreviewSize(W, H) 
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_800_P)
camRgb.setInterleaved(False)
camRgb.setColorOrder(dai.ColorCameraProperties.ColorOrder.BGR)
camRgb.setFps(100)

# stereo property
stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
stereo.setRectifyEdgeFillColor(0)  

# imageManip property
manip.initialConfig.setResize(300, 300) # because the size of the mobilenet model is 300x300
manip.initialConfig.setFrameType(dai.ImgFrame.Type.BGR888p)

# Network specific settings
detectionNetwork.setConfidenceThreshold(0.5)
detectionNetwork.setNumClasses(80) # there are 80 objects can be detected in the labelMap of Yolo model.
detectionNetwork.setCoordinateSize(4)
detectionNetwork.setAnchors(
    [
        12, 16, 19, 36, 40, 28,
        36, 75, 76, 55, 72, 146,
        142, 110, 192, 243, 459, 401
    ]
)
detectionNetwork.setAnchorMasks(
    {
        "side{}".format(W // 8): [0, 1, 2],
        "side{}".format(W // 16): [3, 4, 5],
        "side{}".format(W // 32): [6, 7, 8],
    }
)
detectionNetwork.setIouThreshold(0.5)
detectionNetwork.setBlobPath(nnPath)
detectionNetwork.setNumInferenceThreads(2)
detectionNetwork.input.setBlocking(False)

nn.setConfidenceThreshold(0.5)
nn.setBlobPath(nnPathMONO)
nn.setNumInferenceThreads(2)
nn.input.setBlocking(False)

# Linking
camRgb.preview.link(detectionNetwork.input)
if syncNN:
    detectionNetwork.passthrough.link(xoutRgb.input)
else:
    camRgb.preview.link(xoutRgb.input)

detectionNetwork.out.link(nnOutRGB.input)

monoRight.out.link(stereo.right)
monoLeft.out.link(stereo.left)
stereo.rectifiedRight.link(manip.inputImage)
stereo.disparity.link(disparityOut.input)
manip.out.link(nn.input)
manip.out.link(xoutRight.input)
nn.out.link(nnOutMONO.input)


# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    qRight = device.getOutputQueue("rectifiedRight", maxSize=16, blocking=False)
    qDisparity = device.getOutputQueue("disparity", maxSize=16, blocking=False)
    qDetMONO = device.getOutputQueue("nnMONO", maxSize=16, blocking=False)

    rightFrame = None
    disparityFrame = None
    detectionsMONO = []

    # Output queues will be used to get the rgb frames and nn data from the outputs defined above
    qRgb = device.getOutputQueue(name="rgb", maxSize=16, blocking=False)
    qDet = device.getOutputQueue(name="nnRGB", maxSize=16, blocking=False)

    frame = None
    detections = []
    startTime = time.monotonic()
    counter = 0
    color2 = (255, 255, 255)

    # nn data, being the bounding box locations, are in <0..1> range - they need to be normalized with frame width/height
    def frameNorm(frame, bbox):
        normVals = np.full(len(bbox), frame.shape[0])
        normVals[::2] = frame.shape[1]
        return (np.clip(np.array(bbox), 0, 1) * normVals).astype(int)

    def displayFrame(name, frame):
        color = (255, 0, 0)
        for detection in detections:
            bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.putText(frame, labelMap[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, 255)
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color, 2)
        # Show the frame
        cv2.imshow(name, frame)

    def show(name, frame):
        color = (255, 0, 0)
        for detection in detectionsMONO:
            bbox = frameNorm(frame, (detection.xmin, detection.ymin, detection.xmax, detection.ymax))
            cv2.putText(frame, labelMapMONO[detection.label], (bbox[0] + 10, bbox[1] + 20), cv2.FONT_HERSHEY_TRIPLEX, 0.5, color)
            cv2.putText(frame, f"{int(detection.confidence * 100)}%", (bbox[0] + 10, bbox[1] + 40), cv2.FONT_HERSHEY_TRIPLEX, 0.5, color)
            cv2.rectangle(frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color, 2)
      
        cv2.imshow(name, frame)

    disparityMultiplier = 255 / stereo.initialConfig.getMaxDisparity()

    while True:
        if syncNN:
            inRgb = qRgb.get()
            inDet = qDet.get()
        else:
            inRgb = qRgb.tryGet()
            inDet = qDet.tryGet()

        if inRgb is not None:
            frame = inRgb.getCvFrame()
            cv2.putText(frame, "NN fps: {:.2f}".format(counter / (time.monotonic() - startTime)),
                        (2, frame.shape[0] - 4), cv2.FONT_HERSHEY_TRIPLEX, 0.4, color2)

        if inDet is not None:
            detections = inDet.detections
            counter += 1

        if frame is not None:
            displayFrame("rgb", frame)

        if qDetMONO.has():
            detectionsMONO = qDetMONO.get().detections

        if qRight.has():
            rightFrame = qRight.get().getCvFrame()

        if qDisparity.has(): 
            disparityFrame = qDisparity.get().getFrame()
            disparityFrame = (disparityFrame*disparityMultiplier).astype(np.uint8)
          
            disparityFrame = cv2.applyColorMap(disparityFrame, cv2.COLORMAP_JET)
            show("disparity", disparityFrame)

        if rightFrame is not None:
            show("rectified right", rightFrame)

        if cv2.waitKey(1) == ord('q'):
            break
