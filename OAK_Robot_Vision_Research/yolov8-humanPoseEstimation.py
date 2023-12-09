from datetime import datetime
from pathlib import Path
import cv2
import depthai as dai
import numpy as np
import collections
import time


def xywh2xyxy(x):
    # Convert nx4 boxes from [x, y, w, h] to [x1, y1, x2, y2] where xy1=top-left, xy2=bottom-right
    y = np.zeros_like(x)
    y[..., 0] = x[..., 0] - x[..., 2] / 2  # top left x
    y[..., 1] = x[..., 1] - x[..., 3] / 2  # top left y
    y[..., 2] = x[..., 0] + x[..., 2] / 2  # bottom right x
    y[..., 3] = x[..., 1] + x[..., 3] / 2  # bottom right y
    return y


def scale_boxes(img1_shape, boxes, img0_shape, ratio_pad=None):
    if ratio_pad is None:  # calculate from img0_shape
        gain = min(
            img1_shape[0] / img0_shape[0], img1_shape[1] / img0_shape[1]
        )  # gain  = old / new
        pad = (img1_shape[1] - img0_shape[1] * gain) / 2, (
            img1_shape[0] - img0_shape[0] * gain
        ) / 2  # wh padding
    else:
        gain = ratio_pad[0][0]
        pad = ratio_pad[1]

    boxes[..., [0, 2]] -= pad[0]  # x padding
    boxes[..., [1, 3]] -= pad[1]  # y padding
    boxes[..., :4] /= gain
    clip_boxes(boxes, img0_shape)
    return boxes


def clip_boxes(boxes, shape, step=None):
    # Clip bounding xyxy bounding boxes to image shape (height, width)

    if step is not None:
        boxes[:, 0::step].clamp_(0, shape[1])  # x1
        boxes[:, 1::step].clamp_(0, shape[0])  # y1
    else:  # np.array (faster grouped)
        boxes[:, [0, 2]] = boxes[:, [0, 2]].clip(0, shape[1])  # x1, x2
        boxes[:, [1, 3]] = boxes[:, [1, 3]].clip(0, shape[0])  # y1, y2


def nms(boxes, scores, nms_thr):
    x1 = boxes[:, 0]
    y1 = boxes[:, 1]
    x2 = boxes[:, 2]
    y2 = boxes[:, 3]

    areas = (x2 - x1 + 1) * (y2 - y1 + 1)
    order = scores.argsort()[::-1]  # get boxes with more ious first

    keep = []
    while order.size > 0:
        i = order[0]  # pick maxmum iou box
        keep.append(i)
        xx1 = np.maximum(x1[i], x1[order[1:]])
        yy1 = np.maximum(y1[i], y1[order[1:]])
        xx2 = np.minimum(x2[i], x2[order[1:]])
        yy2 = np.minimum(y2[i], y2[order[1:]])

        w = np.maximum(0.0, xx2 - xx1 + 1)  # maximum width
        h = np.maximum(0.0, yy2 - yy1 + 1)  # maximum height
        inter = w * h
        ovr = inter / (areas[i] + areas[order[1:]] - inter)

        inds = np.where(ovr <= nms_thr)[0]
        order = order[inds + 1]

    return keep


