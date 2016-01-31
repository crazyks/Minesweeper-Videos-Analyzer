#pragma once
class CCRC32Calculator
{
private:
	CCRC32Calculator(void);
	~CCRC32Calculator(void);
	void MakeTable();
public:
	static unsigned int GetCRC32(CString& str);
};

