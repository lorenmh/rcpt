#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
  Mat image, image_grey, output, edges;

  string window_name = "Edge Detection";

  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  output.create( image.size(), image.type() );

  cvtColor( image, image_grey, CV_BGR2GRAY );

  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  blur( image_grey, edges, Size(3,3) );

  int threshold = 50;
  int size = 3;
  int ratio = 3;
  Canny( edges, edges, threshold, threshold * ratio, size );

  output = Scalar::all(0);

  image.copyTo( output, edges );
  imshow( window_name, output );

  waitKey(0);

  return 0;
}
