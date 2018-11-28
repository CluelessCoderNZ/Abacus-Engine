/*
	============================================
	Project:	Abacus Engine
	File: 		math_helper.h
	Author: 	Connor Macdonald
	--------------------------------------------
	Provides all the math functions for the 
	engine aswell as abstract data types for 
	math functions.
	============================================
*/


#ifndef MATH_HELPER_H
#define MATH_HELPER_H
#include "atomics.h"
#include <iostream>
#include <string.h>
#include <cmath>

#define TAU 6.28318530716
#define PI 3.14159265358979
#define HALF_PI 1.57079632679

// Matrix Structs
struct Mat2
{
	//		[y][x]
	r32 data[2][2] = {};

	Mat2()
	{
		SetIdentity();
	}

	void SetIdentity()
	{
		data[0][0] = 1;
		data[1][1] = 1;
	}

	// TODO(Connor): Replace with own standard
	// NOTE(Connor): Not Type safe
	void operator=(std::initializer_list<r32> vals)
	{
		assert(vals.size() <= 4);
		memcpy(&data, vals.begin(), vals.size()*sizeof(r32));
	}

	Mat2 operator*(const Mat2& a)
	{
		Mat2 out;
		out.data[0][0] = data[0][0] * a.data[0][0] + data[0][1] * a.data[1][0];
		out.data[0][1] = data[0][0] * a.data[0][1] + data[0][1] * a.data[1][1];

		out.data[1][0] = data[1][0] * a.data[0][0] + data[1][1] * a.data[1][0];
		out.data[1][1] = data[1][0] * a.data[0][1] + data[1][1] * a.data[1][1];
		return out;
	}

	template <typename T>
	T operator*(const T& a)
	{
		T out;
		out.x = data[0][0] * a.x + data[0][1] * a.y;
		out.y = data[1][0] * a.x + data[1][1] * a.y;
		return out;
	}
};

struct Mat4
{
	//		[y][x]
	r32 data[4][4] = {};

	Mat4()
	{
		SetIdentity();
	}

	void SetIdentity()
	{
		data[0][0] = 1;
		data[1][1] = 1;
		data[2][2] = 1;
		data[3][3] = 1;
	}

	// TODO(Connor): Replace with own standard
	void operator=(std::initializer_list<r32> vals)
	{
		assert(vals.size() <= 16);
		memcpy(&data, vals.begin(), vals.size()*sizeof(r32));
	}

	Mat4 operator*(const Mat4& a)
	{
		Mat4 out;
		out.data[0][0] = data[0][0] * a.data[0][0] + data[0][1] * a.data[1][0] + data[0][2] * a.data[2][0] + data[0][3] * a.data[3][0];
		out.data[0][1] = data[0][0] * a.data[0][1] + data[0][1] * a.data[1][1] + data[0][2] * a.data[2][1] + data[0][3] * a.data[3][1];
		out.data[0][2] = data[0][0] * a.data[0][2] + data[0][1] * a.data[1][2] + data[0][2] * a.data[2][2] + data[0][3] * a.data[3][2];
		out.data[0][3] = data[0][0] * a.data[0][3] + data[0][1] * a.data[1][3] + data[0][2] * a.data[2][3] + data[0][3] * a.data[3][3];
	
		out.data[1][0] = data[1][0] * a.data[0][0] + data[1][1] * a.data[1][0] + data[1][2] * a.data[2][0] + data[1][3] * a.data[3][0];
		out.data[1][1] = data[1][0] * a.data[0][1] + data[1][1] * a.data[1][1] + data[1][2] * a.data[2][1] + data[1][3] * a.data[3][1];
		out.data[1][2] = data[1][0] * a.data[0][2] + data[1][1] * a.data[1][2] + data[1][2] * a.data[2][2] + data[1][3] * a.data[3][2];
		out.data[1][3] = data[1][0] * a.data[0][3] + data[1][1] * a.data[1][3] + data[1][2] * a.data[2][3] + data[1][3] * a.data[3][3];

		out.data[2][0] = data[2][0] * a.data[0][0] + data[2][1] * a.data[1][0] + data[2][2] * a.data[2][0] + data[2][3] * a.data[3][0];
		out.data[2][1] = data[2][0] * a.data[0][1] + data[2][1] * a.data[1][1] + data[2][2] * a.data[2][1] + data[2][3] * a.data[3][1];
		out.data[2][2] = data[2][0] * a.data[0][2] + data[2][1] * a.data[1][2] + data[2][2] * a.data[2][2] + data[2][3] * a.data[3][2];
		out.data[2][3] = data[2][0] * a.data[0][3] + data[2][1] * a.data[1][3] + data[2][2] * a.data[2][3] + data[2][3] * a.data[3][3];

		out.data[3][0] = data[3][0] * a.data[0][0] + data[3][1] * a.data[1][0] + data[3][2] * a.data[2][0] + data[3][3] * a.data[3][0];
		out.data[3][1] = data[3][0] * a.data[0][1] + data[3][1] * a.data[1][1] + data[3][2] * a.data[2][1] + data[3][3] * a.data[3][1];
		out.data[3][2] = data[3][0] * a.data[0][2] + data[3][1] * a.data[1][2] + data[3][2] * a.data[2][2] + data[3][3] * a.data[3][2];
		out.data[3][3] = data[3][0] * a.data[0][3] + data[3][1] * a.data[1][3] + data[3][2] * a.data[2][3] + data[3][3] * a.data[3][3];
		return out;
	}

