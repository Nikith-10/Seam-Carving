# Seam-Carving

Implements Seam Carving algorithm to reduce aspect ratio of images

The objective of this seam carving algorithm is to perform content aware resizing of images. This allows image to be resized without losing meaningful content from cropping or scaling. The idea is to locate the image's optimal seams, connected pixel paths going from top to bottom or left to right, to remove or insert while preserving the photorealism of the image. Furthermore, manipulating the gradient energy map that describes how optimal a seam is allows for functionality such as object removal. 
