#include "../inc/face_regconition.hpp"
#include "../inc/func_face_regcontion.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/types_c.h"
#include <opencv2/videoio.hpp>
using namespace std;
using namespace cv;
// Function for Face Detection
void detectAndDraw(Mat &img, CascadeClassifier &cascade, double scale, Ptr<FaceRecognizer> &model);

int main(int argc, const char **argv)
{
    //Initial Variable
	string cascadeName, nestedCascadeName;
    //Detected_model
    string frontal_face_detect_path = "/mnt/models/haarcascade_frontalface_default.xml";

    //Regconized_model
    string model_face_regconition_path = "/mnt/models/model.xml";

    //Database_id
    string database = "/mnt/csv/database.csv";

    Mat frame, image;

    CascadeClassifier cascade; // PreDefined trained XML classifiers with facial features

    double scale = 1; //scale for ... I dont know

    // If the input is the web camera, pass 0 instead of the video file name
//    string url="192.168.1.16:8081";

    // GUI
    cout << "usage: " << argv[0] << endl;
    cout << "<frontal_face_detect_path> <model_face_regconition_path>" << endl;
    cout << "\n----------------------------------------------------------\n"
         << endl;
    if (argc == 1)
    {
        cout << "No other arguments other than default application name, using default value (Y/N)?" << endl;
    }
    else if (argc == 2)
    {
        frontal_face_detect_path = string(argv[1]);
        cout << "You only input frontal_face_detect_path, is this okay (Y/N) ?" << endl;
    }
    else if (argc == 3)
    {
        frontal_face_detect_path = string(argv[1]);
        model_face_regconition_path = string(argv[2]);
        cout << "Is this okay (Y/N) ?" << endl;
    }
    else
        exit(1);
    cout << "- Face detector model: " << frontal_face_detect_path << endl;
    cout << "- Face regconizer model: " << model_face_regconition_path << endl;
    if (!checkUserEnter())
    {
        cout << "Well, BYE " << endl;
        cin.get();
        return 0;
    }

    // Load classifiers from "opencv/data/haarcascades" directory
    cout << "Loading model object detected ..." << endl;
    if (cascade.load(frontal_face_detect_path))
        cout << "OK" << endl;
    else {
    	cout<<"Loading fail, stop program"<<endl;
    	return 0;
    }
	cout<<"Loading model..."<<endl;
	Ptr<FaceRecognizer> model = LBPHFaceRecognizer::create();
	model->read(model_face_regconition_path);
	if (model->empty())
	{
		cout << "Model load khong thanh cong, dung chuong trinh " << endl;
		return 0;
	}
	cout << "Load model successful" << endl;
    /*Loading dataset_id*/
    vector<Data> data;

    if (!read_dataset_id(database, data))
    {
        cout << "There are line(s) couldn't be read correctly" << endl;
    }
    /* This help access data quicker*/
//    string *names = new string[data.size()];
//    for (int i = 0; i < (int)data.size(); i++)
//    {
//        names[i] = getName(i, data);
//    }

    // Start Video..1) 0 for WebCam 2) "Path to Video" for a Local Video

    cout << "Face Detection Started...." << endl;
    // Config ip cam
    string url="/mnt/video/baohan.mp4";

    VideoCapture capture(url,CAP_FFMPEG);

    if(!capture.isOpened()) cout<<"Couldn't open camera"<<endl;
    while (capture.isOpened())
    {
//        // Capture frames from video and detect faces
//
            capture >> frame;
            if (frame.empty())
                break;
            Mat frame1 = frame.clone();
            detectAndDraw(frame1, cascade, scale, model);
            char c = (char)waitKey(10);
            // Press q to exit from window
            if (c == 27 || c == 'q' || c == 'Q')
                break;
    }
//
////    delete[] names;
//
    return 0;
}

void detectAndDraw(Mat &img, CascadeClassifier &cascade,
                   double scale,
                   Ptr<FaceRecognizer> &model

)
{
    vector<Rect> faces, faces2;
    Mat gray, smallImg;

    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale

    double fx = 1 / scale;

    // Resize the Grayscale Image
    resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
    cascade.detectMultiScale(smallImg, faces, 1.33,
                             2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    //Draw retangle around the faces
    for (size_t i = 0; i < faces.size(); i++)
    {
        Mat smallImgROI;
        smallImgROI = smallImg(faces[i]);
        resize(smallImgROI, smallImgROI, Size(180, 200));

        cout << model->predict(smallImgROI) << endl;
    }
}
