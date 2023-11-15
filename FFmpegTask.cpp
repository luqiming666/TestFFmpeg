#include "pch.h"
#include "framework.h"
#include "FFmpegTask.h"
#include "UMiscUtils.h"

FFmpegTask::FFmpegTask()
	: m_bThreadRunning(false)
    , m_Callback(NULL)
{
}

FFmpegTask::~FFmpegTask()
{
}

void FFmpegTask::Run(CString& cmdParams, ITaskCallback* callback)
{
    m_CmdParams = cmdParams;
    m_Callback = callback;
    m_TaskMode = FFMPEG;

    m_bThreadRunning = true;
    m_thread = std::thread(&FFmpegTask::DoRealTask, this);
}

void FFmpegTask::Stop()
{
    m_bThreadRunning = false;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void FFmpegTask::DoRealTask()
{
    while (m_bThreadRunning)
    {
        CString strToolPath = m_FFmpegFile;
        bool bToolWndVisible = false;
        if (m_TaskMode == FFPLAY) {
            strToolPath = m_FFplayFile;
            bToolWndVisible = true;
        }

        UMiscUtils::RunExternalApp(strToolPath.GetBuffer(), m_CmdParams.GetBuffer(), bToolWndVisible, true);
        strToolPath.ReleaseBuffer();
        m_CmdParams.ReleaseBuffer();

        //std::this_thread::sleep_for(std::chrono::seconds(1));
        m_bThreadRunning = false;
    }

    if (m_Callback) {
        m_Callback->OnTaskCompleted();
    }
    m_thread.detach();
}

void FFmpegTask::Play(CString& cmdParams)
{
    m_CmdParams = cmdParams;
    m_TaskMode = FFPLAY;

    m_bThreadRunning = true;
    m_thread = std::thread(&FFmpegTask::DoRealTask, this);
}
