#include <iostream>
#include<windows.h>
#include<string>
#include<time.h>
using namespace std;
class Disc {
protected:
	class File {

		double file_size;
	public:
		string filename;
		string content;
		string createdTime;
		string changedTime;


		File(string filename, string content, string createtime = __TIMESTAMP__, string changedtime = __TIMESTAMP__) {
			this->filename = filename;
			this->content = content;
			this->file_size = content.length();
			SetCreateTime(createtime);
			SetChangedTime(changedtime);
		}
		void SetCreateTime(const string createtime) {
			if (createtime.empty()) return;

			time_t currently = time(0);
			char dt[30]{};
			ctime_s(dt, sizeof dt, &currently);
			string time = dt;

			this->createdTime = dt;
		}
		void SetChangedTime(const string changedtime) {
			if (changedtime.empty()) return;

			time_t currently = time(0);
			char dt[30]{};
			ctime_s(dt, sizeof dt, &currently);
			string time = dt;

			this->changedTime = dt;
		}


		double getfile_size() const { return file_size; }
	};
public:
	File** files = NULL;
	int file_count = 0;
	double current_size = 0;
	const static double max_size;

	File& getFile(string filename) {
		for (size_t i = 0; i < file_count; i++)
		{
			if (filename == files[i]->filename) {
				return *(files[i]);

			}
		}
		throw "File is not found";

	};

	bool isExistsFile(string filename) {
		for (size_t i = 0; i < file_count; i++)
		{
			if (filename == files[i]->filename) return true;
		}
		return false;
	}

	virtual void createFile(string filename, string content = "",string createtime = __TIMESTAMP__) {}

	virtual void setFileContent(string filename, string content = "", string changedtime = __TIMESTAMP__) {};
	void showAllFiles() {
		cout << "--------------FILES INFO--------------" << endl;
		for (size_t i = 0; i < file_count; i++)
		{

			cout << "File name: " << files[i]->filename << endl;
			cout << "File content: " << files[i]->content << endl;
			cout << "File create time: " << files[i]->createdTime << endl;
			cout << "Files change time: " << files[i]->changedTime << endl;
		}
	}

