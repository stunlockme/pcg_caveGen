#pragma once
#ifndef _CAVECLASS_
#define _CAVECLASS_
#define WIDTH 60
#define HEIGHT 80


#include <string>
#include <iostream>
#include <time.h>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace System;

class CaveClass
{
public:
	CaveClass();
	~CaveClass();
	void CreateMap();
private:
	int m_map[WIDTH][HEIGHT];
	std::string * m_seed;
	std::hash<std::string*> m_hash;
	bool m_useRandomSeed;
	int m_randomFillPercent;
	std::ofstream * m_ofs;
private:
	void RandomFillMap(size_t hashCode);
	void SmoothMap();
	int GetNeighbourWallCount(int gridX, int gridY);
	char RandomChar();
};

#endif
