#pragma once
#include <vector>
#include <cassert>
#include <iostream>

template <typename T>

class Matrix
{
public:
	static Matrix<T> NaiveMultiplication(const Matrix& in_mat1, const Matrix& in_mat2);
	static Matrix<T> Multiplication(const Matrix& in_mat1, const Matrix& in_mat2);
	inline bool Matrix<T>::Get3By3InversedMatrix(const Matrix & in_mat, Matrix<T>& out_inversedMatrix);

	enum class COPY_STYLE {
		SHALLOW_COPY,
		DEEP_COPY
	};

public:
	int GetRows() const
	{
		return m_nRows;
	}
	int GetCols() const
	{
		return m_nCols;
	}
	int GetTotalSize() const
	{
		return m_nRows * m_nCols;
	}
	void Print() const
	{
		for (int rowIndex = 0; rowIndex < m_nRows; ++rowIndex)
		{
			for (int colIndex = 0; colIndex < m_nCols; ++colIndex)
			{
				std::cout << GetAt(rowIndex, colIndex) << ' ';
			}
			std::cout << "\n";
		}
	}
	T GetAt(int in_rowIndex, int in_colIndex) const
	{
		assert(in_rowIndex < m_nRows && in_colIndex < m_nCols);
		return m_rowMajorData[in_rowIndex * m_nCols + in_colIndex];
	}
	Matrix<T> GetTranpose() const;
	const T* GetColumnVector(const int in_colIndex) const
	{
		return m_colMajorData + (m_nRows * in_colIndex);
	}
	const T* GetRowVector(const int in_rowIndex) const
	{
		return m_rowMajorData + (m_nCols * in_rowIndex);
	}

public:
	void SetAt(int in_rowIndex, int in_colIndex, const T in_value)
	{
		m_rowMajorData[in_rowIndex * m_nCols + in_colIndex] = in_value;
		m_colMajorData[in_colIndex * m_nRows + in_rowIndex] = in_value;
	}
	Matrix& operator=(const Matrix& in_copiedMat)
	{
		this->m_nRows = in_copiedMat.GetRows();
		this->m_nCols = in_copiedMat.GetCols();
		this->m_rowMajorData = new T[this->GetTotalSize()];
		this->m_colMajorData = new T[this->GetTotalSize()];
		std::memcpy(this->m_rowMajorData, in_copiedMat.m_rowMajorData, sizeof(T) * GetTotalSize());
		std::memcpy(this->m_colMajorData, in_copiedMat.m_colMajorData, sizeof(T) * GetTotalSize());

		return *this;
	}

public:
	Matrix(const int in_nRows, const int in_nCols, const T in_initialValue)
		:m_rowMajorData(new T[in_nRows * in_nCols]),
		m_colMajorData(new T[in_nRows * in_nCols]),
		m_nCols(in_nCols),
		m_nRows(in_nRows)
	{
		for (int rowIndex = 0; rowIndex < m_nRows; ++rowIndex)
		{
			for (int colIndex = 0; colIndex < m_nCols; ++colIndex)
			{
				this->SetAt(rowIndex, colIndex, in_initialValue);
			}
		}
	}
	Matrix(const int in_nRows, const int in_nCols, const T* in_data, COPY_STYLE in_copyStyle)
		:m_nRows(in_nRows),
		m_nCols(in_nCols),
		m_rowMajorData(new T[in_nRows * in_nCols]),
		m_colMajorData(new T[in_nRows * in_nCols])
	{
		for (int rowIndex = 0; rowIndex < in_nRows; ++rowIndex)
		{
			for (int colIndex = 0; colIndex < in_nCols; ++colIndex)
			{
				this->SetAt(rowIndex, colIndex, in_data[rowIndex * in_nCols + colIndex]);
			}
		}
	}
	

	Matrix(const Matrix& in_matrix) :Matrix(in_matrix.GetRows(), in_matrix.GetCols(), 0.0)
	{
		std::memcpy(m_rowMajorData, in_matrix.m_rowMajorData, sizeof(T) * GetTotalSize());
		std::memcpy(m_colMajorData, in_matrix.m_colMajorData, sizeof(T) * GetTotalSize());
	}
	~Matrix()
	{
		delete[] m_rowMajorData;
		delete[] m_colMajorData;
		m_nCols = m_nRows = 0;
	}

private:
	T* m_rowMajorData;
	T* m_colMajorData;
	int m_nRows;
	int m_nCols;
};

/*************************************************/
/*********  Class Function  Implementation *******/
/*************************************************/
template<typename T>
inline Matrix<T> Matrix<T>::NaiveMultiplication(const Matrix & in_mat1, const Matrix & in_mat2)
{
	const int kResultRows = in_mat1.GetRows();
	const int kResultCols = in_mat2.GetCols();
	const int kDim = in_mat1.GetCols();
	T result = 0.0;

	Matrix<T> multipliedMat(kResultRows, kResultCols, 0.0);

	for (int rowIndex = 0; rowIndex < kResultRows; ++rowIndex)
	{
		for (int colIndex = 0; colIndex < kResultCols; ++colIndex)
		{
			for (int i = 0; i < kDim; ++i)
			{
				result += (in_mat1.GetAt(rowIndex, i) * in_mat2.GetAt(i, colIndex));
			}
			multipliedMat.SetAt(rowIndex, colIndex, result);
			result = 0.0;
		}
	}
	return multipliedMat;
}

