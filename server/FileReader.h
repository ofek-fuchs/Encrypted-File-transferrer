#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <filesystem>

/**
 * @brief A utility class for reading and manipulating files.
 *
 * This class provides various static methods for reading specific types of files,
 * checking file existence, converting file content to a string, and saving strings
 * as files. It also includes instance methods for opening, reading, and closing a file.
 */
class FileReader {
public:
    /**
     * @brief Constructor for FileReader.
     *
     * @param filename The name of the file to be operated on.
     */
    FileReader(const std::string filename);

    /**
     * @brief Reads the contents of a 'Me' file and extracts information.
     *
     * @param meFileLocation The location of the 'Me' file.
     * @param Name Output parameter for the extracted name.
     * @param hexRep Output parameter for the extracted hex representation.
     * @param key Output parameter for the extracted key.
     * @return True if the 'Me' file is successfully read, false otherwise.
     */
    static bool readMeFile(std::string meFileLocation, std::string& Name, std::string& hexRep, std::string& key);

    /**
     * @brief Reads the contents of a transfer file and extracts information.
     *
     * @param transferFileLocation The location of the transfer file.
     * @param address Output parameter for the extracted address.
     * @param name Output parameter for the extracted name.
     * @param port Output parameter for the extracted port.
     * @param filePath Output parameter for the extracted file path.
     * @return True if the transfer file is successfully read, false otherwise.
     */
    static bool readTransferFile(const std::string transferFileLocation, std::string& address, std::string& name, std::string& port, std::string& filePath);

    /**
     * @brief Checks if a file exists.
     *
     * @param filename The name of the file to check.
     * @return True if the file exists, false otherwise.
     */
    static bool fileExists(const std::string& filename);

    /**
     * @brief Reads the contents of a file and returns it as a string.
     *
     * @param FileLocation The location of the file to read.
     * @return A string containing the contents of the file.
     */
    static std::string fileToString(std::string FileLocation);

    /**
     * @brief Saves a string as a file.
     *
     * @param content The content to be saved.
     * @param location The location where the file will be saved.
     */
    static void saveStringAsFile(const std::string& content, const std::string& location);

    /**
     * @brief Checks if the file associated with the instance exists.
     *
     * @return True if the file exists, false otherwise.
     */
    bool fileExists();

    /**
     * @brief Opens the file associated with the instance.
     *
     * @return True if the file is successfully opened, false otherwise.
     */
    bool openFile();

    /**
     * @brief Reads the contents of the opened file and saves it to the provided string.
     *
     * @param FileContent Output parameter for the contents of the file.
     */
    void readFileAndSave(std::string& FileContent);

    /**
     * @brief Closes the opened file.
     */
    void closeFile();

private:
    /**
     * @brief Member variable for storing the filename.
     */
    std::string filename;

    /**
     * @brief Member variable representing the input file stream.
     */
    std::ifstream inputFile;
};

#endif  // FILEREADER_H
