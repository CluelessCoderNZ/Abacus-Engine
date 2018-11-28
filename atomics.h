/*
	============================================
	Project:	Abacus Engine
	File: 		atomics.h
	Author: 	Connor Macdonald
	--------------------------------------------
	Provides the atomic data types for the
	engine.
	============================================
*/

#ifndef ATOMICS_H
#define ATOMICS_H
#include <assert.h>
#include <stdint.h>

// Standard Ints
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Floating point
typedef float r32;
typedef double r64;

// Misc Types
typedef bool b8;
typedef bool flag;
typedef unsigned char byte;

// Special Types
struct Vec2i
{
	s32 x;
	s32 y;
};

struct Vec3i
{
	s32 x;
	s32 y;
	s32 z;
};

struct Vec2f
{
	r32 x;
	r32 y;
};

struct Vec3f
{
	r32 x;
	r32 y;
	r32 z;
};

struct Vec4f
{
	r32 x;
	r32 y;
	r32 z;
	r32 w;
};

#endif