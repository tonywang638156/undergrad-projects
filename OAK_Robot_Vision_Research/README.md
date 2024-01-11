# OAK-D PRO Project


## File Overview
Here are the main directories you need to worry about this semester:
+ `stereoDepth-doubleModelDetection.py` - this file will perform stereo depth estimation, objection detections from two models (mobilenet vs yolov7)
+ `yolov7-objectDetection` - this file will just simply perform the objection detection using the yolov7 model
+ `yolov8-humanPoseEstimation` - this file will perform the human pose estimation on multiple people using yolov8 model
+ `blob` - the [blob] files represented the converted models that can be used on the OAK-D Camera

## Model Conversion
+ Begin with converting the model into onnx form using the built-in conversion file located in the repo where you get your model.
+ Navigate to this web: http://blobconverter.luxonis.com/ where you can convert any onnx model to blob forms (easiest way doing model conversion)
+ More comprehensive way of doing conversion is using openvino manually, please refer to this link to start: https://docs.openvino.ai/2023.2/openvino_docs_MO_DG_prepare_model_convert_model_tutorials.html

## Helpful link to start
Here are some helpful links that're beginner friendly:
+ https://www.youtube.com/watch?v=e_uPEE_zlDo
+ https://www.youtube.com/watch?v=fx4uXLQWWi4
+ https://learnopencv.com/anti-spoofing-face-recognition-system-using-oak-d-and-depthai/
+ https://learnopencv.com/depthai-pipeline-overview-creating-a-complex-pipeline/
+ https://learnopencv.com/object-detection-with-depth-measurement-with-oak-d/
