﻿#pragma once
#include "IncludeLib.h"
#include "Random.h"
#include "Search.h"

string makeNormalBoard(string board, int myBnum, int myRnum, int eBnum, int eRnum) {
	random_device rnd;
	int r = 0;
	for (int i = 0; i < myBnum; i++) {
		do r = IntRandom::get_rand_range(1, 35);
		while (board[r] != '.' || r == 0 || r == 5);
		board[r] = 'B';
}
	for (int i = 0; i < myRnum; i++) {
		do r = IntRandom::get_rand_range(0, 35);
		while (board[r] != '.');
		board[r] = 'R';
	}

	for (int i = 0; i < eBnum + eRnum; i++) {
		do r = IntRandom::get_rand_range(0, 35);
		while (board[r] != '.');
		board[r] = 'u';
	}
	return board;
}

string makePartBoard(string board, int myBnum, int myRnum, int eBnum, int eRnum, int openB, int openR) {
	random_device rnd;
	int r = 0;
	for (int i = 0; i < myBnum; i++) {
		do r = IntRandom::get_rand_range(1, 35);
		while (board[r] != '.' || r == 0 || r == 5);
		board[r] = 'B';
	}
	for (int i = 0; i < myRnum; i++) {
		do r = IntRandom::get_rand_range(0, 35);
		while (board[r] != '.');
		board[r] = 'R';
	}

	for (int i = 0; i < openR; i++) {
		do r = IntRandom::get_rand_range(0, 35);
		while (board[r] != '.');
		board[r] = 'r';
	}
	for (int i = 0; i < openB; i++) {
		do r = IntRandom::get_rand_range(0, 35);
		while (board[r] != '.');
		board[r] = 'b';
	}
	for (int i = 0; i < eBnum + eRnum - openR - openB; i++) {
		do r = IntRandom::get_rand_range(0, 35);
		while (board[r] != '.');
		board[r] = 'u';
	}
	return board;
}

void HelpMessageRandGen(const string& prog_name)
{
	cout << "Usage: " << prog_name << " RandomGen" <<
		" [mode] [#B] [#R] [#b] [#r] [min #actions] [max #depth] [#trails] [(random seed)]" << endl;
#if OUTPUT_JP_MESSAGE
	cout << "- mode: normal or partial" << endl;
	cout << "  * normal: 非公開問題" << endl;
	cout << "  * partial: 一部公開問題" << endl;
	cout << "- #B: 整数，自青駒の数" << endl;
	cout << "- #R: 整数，自赤駒の数" << endl;
	cout << "- #b: 整数，敵青駒の数" << endl;
	cout << "- #r: 整数，敵赤駒の数" << endl;
	cout << "- min #actions: 整数，最低手数" << endl;
	cout << "- max #depth: 19 までの整数，最大探索深さ" << endl;
	cout << "- #trails: 整数，試行回数（失敗問も含む）" << endl;
	cout << "- random seed: 整数，乱数種 (選択的)" << endl;
	cout << "  * 指定がなければ time(NULL) になり，記録する．" << endl;
	cout << "例：" << prog_name << " RandomGen normal 1 2 2 1 5 9 500 0" << endl;
	cout << "5~9手自青駒1自赤駒2敵青駒2敵赤駒1の非公開問題を500試行から作ってみる（乱数種は0）．" << endl;
#else
	cout << "- mode: normal or partial" << endl;
	cout << "  * normal: not-revealed" << endl;
	cout << "  * partial: partly-revealed" << endl;
	cout << "- #B: integer, the number of the player's blue pieces" << endl;
	cout << "- #R: integer, the number of the player's red pieces" << endl;
	cout << "- #b: integer, the number of the opponent's blue pieces" << endl;
	cout << "- #r: integer, the number of the opponent's red pieces" << endl;
	cout << "- min #actions: integer, the minimum move number required" << endl;
	cout << "- max #depth: integer up to 19, the maximum move number to search" << endl;
	cout << "- #trails: integer, the number of trials (including failed ones)" << endl;
	cout << "- random seed: integer, random seed (optional)" << endl;
	cout << "  * If not specified, time(NULL) is used, which will be recorded." << endl;
	cout << "Example: " << prog_name << " RandomGen normal 1 2 2 1 5 9 500 0" << endl;
	cout << "Try to make 5-9 moves not-revealed puzzles with 1 own blue piece, 2 own red pieces,"
		<< " 2 opponent blue pieces, and 1 opponent red piece from 500 trials (random seed of 0)." << endl;
#endif
}

