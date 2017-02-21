//
//  util.h
//  Bedrock
//
//  Created by Quinn on 9/14/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>
#include <stdlib.h>
#include "math.h"
#include <climits>

#include "ortho.h"

namespace util {
	
	//// Graphics ////
	void swapBuffers();
	
	//// Maths ////
#define EXPAND_VEC3(vec) (vec).x, (vec).y, (vec).z
	struct vec2f {
		float x,y;
		inline vec2f operator+(const vec2f& other) const {
			return {x+other.x, y+other.y};
		}
		inline vec2f operator-(const vec2f& other) const {
			return {x-other.x, y-other.y};
		}
		inline vec2f operator*(const vec2f& other) const {
			return {x*other.x, y*other.y};
		}
		inline vec2f operator/(const vec2f& other) const {
			return {x/other.x, y/other.y};
		}
		inline vec2f operator+(const float& f) const {
			return {x+f, y+f};
		}
		inline vec2f operator-(const float& f) const {
			return {x-f, y-f};
		}
		inline vec2f operator*(const float& d) const {
			return {x*d, y*d};
		}
		inline vec2f operator/(const float& d) const {
			return {x/d, y/d};
		}
		inline bool operator==(const vec2f& other) const {
			return x==other.x && y==other.y;
		}
		inline bool operator!=(const vec2f& other) const {
			return !operator==(other);
		}
		inline float length() const {
			return sqrtf((x*x) + (y*y));
		}
		inline vec2f normalize() const {
			return *this/length();
		}
		inline float dot(const vec2f& other) const {
			return (x*other.x) + (y*other.y);
		}
	};
	struct vec3i {
		int x,y,z;
		inline vec3i operator+(const vec3i& other) const {
			return {x+other.x, y+other.y, z+other.z};
		}
		inline vec3i operator-(const vec3i& other) const {
			return {x-other.x, y-other.y, z-other.z};
		}
		inline vec3i operator+(const int& i) const {
			return {x+i, y+i, z+i};
		}
		inline vec3i operator-(const int& i) const {
			return {x-i, y-i, z-i};
		}
		inline vec3i operator*(const int& d) const {
			return {x*d, y*d, z*d};
		}
		inline bool operator==(const vec3i& other) const {
			return x==other.x && y==other.y && z==other.z;
		}
		inline bool operator!=(const vec3i& other) const {
			return !operator==(other);
		}
		inline float length() const {
			return sqrtf((x*x) + (y*y) + (z*z));
		}
	};
	struct vec3f {
		float x,y,z;
		inline vec3f operator+(const vec3f& other) const {
			return {x+other.x, y+other.y, z+other.z};
		}
		inline vec3f operator-(const vec3f& other) const {
			return {x-other.x, y-other.y, z-other.z};
		}
		inline vec3f operator*(const vec3f& other) const {
			return {x*other.x, y*other.y, z*other.z};
		}
		inline vec3f operator/(const vec3f& other) const {
			return {x/other.x, y/other.y, z/other.z};
		}
		inline vec3f operator+(const float& f) const {
			return {x+f, y+f, z+f};
		}
		inline vec3f operator-(const float& f) const {
			return {x-f, y-f, z-f};
		}
		inline vec3f operator*(const float& d) const {
			return {x*d, y*d, z*d};
		}
		inline vec3f operator/(const float& d) const {
			return {x/d, y/d, z/d};
		}
		inline bool operator==(const vec3f& other) const {
			return x==other.x && y==other.y && z==other.z;
		}
		inline bool operator!=(const vec3f& other) const {
			return !operator==(other);
		}
		inline float length() const {
			return sqrtf((x*x) + (y*y) + (z*z));
		}
		inline vec3f normalize() const {
			return *this/length();
		}
		inline float dot(const vec3f& other) const {
			return (x*other.x) + (y*other.y) + (z*other.z);
		}
		// casting to/from vec3i
		vec3f() {}
		vec3f(float x, float y, float z):x(x),y(y),z(z) {}
		vec3f(const vec3i& v):x(v.x),y(v.y),z(v.z) {}
		operator vec3i() const {
			return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)};
		}
	};
	struct vec4f {
		float x,y,z,w;
		inline vec4f operator+(const vec4f& other) const {
			return {x+other.x, y+other.y, z+other.z, w+other.w};
		}
		inline vec4f operator-(const vec4f& other) const {
			return {x-other.x, y-other.y, z-other.z, w-other.w};
		}
		inline vec4f operator*(const float& d) const {
			return {x*d, y*d, z*d, w*d};
		}
		inline vec4f operator/(const float& d) const {
			return {x/d, y/d, z/d, w/d};
		}
		inline bool operator==(const vec4f& other) const {
			return x==other.x && y==other.y && z==other.z && w==other.w;
		}
		inline bool operator!=(const vec4f& other) const {
			return !operator==(other);
		}
		inline float length() const {
			return sqrtf((x*x) + (y*y) + (z*z) + (w*w));
		}
		inline vec4f normalize() const {
			return *this/length();
		}
		inline vec3f xyz() const {
			return {x, y, z};
		}
	};
	struct mat4f {
		float mat[16]; // "column major order"
		mat4f() {}
		mat4f(float m[16]) {
			for (int i = 0; i < 16; i++)
				mat[i] = m[i];
		}
		inline vec4f operator*(const vec4f& vec) {
			vec4f res;
			res.x = mat[0]*vec.x + mat[4]*vec.y + mat[8]*vec.z + mat[12]*vec.w;
			res.y = mat[1]*vec.x + mat[5]*vec.y + mat[9]*vec.z + mat[13]*vec.w;
			res.z = mat[2]*vec.x + mat[6]*vec.y + mat[10]*vec.z + mat[14]*vec.w;
			res.w = mat[3]*vec.x + mat[7]*vec.y + mat[11]*vec.z + mat[15]*vec.w;
			return res;
		}
		inline mat4f inverse() {
			float inv[16], det;
			int i;
			
			inv[0] = mat[5]  * mat[10] * mat[15] -
			mat[5]  * mat[11] * mat[14] -
			mat[9]  * mat[6]  * mat[15] +
			mat[9]  * mat[7]  * mat[14] +
			mat[13] * mat[6]  * mat[11] -
			mat[13] * mat[7]  * mat[10];
			
			inv[4] = -mat[4]  * mat[10] * mat[15] +
			mat[4]  * mat[11] * mat[14] +
			mat[8]  * mat[6]  * mat[15] -
			mat[8]  * mat[7]  * mat[14] -
			mat[12] * mat[6]  * mat[11] +
			mat[12] * mat[7]  * mat[10];
			
			inv[8] = mat[4]  * mat[9] * mat[15] -
			mat[4]  * mat[11] * mat[13] -
			mat[8]  * mat[5] * mat[15] +
			mat[8]  * mat[7] * mat[13] +
			mat[12] * mat[5] * mat[11] -
			mat[12] * mat[7] * mat[9];
			
			inv[12] = -mat[4]  * mat[9] * mat[14] +
			mat[4]  * mat[10] * mat[13] +
			mat[8]  * mat[5] * mat[14] -
			mat[8]  * mat[6] * mat[13] -
			mat[12] * mat[5] * mat[10] +
			mat[12] * mat[6] * mat[9];
			
			inv[1] = -mat[1]  * mat[10] * mat[15] +
			mat[1]  * mat[11] * mat[14] +
			mat[9]  * mat[2] * mat[15] -
			mat[9]  * mat[3] * mat[14] -
			mat[13] * mat[2] * mat[11] +
			mat[13] * mat[3] * mat[10];
			
			inv[5] = mat[0]  * mat[10] * mat[15] -
			mat[0]  * mat[11] * mat[14] -
			mat[8]  * mat[2] * mat[15] +
			mat[8]  * mat[3] * mat[14] +
			mat[12] * mat[2] * mat[11] -
			mat[12] * mat[3] * mat[10];
			
			inv[9] = -mat[0]  * mat[9] * mat[15] +
			mat[0]  * mat[11] * mat[13] +
			mat[8]  * mat[1] * mat[15] -
			mat[8]  * mat[3] * mat[13] -
			mat[12] * mat[1] * mat[11] +
			mat[12] * mat[3] * mat[9];
			
			inv[13] = mat[0]  * mat[9] * mat[14] -
			mat[0]  * mat[10] * mat[13] -
			mat[8]  * mat[1] * mat[14] +
			mat[8]  * mat[2] * mat[13] +
			mat[12] * mat[1] * mat[10] -
			mat[12] * mat[2] * mat[9];
			
			inv[2] = mat[1]  * mat[6] * mat[15] -
			mat[1]  * mat[7] * mat[14] -
			mat[5]  * mat[2] * mat[15] +
			mat[5]  * mat[3] * mat[14] +
			mat[13] * mat[2] * mat[7] -
			mat[13] * mat[3] * mat[6];
			
			inv[6] = -mat[0]  * mat[6] * mat[15] +
			mat[0]  * mat[7] * mat[14] +
			mat[4]  * mat[2] * mat[15] -
			mat[4]  * mat[3] * mat[14] -
			mat[12] * mat[2] * mat[7] +
			mat[12] * mat[3] * mat[6];
			
			inv[10] = mat[0]  * mat[5] * mat[15] -
			mat[0]  * mat[7] * mat[13] -
			mat[4]  * mat[1] * mat[15] +
			mat[4]  * mat[3] * mat[13] +
			mat[12] * mat[1] * mat[7] -
			mat[12] * mat[3] * mat[5];
			
			inv[14] = -mat[0]  * mat[5] * mat[14] +
			mat[0]  * mat[6] * mat[13] +
			mat[4]  * mat[1] * mat[14] -
			mat[4]  * mat[2] * mat[13] -
			mat[12] * mat[1] * mat[6] +
			mat[12] * mat[2] * mat[5];
			
			inv[3] = -mat[1] * mat[6] * mat[11] +
			mat[1] * mat[7] * mat[10] +
			mat[5] * mat[2] * mat[11] -
			mat[5] * mat[3] * mat[10] -
			mat[9] * mat[2] * mat[7] +
			mat[9] * mat[3] * mat[6];
			
			inv[7] = mat[0] * mat[6] * mat[11] -
			mat[0] * mat[7] * mat[10] -
			mat[4] * mat[2] * mat[11] +
			mat[4] * mat[3] * mat[10] +
			mat[8] * mat[2] * mat[7] -
			mat[8] * mat[3] * mat[6];
			
			inv[11] = -mat[0] * mat[5] * mat[11] +
			mat[0] * mat[7] * mat[9] +
			mat[4] * mat[1] * mat[11] -
			mat[4] * mat[3] * mat[9] -
			mat[8] * mat[1] * mat[7] +
			mat[8] * mat[3] * mat[5];
			
			inv[15] = mat[0] * mat[5] * mat[10] -
			mat[0] * mat[6] * mat[9] -
			mat[4] * mat[1] * mat[10] +
			mat[4] * mat[2] * mat[9] +
			mat[8] * mat[1] * mat[6] -
			mat[8] * mat[2] * mat[5];
			
			det = mat[0] * inv[0] + mat[1] * inv[4] + mat[2] * inv[8] + mat[3] * inv[12];
			
			if (det == 0) {
				printf("ERROR: matrix does not have an inverse\n");
				return mat4f();
			}
			
			det = 1.0 / det;
			mat4f res;
			for (i = 0; i < 16; i++)
				res.mat[i] = inv[i] * det;
			
			return res;
		}
	};
	struct AABB {
		float cx, cy, cz, rx, ry, rz; // center coords and halfwidths
	};
	vec3i rayTrace(vec3f origin, vec3f direction, float radius, bool callback(int,int,int,vec3f));
	vec3f rayToAABB(AABB box, vec3f rayOrigin, vec3f rayDir);
	bool intersects(const AABB & a, const AABB & b);
	float mod(float f1, float f2);
	template <typename T> int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}
	
	//// Files & Data ////
	void saveWorld();
	void loadWorld();
	bool fileExists(std::string path);
	void intToBytes(int i, char* dst);
	void intFromBytes(int& i, char* src);
	std::string getDataDir();
	bool createDirectory(std::string path);
	void rleEncode(char* data, unsigned long dataLen, char* res, unsigned long& resLen);
	void rleDecode(char* data, unsigned long dataLen, char* res, unsigned long& resLen, unsigned long limit = ULONG_MAX);
}

//// Operators ////
#define CONCAT_FUNCS(ntype) \
inline std::string operator+(std::string str, ntype n) {\
	return str + ortho::toString(n);\
}\
inline std::string operator+(ntype n, std::string str) {\
	return ortho::toString(n) + str;\
}
CONCAT_FUNCS(int)
CONCAT_FUNCS(long)
CONCAT_FUNCS(float)
CONCAT_FUNCS(double)
#undef CONCAT_FUNCS
