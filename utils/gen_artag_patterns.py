#! /usr/bin/env python
import os
import cv2

image_folder = '../images/artag'
patt_folder = '../artoolkit_patterns/artag'

show_tags = False

if show_tags:
  cv2.namedWindow("smalltag", cv2.cv.CV_WINDOW_NORMAL)
  cv2.namedWindow("smalltag_ccw90", cv2.cv.CV_WINDOW_NORMAL)
  cv2.namedWindow("smalltag_ccw180", cv2.cv.CV_WINDOW_NORMAL)
  cv2.namedWindow("smalltag_cw90", cv2.cv.CV_WINDOW_NORMAL)

valid_ids = []
for i in xrange(1024):
  imgpath = '%s/artag_%04d.png' % (image_folder, i)
  rgb = cv2.imread(imgpath)
  if rgb is None:
    continue
  valid_ids.append(i)
  if show_tags:
    cv2.imshow("rgb", rgb)
  gray = cv2.cvtColor(rgb, cv2.COLOR_RGB2GRAY)
  offset = 130
  tag = gray[offset:500-offset, offset:500-offset]
  if show_tags:
    cv2.imshow("tag", tag)
  smalltag = cv2.resize(tag, (16, 16), interpolation=cv2.INTER_AREA)
  if show_tags:
    cv2.imshow("smalltag", smalltag)
  smalltag_ccw90 = cv2.flip(smalltag.transpose(), 0)
  if show_tags:
    cv2.imshow("smalltag_ccw90", smalltag_ccw90)
  smalltag_ccw180 = cv2.flip(smalltag, -1)
  if show_tags:
    cv2.imshow("smalltag_ccw180", smalltag_ccw180)
  smalltag_cw90 = cv2.flip(smalltag.transpose(), 1)
  if show_tags:
    cv2.imshow("smalltag_cw90", smalltag_cw90)
  
  out = ''
  out_ccw90 = ''
  out_ccw180 = ''
  out_cw90 = ''
  for y in xrange(16):
    out += ''.join('%4d' % n for n in smalltag[y,:]) + '\n'
    out_ccw90 += ''.join('%4d' % n for n in smalltag_ccw90[y,:]) + '\n'
    out_ccw180 += ''.join('%4d' % n for n in smalltag_ccw180[y,:]) + '\n'
    out_cw90 += ''.join('%4d' % n for n in smalltag_cw90[y,:]) + '\n'
  pattfile = '%s/artag_%04d.pat' % (patt_folder, i)
  f = open(pattfile, 'w')
  f.write(out)
  f.write(out)
  f.write(out)
  f.write('\n')
  f.write(out_ccw90)
  f.write(out_ccw90)
  f.write(out_ccw90)
  f.write('\n')
  f.write(out_ccw180)
  f.write(out_ccw180)
  f.write(out_ccw180)
  f.write('\n')
  f.write(out_cw90)
  f.write(out_cw90)
  f.write(out_cw90)
  f.write('\n')
  f.close()
  print 'Wrote to', pattfile
  
  if show_tags:
    k = cv2.waitKey(0) & 0x0FF
    if k == ord('x') or k == ord('X'):
      break

datafile = '%s/object_data' % patt_folder
f = open(datafile, 'w')
f.write('#the number of patterns to be recognized\n')
f.write('%d\n\n' % len(valid_ids))
for i in valid_ids:
  f.write('%d\nartag_%04d.pat\n86.0\n0.0 0.0\n\n' % (i, i))
f.write('\n')
f.close()
print 'Wrote to', datafile
