/*
	HAPI Start
	----------
	This solution contains an already set up HAPI project and this main file

	The directory structure and main files are:

	HAPI_Start - contains the Visual Studio solution file (.sln)
		HAPI_Start - contains the Visual Studio HAPI_APP project file (.vcxproj) and source code
			HAPI - the directory with all the HAPI library files
			Data - a place to put your data files with a few sample ones provided

	Additionally in the top directory there is a batch file for creating a redistributable Demo folder

	For help using HAPI please see the Reference folder in the HAPI sub-directory
*/

// Include the HAPI header to get access to all of HAPIs interfaces
#include <HAPI_lib.h>

// HAPI itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

const int kNumStars{ 1000 };
int eyeDistance{ 100 };

void ClearScreenToColour(HAPI_TColour col, int width, int height, BYTE* screen);

struct Star
{
	int starX{ 0 };
	int starY{ 0 };
	int starZ{ 0 };
};

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main()
{
	int screenWidth{ 1920 };
	int screenHeight{ 1080 };

	if (!HAPI.Initialise(screenWidth, screenHeight, "My Window"))
	{
		return;
	}

	HAPI.SetShowFPS(true);

	BYTE* screen = HAPI.GetScreenPointer();

	HAPI_TColour colour(HAPI_TColour::BLACK);

	Star stars[kNumStars];

	//sets position of each star randomly across screen space
	for (int i = 0; i < kNumStars; i++)
	{
		stars[i].starX = rand() % screenWidth;
		stars[i].starY = rand() % screenHeight;
		stars[i].starZ = rand() % 1000;
	}

	const HAPI_TKeyboardData& keyData = HAPI.GetKeyboardData();

	while (HAPI.Update())
	{
		ClearScreenToColour(colour, screenWidth, screenHeight, screen);

		int centreX = screenWidth / 2;
		int centreY = screenHeight / 2;

		for (int i = 0; i < kNumStars; i++)
		{
			//sets default star colour to yellow
			HAPI_TColour star(HAPI_TColour::WHITE);

			//randomises the colour of each star


			/*star.red = rand() % 256;
			star.green = rand() % 256;
			star.blue = rand() % 256; */

			int Sx = ((eyeDistance * (stars[i].starX - centreX)) / (stars[i].starZ + eyeDistance)) + centreX;
			int Sy = ((eyeDistance * (stars[i].starY - centreY)) / (stars[i].starZ + eyeDistance)) + centreY;

			//press W for fast stars
			if (keyData.scanCode['W'])
			{
				stars[i].starZ += -1;
			}

			//press S for slow stars
			if (keyData.scanCode['S'])
			{
				stars[i].starZ += 1;
			}

			if (Sx < screenWidth && Sy < screenHeight)
			{
				int offset = (Sx + Sy * screenWidth) * 4;

				memcpy(screen + offset, &star, 4);
			}

			int squareSize{ 12 - (stars[i].starZ / 18) };

			for (int y = Sy - squareSize / 2; y < Sy + squareSize / 2; y++)
			{
				for (int x = Sx - squareSize / 2; x < Sx + squareSize / 2; x++)
				{
					if (x < screenWidth && y < screenHeight)
					{
						int offset = (x + y * screenWidth) * 4;

						memcpy(screen + offset, &star, 4);
					}
				}
			}
		}

		//moves stars in z axis
		for (int i = 0; i < kNumStars; i++)
		{
			stars[i].starZ -= 2;

			//resets z position
			if (stars[i].starZ <= 0)
			{
				stars[i].starZ = 1000;
				stars[i].starX = rand() % screenWidth;
				stars[i].starY = rand() % screenHeight;
			}
		}

		//renders text
		HAPI.RenderText(1670, 0, HAPI_TColour::WHITE, "W and S to change speed", 20);
	}
}


//Function clears screen to a set colour
void ClearScreenToColour(HAPI_TColour col, int width, int height, BYTE* screen)
{
	for (int i = 0; i < width * height; i++)
	{
		memcpy(screen + (int)i * 4, &col, 4);
	}
}