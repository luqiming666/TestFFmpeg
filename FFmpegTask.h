#pragma once

#include <thread>
#include <chrono>
#include <atomic>
#include <string>

class ITaskCallback {
public:
	virtual void OnTaskCompleted() = 0;
};

enum TASK_MODE
{
	FFMPEG,
	FFPLAY
};

class FFmpegTask 
{
public:
	FFmpegTask();
	~FFmpegTask();

	bool LocateTools(CString& toolFolder);
	std::string Probe(CString& srcFile);

	void Run(CString& cmdParams, ITaskCallback* callback = NULL);
	void Play(CString& cmdParams);
	bool IsRunning() { return m_bThreadRunning; }
	void Stop();

private:
	void DoRealTask();

private:
	std::thread m_thread;
	std::atomic<bool> m_bThreadRunning;

	CString m_FFmpegFile;
	CString m_FFplayFile;
	CString m_FFprobeFile;
	TASK_MODE m_TaskMode;

	CString m_CmdParams;
	ITaskCallback* m_Callback;
};