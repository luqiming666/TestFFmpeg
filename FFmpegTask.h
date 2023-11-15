#pragma once

#include <thread>
#include <chrono>
#include <atomic>

class ITaskCallback {
public:
	virtual void OnTaskCompleted() = 0;
};

class FFmpegTask 
{
public:
	FFmpegTask();
	~FFmpegTask();

	void SetFFmpegPath(CString& exeFile) { m_FFmpegFile = exeFile; }

	void Run(CString& cmdParams, ITaskCallback* callback = NULL);
	bool IsRunning() { return m_bThreadRunning; }
	void Stop();

private:
	void DoRealTask();

private:
	std::thread m_thread;
	std::atomic<bool> m_bThreadRunning;

	CString m_FFmpegFile;
	CString m_CmdParams;
	ITaskCallback* m_Callback;
};