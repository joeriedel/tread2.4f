#define READ 1
#define WRITE 2

#define ACCEL_FILE "ACEL"
#define ACCEL_VER 1

class CAccelFile : public CFile
{
public:
	CAccelFile();
	~CAccelFile();

	int Open(CString sFilePath, UINT nMode);
	void Abort();
	void Close();

	void WriteTable(ACCEL* pTable, int nMax);
	int ReadTable(ACCEL* pTable, int nMax);
	ACCEL* ReadTable(int &nCount);

private:
	CFile m_hFile;
	//int m_nPos;
	UINT m_nVersion;
};