	void deleteFileContent(string filename) {

		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {
				files[i]->content = "";
				return;
			}
		}
		throw "File is not found";

	};
	void deleteFileByFilename(string filename) {
		int index;
		for (size_t i = 0; i < file_count; i++)
		{

			if (files[i]->filename == filename) {
				index = i;
				current_size -= files[i]->content.length();
				break;
			}
		}

		auto newFiles = new File * [file_count - 1]{};
		for (size_t i = 0, k = 0; i < file_count; i++)
		{
			if (i != index) {

				newFiles[k] = files[i];
				k++;
			}
		}
		file_count--;
		delete[]files;
		files = newFiles;
		newFiles = nullptr;

	}
	long long polynomialRollingHash(
		string  str)
	{
		// P and M
		int p = 31;
		int m = 1e9 + 9;
		long long power_of_p = 1;
		long long hash_val = 0;

		// Loop to calculate the hash value
		// by iterating over the elements of string
		for (int i = 0; i < str.length(); i++) {
			hash_val = (hash_val + (str[i] - 'a' + 1) * power_of_p) % m;
			power_of_p = (power_of_p * p) % m;
		}
		return hash_val;
	}
	void updateFilename(string oldfilename, string newfilename) {
		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->filename == oldfilename) {
				files[i]->filename = newfilename;
				return;
			}
		}
		throw "Files is not found";
	}
	void hashFile(string filename) {

		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {
				int hashvalue = polynomialRollingHash(files[i]->content);

				files[i]->content = to_string(hashvalue);
				return;
			}
		}
	}

};
class SSD :public Disc {
public:
	void createFile(string filename, string content = "", string createtime = __TIMESTAMP__) {
		if (isExistsFile(filename)) throw "File already exist";
		double counter = (content.length()) / 10;

		auto newFiles = new File * [file_count + 1];

		for (size_t i = 0; i < file_count; i++)
		{

			newFiles[i] = files[i];

		}
		while (counter >= 0) {
			cout << "Time remaining of the creating file(SSD): " << counter << " seconds" << endl;

			Sleep(1000);
			counter--;
		}
		if (current_size + content.length() > max_size) {
			throw "File size is bigger than  max size";
		}
		File* file = new File(filename, content);

		newFiles[file_count] = file;

		delete[] files;
		files = newFiles;
		newFiles = nullptr;

		current_size += content.length();
		file_count++;

		cout << "!!!!!!!File created successfully(SSD)!!!!!!!" << endl;
	}
	void setFileContent(string filename, string content = "", string changedtime = __TIMESTAMP__) {
		double counter = (content.length()) / 10;


		for (size_t i = 0; i < file_count; i++)
		{

			if (files[i]->filename == filename) {
				current_size -= files[i]->content.length();
				if (current_size + content.length() > max_size)	throw "This content  exceeds SSD size ";
				files[i]->content = content;
			}
		}
		while (counter >= 0) {
			cout << "Time remaining of the setting file(SSD): " << counter << " seconds" << endl;
			Sleep(1000);
			counter--;
		}
		cout << "!!!!!!!File succesfully changed(SSD)!!!!!!" << endl;
	};



};
class HDD :public Disc {
public:
	void createFile(string filename, string content = "", string create = __TIMESTAMP__) {
		if (isExistsFile(filename)) throw "File already exist";
		double counter = (content.length()) / 10;
		counter *= 3;
		auto newFiles = new File * [file_count + 1];

		for (size_t i = 0; i < file_count; i++)
		{

			newFiles[i] = files[i];

		}
		while (counter >= 0) {
			cout << "Time remaining of the creating file(HDD): " << counter << " seconds" << endl;
			Sleep(1000);
			counter--;
		}
		if (current_size + content.length() > max_size) {
			throw "File size is bigger than  max size";
		}
		File* file = new File(filename, content);

		newFiles[file_count] = file;

		delete[] files;
		files = newFiles;
		newFiles = nullptr;

		current_size += content.length();
		file_count++;

		cout << "!!!!!!!File created successfully(HDD)!!!!!!!" << endl;
	}
	void setFileContent(string filename, string content = "", string changedtime = __TIMESTAMP__) {
		double counter = (content.length()) / 10;
		counter *= 3;

		for (size_t i = 0; i < file_count; i++)
		{

			if (files[i]->filename == filename) {
				current_size -= files[i]->content.length();
				if (current_size + content.length() > max_size)	throw "This content  exceeds Hdd size ";
				files[i]->content = content;
			}
		}
		while (counter >= 0) {
			cout << "Time remaining of the setting file(HDD): " << counter << " seconds" << endl;
			Sleep(1000);
			counter--;
		}
		cout << "!!!!!!!File succesfully changed(HDD)!!!!!!" << endl;
	};

};

const double HDD::max_size = 1024;


class OperationSystem {
	OperationSystem() {}
public:
	static void controlPC();
};

void OperationSystem::controlPC() {
	HDD hdd;
	SSD ssd;

	try
	{
		ssd.createFile("newtext.txt", "hello world");
		hdd.createFile("HddFile1.txt", "hello world peoples");
		hdd.setFileContent("HddFile1.txt", "Come back bro");
		ssd.createFile("newtext.txt1", "Hakuna matata 1238423");
		ssd.createFile("newtext.txt2", "Reenkarne");
		//hdd.getFile("newtext.txt");
		//ssd.setFileContent("newtext.txt", "Salam Aleykum");
		//ssd.deleteFileContent("newtext.txt");
		//ssd.deleteFileByFilename("newtext.txt1");
		//ssd.updateFilename("newtext.txt1", "UpdatedName");
		//ssd.hashFile("newtext.txt");
		cout << "-------SSD-------" << endl;
		ssd.showAllFiles();

		cout << "-------HDD-------" << endl;
		hdd.showAllFiles();

	}
	catch (const char* ex)
	{
		cout << "Exception : " << ex << endl;
	}
}

void main() {
	OperationSystem::controlPC();

}