#include "../inc/face_regconition.hpp"
#include "../inc/func_face_regcontion.hpp"
int main(int argc, const char *argv[])
{
	//Checking arguments
	cout << "usage: <path_training_set> <output_folder>" << endl;
	cout << "\n----------------------------------------------------------\n"
		 << endl;

	 if (argc < 3)
	{
		cout << "Missing arguments, exit program" << endl;
		exit(-1);
	}
	static string path_training_set_file = string(argv[1]);
	static string output_folder = string(argv[2]);

	cout << "- path_training_set: " << path_training_set_file << endl;
	cout << "- output_folder: " << output_folder << endl;

	//For user check
	cout << "Is this okay (Y/N) ?" << endl;
	if (!checkUserEnter())
	{
		cout << "See ya later, bye !" << endl;
		return 0;
	}

	// These vectors hold the images and corresponding labels.
	vector<Mat> images;
	vector<int> labels;
	// Read in the data. This can fail if no valid
	// input filename is given.
	try
	{
		read_csv(path_training_set_file, images, labels);
	}
	catch (const cv::Exception &e)
	{
		cerr << "Error opening training file\n. Reason: " << e.msg << endl;
		// nothing more we can do
		exit(-1);
	}
	// Quit if there are not enough images for this demo.
	if (images.size() <= 1)
	{
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		cout<<error_message<<endl;
		exit(-1);
	}
	//size 180x200;
	bool ok = true;
	for (unsigned int i = 0; i < images.size(); i++)
	{
		if (images[i].size() != Size(180,200))
		{
			cout << "Test Errror: " << "image["<<i<<": "<<images[i].size()<<endl;
			ok = false;
		}

	}
	if (!ok)
	{
		cout << "Require size 180x200, try again with another set" << endl;
		return -1;
	}
	else{
		cout<<"Test size: pass"<<endl;
	}
//
	/* create output folder */
	const int dir_err = mkdir(output_folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err)
	{
		printf("Couldn't creating directory output, maybe already exit!\n");
	}
	else cout<<"Created "<<output_folder<<endl;

//	/*---TRAINING---*/
	cout<<"Start training ..."<<endl;
	Ptr<FaceRecognizer> model = LBPHFaceRecognizer::create();
	model->train(images, labels);

	string filename = output_folder+"/model.yml";
	cout<<"Done training, saving file to "<<filename<<endl;

	FileStorage fs;
	fs.open(filename, FileStorage::WRITE | FileStorage::MEMORY);
	model->write(filename);
	fs.release();

	bool temp = !model.empty();
	if (temp) model->clear();

	//Double check
	cout<<"Checking output file "<<endl;
	model->read(filename);
	if (model->empty())
	{
		cout << "Error: Model is empty" << endl;
	}
	else cout << "Save model successful" << endl;
	return 0;
}
