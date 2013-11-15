Human-Detection
===============

Full source code for Human detection in still image.

This is my Computer Vision course's mid-term project (in 2011).

===============

Test Dataset: INRIAPerson

Test image's size: From 480 x 680 (pixels)  to   1260 x 930 (pixels)

Detection's Time:    0 - 30 (s), correspond to image's size.

Program's Features:

    Self-coding: feature description, and object localization methods. (Do not use OpenCV Object Detection Library).
    
    1. Use HOG as Feature Descriptor, 
    
    2. Use Meanshift for localization and Non-maximum suppression for result convergence
    
    3. For classification, we used polynomial SVM.

Future Objective: continue to optimize the program to be able to detect human in real-time and improve accuracy.


Reference: 

Histograms of Oriented Gradients for Human Detection - Navneet Dalal, Bill Triggs (site: http://lear.inrialpes.fr/pubs/2005/DT05/)


DEMOS:
1. https://www.youtube.com/watch?v=DLtSvSmTcDc (Image resolution: 1260 x 930)

2. Faster detection demo (with smaller image resolution - 640 x 480): 

    https://www.youtube.com/watch?v=O7EVwCGhIoQ
