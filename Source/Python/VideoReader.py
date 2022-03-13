import ffmpeg
import numpy
import cv2
import sys
import random
 
def read_frame_by_time(in_file, time):
  """
  指定时间节点读取任意帧
  """
  out, err = (
    ffmpeg.input(in_file, ss=time)
       .output('pipe:', vframes=1, format='image2', vcodec='mjpeg')
       .run(capture_stdout=True)
  )
  return out
 
def get_video_info(in_file):
  """
  获取视频基本信息
  """
  try:
    probe = ffmpeg.probe(in_file)
    video_stream = next((stream for stream in probe['streams'] if stream['codec_type'] == 'video'), None)
    if video_stream is None:
      print('No video stream found', file=sys.stderr)
      sys.exit(1)
    return video_stream
  except ffmpeg.Error as err:
    print(str(err.stderr, encoding='utf8'))
    sys.exit(1)
 
if __name__ == '__main__':
  file_path = r'C:\Users\Administrator\Desktop\TestVideo\Test6.mp4'
  video_info = get_video_info(file_path)
  total_duration = video_info['duration']
  print('总时间：' + total_duration + 's')
  random_time = random.randint(1, int(float(total_duration)) - 1) + random.random()
  out = read_frame_by_time(file_path, 3)
  image_array = numpy.asarray(bytearray(out), dtype="uint8")
  image = cv2.imdecode(image_array, cv2.IMREAD_COLOR)
  cv2.imwrite("1.PNG",image)