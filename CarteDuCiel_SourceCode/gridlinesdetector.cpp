#include "gridlinesdetector.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

GridlinesDetector::GridlinesDetector() {}//GridlinesDetector


void GridlinesDetector::findPeaks(
    vector<double> A, // input data 
    vector<double> &pos, // indices
    vector<double> &peaks, // values
    const double thr)
{
    // prev is intended to hold the index of the previous maximum value, i.e., store the index of the previous maximum value found.
    int prev = 0;
    // keeps track of the index of the current maximum value encountered so far in the array.
    int cur = 0;

//The loop starts from index 1 and goes through each element of the input array until the last element. 
//It effectively checks each element against the current maximum.
    for(int k = 1; k < A.size(); k++)//for k in interval [1,n]
    {
        // If the current element A[k] is greater than A[cur], 
        // this means we've found a new maximum value in the array.
        if(A[k] > A[cur])//Condition to check if the current element is greater than the last known maximum:
        {
            //The Value is updated to current. 
            //This means it stores the index of the previous maximum value.
            prev = cur;
            // The current is updated to i. 
            //This means it now tracks the index of the new maximum value.
            cur = k;
        }
        else //else condition for when the current element is less than the known maximum:
        {
            if(A[k] < A[cur])
            {
                if(A[prev] < A[cur])
                {
                    if(A[cur] > thr)
                    //if(A[cur] > 0)
                    {
                        pos.push_back(cur);
                        peaks.push_back(A[cur]);
                    }
                }
                prev = cur;
                cur = k;
            }
        }
    }
}// findPeaks

// Define threshold
void GridlinesDetector::threshold(double max_h_frame1,
                            double max_h_frame2,
                            double min_value,
                            double &thr)
{
          double max_pvalue = 0.0;

          if(max_h_frame1 > max_h_frame2)
          {
              max_pvalue = max_h_frame1;
          }
          else if(max_h_frame1 < max_h_frame2)
          {
              max_pvalue = max_h_frame2;
          }
           // Range rule of the thumb
          thr = (max_pvalue - min_value)/4.0;
          //cout << "\nthr (max/4) =" << thr << endl;
}// threshold

// Find index
double GridlinesDetector::findIndex(vector<double> const &A, double const &index) //return index
{
    for (int i = 0; i < A.size(); i++)
    {
        if (A[i] == index)
        {
            return i;
        }
    }
    // Return -1 if no match is found
    return -1;
}// findIndex


// Find value
vector<double> GridlinesDetector::findValues(vector<double> const &A, vector<double> const &vals)
{
    vector<double> elems;
    for (int i = 0; i < vals.size(); i++)
    {
        double indx = vals[i];
        if (indx >= 0 && indx < A.size())
        {
            elems.push_back(A[indx]);
        }
        else
        {
            // throw an exception or return a default value if index is out of range
            throw std::out_of_range("Index out of range");
        }
    }
    // Return the vector of items
    return elems;
}// getValuesAtIndices


// FIND INDICES
vector<double> GridlinesDetector::findIndices(vector<double> const &A, vector<double> const &indices)
{
    vector<double> results;
    for (int i = 0; i < A.size(); i++)
    {
        if (find(indices.begin(), indices.end(), A[i]) != indices.end())
        {
            results.push_back(i);
        }
    }
    return results;
}// findIndices

// Print out vector 
void GridlinesDetector::print(vector<double> vect)
{
    for(int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << ", ";
    }
    cout << endl;
}// print


vector<double> GridlinesDetector::excludeNeighbours(const vector<double> numbers)
{
    vector<double> result;
    int count = numbers.size();

    if (count <= 27)
    {
        return numbers;
    }
    else if (count > 27)
    {
        // Check the first value
        int diffFirstSecond = abs(numbers[0] - numbers[1]);
        if (diffFirstSecond >= 200)
        {
            result.push_back(numbers[0]); // Keep the first element
        }

            // size = 6
            //0 - 5 (5 included) indices i.e. for (int i = 0; i < count; ++i)
                //  indices:   0,  1,  2,  3,  4,  5
                //  numbers: 10, 11, 12, 13, 14, 15
            //1 - 3 (3 included) indices i.e. for (int i = 1; i < count - 2; ++i)
                //   numbers:  11, 12, 13,
                //  indices:   1,  2,  3,
        // Check the middle values
        for (int i = 1; i < count - 2; ++i)
        {
            int diffCurrentNext = abs(numbers[i] - numbers[i + 1]);

            if (diffCurrentNext >= 200)
            {
                result.push_back(numbers[i]); // Keep the current element
            }
        }

        int diffLastPrev = abs(numbers[count - 1] - numbers[count - 2]);

        // Check the second-to-last and the last element in the input sequence
        if (diffLastPrev >= 200)
        {
            // Keep both: the second-to-last element and the last element
            result.push_back(numbers[count - 2]);
            result.push_back(numbers[count - 1]);
        }
        else if (diffLastPrev < 200)
        {
            // Keep only the second-to-last element
            result.push_back(numbers[count - 2]);
        }

        return result;
    }
}//excludeNeighbours
