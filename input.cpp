


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <963906>

  This file contains the code responsible for opening and closing file
  streams. The actual handling of the data from that stream is handled
  by the functions in data.cpp. See the header file for additional comments.
  
  Each function you must implement has a TODO in its comment block. You may
  have to implement additional functions. You may implement additional
  functions not specified.
 */
#include <iostream>
#include <vector>
#include <sstream>

#include "datasets.h"
#include "input.h"
#include "bethyw.h"

/*
  TODO: InputSource::InputSource(source)

  Constructor for an InputSource.

  @param source
    A unique identifier for a source (i.e. the location).
*/
InputSource::InputSource(const std::string& source) {
  this -> InputSource::fileSource = source;
}

/*
  TODO: InputSource::getSource()

  This function should be callable from a constant context.

  @return
    A non-modifable value for the source passed into the construtor.
*/
std::string InputSource::getSource() const {
    return this -> fileSource;
}

/*
  TODO: InputFile:InputFile(path)

  Constructor for a file-based source.

  @param path
    The complete path for a file to import.

  @example
    InputFile input("data/areas.csv");
*/
InputFile::InputFile(const std::string& filePath) : InputSource(filePath) {
    // This is when there is a hyphen and it is within the bounds
    std::istringstream fileChars;
    fileChars.str(filePath);
    std::vector<std::string> givenArgs;
    std::string tempFile;
    while (std::getline(fileChars, tempFile, '/')) {
        givenArgs.push_back(tempFile);
    }
    if (givenArgs.size() == 1) {
        this->directory = givenArgs.at(0);
    } else if (givenArgs.size() > 1) {
        this->directory = givenArgs.at(0);
        this->file = givenArgs.at(1);
    }
}

std::string InputFile::getFile() const {
    return this -> file;
}

/**
 * Destructor to close filestream
 */
InputFile::~InputFile() {
    fileStream.close();
}
/*
  TODO: InputFile::open()

  Open a file stream to the file path retrievable from getSource()
  and return a reference to the stream.

  @return
    A standard input stream reference

  @throws
    std::runtime_error if there is an issue opening the file, with the message:
    InputFile::open: Failed to open file <file name>

  @example
    InputFile input("data/areas.csv");
    input.open();
*/

std::istream& InputFile::open() {
    std::string datasetsStr = "datasets/";
    std::string pathToOpen = InputSource::getSource();

    bool fileFound = false;
    // Retrieve all valid datasets, see datasets.h
    size_t numDatasets = BethYw::InputFiles::NUM_DATASETS;
    auto &allDatasets = BethYw::InputFiles::DATASETS;


    for (unsigned int i = 0; i < numDatasets; i++) {
        if ((datasetsStr+allDatasets[i].FILE) == pathToOpen) {
            fileFound = true;
            break;
        } else if ((datasetsStr+BethYw::InputFiles::AREAS.FILE) == pathToOpen) {
            fileFound = true;
            break;
        }
    }
    if (fileFound) {
        fileStream.open(pathToOpen, std::fstream::in);
        return fileStream;
    } else {
        std::string error1 = "";
        if ((pathToOpen.find('/') != std::string::npos) && (pathToOpen.find("datasets") == std::string::npos)) {
            error1 = "Error importing dataset:\n";
        }
        std::string exception = "InputFile::open: Failed to open file " + pathToOpen;
        throw std::runtime_error(error1 + exception);
    }

}

std::string InputFile::getDir() const {
    std::string returnStr =  this -> directory + DIR_SEP;
    return returnStr;
}
