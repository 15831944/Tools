#pragma once
class CTestObj
{
public:
	static CTestObj& GetMe(){ static CTestObj one; return one; }

public:
	void StartTest();
};

