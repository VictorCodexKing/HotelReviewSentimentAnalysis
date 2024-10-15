#pragma once

#include <iostream>
#include <chrono> 
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#ifndef ARRAY_BUBBLE_LINEAR_HPP
#define ARRAY_BUBBLE_LINEAR_HPP


using namespace std;
using namespace std::chrono;

const int MAX_REVIEWS = 500;
const int MAX_WORDS = 10000;

// Struct to store each review's feedback
struct Review {
    string comment;
    int rating;
    int positiveCount = 0;
    int negativeCount = 0;
    double sentimentScore;
};
// Struct for word frequencies
struct WordFrequency {
    string word;
    int frequency;
};

// Arrays to hold reviews and word frequencies
Review reviews[MAX_REVIEWS];
WordFrequency wordFreqArray[MAX_WORDS];
int wordCount = 0;
// Function to clean and remove punctuation from a word
string cleanWord(const string& word) {
    string cleaned = word;
    cleaned.erase(remove_if(cleaned.begin(), cleaned.end(), ::ispunct), cleaned.end());
    transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ::tolower); // convert to lowercase
    return cleaned;
}
// Linear search to find word and return true if found
bool linearSearch(const string& word, const string wordArray[], int size) {
    for (int i = 0; i < size; i++) {
        if (wordArray[i] == word) {
            return true; // Word found
        }
    }
    return false; // Word not found
}
// Function to load words from a file into an array
int loadtxtFile(const string& fileName, string wordArray[], int maxWords) {
    ifstream file(fileName);
    if (!file) {
        cerr << "Error open file: " << fileName << endl;
        return 0;
    }

    string word;
    int count = 0;
    while (file >> word && count < maxWords) {
        wordArray[count++] = word;
    }

    file.close();
    return count;
}
// Function to load reviews from CSV file
int loadCSV(const string& fileName, Review reviews[], int maxReviews) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error open file: " << fileName << endl;
        return 0;
    }

    string line;
    int reviewCount = 0;
    while (getline(file, line) && reviewCount < maxReviews) {
        stringstream ss(line);
        string reviewText;
        int rating;

        getline(ss, reviewText, ',');  // Extract review text
        ss >> rating;                  // Extract rating

        reviews[reviewCount].comment = reviewText;
        reviews[reviewCount].rating = rating;
        reviewCount++;
    }

    file.close();
    return reviewCount;
}
// Function to count sentiment words in a review using linear search
void countSentimentWords(Review& review, const string positiveWords[], int posSize, const string negativeWords[], int negSize) {
    istringstream iss(review.comment);
    string word;

    // Variables to store total time taken for positive and negative searches
    auto totalPosTime = microseconds(0);
    auto totalNegTime = microseconds(0);

    while (iss >> word) {
        word = cleanWord(word); // Clean the word

        // Measure time for positive word search
        auto startPosSearch = high_resolution_clock::now();
        if (linearSearch(word, positiveWords, posSize)) {
            review.positiveCount++;
        }
        auto endPosSearch = high_resolution_clock::now();
        totalPosTime += duration_cast<microseconds>(endPosSearch - startPosSearch);

        // Measure time for negative word search
        auto startNegSearch = high_resolution_clock::now();
        if (linearSearch(word, negativeWords, negSize)) {
            review.negativeCount++;
        }
        auto endNegSearch = high_resolution_clock::now();
        totalNegTime += duration_cast<microseconds>(endNegSearch - startNegSearch);
    }

    // Output the total time taken for positive and negative word searches
    cout << "Time taken (positive word searches): " << totalPosTime.count() << " microseconds" << endl;
    cout << "Time taken (negative word searches): " << totalNegTime.count() << " microseconds" << endl;
}
// Function to calculate sentiment score between 1-5
int calculateSentimentScore(int positiveCount, int negativeCount) {
    int totalWords = positiveCount + negativeCount;
    if (totalWords == 0) return 3; // Neutral if no sentiment words found

    int rawScore = positiveCount - negativeCount;
    int N = totalWords;
    int minRawScore = -N;
    int maxRawScore = N;
    float normalizedScore = (float)(rawScore - minRawScore) / (maxRawScore - minRawScore);
    return 1 + round(4 * normalizedScore);
}
// Function to add or update word frequency using linear search
void addOrUpdateWordFrequency(const string& word) {
    // Search if word exists
    for (int i = 0; i < wordCount; i++) {
        if (wordFreqArray[i].word == word) {
            wordFreqArray[i].frequency++; // Update frequency
            return;
        }
    }

    // If word does not exist, add it to array
    if (wordCount < MAX_WORDS) {
        wordFreqArray[wordCount].word = word;
        wordFreqArray[wordCount].frequency = 1; // Initialize frequency to 1
        wordCount++;
    }
}
// Function to process word frequencies for each review
void processWordFrequency(const string& review) {
    istringstream iss(review);
    string word;
    while (iss >> word) {
        word = cleanWord(word);
        addOrUpdateWordFrequency(word); // Add or update word frequency
    }
}
// Bubble sort to sort word frequencies in ascending order
void bubbleSort(WordFrequency arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].frequency > arr[j + 1].frequency) {
                WordFrequency temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
// Function to display sorted word frequencies
void displaySortedWordFrequencies(WordFrequency arr[], int n) {
    cout << "Word Frequencies (ascending order):\n";
    for (int i = 0; i < n; i++) {
        cout << arr[i].word << " = " << arr[i].frequency << " times\n";
    }
}
void compareRatingWithSentiment(const Review& review) {
    int posCount = review.positiveCount;
    int negCount = review.negativeCount;

    // Step 1: Calculate Raw Sentiment Score
    int rawSentimentScore = posCount - negCount;
    int totalWords = posCount + negCount;

    // Step 2: Calculate Normalized Score
    int minRawScore = -totalWords;
    int maxRawScore = totalWords;
    float normalizedScore = (totalWords == 0) ? 0 : float(rawSentimentScore - minRawScore) / (maxRawScore - minRawScore);

    // Step 3: Calculate Sentiment Score (1 - 5)
    float sentimentScore = 1 + (4 * normalizedScore);

    // Output the detailed analysis
    cout << "Details of analysis" << endl;
    cout << "Review: " << review.comment << endl;
    cout << "Positive Word Count: " << posCount << endl;
    cout << "Negative Word Count: " << negCount << endl;
    cout << "Raw Sentiment Score: " << posCount << " – " << negCount << " = " << rawSentimentScore << endl;
    cout << "Normalized Score = ((" << rawSentimentScore << ") – (" << minRawScore << ")) / ((" << maxRawScore << ") – (" << minRawScore << ")) = " << normalizedScore << endl;
    cout << "Sentiment Score (1 – 5) = 1 + ( 4 × " << normalizedScore << " ) = " << fixed << setprecision(2) << sentimentScore << endl;

    int userRating = review.rating;
    cout << "Rating by user = " << userRating << endl;

    // Output if sentiment score matches user rating
    if (round(sentimentScore) == userRating) {
        cout << "User's evaluation matches the sentiment score given by the analysis." << endl;
    }
    else {
        cout << "User's evaluation does not match the sentiment score given by the analysis." << endl;
    }
    cout << "--------------------------------" << endl;
}



#endif
