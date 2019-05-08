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
static int userChooseAlg() {
	int what = -1;
	//cout << "012345678901234567890123456789";
	cout << "____________________________" << endl;
	cout << "Danh sach giai thuat:       |" << endl;
	cout << setw(2) << EIGENFACES << ". " << left << setw(24) << "Eigenfaces" << "|" << endl << right;
	cout << setw(2) << FISHERFACES << ". " << left << setw(24) << "Fisherfaces" << "|" << endl << right;
	cout << setw(2) << LBPHFACES << ". " << left << setw(24) << "LBPHFaces" << "|" << endl << right;
	for (int i = 0; i < 28; i++)cout << "_";
	cout << "\nNhap giai thuat ban muon: ";
	cin >> what;
	cout << "\nBan da chon giai thuat so " << what << endl;
	return what;
}
//string getName(int id, vector<Data> &data) {
//	for (int i = 0; i < data.size(); i++) {
//		if (id == data[i].id) return data[i].name;
//	}
//	string fail = "unknow_cannotfind";
//	return fail;
//}
//bool read_dataset_id(const string &path, vector<Data> &data) {
//	ifstream file(path.c_str(), ifstream::in);
//	if (!file) {
//		string error_message = "No valid input file was given, please check the given filename.";
//		CV_Error(Error::StsBadArg, error_message);
//		return true;
//	}
//	string line, name, id;
//	const char separator = ';';
//	bool check = true;
//	while (getline(file, line)) {
//		stringstream liness(line);
//		getline(liness, id, separator);
//		getline(liness, name, separator);
//		if (!id.empty() && !name.empty()) {
//			data.push_back(Data(atoi(id.c_str()), name));
//		}
//		else check = false;
//	}
//	return check;
//}
