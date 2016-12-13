#include <iostream>
#include "Image.h"
#include "LargeImage.h"
#include "MatchImage.h"
#include <cstring>
#include <sstream>
#include <ctime>
#include <thread>

using namespace std;

int getUserInt(int min, int max, string message, int defaultValue)
{
	string line;
	int val;
	cout << message << " (" << defaultValue << "):";
	while (getline(cin, line))
	{
		if (line == "")
		{
			return defaultValue;
		}
		stringstream ss(line);
		if (ss >> val)
		{
			if (val <= max && val >= min)
				return val;
		}
		cout << message;
	}
	return defaultValue;//this should never happen :/
}

string getUserString(string message, string defaultValue)
{
	string line;
	string val;
	cout << message << " (" << defaultValue << "):";
	while (getline(cin, line))
	{
		if (line == "")
		{
			return defaultValue;
		}
		
		return line;
		cout << message;
	}
	return defaultValue;//this should never happen :/
}

int main(int argc, char* argv[])
{
	cout << "Leave input blank for default values" << endl;
	//User input:
	//get scene file

	string scenefilename = getUserString("enter scene file name","Cluttered_scene.txt");
	int scenewidth = getUserInt(0,INT_MAX,"enter width",1024);
	int sceneheight = getUserInt(0, INT_MAX, "enter height", 768);
	LargeImage * scene = new LargeImage(scenewidth, sceneheight, scenefilename);
	
	//get template file
	string templatefilename = getUserString("enter template file name", "Wally_grey.txt");
	int templatewidth = getUserInt(0, INT_MAX, "enter width", 36);
	int templateheight = getUserInt(0, INT_MAX, "enter height", 49);
	Image * templateImage = new Image(templatewidth, templateheight, templatefilename);

	//choose NNS algorithm
	bool normalisedCorrelation = getUserInt(1, 2, "Choose NNS Algorithm:\n [1] Normalised Correlation\n [2] Sum of Squared Differences\n", 1) == 1 ? true : false;

	//choose how many top matches to keep
	int topx = getUserInt(1,INT_MAX,"Get top x results",5);

	//choose number oif threads to use
	int numthreads = thread::hardware_concurrency();
	stringstream threadsmessage;
	threadsmessage << "Use x threads [1 - " << numthreads << "]";
	numthreads = getUserInt(1, numthreads, threadsmessage.str(), numthreads);

	//clock_t startTime = clock();

	if (normalisedCorrelation)
	{
		scene->NNS_NormalisedCorrelation(templateImage,topx,numthreads);
		scene->printMatchesNormalisedCorrelation();
	}
	else
	{
		scene->NNS_SquaredDifference(templateImage,topx, numthreads);
		scene->printMatchesSquaredDifference();
	}

	//check if drawing matches on scene
	bool drawResults = getUserInt(1, 2, "Draw results on scene?\n [1] Yes\n [2] No\n", 1) == 1 ? true : false;
	if (drawResults)
	{
		scene->drawMatches();
		scene->generatePGM("ResultsScene.pgm");
		cout << "Scene with matches can be found at ResultsScene.pgm";
	}
		

	//cout << "Time Taken: " << float(clock() - startTime) / (double)CLOCKS_PER_SEC << endl;
	delete templateImage;
	delete scene;
	cin.get();
	return 0;
}