def non_max_suppression(
    prediction: np.ndarray, conf_thres=0.25, iou_thres=0.3, classes=None,
        nc=0,  # number of classes (optional)
):

    # Checks
    assert (
        0 <= conf_thres <= 1
    ), f"Invalid Confidence threshold {conf_thres}, valid values are between 0.0 and 1.0"
    assert (
        0 <= iou_thres <= 1
    ), f"Invalid IoU {iou_thres}, valid values are between 0.0 and 1.0"

    bs = prediction.shape[0]  # batch size
    nc = nc or (prediction.shape[1] - 4)  # number of classes
    nm = prediction.shape[1] - nc - 4
    mi = 4 + nc
    xc = prediction[:, 4:mi].max(1) > conf_thres  # candidates

    # Settings
    min_wh, max_wh = 2, 7680  # (pixels) minimum and maximum box width and height
    max_det = 300  # maximum number of detections per image
    max_nms = 30000  # maximum number of boxes into torchvision.ops.nms()

    output = [np.zeros((0, 6 + nm))] * bs
    for xi, x in enumerate(prediction):  # image index, image inference
        # Apply constraints
        # x[((x[..., 2:4] < min_wh) | (x[..., 2:4] > max_wh)).any(1), 4] = 0  # width-height

        x = x.transpose(1, 0)[xc[xi]]  # confidence

        # If none remain process next image
        if not x.shape[0]:
            continue

        box, cls, mask, _ = np.split(x, (4, 4+nc, 4+nc+nm), 1)
        box = xywh2xyxy(box)  # center_x, center_y, width, height) to (x1, y1, x2, y2)

        # Detections matrix nx6 (xyxy, conf, cls)
        conf = cls.max(1, keepdims=True)
        j = cls.argmax(1, keepdims=True)
        x = np.concatenate((box, conf, j, mask), 1)[conf.ravel() > conf_thres]

        # Filter by class
        if classes is not None:
            x = x[(x[:, 5:6] == np.array(classes)).any(1)]

        # Check shape
        n = x.shape[0]  # number of boxes
        if not n:  # no boxes
            continue

        # sort by confidence and remove excess boxes
        x = x[x[:, 4].argsort()[::-1][:max_nms]]

        # Batched NMS
        c = x[:, 5:6] * max_wh  # classes
        boxes, scores = x[:, :4] + c, x[:, 4]  # boxes (offset by class), scores
        i = nms(boxes, scores, iou_thres)  # NMS
        i = i[:max_det]  # limit detections

        output[xi] = x[i]

    return output


def plot_skeleton_kpts(im, kpts, steps):
    # Plot the skeleton and keypointsfor coco datatset
    palette = np.array(
        [
            [255, 128, 0],
            [255, 153, 51],
            [255, 178, 102],
            [230, 230, 0],
            [255, 153, 255],
            [153, 204, 255],
            [255, 102, 255],
            [255, 51, 255],
            [102, 178, 255],
            [51, 153, 255],
            [255, 153, 153],
            [255, 102, 102],
            [255, 51, 51],
            [153, 255, 153],
            [102, 255, 102],
            [51, 255, 51],
            [0, 255, 0],
            [0, 0, 255],
            [255, 0, 0],
            [255, 255, 255],
        ]
    )

    skeleton = [
        [16, 14],
        [14, 12],
        [17, 15],
        [15, 13],
        [12, 13],
        [6, 12],
        [7, 13],
        [6, 7],
        [6, 8],
        [7, 9],
        [8, 10],
        [9, 11],
        [2, 3],
        [1, 2],
        [1, 3],
        [2, 4],
        [3, 5],
        [4, 6],
        [5, 7],
    ]

    pose_limb_color = palette[
        [9, 9, 9, 9, 7, 7, 7, 0, 0, 0, 0, 0, 16, 16, 16, 16, 16, 16, 16]
    ]
    pose_kpt_color = palette[[16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9]]
    radius = 5
    num_kpts = len(kpts) // steps

    for kid in range(num_kpts):
        r, g, b = pose_kpt_color[kid]
        x_coord, y_coord = kpts[steps * kid], kpts[steps * kid + 1]
        if not (x_coord % 640 == 0 or y_coord % 640 == 0):
            if steps == 3:
                conf = kpts[steps * kid + 2]
                if conf < 0.5:
                    continue
            cv2.circle(
                im, (int(x_coord), int(y_coord)), radius, (int(r), int(g), int(b)), -1
            )

    for sk_id, sk in enumerate(skeleton):
        r, g, b = pose_limb_color[sk_id]
        pos1 = (int(kpts[(sk[0] - 1) * steps]), int(kpts[(sk[0] - 1) * steps + 1]))
        pos2 = (int(kpts[(sk[1] - 1) * steps]), int(kpts[(sk[1] - 1) * steps + 1]))
        if steps == 3:
            conf1 = kpts[(sk[0] - 1) * steps + 2]
            conf2 = kpts[(sk[1] - 1) * steps + 2]
            if conf1 < 0.5 or conf2 < 0.5:
                continue
        if pos1[0] % 640 == 0 or pos1[1] % 640 == 0 or pos1[0] < 0 or pos1[1] < 0:
            continue
        if pos2[0] % 640 == 0 or pos2[1] % 640 == 0 or pos2[0] < 0 or pos2[1] < 0:
            continue
        cv2.line(im, pos1, pos2, (int(r), int(g), int(b)), thickness=2)
    return im


