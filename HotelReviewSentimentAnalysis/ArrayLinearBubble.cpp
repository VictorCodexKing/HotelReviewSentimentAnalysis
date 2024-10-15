#include <iostream>
#include <chrono> 
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>

#include "ArrayLinearBubble.hpp"

using namespace std;
using namespace std::chrono;

int main() {
    // Arrays to hold positive and negative words
    string positiveWords[MAX_WORDS], negativeWords[MAX_WORDS];
    // Load positive and negative words
    int positiveCount = loadtxtFile("positive-words.txt", positiveWords, MAX_WORDS);
    int negativeCount = loadtxtFile("negative-words.txt", negativeWords, MAX_WORDS);
    // Load reviews from CSV file
    int reviewCount = loadCSV("tripadvisor_hotel_reviews.csv", reviews, MAX_REVIEWS);
    const int batch_Size = 50;
    // Variables to store total positive and negative word counts across batches
    int totalPositiveWords = 0, totalNegativeWords = 0;
    // Process reviews in two batches
    for (int batch_Start = 0; batch_Start < reviewCount; batch_Start += batch_Size) {
        int batchEnd = min(batch_Start + batch_Size, reviewCount); // Ensure we don't exceed the number of reviews
        //cout << "\n--- Analyzing batch of reviews from " << batch_Start + 1 << " to " << batchEnd << " ---" << endl;
        // Process each review in the current batch
        for (int i = batch_Start; i < batchEnd; i++) {
            countSentimentWords(reviews[i], positiveWords, positiveCount, negativeWords, negativeCount);
            totalPositiveWords += reviews[i].positiveCount;
            totalNegativeWords += reviews[i].negativeCount;
            compareRatingWithSentiment(reviews[i]);  // Call updated function
            processWordFrequency(reviews[i].comment); // Process word frequency for each review
        }
    }


    // Measure time for bubble sort after processing all batches
    auto startBubbleSort = high_resolution_clock::now();
    bubbleSort(wordFreqArray, wordCount);
    auto stopBubbleSort = high_resolution_clock::now();
    auto durationBubbleSort = duration_cast<microseconds>(stopBubbleSort - startBubbleSort);
    cout << "Time taken (bubble sort): " << durationBubbleSort.count() << " microseconds" << endl;

    // Display sorted word frequencies
    displaySortedWordFrequencies(wordFreqArray, wordCount);

    // Summary output after analyzing all batches
    cout << "Number of positive words = " << totalPositiveWords << endl;
    cout << "Number of negative words = " << totalNegativeWords << endl;

    return 0;
}