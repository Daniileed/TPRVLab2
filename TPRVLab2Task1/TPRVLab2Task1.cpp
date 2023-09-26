// TPRVLab2Task1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int MAXTHREADS = 4;

const int Length = 1024; 
const int Height = 768; 
string filename = "C://Users//danii//source//repos//TPRVLab2Task1//" + std::to_string(Length) + "x"+ std::to_string(Height) +".jpg";

int**  Rv = new int* [Height];
int**  Gv = new int*[Height];
int**  Bv = new int* [Height];
int** Iv = new int* [Height];

int** MRv = new int* [Height];
int** MX = new int* [Height];
int** MY = new int* [Height];

int MRMax = 0;

const int FSX[3][3] = { 
{ -1,0,1 },
{-2,0,2},
{-1,0,1 } 
};

const int FSY[3][3] = { 
    {-1,-2,-1},
    {0,0,0},
    {1,2,1 }};


void check_elementX(int i, int j, int elX, int elY) {
    if ((i>= 0) && (i< Height) && (j>=0) && (j< Length))
    MX[i][j] = Iv[i][j] + FSX[elX][elY];
}

void check_elementY(int i, int j, int elX, int elY) {
    if ((i >= 0) && (i < Height) && (j >= 0) && (j < Length))
        MY[i][j] = Iv[i][j] + FSY[elX][elY];
}

int main() {
    for (int i = 0; i < Height; i++) {
        Rv[i] = new int[Length];
        Gv[i] = new int[Length];
        Bv[i] = new int[Length];

        Iv[i] = new int [Length];

        MRv[i] = new int [Length];
        MX[i] = new int [Length];
        MY[i] = new int [Length];
    }

    Mat img_color = imread(filename, 1);
    namedWindow("color image", WINDOW_NORMAL);
    imshow("color image", img_color);
    waitKey(0);
    destroyAllWindows();
    
    //Single thread
    auto start_1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Length; j++) {
            Bv[i][j] = img_color.at<cv::Vec3b>(i, j)[0];
            Gv[i][j] = img_color.at<cv::Vec3b>(i, j)[1];
            Rv[i][j] = img_color.at<cv::Vec3b>(i, j)[2];

            Iv[i][j] = floor((Rv[i][j] + Gv[i][j] + Bv[i][j]) / 3);
        }
    }

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Length; j++) {
            check_elementX(i - 1, j - 1, 0,0);
            check_elementX(i , j - 1, 1, 0);
            check_elementX(i + 1, j - 1, 2, 0);

            check_elementX(i - 1, j + 1, 0, 2);
            check_elementX(i , j + 1, 1, 2);
            check_elementX(i + 1, j + 1, 2, 2);

                }
            }

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Length; j++) {
           check_elementY(i - 1, j - 1, 0, 0);
            check_elementY(i - 1, j , 0, 1);
            check_elementY(i - 1, j + 1, 0, 2);
            
            check_elementY(i + 1, j - 1, 2, 0);
            check_elementY(i + 1, j, 2, 1);
            check_elementY(i + 1, j + 1, 2, 2);
        }
    }

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Length; j++) {
           MRv[i][j] = floor(sqrt(pow(MX[i][j], 2) + pow(MY[i][j], 2)));
        }
    }

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Length; j++) {
            if (MRv[i][j] >= MRMax)
                MRMax = MRv[i][j];
        }
    }
 
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Length; j++) {
            MRv[i][j] = (MRv[i][j] * 255) / MRMax;
        }
    }

    Mat Sobel_scale = Mat::zeros(Height, Length, CV_8UC1);

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Length; j++) {
            Sobel_scale.at<uchar>(i, j) = MRv[i][j];
        }
    }

    namedWindow("Sobel_scale", WINDOW_NORMAL);
    imshow("Sobel_scale", Sobel_scale);
    waitKey(0);
    destroyAllWindows();
   


    auto end_1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> dur1 = (end_1 - start_1) * 1000;
    std::cout << "Time: " << dur1.count() << " milliseconds\n\n";

    //OpenMP
    auto start_2 = std::chrono::high_resolution_clock::now();

    auto end_2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> dur2 = (end_2 - start_2) * 1000;
    std::cout << "Time: " << dur2.count() << " milliseconds\n\n";

    return 0;
}


