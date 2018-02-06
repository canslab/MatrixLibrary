
#include <iostream>
#include "Matrix.h"
#include "Timer.h"
#include <opencv2\opencv.hpp>

int main(void)
{
	//{
	//	const int ROWS = 1000;
	//	const int COLS = 3;
	//	const int N = ROWS * COLS;
	//	int *arr = new int[N];
	//	int *brr = new int[N];
	//	for (int i = 0; i < N; ++i)
	//	{
	//		arr[i] = i;
	//		brr[i] = i;
	//	}
	//	Timer timer;
	//	Matrix<int> matrix1(ROWS, COLS, arr);
	//	Matrix<int> matrix2(COLS, ROWS, brr);


	//	for (int i = 0; i < 10; ++i)
	//	{
	//		timer.StartStopWatch();
	//		Matrix<int>::NaiveMultiplication(matrix1, matrix2);
	//		std::cout << timer.EndStopWatch() << " ";

	//		timer.StartStopWatch();
	//		Matrix<int>::Multiplication(matrix1, matrix2);
	//		std::cout << timer.EndStopWatch() << std::endl;
	//	}
	//}


	//int arr[] = { 1,2,3, 0,1,4, 5,6,0 };
	//Matrix<int> aa(3, 3, arr);

	//aa.Print();
	////Matrix<double> tt;
	//Matrix<int> testing(3, 3, 0.0);
	//bool bExist = testing.Get3By3InversedMatrix(aa, testing);

	//std::cout << bExist << "\n\n\n\n";
	//testing.Print();
	double arr[] = { 1,2,3,4,5,6,7,8,9 };
	double *brr = nullptr;
	TransposeMatrix(3, 3, arr, brr);
	return 0;
}