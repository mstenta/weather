#!/bin/bash
mkdir temp
convert *.jpg temp/%05d.stitch.jpg
ffmpeg -r 10 -qscale 1 -i temp/%05d.stitch.jpg output.mp4
rm -r temp
