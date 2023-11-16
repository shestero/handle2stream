/**
 * CreateChildProcess - global function to create a child process to query.
 * @return - process handle.
 * It fills global variables:
 * HANDLE g_hChildStd_IN_Wr (stdin),
 * HANDLE g_hChildStd_OUT_Rd (stdout),
 * HANDLE g_hChildStd_ERR_Rd (stderr)
 **/

#ifndef CREATEPROC_HPP
#define CREATEPROC_HPP

#include <windows.h>
#include <fcntl.h>
#include <iostream>

/* child process's STDIN is the user input or data that you enter into the child process - READ */
HANDLE g_hChildStd_IN_Rd = NULL;
/* child process's STDIN is the user input or data that you enter into the child process - WRITE */
HANDLE g_hChildStd_IN_Wr = NULL;
/* child process's STDOUT is the program output or data that child process returns - READ */
HANDLE g_hChildStd_OUT_Rd = NULL;
/* child process's STDOUT is the program output or data that child process returns - WRITE */
HANDLE g_hChildStd_OUT_Wr = NULL;
/* child process's STDERR is the program output or data that child process returns - READ */
HANDLE g_hChildStd_ERR_Rd = NULL; // GetStdHandle(STD_ERROR_HANDLE);
/* child process's STDERR is the program output or data that child process returns - WRITE */
HANDLE g_hChildStd_ERR_Wr = NULL;

using std::cerr, std::endl;

HANDLE CreateChildProcess(LPSTR szCmdline)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
// return process HANDLE
{
    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&g_hChildStd_ERR_Rd, &g_hChildStd_ERR_Wr, &saAttr, 0))
        cerr << "StdErrRd CreatePipe" << endl;
    // Ensure the read handle to the pipe for STDERR is not inherited.
    if (!SetHandleInformation(g_hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0))
        cerr << "StdErr SetHandleInformation" << endl;

    //child process's STDOUT is the program output or data that child process returns
    // Create a pipe for the child process's STDOUT.
    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        cerr << "StdoutRd CreatePipe" << endl;

    // Ensure the read handle to the pipe for STDOUT is not inherited.
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        cerr << "Stdout SetHandleInformation" << endl;

    //child process's STDIN is the user input or data that you enter into the child process
    // Create a pipe for the child process's STDIN.
    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
        cerr << "Stdin CreatePipe" << endl;

    // Ensure the write handle to the pipe for STDIN is not inherited.
    if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
        cerr << "Stdin SetHandleInformation" << endl;

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure.
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure.
    // This structure specifies the STDIN and STDOUT handles for redirection.
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_ERR_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process.
    bSuccess = CreateProcess(NULL,
        szCmdline,     // command line
        NULL,          // process security attributes
        NULL,          // primary thread security attributes
        TRUE,          // handles are inherited
        0,             // creation flags
        NULL,          // use parent's environment
        NULL,          // use parent's current directory
        &siStartInfo,  // STARTUPINFO pointer
        &piProcInfo);  // receives PROCESS_INFORMATION

    // If an error occurs, exit the application.
    if (!bSuccess) {
        cerr << "Error: cannot create child process!" << endl;
        return NULL;
    } else {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example.
        //CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
    }
    return piProcInfo.hProcess;
}

#endif // CREATEPROC_HPP