template<typename T>
inline Matrix<T> Matrix<T>::Multiplication(const Matrix & in_mat1, const Matrix & in_mat2)
{
	const int kResultRows = in_mat1.GetRows();
	const int kResultCols = in_mat2.GetCols();
	const int kDim = in_mat1.GetCols();
	T result = 0.0;
	Matrix<T> multipliedMat(kResultRows, kResultCols, 0.0);

	for (int rowIndex = 0; rowIndex < kResultRows; ++rowIndex)
	{
		for (int colIndex = 0; colIndex < kResultCols; ++colIndex)
		{
			const T* rowVec = in_mat1.GetRowVector(rowIndex);
			const T* colVec = in_mat2.GetColumnVector(colIndex);
			for (int i = 0; i < kDim; ++i)
			{
				result += (rowVec[i] * colVec[i]);
			}
			multipliedMat.SetAt(rowIndex, colIndex, result);
			result = 0.0;
		}
	}
	return multipliedMat;
}

template<typename T>
inline bool Matrix<T>::Get3By3InversedMatrix(const Matrix & in_mat, Matrix<T>& out_inversedMatrix)
{
	const T* rowVector = in_mat.GetRowVector(0);
	out_inversedMatrix = in_mat;
	double det = 0.0;
	bool bInverseExist = false;

	det += +(rowVector[0] * ((in_mat.GetAt(1, 1) * in_mat.GetAt(2, 2)) - (in_mat.GetAt(1, 2) * in_mat.GetAt(2, 1))));
	det += -(rowVector[1] * ((in_mat.GetAt(1, 0) * in_mat.GetAt(2, 2)) - (in_mat.GetAt(1, 2) * in_mat.GetAt(2, 0))));
	det += +(rowVector[2] * ((in_mat.GetAt(1, 0) * in_mat.GetAt(2, 1)) - (in_mat.GetAt(2, 0) * in_mat.GetAt(1, 1))));

	auto transposedInputMatrix = in_mat.GetTranpose();

	bInverseExist = (std::abs(det) < 0.000001) ? false : true;

	out_inversedMatrix.SetAt(0, 0, +(transposedInputMatrix.GetAt(1, 1)*transposedInputMatrix.GetAt(2, 2) - transposedInputMatrix.GetAt(2, 1) * transposedInputMatrix.GetAt(1, 2)) / det);
	out_inversedMatrix.SetAt(0, 1, -(transposedInputMatrix.GetAt(1, 0)*transposedInputMatrix.GetAt(2, 2) - transposedInputMatrix.GetAt(2, 0) * transposedInputMatrix.GetAt(1, 2)) / det);
	out_inversedMatrix.SetAt(0, 2, +(transposedInputMatrix.GetAt(1, 0)*transposedInputMatrix.GetAt(2, 1) - transposedInputMatrix.GetAt(1, 1) * transposedInputMatrix.GetAt(2, 0)) / det);

	out_inversedMatrix.SetAt(1, 0, -(transposedInputMatrix.GetAt(0, 1)*transposedInputMatrix.GetAt(2, 2) - transposedInputMatrix.GetAt(0, 2)*transposedInputMatrix.GetAt(2, 1)) / det);
	out_inversedMatrix.SetAt(1, 1, +(transposedInputMatrix.GetAt(0, 0)*transposedInputMatrix.GetAt(2, 2) - transposedInputMatrix.GetAt(0, 2)*transposedInputMatrix.GetAt(2, 0)) / det);
	out_inversedMatrix.SetAt(1, 2, -(transposedInputMatrix.GetAt(0, 0)*transposedInputMatrix.GetAt(2, 1) - transposedInputMatrix.GetAt(2, 0)*transposedInputMatrix.GetAt(0, 1)) / det);

	out_inversedMatrix.SetAt(2, 0, +(transposedInputMatrix.GetAt(0, 1)*transposedInputMatrix.GetAt(1, 2) - transposedInputMatrix.GetAt(1, 1)*transposedInputMatrix.GetAt(0, 2)) / det);
	out_inversedMatrix.SetAt(2, 1, -(transposedInputMatrix.GetAt(0, 0)*transposedInputMatrix.GetAt(1, 2) - transposedInputMatrix.GetAt(1, 0)*transposedInputMatrix.GetAt(0, 2)) / det);
	out_inversedMatrix.SetAt(2, 2, +(transposedInputMatrix.GetAt(0, 0)*transposedInputMatrix.GetAt(1, 1) - transposedInputMatrix.GetAt(1, 0)*transposedInputMatrix.GetAt(0, 1)) / det);

	return bInverseExist;
}

template<typename T>
inline Matrix<T> Matrix<T>::GetTranpose() const
{
	// Copy Matrix
	Matrix<T> transposedMatrix(*this);

	transposedMatrix.m_nCols = this->GetRows();
	transposedMatrix.m_nRows = this->GetCols();

	T* p1 = transposedMatrix.m_rowMajorData;
	T* p2 = transposedMatrix.m_colMajorData;

	T* temp = p1;
	p1 = p2;
	p2 = temp;

	// Swap it!
	transposedMatrix.m_rowMajorData = p1;
	transposedMatrix.m_colMajorData = p2;

	return transposedMatrix;
}



void TransposeMatrix(const int in_kRowSize, const int in_kColSize, const double* in_mat, double* out_transposedMat);

/*************************************************/
/*********  User Function     Implementation *****/
/*************************************************/
template<typename T>
inline T * TransposeMatrix(const int in_nRowSize, const int in_nColSize, const T * in_matrix)
{
	T* out_matrix = new T[in_nRowSize * in_nColSize];


	return NULL;
}

template<typename T>
inline T * Get3By3Inverse(const int in_nRowSize, const int n_ColSize, const T * in_matrix)
{
	return NULL;
}

template<typename T>
inline T * Multiplication(const int in_nRowSize1, const int in_nColSize1, const T * in_matrix1, const int in_nRowSize2, const int in_nColSize2, const T * in_matrix2)
{
	return NULL;
}