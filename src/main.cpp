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
int frame = 0;
Point lightPos(50, 50);
int simPhase = 0;

void mouseCallback(int event, int x, int y, int flag, void *param) {
  oriCanvas.copyTo(canvas);

  Point2f offset(150.f, 150.f);
  float w = float(object.size().width);
  float h = float(object.size().height);

  // move vertices along delta direction
  // to get dstTri[i]
  Point2f center = offset + Point2f(w / 2.f, h / 2.f);
  // Vec2f delta = center - Point2f(lightPos.x, lightPos.y);
  Vec2f delta = center - Point2f(x, y);
  delta /= norm(delta);

  // scale delta to adjust shadow shape
  float lambda = 0.75f;
  delta *= lambda;

  // compute affine transformation matrix
  Point2f srcTri[3];
  srcTri[0] = Point2f(0.f, 0.f);
  srcTri[1] = Point2f(0.f, 1.f);
  srcTri[2] = Point2f(1.f, 1.f);

  Point2f dstTri[3];
  dstTri[0] = Point2f(0.f, 0.f) + offset;
  dstTri[1] = dstTri[0] + Point2f(delta[0], delta[1]);
  dstTri[2] = dstTri[1] + Point2f(1.f, 0.f);

  Mat warpMat = getAffineTransform(srcTri, dstTri);

  warpAffine(shadow, canvas, warpMat, canvas.size());

  // invert image to get a black shadow
  bitwise_not(canvas, canvas);

  // draw object over the shadow
  int sx, sy;
  sx = int(offset.x);
  sy = int(offset.y);

  object.copyTo(canvas.rowRange(sy, sy + object.size().height)
                    .colRange(sx, sx + object.size().width));

  // screenshot
  // circle(canvas, Point(lightPos.x, lightPos.y), 3, iRED, -1);
  //
  // save frame
  // flip(canvas, canvas, 0);
  // imshow(wndName, canvas);
  // imwrite(format("./result/sim%03d.png", frame++), canvas);
  // std::cout << "Image saved." << '\n';
  //
  // if (simPhase == 0) {
  //   lightPos.x += 5;
  //   if (lightPos.x >= 400)
  //     simPhase++;
  // } else if (simPhase == 1) {
  //   lightPos.y += 5;
  //   if (lightPos.y >= 500)
  //     simPhase++;
  // } else if (simPhase == 2) {
  //   lightPos.x -= 5;
  //   if (lightPos.x <= 50)
  //     simPhase++;
  // } else if (simPhase == 3) {
  //   lightPos.y -= 5;
  //   if (lightPos.y <= 50)
  //     simPhase++;
  // } else {
  //   // end;
  // }

  switch (event) {
  case EVENT_MOUSEMOVE:

    circle(canvas, Point(x, y), 3, iRED, -1);
    flip(canvas, canvas, 0);
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
  shadow = imread("./res/shadow.png");
  object = imread("./res/object.png");

  if (shadow.empty()) {
    cout << "Could not open or find the image!\n" << endl;
    cout << "Usage: " << argv[0] << " <Input image>" << endl;
    return -1;
  }

  canvas = Mat(WND_HEIGHT, WND_WIDTH, CV_8UC3, iBG_COLOR);
  oriCanvas = Mat(WND_HEIGHT, WND_WIDTH, CV_8UC3, iBG_COLOR);
  namedWindow(wndName);
  setMouseCallback(wndName, mouseCallback);

  imshow(wndName, canvas);

  waitKey(0);

  return 0;
}