class FPSHandler:

    _fpsBgColor = (0, 0, 0)
    _fpsColor = (255, 255, 255)
    _fpsType = cv2.FONT_HERSHEY_SIMPLEX
    _fpsLineType = cv2.LINE_AA

    def __init__(self, cap=None, maxTicks=100):
        self._timestamp = None
        self._start = None
        self._framerate = cap.get(cv2.CAP_PROP_FPS) if cap is not None else None
        self._useCamera = cap is None

        self._iterCnt = 0
        self._ticks = {}

        if maxTicks < 2:
            raise ValueError(
                f"Proviced maxTicks value must be 2 or higher (supplied: {maxTicks})"
            )

        self._maxTicks = maxTicks

    def nextIter(self):
        if self._start is None:
            self._start = time.monotonic()

        if not self._useCamera and self._timestamp is not None:
            frameDelay = 1.0 / self._framerate
            delay = (self._timestamp + frameDelay) - time.monotonic()
            if delay > 0:
                time.sleep(delay)
        self._timestamp = time.monotonic()
        self._iterCnt += 1

    def tick(self, name):
        if name not in self._ticks:
            self._ticks[name] = collections.deque(maxlen=self._maxTicks)
        self._ticks[name].append(time.monotonic())

    def tickFps(self, name):
        if name in self._ticks and len(self._ticks[name]) > 1:
            timeDiff = self._ticks[name][-1] - self._ticks[name][0]
            return (len(self._ticks[name]) - 1) / timeDiff if timeDiff != 0 else 0.0
        else:
            return 0.0

    def fps(self):
        if self._start is None or self._timestamp is None:
            return 0.0
        timeDiff = self._timestamp - self._start
        return self._iterCnt / timeDiff if timeDiff != 0 else 0.0

    def printStatus(self):
        print("=== TOTAL FPS ===")
        for name in self._ticks:
            print(f"[{name}]: {self.tickFps(name):.1f}")

    def drawFps(self, frame, name):
        frameFps = f"{name.upper()} FPS: {round(self.tickFps(name), 1)}"
        # cv2.rectangle(frame, (0, 0), (120, 35), (255, 255, 255), cv2.FILLED)
        cv2.putText(
            frame,
            frameFps,
            (5, 15),
            self._fpsType,
            0.5,
            self._fpsBgColor,
            4,
            self._fpsLineType,
        )
        cv2.putText(
            frame,
            frameFps,
            (5, 15),
            self._fpsType,
            0.5,
            self._fpsColor,
            1,
            self._fpsLineType,
        )

        if "nn" in self._ticks:
            cv2.putText(
                frame,
                f"NN FPS:  {round(self.tickFps('nn'), 1)}",
                (5, 30),
                self._fpsType,
                0.5,
                self._fpsBgColor,
                4,
                self._fpsLineType,
            )
            cv2.putText(
                frame,
                f"NN FPS:  {round(self.tickFps('nn'), 1)}",
                (5, 30),
                self._fpsType,
                0.5,
                self._fpsColor,
                1,
                self._fpsLineType,
            )
ROOT = Path(__file__).parent

