#include "FileReader.h"

FileReader::FileReader(const std::string filename) {
	this->filename = filename;
}
bool FileReader::fileExists()
{
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}
bool FileReader::fileExists(const std::string& filename)
{
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}

bool FileReader::openFile() {
	inputFile.open(filename);
	std::cout << "File " << filename << " opened" << std::endl;
	return inputFile.is_open();
}

void FileReader::readFileAndSave(std::string& FileContent) {
	std::string line;
	while (std::getline(inputFile, line)) {
		FileContent += line + "\n";
	}
}

void FileReader::closeFile() {
	inputFile.close();
}

bool FileReader::readMeFile(std::string meFileLocation, std::string& name, std::string& hexRep, std::string& key) {
	std::string fileContent;
	FileReader fileReader(meFileLocation);
	fileReader.openFile();
	try {
		fileReader.readFileAndSave(fileContent);
		fileReader.closeFile();
		std::cout << "Read 'Me' file successfully " << meFileLocation << std::endl;
		std::cout << "-------------'me file content'-------------" << std::endl;
		std::cout << fileContent << std::endl;
		std::cout << "-------------------------------------------" << std::endl;
		bool wasNameEmpty = name.empty();
		int i = 0;

		while (fileContent[i] != '\n' && i < 100) {
			if (wasNameEmpty) {
				name += fileContent[i];
			}
			i++;
		}
		i++;
		while (fileContent[i] != '\n' && i < 100) {
			hexRep += fileContent[i];
			i++;
		}
		i++;
		while (fileContent[i] != '\0' && i < 2000 ) {
			key += fileContent[i];
			i++;
		}
		return 1;
	}
	catch (...) {
		std::cerr << "ERROR reading 'me' file content...  " << std::endl;
		return 0;
	}
	return 0;
}

bool FileReader::readTransferFile(const std::string transferFileLocation, std::string& address, std::string& name, std::string& port, std::string& filePath)
{
	/*trying to read "transfer" file contents*/
	FileReader fileReader(transferFileLocation);
	std::string fileContent;
	std::cout << "attemping to read 'transfer' file " << std::endl;
	if (fileReader.fileExists()) {
		try {

			fileReader.openFile();
			fileReader.readFileAndSave(fileContent);
			fileReader.closeFile();
			std::cout << "\ntransfer file content:" << std::endl;
			std::cout << fileContent << std::endl;
			std::cout << "\n" << std::endl;
			int i = 0;
			while (fileContent[i] != ':' && i < 20) {
				address += fileContent[i];
				i++;
			}
			i++;
			int left = i; int right = i;
			while (fileContent[i] != '\n' && i < 100) {
				port += fileContent[i];
				i++;
			}
			i++;
			while (fileContent[i] != '\n' && i < 100) {
				name += fileContent[i];
				i++;
			}
			i++;
			while (fileContent[i] != '\n' && i < 100) {
				filePath += fileContent[i];
				i++;
			}
			std::cout << "'transfer' file successfully read" << std::endl;
			return 1;
		}
		catch (...) {
			std::cout << "failed to read 'transfer' file" << std::endl;
			return 0;
		}
	}
	else { std::cout << "failed to find 'transfer' file" << std::endl; return 0; }
}


void FileReader::saveStringAsFile(const std::string& content, const std::string& location) {
	std::string filePath = location;

	std::ofstream outputFile(filePath);
	if (outputFile.is_open()) {
		outputFile << content;
		outputFile.close();
		std::cout << "String saved as '" << filePath << "' file successfully." << std::endl;
	}
	else {
		std::cerr << "Error: Unable to open '" << filePath << "' file for writing." << std::endl;
	}
}


std::string FileReader::fileToString(std::string fname) {
	
	std::string emptyString = "";
	if (std::filesystem::exists(fname)) {
		std::filesystem::path fpath = fname;
		std::ifstream f1(fname.c_str(), std::ios::binary);

		size_t size = std::filesystem::file_size(fpath);
		char* b = new char[size];
		f1.seekg(0, std::ios::beg);
		f1.read(b, size);
		std::cout << "FILE content " << b << std::endl;
		std::string fileContent(b, size);
		return fileContent;

}
	return emptyString;
	}
