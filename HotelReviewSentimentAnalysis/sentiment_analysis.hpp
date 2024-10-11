#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <regex>
#include <windows.h>

using namespace std;
using namespace chrono;

using Ms = chrono::milliseconds;
using Mu = chrono::microseconds;
using Timer = chrono::high_resolution_clock;

// Templated resizable array class (similar to std::vector functionality)
template <typename T>
class ResizableArray {
private:
    T* data;
    int capacity;
    int size;

    // Resize the array when it's full
    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Constructor
    explicit ResizableArray(int initialCapacity = 10)
        : size(0), capacity(initialCapacity) {
        if (capacity <= 0) {
            throw invalid_argument("Capacity must be greater than 0");
        }
        data = new T[capacity];
    }

    // Destructor
    ~ResizableArray() {
        delete[] data;
    }

    // Add an element
    void add(const T& element) {
        if (size == capacity) {
            resize();
        }
        data[size++] = element;
    }

    // Get an element by index
    T get(int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    // Get the current size
    int getSize() const {
        return size;
    }

    // Binary Search
    int binarySearch(const T& element) const {
        int low = 0;
        int high = size - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (data[mid] == element) {
                return mid; // Found the element
            }
            else if (data[mid] < element) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }
        return -1; // Element not found
    }

    // Insertion Sort by Frequency (used in the "4 - Sort words by frequency" option)
    void insertionSort(ResizableArray<int>& frequencies) {
        for (int i = 1; i < size; i++) {
            T wordKey = data[i];
            int freqKey = frequencies.get(i);
            int j = i - 1;

            // Sort by frequency, then alphabetically if frequencies are equal
            while (j >= 0 && (frequencies.get(j) > freqKey || (frequencies.get(j) == freqKey && data[j] > wordKey))) {
                data[j + 1] = data[j];
                frequencies.add(frequencies.get(j + 1));  // Adjust the frequency array accordingly
                j--;
            }
            data[j + 1] = wordKey;
            frequencies.add(freqKey);
        }
    }
};


// Function prototypes
void loadWordsFromFile(const string& filename, ResizableArray<string>& words);
void loadReviewsAndRatingsFromCSV(const string& filename, ResizableArray<string>& reviews, ResizableArray<int>& userRatings, int& totalWords);
void clearScreen();
string cleanWord(const string& word);
int roundSentimentScore(double score);
double calculateSentimentScore(int positiveCount, int negativeCount);
void countWordsInReview(const string& review, const ResizableArray<string>& positiveWords, const ResizableArray<string>& negativeWords, ResizableArray<string>& positiveWordsFound, ResizableArray<string>& negativeWordsFound, int& positiveCount, int& negativeCount);
void generateSentimentAnalysis(const ResizableArray<string>& reviews, const ResizableArray<int>& userRatings, const ResizableArray<string>& positiveWords, const ResizableArray<string>& negativeWords, int reviewNumber);
void sortWordsByFrequency(ResizableArray<string>& positiveWords, ResizableArray<int>& posFrequencies, ResizableArray<string>& negativeWords, ResizableArray<int>& negFrequencies);
int countOccurrences(const ResizableArray<int>& indices, int index);
void displayWordsWithOccurrences(ResizableArray<string>& words, ResizableArray<int>& counts);
void countWordsInReviewBinary(const string& review, const ResizableArray<string>& positiveWords, ResizableArray<int>& posCounts, const ResizableArray<string>& negativeWords, ResizableArray<int>& negCounts);
void performSemanticAnalysis(const ResizableArray<string>& reviews, const ResizableArray<string>& positiveWords, ResizableArray<int>& posCounts, const ResizableArray<string>& negativeWords, ResizableArray<int>& negCounts, int totalWords, Timer::time_point start);
void displaySummary(int totalReviews, int totalWords, int positiveWordCount, int negativeWordCount, Ms timeElapsed);
void displayMenu(ResizableArray<string>& positiveWords, ResizableArray<int>& posCounts, ResizableArray<string>& negativeWords, ResizableArray<int>& negCounts, ResizableArray<string>& reviews, ResizableArray<int>& userRatings, int totalWords, Ms timeElapsed);

