# Multithreaded-File-Compression-Tool
Creating a multithreaded file compression and decompression tool in C++ can be a great project to demonstrate your understanding of file handling, multithreading, and performance optimization. Below is a simple implementation using the zlib library for compression and decompression, along with C++11 threads for multithreading.
Prerequisites
Install zlib: Make sure you have the zlib library installed on your system. You can download it from zlib.net or install it via a package manager.


#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <zlib.h>
#include <string>
#include <chrono>
#include <iostream>: For input and output operations.
#include <fstream>: For file stream operations (reading and writing files).
#include <thread>: For using threads to perform multithreading.
#include <vector>: Although not used in this code, it can be useful for managing collections of data.
#include <zlib.h>: For using the zlib compression library.
#include <string>: For using the std::string class.
#include <chrono>: For measuring the time taken for compression and decompression.


void compressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    ...
}
This function takes the input file name and output file name as parameters.
It opens the input file for reading and the output file for writing in binary mode.
If either file fails to open, an error message is displayed.


const size_t bufferSize = 128 * 1024; // 128 KB buffer
char* inBuffer = new char[bufferSize];
char* outBuffer = new char[bufferSize];
A buffer of 128 KB is allocated for reading data from the input file and for storing compressed data.


while (inFile.read(inBuffer, bufferSize) || inFile.gcount() > 0) {
    uLongf compressedSize = bufferSize;
    if (compress((Bytef*)outBuffer, &compressedSize, (const Bytef*)inBuffer, inFile.gcount()) != Z_OK) {
        std::cerr << "Compression failed!" << std::endl;
        break;
    }
    outFile.write(outBuffer, compressedSize);
}
The loop reads data from the input file into inBuffer until the end of the file is reached.
The compress function from zlib is called to compress the data. If compression fails, an error message is displayed.
The compressed data is then written to the output file.


delete[] inBuffer;
delete[] outBuffer;
inFile.close();
outFile.close();
The allocated buffers are deleted, and the files are closed


void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    ...
}
This function is similar to the compressFile function but performs decompression using the uncompress function from zlib.
The logic for reading from the input file and writing to the output file is similar to the compression function.


void compressFileThreaded(const std::string& inputFile, const std::string& outputFile) {
    std::thread compressThread(compressFile, inputFile, outputFile);
    compressThread.join();
}

void decompressFileThreaded(const std::string& inputFile, const std::string& outputFile) {
    std::thread decompressThread(decompressFile, inputFile, outputFile);
    decompressThread.join();
}
These functions create a new thread for the compression and decompression functions, respectively.
The join() method is called to wait for the thread to finish before proceeding.


int main() {
    std::string inputFile, outputFile;
    int choice;

    std::cout << "File Compression Tool" << std::endl;
    std:: cout << "1. Compress a file" << std::endl;
    std::cout << "2. Decompress a file" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    std::cin.ignore(); // Ignore the newline character left in the input buffer

    std::cout << "Enter input file name: ";
    std::getline(std::cin, inputFile);
    std::cout << "Enter output file name: ";
    std::getline(std::cin, outputFile);

    auto start = std::chrono::high_resolution_clock::now();

    if (choice == 1) {
        compressFileThreaded(inputFile, outputFile);
    } else if (choice == 2) {
        decompressFileThreaded(inputFile, outputFile);
    } else {
        std::cerr << "Invalid choice!" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Operation completed in " << duration.count() << " seconds." << std::endl;

    return 0;
}
The main function serves as the entry point of the program.
It prompts the user to choose between compressing or decompressing a file and to enter the respective file names.
The time taken for the operation is measured using std::chrono to demonstrate performance gains.
Depending on the user's choice, it calls the appropriate threaded function to perform the operation.
Finally, it outputs the duration of the operation, providing feedback on performance.
    
