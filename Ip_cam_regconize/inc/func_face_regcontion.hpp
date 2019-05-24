/*
 *   See <http://www.opensource.org/licenses/bsd-license>
 */
#include "face_regconition.hpp"

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(Error::StsBadArg, error_message);
	}
	string line, path, classlabel, className;
	Mat gray;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
//		cout<<"path: "<<path<<"; id: "<<classlabel<<endl;
		if (!path.empty() && !classlabel.empty()) {
			//XU LI ANH MAU BGR SANG GRAY
			gray = imread(path,IMREAD_GRAYSCALE);
			if (gray.size() != Size(180,200)) cout <<"error: "<<path<<endl;
//			cvtColor(imread(path,3), gray, COLOR_BGR2GRAY);
			//resize(gray, gray, Size(180, 200));
			images.push_back(gray);
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
	cout << "\nReading " << images.size() << " examples successful" << endl;
}
static bool checkUserEnter() { //default attempt time =5
	int def = 5;
	return checkUserEnter(def);
}
static bool checkUserEnter(int &attemp_number) {
	//Exist if user want
	char c[30];
	while (attemp_number > 0) {
		cin.clear();
		cin.getline(c, 30);
		if (strcmp("y", c) == 0 || strcmp("Y", c) == 0 || strcmp("yes", c) == 0 || strcmp("YES", c) == 0) return true;
		else if (strcmp("n", c) == 0 || strcmp("N", c) == 0 || strcmp("NO", c) == 0 || strcmp("no", c) == 0 || strcmp("exit", c) == 0 || strcmp("EXIT", c) == 0) {
			return false;
		}
		else {
			cout << "Nhap sai, Vui long nhap lai" << endl;
			attemp_number--;
		}
	}
	//Exit after attempt
	if (attemp_number == 0) return false;
}

bool read_database(string &filename, vector<string> &data)
{
	int idx = 0;
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file)
	{
		string error_message = "Check database file name.";
		CV_Error(Error::StsBadArg, error_message);
	}
	string line, id, classlabel;
	while (getline(file, line))
	{
		stringstream liness(line);
		getline(liness, id, ';');
		getline(liness, classlabel);
		if (!id.empty() && !classlabel.empty())
		{
			if (atoi(id.c_str()) != idx++)
			{
				cout << "ERROR: Label " << id << " saved as data[" << idx << "]" << endl;
				return false;
			}
			data.push_back(classlabel);
			//XU LI ANH MAU BGR SANG GRAY
		}
	}
	return true;
}

