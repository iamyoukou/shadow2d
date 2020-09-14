#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define iBLUE Scalar(255, 0, 0)
#define iGREEN Scalar(0, 255, 0)
#define iRED Scalar(0, 0, 255)
#define iWHITE Scalar(255, 255, 255)
#define iBG_COLOR Scalar(222, 187, 171)

#define WND_WIDTH 600
#define WND_HEIGHT 600

using namespace glm;
using namespace std;
using namespace cv;

Mat canvas, oriCanvas;
Mat object, shadow;
string wndName = "2D Shadow";

void mouseCallback(int event, int x, int y, int flag, void *param) {
  oriCanvas.copyTo(canvas);

  switch (event) {
  case EVENT_MOUSEMOVE:

    // std::cout << "(" << x << ", " << y << ") " << '\n';

    // draw object
    int hObject = object.size().height;
    int wObject = object.size().width;
    int hShadow = shadow.size().height;
    int wShadow = shadow.size().width;

    int sxObject = 200;
    int syObject = 200;
    int sxShadow = 220;
    int syShadow = 220;

    shadow.copyTo(canvas.rowRange(syShadow, syShadow + hShadow)
                      .colRange(sxShadow, sxShadow + wShadow));

    object.copyTo(canvas.rowRange(syObject, syObject + hObject)
                      .colRange(sxObject, sxObject + wObject));

    circle(canvas, Point(x, y), 3, iRED, -1);

    imshow(wndName, canvas);
    waitKey(1);

    break;
  }
}

void images2video() {
  string command =
      "ffmpeg -r 60 -start_number 0 -i ./result/sim%03d.png -vcodec mpeg4 "
      "-b 20M -s 600x600 ./result.mp4";
  system(command.c_str());
}

int main(int argc, char const *argv[]) {
  // create canvas
  canvas = Mat(WND_HEIGHT, WND_WIDTH, CV_8UC3, iBG_COLOR);
  oriCanvas = Mat(WND_HEIGHT, WND_WIDTH, CV_8UC3, iBG_COLOR);
  namedWindow(wndName);
  setMouseCallback(wndName, mouseCallback);

  object = imread("./res/object.png");
  shadow = imread("./res/shadow.png");

  // convert images to video
  // images2video();

  imshow(wndName, canvas);
  waitKey(0);

  return 0;
}