	template <typename T>
	T operator*(const T& a)
	{
		Vec4f out;
		out.x = data[0][0] * a.x + data[0][1] * a.y + data[0][2] * a.z + data[0][3] * a.w;
		out.y = data[1][0] * a.x + data[1][1] * a.y + data[1][2] * a.z + data[1][3] * a.w;
		out.z = data[2][0] * a.x + data[2][1] * a.y + data[2][2] * a.z + data[2][3] * a.w;
		out.w = data[3][0] * a.x + data[3][1] * a.y + data[3][2] * a.z + data[3][3] * a.w;
		return out;
	}
};


// General Math Functions
template <typename T>
inline T abs (T const& a) { 
   return a < 0 ? (-a) : a;
}

template <typename T>
inline T const& max (T const& a, T const& b) { 
   return a < b ? b:a;
}

template <typename T>
inline T const& min (T const& a, T const& b) { 
   return a > b ? b:a;
}

template <typename T>
inline T const& clamp (T const& a, T const& lower, T const& upper) { 
   return max(min(a, upper), lower);
}

template <typename T>
inline void swap (T &a, T &b) { 
   T c = a;
   a = b;
   b = c;
}

// Interpolation Functions
template <typename T>
inline T const& lerp(T const& a, T const& b, r32 const& precent)
{
	return a + ((b - a)*precent);
}

// Geometric Math Functions

template <typename T>
inline s32 const DistanceFromEdge(T const &p, T const &e0, T const &e1)
{
	return (p.x - e0.x)*(e1.y - e0.y) - (p.y - e0.y)*(e1.x - e0.x);
}

template <typename T>
inline bool IsTriangleWindingCW(T const &t0, T const &t1, T const &t2)
{
	return (((t1.x - t0.x)*(t2.y - t0.y) - (t1.y - t0.y)*(t2.x - t0.x)) < 0);
}


template <typename T>
bool TrianglePointIntersection(T const &p, T const &t0, T const &t1, T const &t2)
{
	b8 is_inside = true;

	// NOTE(Connor): Winding order determines if negative inside or positive
	#ifdef TRIANGLE_WINDING_CCW
		is_inside &= DistanceFromEdge(p, t0, t1) >= 0;
		is_inside &= DistanceFromEdge(p, t1, t2) >= 0;
		is_inside &= DistanceFromEdge(p, t2, t0) >= 0;
	#else
		is_inside &= DistanceFromEdge(p, t0, t1) <= 0;
		is_inside &= DistanceFromEdge(p, t1, t2) <= 0;
		is_inside &= DistanceFromEdge(p, t2, t0) <= 0;
	#endif

	return is_inside;
}

template <typename T>
Vec3f GetBarycentricCoordinates(T const &p, T const &t0, T const &t1, T const &t2)
{
	Vec3f bary_coords;

	s32 triangle_area = DistanceFromEdge(t0, t1, t2);
	s32 t0_area = DistanceFromEdge(t1, t2, p);
	s32 t1_area = DistanceFromEdge(t2, t0, p);
	s32 t2_area = DistanceFromEdge(t0, t1, p);

	bary_coords.x = ((r32)t0_area)/triangle_area;
	bary_coords.y = ((r32)t1_area)/triangle_area;
	bary_coords.z = ((r32)t2_area)/triangle_area;

	return bary_coords;
};

template <typename T>
inline bool WithinRectBounds(T const &p, T const &size)
{
	return (p.x > 0 && p.y > 0 && p.x < size.x && p.y < size.y);
}

#endif