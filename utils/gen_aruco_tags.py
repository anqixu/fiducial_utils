#! /usr/bin/env python
import os

tag_width = 400
tag_with_border_width = 500
border_ratio = float(tag_with_border_width-tag_width)/tag_width/2*100
image_folder = '../images/aruco/'

for i in xrange(1024):
  cmd = 'aruco_create_marker %d %s/aruco_%04d.png %d' % (i, image_folder, i, tag_width)
  print cmd + '\n'
  os.system(cmd)
  cmd = 'convert %s/aruco_%04d.png -bordercolor white -border %.2f%%x%.2f%% %s/aruco_%04d.png' % (image_folder, i, border_ratio, border_ratio, image_folder, i)
  print cmd + '\n'
  os.system(cmd)
