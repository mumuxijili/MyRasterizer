#pragma once

static int WinWidth = 1024;
static int WinHeight = 768;

static DWORD dwFrameBuffer[786432];

std::string numberToStr(int n) {//int to string
	char numbuf[128];
	sprintf_s(numbuf, "%i", n);//there is no itoa in xcode
	std::string numStr = numbuf;
	return numStr;
}