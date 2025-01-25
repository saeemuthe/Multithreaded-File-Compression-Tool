#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <zlib.h>

using namespace std;

const int CHUNK_SIZE = 1024 * 1024; // 1 MB
mutex file_mutex;

// Function to compress a chunk of data
void compressChunk(const vector<char>& input, vector<char>& output) {
    uLongf compressedSize = compressBound(input.size());
    output.resize(compressedSize);

    if (compress((Bytef*)output.data(), &compressedSize, (const Bytef*)input.data(), input.size()) != Z_OK) {
        cerr << "Compression failed.\n";
    }
    output.resize(compressedSize); // Resize to actual compressed size
}

// Multithreaded file compression
void compressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);
    if (!inFile || !outFile) {
        cerr << "Error opening files.\n";
        return;
    }

    vector<thread> threads;
    vector<vector<char>> compressedChunks;
    int chunkIndex = 0;

    while (true) {
        vector<char> buffer(CHUNK_SIZE);
        inFile.read(buffer.data(), CHUNK_SIZE);
        streamsize bytesRead = inFile.gcount();
        if (bytesRead == 0) break;

        buffer.resize(bytesRead);
        compressedChunks.emplace_back();

        threads.emplace_back([&, chunkIndex]() {
            compressChunk(buffer, compressedChunks[chunkIndex]);

            lock_guard<mutex> lock(file_mutex);
            uint32_t size = compressedChunks[chunkIndex].size();
            outFile.write(reinterpret_cast<char*>(&size), sizeof(size));
            outFile.write(compressedChunks[chunkIndex].data(), size);
        });

        chunkIndex++;
    }

    for (auto& thread : threads) if (thread.joinable()) thread.join();
    cout << "File compression completed.\n";
}

// File decompression
void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);
    if (!inFile || !outFile) {
        cerr << "Error opening files.\n";
        return;
    }

    while (true) {
        uint32_t compressedSize;
        inFile.read(reinterpret_cast<char*>(&compressedSize), sizeof(compressedSize));
        if (inFile.eof()) break;

        vector<char> compressedData(compressedSize);
        inFile.read(compressedData.data(), compressedSize);

        uLongf decompressedSize = CHUNK_SIZE;
        vector<char> decompressedData(decompressedSize);

        if (uncompress((Bytef*)decompressedData.data(), &decompressedSize, 
                       (const Bytef*)compressedData.data(), compressedSize) != Z_OK) {
            cerr << "Decompression failed.\n";
            break;
        }

        outFile.write(decompressedData.data(), decompressedSize);
    }
    cout << "File decompression completed.\n";
}

// Main function
int main() {
    string inputFile = "input.txt";
    string compressedFile = "compressed.dat";
    string decompressedFile = "decompressed.txt";

    cout << "Starting compression...\n";
    compressFile(inputFile, compressedFile);

    cout << "Starting decompression...\n";
    decompressFile(compressedFile, decompressedFile);

    return 0;
}