void OutputCount(const string& puzzleType, const string& countType,
	int myBnum, int myRnum, int eBnum, int eRnum,
	int minActionNum, int maxActionNum,
	int count[20], int trialNum, double timeS,
	unsigned long long sumCalCount, double sumTime)
{
	string resultFileName = puzzleType + "-" + countType + ".csv";
	if (!ifstream(resultFileName.c_str())) {
		ofstream ofsResult(resultFileName.c_str(), ofstream::out);
		ofsResult << "BRbr,#B,#R,#b,#r,#minAction,#maxAction";
		for (int j = 3; j <= 19; j += 2) {
			ofsResult << "," << j << "-move";
		}
		ofsResult << ",#trials,totalTimeS,#nodes,genTimeS" << endl;
		ofsResult.close();
	}
	ofstream ofsResult(resultFileName.c_str(), ofstream::out | ofstream::app);
	ofsResult << myBnum << myRnum << eBnum << eRnum
		<< "," << myBnum << "," << myRnum << "," << eBnum << "," << eRnum;
	ofsResult << "," << minActionNum << "," << maxActionNum;
	for (int j = 3; j <= 19; j += 2) {
		ofsResult << "," << count[j];
	}
	ofsResult << "," << trialNum;
	ofsResult << "," << timeS;
	ofsResult << "," << sumCalCount;
	ofsResult << "," << sumTime << endl;
	ofsResult.close();
}

