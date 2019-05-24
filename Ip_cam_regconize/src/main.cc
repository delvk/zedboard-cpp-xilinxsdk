#include "../inc/face_regconition.hpp"
#include "../inc/func_face_regcontion.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/types_c.h"
#include <opencv2/videoio.hpp>
#include <time.h>
using namespace std;
using namespace cv;
// Function for Face Detection
void detectAndDraw(Mat &img, CascadeClassifier &cascade, double scale, Ptr<FaceRecognizer> &model, vector<string> &data);
VideoWriter video;
VideoCapture capture;
static int numframe = 0;
int main(int argc, const char **argv)
{
    //Initial Variable
	string cascadeName, nestedCascadeName;
    //Detected_model
    string frontal_face_detect_path = "/mnt/models/haarcascade_frontalface_default.xml";

    //Regconized_model
    string model_face_regconition_path = "/mnt/models/lbph.yml";

    //Database_id
    string file_database = "/mnt/csv/database.csv";

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
    cout << "Loading object detected ..." << endl;
    if (cascade.load(frontal_face_detect_path))
        cout << "OK" << endl;
    else {
    	cout<<"Loading fail, stop program"<<endl;
    	return 0;
    }
	cout<<"Loading face detection..."<<endl;
	Ptr<FaceRecognizer> model = LBPHFaceRecognizer::create();
	model->read(model_face_regconition_path);
	if (model->empty())
	{
		cout << "Model load khong thanh cong, dung chuong trinh " << endl;
		return 0;
	}
	cout << "Load model successful" << endl;
	model->setThreshold(70.0);
    /*Loading dataset_id*/
    vector<string> data;
    if(!read_database(file_database, data)) exit(-1);
    cout<<"Load database successul"<<endl;

    cout << "Face Detection Started...." << endl;

    string url="/mnt/video/khuong.webm";
//    string url="http://192.168.1.16:8081/video.cgi?.mjpg";
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');
    capture.open(url,CAP_FFMPEG);
	video.open("/mnt/outcpp.avi", codec, 20.0, Size(int(capture.get(3)), int(capture.get(4))));
    if(!capture.isOpened()) cout<<"Couldn't open camera"<<endl;
//    time capture
    clock_t t;
	t = clock();

    while (capture.isOpened())
    {
            capture >> frame;
            if (frame.empty())
                break;
            Mat frame1 = frame.clone();
            detectAndDraw(frame1, cascade, scale, model, data);

    }

    t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("took %f seconds to execute %d frame\n", time_taken, numframe);
	capture.release();
    video.release();
    cout<<"DONE"<<endl;
    return 0;
}

void detectAndDraw(Mat &img, CascadeClassifier &cascade,
                   double scale,
                   Ptr<FaceRecognizer> &model,
				   vector<string> &data

)
{

    vector<Rect> faces, faces2;
    Mat gray, smallImg;

    cout<<endl;
    cout <<"\nFrame: "<<numframe++ << endl;
    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale

    double fx = 1 / scale;
    int predict=-1;
    double confident =0.0;
    // Resize the Grayscale Image
    resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
    cascade.detectMultiScale(smallImg, faces, 1.33,
                             2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    for (size_t i = 0; i < faces.size(); i++)
    {
    	Mat smallImgROI;
    	        smallImgROI = smallImg(faces[i]);
    	        resize(smallImgROI, smallImgROI, Size(180, 200));
    	        //hello=model->predict(smallImgROI);
    	        Rect r = faces[i];
    	        vector<Rect> nestedObjects;
    	        Point center;
    	        Scalar color = Scalar(255, 255, 0); // Color for Drawing tool
    	        double aspect_ratio = (double)r.width / r.height;
    	        rectangle(img, cvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
    	                  cvPoint(cvRound((r.x + r.width - 1) * scale),
    	                          cvRound((r.y + r.height - 1) * scale)),
    	                  color, 3, 8, 0);
    	        model->predict(smallImgROI,predict,confident);
				string name;
    	        if(predict <0){
					cout<<"Unknow"<<";";
					name = "Unknow";
				}
				else {
					cout << data[model->predict(smallImgROI)]<<" confident: "<<confident	<<";";
					name = data[model->predict(smallImgROI)];
				}
    	        putText(img, name, Point(cvRound(r.x * scale),cvRound((r.y + r.height - 1) * scale+10)), FONT_HERSHEY_COMPLEX_SMALL, 0.75, Scalar(0,255,255), 2);
    	        //char path[255];
    	        //sprintf(path, "/mnt/image/%d.jpg", count);
    	        //imwrite(path,img);



    }

    video.write(img);
}
