#! /usr/bin/env python
import os
import rospkg

rospack = rospkg.RosPack()
apriltag_dir = rospack.get_path('apriltag_ros')
source_dir = apriltag_dir + '/../tag36h11'

original_width = 10
tag_width = 400
tag_with_border_width = 500
resize_scale = float(tag_with_border_width)/original_width*100
image_folder = '../images/apriltag_36h11'

for i in xrange(587):
  cmd = 'cp %s/tag36_11_%05d.png %s/apriltag_36h11_%04d.png' % (source_dir, i, image_folder, i)
  print cmd + '\n'
  os.system(cmd)
  cmd = 'convert %s/apriltag_36h11_%04d.png -scale %.2f%% %s/apriltag_36h11_%04d.png' % (image_folder, i, resize_scale, image_folder, i)
  print cmd + '\n'
  os.system(cmd)