void RandomGeneration(const vector<string>& argvVec) {
	if (argvVec.size() != 9 && argvVec.size() != 10) { HelpMessageRandGen(argvVec[0]); }

	string mode;
	string board;
	if (argvVec[1] == string("normal")) { mode = "n"; }
	else if (argvVec[1] == string("partial")) { mode = "p"; }
	else {
		HelpMessageRandGen(argvVec[0]);
		return;
	}

	int myBnum, myRnum;
	int eBnum, eRnum;
	int openBnum, openRnum;
	int maxDepth;
	int trialNum;
	int actionNum, minAction;
	int makeCountAll = 0;
	int makeCount[20] = {};
	memset(makeCount, 0, sizeof(makeCount));
	int redWallCount[20] = {};
	memset(redWallCount, 0, sizeof(redWallCount));
	int captureBlueCount[20] = {};
	memset(captureBlueCount, 0, sizeof(captureBlueCount));
	int bothCount[20] = {};
	memset(bothCount, 0, sizeof(bothCount));
	double makeTimeCount[20] = {};
	memset(makeTimeCount, 0, sizeof(makeTimeCount));

	unsigned int calCount;
	CN<int> maxpn, maxdn;

	myBnum = stoi(argvVec[2]);
	myRnum = stoi(argvVec[3]);
	eBnum = stoi(argvVec[4]);
	eRnum = stoi(argvVec[5]);
	minAction = stoi(argvVec[6]);
	maxDepth = stoi(argvVec[7]);
	trialNum = stoi(argvVec[8]);
	int randomSeed;
	if (argvVec.size() == 10) {
		randomSeed = stoi(argvVec[9]);
	}
	else {
		struct tm stm;
		time_t tim = time(NULL);
#if defined(_MSC_VER)
		localtime_s(&stm, &tim);
#elif defined(__GNUC__)
		localtime_r(&tim, &stm);
#endif
		char t[100];
		strftime(t, 100, "%Y-%m-%d_%H:%M:%S", &stm);
		randomSeed = int(tim);
		ofstream ofsRandSeed("RandSeed.csv", ofstream::out | ofstream::app);
		ofsRandSeed << myBnum << myRnum << eBnum << eRnum << ","
			<< argvVec[1] << "," << t << "," << randomSeed << endl;
		ofsRandSeed.close();
	}
	IntRandom::reset(randomSeed);

	if ((mode == "p") && (eBnum == 1) && (eRnum == 1)) { return; }

	string dir_name = argvVec[1];
	makeDir(dir_name.c_str());
	string allPuzzleFileName = dir_name + string("/") + mode + string("-")
		+ to_string(myBnum) + to_string(myRnum) + to_string(eBnum) + to_string(eRnum)
		+ string("-") + to_string(minAction) + string("_") + to_string(maxDepth) + string("ply-")
		+ to_string(trialNum) + string("-sd=") + to_string(randomSeed) + string(".txt");
	ofstream ofsAllPuzzle;
	ofsAllPuzzle.open(allPuzzleFileName.c_str());
	ofstream ofsPly;
	ofstream ofsRedWall;
	ofstream ofsCaptureBlue;
	ofstream ofsBoth;

	double sumTime = 0.0;
	unsigned long long sumCalCount = 0;

	clock_t makeStart;
	clock_t makeEnd;

	clock_t start_clock = clock();
	for (int i = 0; i < trialNum; i++) {
		bool isRedWall = false;
		bool isCaptureBlue = false;

		makeStart = clock();

		makeCountAll += 1;

		Search search(mode, "n");

		board = "";
		for (int j = 0; j < 6; j++) board += "......";

		if (mode == "n") {
			board = makeNormalBoard(board, myBnum, myRnum, eBnum, eRnum);
			actionNum = search.think(board, eBnum, maxDepth);
			calCount = search.returnCount();
			sumCalCount += calCount;

			maxpn = search.returnMaxPn();
			maxdn = search.returnMaxDn();
		}
		else {
			openBnum = 0;
			openRnum = 0;
			while ((openBnum + openRnum) == 0) {
				openBnum = IntRandom::get_rand_range(0, eBnum - 1);
				openRnum = IntRandom::get_rand_range(0, eRnum - 1);
			}

			board = makePartBoard(board, myBnum, myRnum, eBnum, eRnum, openBnum, openRnum);

			actionNum = search.think(board, eBnum, maxDepth);
			calCount = search.returnCount();
			sumCalCount += calCount;

			maxpn = search.returnMaxPn();
			maxdn = search.returnMaxDn();
		}

		makeEnd = clock();
		sumTime += static_cast<double>((makeEnd - makeStart) / double(CLOCKS_PER_SEC));

		if (minAction > actionNum) { continue; }

		if (mode == "n") {
			Search subSearch("n", "r");
			int actionNumRed = subSearch.think(board, eBnum, actionNum);
			if (actionNumRed == 0) {
				isRedWall = true;
			}
		}
		else if (mode == "p") {
			Search subSearchRed("p", "r");
			int actionNumRed = subSearchRed.think(board, eBnum, actionNum);
			if (actionNumRed == 0) {
				isRedWall = true;
			}
			Search subSearchBlue("p", "a");
			int actionNumBlueCapture = subSearchBlue.think(board, eBnum, actionNum, actionNum - 1);
			if (actionNumBlueCapture == 0) {
				isCaptureBlue = true;
			}
		}

		makeCount[actionNum] += 1;
		if (isRedWall) { redWallCount[actionNum] += 1; }
		if (isCaptureBlue) { captureBlueCount[actionNum] += 1; }
		if (isRedWall && isCaptureBlue) { bothCount[actionNum] += 1; }
		makeTimeCount[actionNum] += static_cast<double>((makeEnd - makeStart) / CLOCKS_PER_SEC * 1000.0);

		string sub_dir_name = dir_name + string("/") 
			+ to_string(myBnum) + to_string(myRnum)
			+ to_string(eBnum) + to_string(eRnum);
		makeDir(sub_dir_name.c_str()); //
		string puzzle_file_name = sub_dir_name + string("/")
			+ to_string(actionNum) + string(".txt");
		ofsPly.open(puzzle_file_name.c_str(), ofstream::out | ofstream::app);
		if (isRedWall) {
			puzzle_file_name = sub_dir_name + string("/")
				+ to_string(actionNum) + string("-RedWall.txt");
			ofsRedWall.open(puzzle_file_name.c_str(), ofstream::out | ofstream::app);
		}
		if (isCaptureBlue) {
			puzzle_file_name = sub_dir_name + string("/")
				+ to_string(actionNum) + string("-CaptureBlue.txt");
			ofsCaptureBlue.open(puzzle_file_name.c_str(), ofstream::out | ofstream::app);
		}
		if (isRedWall && isCaptureBlue) {
			puzzle_file_name = sub_dir_name + string("/")
				+ to_string(actionNum) + string("-WallCapture.txt");
			ofsBoth.open(puzzle_file_name.c_str(), ofstream::out | ofstream::app);
		}

		stringstream ss;
		//cout << actionNum << "手詰  " << i << "問目" << endl << endl;
#if OUTPUT_JP_MESSAGE
		ss << actionNum << "手詰" << "\n";
#else
		ss << actionNum << " moves" << "\n";
#endif
		ss << "myBnum : " << myBnum << ", myRnum ; " << myRnum << ", enBnum : " << eBnum << ", enRnum : " << eRnum << ", maxDepth : " << maxDepth << "\n";
		ss << "calCount = " << calCount << "\n";
		ss << "maxpn = " << maxpn << ", maxdn = " << maxdn << "\n";

		for (int j = 0; j < 6; j++) {
			for (int k = 0; k < 6; k++) {
				ss << board[j * 6 + k];
			}
			ss << "\n";
		}
		ss << "\n";
		for (int j = 0; j < 36; j++) {
			ss << board[j];
		}
		ss << "\n" << "\n";

		for (int j = minAction; j < 20; j++) {
			if (makeCount[j] != 0) {
				ss << "actionCount " << j << " : " << makeCount[j] << "\n";
			}
		}
		ss << "\n";

		ofsAllPuzzle << ss.str();
		ofsPly << ss.str();
		ofsPly.close();
		if (isRedWall) {
			ofsRedWall << ss.str();
			ofsRedWall.close();
		}
		if (isCaptureBlue) {
			ofsCaptureBlue << ss.str();
			ofsCaptureBlue.close();
		}
		if (isRedWall && isCaptureBlue) {
			ofsBoth << ss.str();
			ofsBoth.close();
		}
	}
	clock_t end_clock = clock();
	ofsAllPuzzle.close();

	double timeS = double(end_clock - start_clock) / double(CLOCKS_PER_SEC);
	OutputCount(argvVec[1], "All", myBnum, myRnum, eBnum, eRnum, minAction, maxDepth, makeCount, trialNum, timeS, sumCalCount, sumTime);
	OutputCount(argvVec[1], "RedWall", myBnum, myRnum, eBnum, eRnum, minAction, maxDepth, redWallCount, trialNum, timeS, sumCalCount, sumTime);
	if (string(argvVec[1]) == string("partial")) {
		OutputCount(argvVec[1], "CaptureBlue", myBnum, myRnum, eBnum, eRnum, minAction, maxDepth, captureBlueCount, trialNum, timeS, sumCalCount, sumTime);
		OutputCount(argvVec[1], "Both", myBnum, myRnum, eBnum, eRnum, minAction, maxDepth, bothCount, trialNum, timeS, sumCalCount, sumTime);
	}
}
