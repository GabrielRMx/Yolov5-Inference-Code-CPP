/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <chrono>
#include <ctime>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "yolov5.cc"
  
#define _BASETSD_H
#define PERF_WITH_POST 1

int main(int argc, char** argv){

  char* model_name = NULL;
  
  if (argc != 5) {
    printf("USAGE: %s <RKNN MODEL> </dev/video_index> <video width> <video height>\n", argv[0]);
    return -1;
  }

  model_name = (char*)argv[1];
  int num = atoi((char*)argv[2]);
  printf("Read video%d ...\n", num);
  int capWidth = atoi((char*)argv[3]);
  int capHeight = atoi((char*)argv[4]);

  cv::VideoCapture cap(num);

  if (!cap.isOpened()){ //This section prompt an error message if no video stream is found//
    printf("No video stream detected");
    return -1;
  }

   //Set the resolution
  cap.set(cv::CAP_PROP_FRAME_WIDTH, capWidth);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, capHeight);
  cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

  int ret = -1;
  rknn_context   ctx;
  /* Create the neural network */
  printf("Loading mode...\n");
  int model_data_size = 0;
  unsigned char* model_data = load_model(model_name, &model_data_size);
  ret = rknn_init(&ctx, model_data, model_data_size, 0, NULL);

  if (ret < 0) {
    printf("rknn_init error ret=%d\n", ret);
    return -1;
  }

  cv::VideoWriter capWriter("appsrc ! videoconvert ! mpph264enc ! rtph264pay ! udpsink host=10.42.0.1 port=4000", cv::CAP_GSTREAMER,
   0, 50, cv::Size(capWidth, capHeight), true);

  cv::Mat orig_img;
  while (true){ //Taking an everlasting loop to show the video//
    
    cap.read(orig_img);
    inputKnn(orig_img, ctx);
    capWriter.write(orig_img);

  }

  // release
  ret = rknn_destroy(ctx);

  if (model_data) {
    free(model_data);
  }

  deinitPostProcess();

  cap.release();  //Releasing the buffer memory//
  capWriter.release();
  return 0;
}
 