#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <string>
#include "IStream.h"

class TerminalStream : public IStream {
    std::queue<char> inputQueue; // Queue to hold characters entered by the user

public:
    size_t Write(const void* data, size_t size, int /*timeoutMs*/ = 0) override {
        const char* charData = static_cast<const char*>(data);
        std::cout.write(charData, size);
        std::cout.flush();
        return size;
    }

    size_t Read(void* data, size_t size, int timeoutMs = 0) override {
        char* charData = static_cast<char*>(data);
        size_t bytesRead = 0;

        auto start = std::chrono::steady_clock::now();

        while (bytesRead < size) {
            // Check if there is data in the queue to process
            if (!inputQueue.empty()) {
                charData[bytesRead++] = inputQueue.front();
                inputQueue.pop();
            }
            else {
                // If the queue is empty, wait for new input from the user
                std::string line;

                if (std::getline(std::cin, line)) {
                    // Add each character from the line to the queue, followed by a newline
                    for (char ch : line) {
                        inputQueue.push(ch);
                    }
                    inputQueue.push('\n'); // Simulate the Enter key press as a newline
                }
                else {
                    // If input fails, break out (e.g., end of stream)
                    break;
                }
            }

            // Check for timeout
            if (timeoutMs > 0) {
                auto now = std::chrono::steady_clock::now();
                int elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
                if (elapsedMs > timeoutMs) {
                    break; // Timeout reached
                }
            }
        }

        return bytesRead;
    }
};