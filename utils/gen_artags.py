#! /usr/bin/env python
import os

# requires artag_create_marker binary; cannot be re-distributed currently due to licensing issues

original_width = 100
tag_width = 400
tag_with_border_width = 500
resize_scale = float(tag_width)/original_width*100
border_ratio = float(tag_with_border_width-tag_width)/tag_width/2*100
image_folder = '../images/artag'

for i in xrange(1024):
  cmd = 'artag_create_marker %d %s/artag_%04d.png' % (i, image_folder, i)
  print cmd + '\n'
  os.system(cmd)
  cmd = 'convert %s/artag_%04d.png -trim -scale %.2f%% %s/artag_%04d.png' % (image_folder, i, resize_scale, image_folder, i)
  print cmd + '\n'
  os.system(cmd)
  cmd = 'convert %s/artag_%04d.png -bordercolor white -border %.2f%%x%.2f%% %s/artag_%04d.png' % (image_folder, i, border_ratio, border_ratio, image_folder, i)
  print cmd + '\n'
  os.system(cmd)
