#include <opencv2/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <fstream>
#include <ncurses.h>
#include <unistd.h> // For debugging!

void filter(int xSize, int ySize, int width, int height,cv::Mat* image, char *buffer, char* pos, int pos_num) {
	for (int y = 1;y <= (int)(height/ySize)-1;y++){
                for(int x = 1;x <= (int) (width/xSize)-1;x++) {
                        int sum = 0;
                        for (int j = (y-1)*ySize; j <= y*ySize;j++) { //Have to take -1 at ySize and xSize bcs of 0 1 2 3 => 4 ; 0 1 2 => 3
                                for (int i = (x-1)*xSize;i <= x*xSize;i++) {
                                        sum += image->at<uchar>(j, i);
				}
                        } //TODO:Make normal
                        buffer[(width/xSize)*y+x] = pos[(int)(((float) (sum/((ySize+1)*(xSize+1)))/255.0)*(pos_num-1))]; //Segmenataion fault happens here ! Possible because of pos memeory access illegal
		}
		buffer[(width/xSize) * y] = '\n';
	}
}

void video(char* filename, char* pos, int scalarValue, int frameTime) {
  cv::Mat frame;
  int pos_num = sizeof(pos)/sizeof(char);
  cv::VideoCapture cap(filename);
  initscr();
  while(cap.read(frame)){
        clear();
        usleep(frameTime);
        cap >> frame;
        char tempbuf[(frame.cols/scalarValue) * (frame.rows/scalarValue)];
        cv::Mat pic;
        if (frame.empty()) {
                break;
                std::cout << "Frame Empty";
        }
        cv::cvtColor(frame, pic, cv::COLOR_BGR2GRAY);
        filter(scalarValue,scalarValue, pic.cols, pic.rows, &pic, tempbuf, pos, pos_num);
        for(int i = 0;i <= sizeof(tempbuf)/sizeof(char)-1;i++){
                printw("%c", tempbuf[i]);
        }
        int k = cv::waitKey(10);
        if (k == 27) // esc. pressed
            break;
        refresh();
  }
  getch();
  endwin();
}

void picture() { //TODO:Fix Later
   //Should be letters and dots not special characters CARRIGE RETURN BULLSHIT
cv::Mat image;
  image = cv::imread("image.png" , cv::IMREAD_COLOR);
  cv::VideoCapture cap("test_video.mp4");
  cv::Mat new_image;
  cv::Mat grey_image;
  cv::resize(image, new_image, cv::Size(), 0.5, 0.5);
  char* pos = ".ab"; //Should be letters and dots not special characters CARRIGE RETURN BULLSHIT
  int scalarValue = 3;
  int pos_num = sizeof(pos)/sizeof(char);
  char buffer[(new_image.cols/scalarValue) * (new_image.rows/scalarValue)];
  if(! image.data ) {
      std::cout <<  "Could not open or find the image" << std::endl ;
    }
  cv::cvtColor(new_image, grey_image, cv::COLOR_BGR2GRAY);
  std::ofstream file;
  file.open("test.txt");
   //filter(scalarValue+1,scalarValue+1, grey_image.cols, grey_image.rows, &grey_image, buffer, pos, pos_num);
  initscr();
  filter(scalarValue+1,scalarValue+1, grey_image.cols, grey_image.rows, &grey_image, buffer, pos, pos_num);
  for(int i = 0;i <= sizeof(buffer)/sizeof(char)-1; i++) {
          printw("%c",  buffer[i]);
          //file << buffer[i];
//        usleep(100);
  }
  refresh();
  getch();
  endwin();
  file.close();

}
//TODO:Make buffer smaller
int main( int argc, char** argv ) {	
  cv::Mat image;
  image = cv::imread("image.png" , cv::IMREAD_COLOR);
  cv::VideoCapture cap("test_video.mp4");
  cv::Mat new_image;
  cv::Mat grey_image;
  cv::resize(image, new_image, cv::Size(), 0.5, 0.5);
  char* pos = ".abcde"; //Should be letters and dots not special characters CARRIGE RETURN BULLSHIT
  int scalarValue = 3;
  int pos_num = sizeof(pos)/sizeof(char);
  char buffer[(new_image.cols/scalarValue) * (new_image.rows/scalarValue)];  
  //IMAGE PART
  video("ＮＯ ＭＵＳＩＣ.mp4", pos, 3, 100);
  //END IMAGE PATH
  if(! image.data ) {
      std::cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }
  cv::cvtColor(new_image, grey_image, cv::COLOR_BGR2GRAY);
  std::ofstream file;
  file.open("test.txt");
   //filter(scalarValue+1,scalarValue+1, grey_image.cols, grey_image.rows, &grey_image, buffer, pos, pos_num);
  initscr();
  filter(scalarValue+1,scalarValue+1, grey_image.cols, grey_image.rows, &grey_image, buffer, pos, pos_num);
  for(int i = 0;i <= sizeof(buffer)/sizeof(char)-1; i++) {
	  printw("%c",  buffer[i]);
	  //file << buffer[i];
//	  usleep(100);
  }
  refresh();
  getch();
  endwin();
  file.close(); 
  picture();
  return 0;
}
