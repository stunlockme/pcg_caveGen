#include "caveclass.h"

CaveClass::CaveClass()
{
	m_seed = new std::string();
	m_ofs = new std::ofstream();
	*m_seed = "abc";
	m_useRandomSeed = true;
	m_randomFillPercent = 45;
}

void CaveClass::CreateMap()
{
	srand((unsigned int)time(0));		//use current time as seed for random generator

	std::string str;
	for (unsigned int i = 0; i < 5; ++i)
	{
		str += RandomChar();			//add random characters to the string
	}
	if (m_useRandomSeed)
		*m_seed = str;

	//std::cout << m_seed << std::endl;
	size_t hashCode = m_hash(m_seed);		//get hashcode to the string
	//std::cout << hashCode << std::endl;

	RandomFillMap(hashCode);

	for (int i = 0; i < 5; i++)
	{
		SmoothMap();
	}

	SpawnPlayer(hashCode);

	if (m_ofs != NULL)
	{
		m_ofs->open("map/cave.txt");
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				*m_ofs << m_map[x][y] << " ";
			}
			*m_ofs << std::endl;
		}
		m_ofs->close();
	}
}

char CaveClass::RandomChar()
{
	static const char alphanum[] =	"0123456789"
									"!@#$%^&*"
									"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
									"abcdefghijklmnopqrstuvwxyz";		//characters to choose randomly from
	
	int stringLength = sizeof(alphanum) - 1;		//length of characters as array
	return alphanum[rand() % stringLength];			//returns random character from the array
}

void CaveClass::RandomFillMap(size_t hashCode)
{
	Random^ rng = gcnew Random(hashCode);		//use gcnew for automatic garbage collection
											//pass string hash code as seed for randomness
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
			{
				m_map[x][y] = 1;
			}
			else
			{
				m_map[x][y] = (rng->Next(0, 100) < m_randomFillPercent) ? 1 : 0;
			}
		}
	}
}

void CaveClass::SpawnPlayer(size_t hashCode)
{
	Random^ rng = gcnew Random(hashCode);
	while (true)
	{
		int x = rng->Next(0, WIDTH);
		int y = rng->Next(0, HEIGHT);
		if (m_map[x][y] == 1)
		{
			continue;
		}
		else
		{
			m_map[x][y] = 2;
			break;
		}
	}
}

void CaveClass::SmoothMap()
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			int neighBourWallTiles = GetNeighbourWallCount(x, y);
			if (neighBourWallTiles > 4)
				m_map[x][y] = 1;
			else if (neighBourWallTiles < 4)
				m_map[x][y] = 0;
		}
	}
}

int CaveClass::GetNeighbourWallCount(int gridX, int gridY)
{
	int wallCount = 0;
	for (int neighbourX = gridX - 1; neighbourX <= gridX + 1; neighbourX++)
	{
		for (int neighbourY = gridY - 1; neighbourY <= gridY + 1; neighbourY++)
		{
			if (neighbourX >= 0 && neighbourX < WIDTH && neighbourY < HEIGHT)
			{
				if (neighbourX != gridX || neighbourY != gridY)
				{
					wallCount += m_map[neighbourX][neighbourY];
				}
			}
			else
			{
				wallCount++;
			}
		}
	}
	return wallCount;
}

CaveClass::~CaveClass()
{
	delete m_seed;
	delete m_ofs;
}