#include <iostream>
#include <thread>
#include <filesystem>
#include <fstream>
#include <iomanip>

namespace fs = std::filesystem;
using namespace std;

// ���������
const fs::path path = "file.txt";
const int number_of_threads = 8;
const int X = 526;

// ������� ��� ������ � ��������� ������
void byte_process(int const thread, const int size, const int step, 
				   char* in, char* out) {
	
	int index = 0;
	int count = thread;
	while (count < size) {
		out[count] = static_cast<char>((in[count] * X) % 256);
		count += step;
	}
}

int main() {
	// ��������� ������������� �����
	if (!fs::exists(path)) {
		cout << "Wrong name of file" << endl;
		return 1;
	}

	// ������ ������ �����
	uintmax_t size = fs::file_size(path);
	cout << "File size: " << size << " bytes" << endl;
	int step = size / number_of_threads;

	// ������� ��� �������� ������� � ������ ������
	char* in = new char[size];
	char* out = new char[size];

	// ��������� ����
	ifstream fin(path, ios_base::in|ios_base::binary);
	fin.read(in, size);
	fin.close();

	// ��������� ������
	vector<thread> threads(number_of_threads);
	for (int i = 0; i < number_of_threads; i++) {
		threads[i] = thread(byte_process, i, size, step, in, out);
	}

	// ���� ���������� �������
	for (int i = 0; i < number_of_threads; i++) {
		threads[i].join();
	}

	// ��������� ����� ����
	ofstream fout("new_file.txt", ios_base::out| ios_base::trunc| ios_base::binary);
	for (auto i = 0; i < size; i++) {
		fout << out[i];
	}
	fout.close();
	cout << "Done" << endl;

	return 0;
}