def create_pipeline():
    blob = "yolov8.blob" # relative path
    model = dai.OpenVINO.Blob(blob)
    dim = next(iter(model.networkInputs.values())).dims
    nnWidth, nnHeight = dim[:2]
    print(f"{nnWidth, nnHeight = }")
    print(f"{blob = }")

    # Create pipeline
    pipeline = dai.Pipeline()

    # Define sources and outputs
    camRgb = pipeline.create(dai.node.ColorCamera)
    detectionNetwork = pipeline.create(dai.node.NeuralNetwork)
    xoutRgb = pipeline.create(dai.node.XLinkOut)
    xoutNN = pipeline.create(dai.node.XLinkOut)

    xoutRgb.setStreamName("image")
    xoutNN.setStreamName("detections")

    # Properties
    camRgb.setPreviewSize(nnWidth, nnHeight)
    camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
    camRgb.setBoardSocket(dai.CameraBoardSocket.RGB)
    camRgb.setInterleaved(False)
    camRgb.setColorOrder(dai.ColorCameraProperties.ColorOrder.BGR)
    camRgb.setFps(60)
    camRgb.setPreviewKeepAspectRatio(False)

    # Network specific settings
    detectionNetwork.setBlob(model)

    # Linking
    camRgb.preview.link(detectionNetwork.input)
    camRgb.preview.link(xoutRgb.input)
    # detectionNetwork.passthrough.link(xoutRgb.input)
    detectionNetwork.out.link(xoutNN.input)

    return pipeline


def run():
    # Connect to device and start pipeline
    with dai.Device(create_pipeline()) as device:
        # Output queues will be used to get the rgb frames and nn data from the outputs defined above
        imageQueue = device.getOutputQueue(name="image", maxSize=4, blocking=False)
        detectQueue = device.getOutputQueue(
            name="detections", maxSize=4, blocking=False
        )

        frame = None
        detections = []

        fpsHandler = FPSHandler()

        def drawText(frame, text, org, color=(255, 255, 255)):
            cv2.putText(
                frame,
                text,
                org,
                cv2.FONT_HERSHEY_SIMPLEX,
                0.5,
                (0, 0, 0),
                4,
                cv2.LINE_AA,
            )
            cv2.putText(
                frame, text, org, cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 1, cv2.LINE_AA
            )

        def displayFrame(name, frame):
            color = (255, 0, 0)

            for detection in detections:
                *bbox, conf, cls = detection[:6]
                bbox = np.array(bbox).astype(int)
                kpts = detection[6:]

                # if int(cls) == 0:
                drawText(
                    frame,
                    f"{conf:.2%}",
                    (bbox[0] + 10, bbox[1] + 35),
                )
                cv2.rectangle(
                    frame, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color, 2
                )
                frame = plot_skeleton_kpts(frame, kpts=kpts, steps=3)

            # Show the frame
            cv2.imshow(name, frame)
            return frame

        def toTensorResult(packet):
            data = {}
            for tensor in packet.getRaw().tensors:
                if tensor.dataType == dai.TensorInfo.DataType.INT:
                    data[tensor.name] = np.array(
                        packet.getLayerInt32(tensor.name)
                    ).reshape(tensor.dims)
                elif tensor.dataType == dai.TensorInfo.DataType.FP16:
                    data[tensor.name] = np.array(
                        packet.getLayerFp16(tensor.name)
                    ).reshape(tensor.dims)
                elif tensor.dataType == dai.TensorInfo.DataType.I8:
                    data[tensor.name] = np.array(
                        packet.getLayerUInt8(tensor.name)
                    ).reshape(tensor.dims)
                else:
                    print("Unsupported tensor layer type: {}".format(tensor.dataType))
            return data

        vid_writer = cv2.VideoWriter(
            ROOT.joinpath(f"result_{datetime.now().strftime( '%Y%m%d_%H%M%S' )}.mp4").as_posix(),
            cv2.VideoWriter_fourcc(*"mp4v"),
            20,
            (320, 320),
        )
        bboxColors = np.random.randint(255, size=3)
        while not device.isClosed():
            batch_bboxes, batch_poses, batch_scores = [], [], []
            imageQueueData = imageQueue.tryGet()
            detectQueueData = detectQueue.tryGet()

            if imageQueueData is not None:
                frame = imageQueueData.getCvFrame()
                fpsHandler.tick("color")

            if detectQueueData is not None:
                pred = toTensorResult(detectQueueData)["output0"]
                fpsHandler.tick("nn")
                detections = non_max_suppression(pred, 0.5, nc=80)[0]

            if frame is not None:
                fpsHandler.drawFps(frame, "color")

                frame = displayFrame("image", frame)
                vid_writer.write(frame)
                frame = None

            if cv2.waitKey(1) == ord("q"):
                vid_writer.release()
                break
# run the program
run()