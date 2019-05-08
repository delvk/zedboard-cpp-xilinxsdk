#include "../inc/face_regconition.hpp"
#include "../inc/func_face_regcontion.hpp"
int main(int argc, const char *argv[])
{
	//Checking arguments
	cout << "usage: <model_path> <data_path>" << endl;
	cout << "\n----------------------------------------------------------\n"
		 << endl;

	 if (argc < 3)
	{
		cout << "Missing arguments, exit program" << endl;
		exit(-1);
	}
	//initial variables
	static string model_path =string(argv[1]);
	static string data_path=string(argv[2]);
	cout << "- model_path: " << model_path << endl;
	cout << "- data_path: " << data_path << endl;
	//Reading model
	cout<<"Loading model..."<<endl;
	Ptr<FaceRecognizer> model = LBPHFaceRecognizer::create();
	model->read(model_path);
	if (model->empty())
	{
		cout << "Model load khong thanh cong, dung chuong trinh " << endl;
		return 0;
	}
	cout << "Load model successful" << endl;

	//Loading test_data
	cout<<"Loading test data"<<endl;
	vector<Mat> testSample;
	vector<int> testLabel;
	try
	{

		read_csv(data_path, testSample, testLabel);
	}
	catch (const cv::Exception &e)
	{
		cerr << "Error opening file \""
			 << "\". Reason: " << e.msg << endl;
		exit(-1);
	}
	// Vector predict
	vector<int> predicted_label;
	vector<double> predicted_confidence;
	string check = "";

	//Predict output
	cout<<"Start testing.."<<endl;
	for (unsigned int i = 0; i < testLabel.size(); i++)
	{
		int label = -1;
		double confident = 0.0;
		model->predict(testSample[i], label, confident);
		predicted_label.push_back(label);
		predicted_confidence.push_back(confident);
		if (predicted_label[i] != testLabel[i])
			check = "*";
		else
			check = "";
		cout << check << "predict: " << predicted_label[i] << " | " << testLabel[i] << " -- " << predicted_confidence[i] << endl;
	}
	cout << "Threshold: " << model->getThreshold() << endl;
	//Clean up
	testSample.clear();
	testLabel.clear();
	predicted_label.clear();
	predicted_confidence.clear();
	model->clear();

	cout<<"Finished"<<endl;
	return 0;
}
