#ifndef _OPENCV_HEADER_
#define _OPENCV_HEADER_
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <sys/stat.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace cv;
using namespace cv::face;
using namespace std;
enum Algorithm {

	EIGENFACES = 1,

	FISHERFACES = 2,

	LBPHFACES = 3
};

static void read_csv(const string& filename, vector<Mat>& images, vector<string>& names, vector<int>& labels, char separator);
static bool checkUserEnter();
static bool checkUserEnter(int &attemp_number);
bool read_database(string &filename, vector<string> &data);
#endif
