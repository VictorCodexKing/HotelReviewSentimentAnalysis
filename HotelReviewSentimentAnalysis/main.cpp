#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <regex>
#include <windows.h>

#include "sentiment_analysis.hpp"


int main() {
    try {
        ResizableArray<string> positiveWords;
        ResizableArray<string> negativeWords;
        ResizableArray<string> reviews;
        ResizableArray<int> posCounts;
        ResizableArray<int> negCounts;
        ResizableArray<int> userRatings;

        // Load words from text files
        loadWordsFromFile("positive-words.txt", positiveWords);
        loadWordsFromFile("negative-words.txt", negativeWords);

        // Load reviews and ratings from CSV
        int totalWords = 0;
        loadReviewsAndRatingsFromCSV("tripadvisor_hotel_reviews.csv", reviews, userRatings, totalWords);

        // Ask the user if they want to perform binary search
        cout << "Do you want to perform Binary Search? Yes - 1, No - 0\n>> ";
        int performBinarySearch;
        cin >> performBinarySearch;

        if (performBinarySearch == 1) {
            clearScreen();  // Clear the console before starting the analysis

            // Start timer
            Timer::time_point start = Timer::now();

            // Perform Semantic analysis using binary search and print summary
            performSemanticAnalysis(reviews, positiveWords, posCounts, negativeWords, negCounts, totalWords, start);

            // Stop timer
            Timer::time_point stop = Timer::now();
            Ms duration = std::chrono::duration_cast<Ms>(stop - start);

            // Display the results
            int totalReviews = reviews.getSize();
            int positiveWordCount = posCounts.getSize();
            int negativeWordCount = negCounts.getSize();
            displaySummary(totalReviews, totalWords, positiveWordCount, negativeWordCount, duration);
            cout << "\n";

            displayMenu(positiveWords, posCounts, negativeWords, negCounts, reviews, userRatings, totalWords, duration);
        }

        else {
            cout << "Exiting without performing binary search.\n";
        }

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}