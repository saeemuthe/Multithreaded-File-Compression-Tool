#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <zlib.h>
#include <string>
#include <chrono>

void compressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    
    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    const size_t bufferSize = 128 * 1024; // 128 KB buffer
    char* inBuffer = new char[bufferSize];
    char* outBuffer = new char[bufferSize];

    while (inFile.read(inBuffer, bufferSize) || inFile.gcount() > 0) {
        uLongf compressedSize = bufferSize;
        if (compress((Bytef*)outBuffer, &compressedSize, (const Bytef*)inBuffer, inFile.gcount()) != Z_OK) {
            std::cerr << "Compression failed!" << std::endl;
            break;
        }
        outFile.write(outBuffer, compressedSize);
    }

    delete[] inBuffer;
    delete[] outBuffer;
    inFile.close();
    outFile.close();
}

void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    
    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    const size_t bufferSize = 128 * 1024; // 128 KB buffer
    char* inBuffer = new char[bufferSize];
    char* outBuffer = new char[bufferSize];

    while (inFile.read(inBuffer, bufferSize) || inFile.gcount() > 0) {
        uLongf decompressedSize = bufferSize;
        if (uncompress((Bytef*)outBuffer, &decompressedSize, (const Bytef*)inBuffer, inFile.gcount()) != Z_OK) {
            std::cerr << "Decompression failed!" << std::endl;
            break;
        }
        outFile.write(outBuffer, decompressedSize);
    }

    delete[] inBuffer;
    delete[] outBuffer;
    inFile.close();
    outFile.close();
}

void compressFileThreaded(const std::string& inputFile, const std::string& outputFile) {
    std::thread compressThread(compressFile, inputFile, outputFile);
    compressThread.join();
}

void decompressFileThreaded(const std::string& inputFile, const std::string& outputFile) {
    std::thread decompressThread(decompressFile, inputFile, outputFile);
    decompressThread.join();
}

int main() {
    std::string inputFile, outputFile;
    int choice;

    std::cout << "File Compression Tool" << std::endl;
    std::cout << "1. Compress a file" << std::endl;
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
    std
