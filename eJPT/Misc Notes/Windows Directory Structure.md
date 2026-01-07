Absolutely! Below is a similar `README.md` tailored to Windows directory and folder structure, with a focus on penetration testing, which could be useful for both general assessments and for certifications like eJPT. It includes details on important directories, file permissions, relevant files, and notes for security testing.

---

# Windows Directory and Folder Structure for Penetration Testing

## Table of Contents

1. [Overview](#overview)
2. [Directory Structure](#directory-structure)

   * [Root Directory (`C:\`)](#root-directory-c)
   * [System Directories](#system-directories)
   * [User Directories](#user-directories)
3. [File Permissions and Security Considerations](#file-permissions-and-security-considerations)
4. [Important Files for Penetration Testing](#important-files-for-penetration-testing)
5. [Relevant Notes for Penetration Testing or eJPT Exam](#relevant-notes-for-penetration-testing-or-ejpt-exam)

---

## Overview

Windows operating systems use a hierarchical directory structure, with system files, user data, and application files organized in specific locations. Penetration testing often involves exploring these directories for misconfigurations, identifying sensitive files, and looking for evidence of exploitation or persistence mechanisms.

This document outlines the Windows directory and folder structure, highlighting key locations for security auditing and penetration testing. It also provides tips on file permissions, key files to investigate, and relevant considerations when performing a security assessment.

---

## Directory Structure

### Root Directory (`C:\`)

The `C:\` drive is typically the root directory for most Windows installations. It houses key system directories, user files, and installed applications.

* **Important subdirectories**:

  * `C:\Windows`: System files, core operating system components.

    * Includes `C:\Windows\System32` (system binaries) and `C:\Windows\Temp` (temporary files).
  * `C:\Program Files`: Default location for 64-bit application installations.
  * `C:\Program Files (x86)`: Default location for 32-bit application installations.
  * `C:\Users`: Contains user profile directories, each user’s data, and settings.
  * `C:\Documents and Settings`: Older directory structure from pre-Windows Vista versions.
  * `C:\$Recycle.Bin`: Recycle Bin directory.
  * `C:\inetpub`: Default root directory for IIS web server.
  * `C:\$WINDOWS.~BT`: Temporary folder created during system upgrades or installations.

### System Directories

* `C:\Windows\`: This directory contains all the critical operating system files for Windows, including the kernel and core libraries.

  * Example: `C:\Windows\System32\` (binaries for system operations, e.g., `cmd.exe`, `powershell.exe`).

* `C:\Users\`: Each user has a subdirectory under `C:\Users\` (e.g., `C:\Users\Administrator\`).

  * Files here can contain critical user data, including desktop files, document folders, and sometimes sensitive application data.

* `C:\Program Files\` and `C:\Program Files (x86)\`: The default directories for installed software.

  * Important to review for vulnerable or misconfigured applications.

* `C:\Windows\Temp\` and `C:\Users\<user>\AppData\Local\Temp\`: Temporary file directories that can be used for exploit persistence or storing malicious payloads.

### User Directories

* `C:\Users\`:

  * `C:\Users\<UserName>\`: The user profile directory contains personal data and configuration files for each user.
  * Important subdirectories include:

    * `Desktop`: User’s desktop files.
    * `Documents`: User’s document files.
    * `AppData`: Contains application data for the user.

      * Subdirectories include:

        * `Local`: Data specific to the local machine.
        * `Roaming`: Data that can follow the user across different machines (e.g., settings, profiles).

---

## File Permissions and Security Considerations

Windows uses access control lists (ACLs) to manage file permissions. These control the read, write, and execute rights of users and groups for each file or folder.

* **File permissions**:

  * **Owner**: The user who owns the file.
  * **Group**: The group of users who can access the file.
  * **Others**: All other users.

  Example permissions:

  * `rwx`: Read, write, and execute permissions for the owner.
  * `r-x`: Read and execute permissions for others.

* **Key security considerations**:

  * Check for **world-writable** files and folders (i.e., files that are writable by all users), as these could allow unauthorized users to modify important system files or place malicious executables.
  * Pay attention to files with **SUID/SGID** equivalents on Windows, like files set to run with elevated privileges or files placed in sensitive directories (`System32`, `Program Files`).

---

## Important Files for Penetration Testing

### `C:\Windows\System32`

* This directory contains many essential binaries required for system operations. Some key files to monitor include:

  * `cmd.exe`: Command Prompt.
  * `powershell.exe`: PowerShell, which is a powerful scripting environment for administrators.
  * `taskmgr.exe`: Task Manager, used to monitor processes and system performance.
  * **Persistence point**: Malicious files placed here may be executed with higher privileges, especially if they are granted execution permissions.

### `C:\Users\<UserName>\AppData\Roaming`

* Application data stored here could include sensitive information such as application settings, cached data, and saved credentials. It can be a target for credential harvesting or persistence.

### `C:\Windows\Temp` and `C:\Users\<UserName>\AppData\Local\Temp`

* These directories are commonly used for temporary file storage. Malicious actors often use them to store payloads or scripts for persistence. They should be reviewed for unexpected files during penetration tests.

### `C:\Program Files` and `C:\Program Files (x86)`

* Installed applications might contain misconfigurations, outdated software, or vulnerabilities that can be exploited.

  * Look for applications that are installed with excessive privileges or those that allow for command execution via insecure configurations.
  * Use tools like `AccessChk` to inspect the ACLs on application directories.

### `C:\Windows\System32\drivers`

* This folder contains device drivers for the operating system. Misconfigurations, vulnerable drivers, or malicious drivers placed here can allow privilege escalation or remote code execution.

---

## Relevant Notes for Penetration Testing or eJPT Exam

1. **Enumeration**:

   * Utilize tools like `PowerView`, `PowerUp`, or `BloodHound` for Windows enumeration. These tools can help identify service accounts, misconfigured permissions, and potential attack paths.
   * Look for unquoted service paths or executable files that run with elevated privileges (e.g., system services).

2. **Privilege Escalation**:

   * **Sticky Keys**: The `sethc.exe` file in `C:\Windows\System32\` can sometimes be replaced with a command prompt to gain administrative access.
   * **DLL Hijacking**: Check if the system is vulnerable to DLL hijacking, where a malicious DLL can be loaded instead of a legitimate system DLL.
   * **Scheduled Tasks**: Examine scheduled tasks under `C:\Windows\System32\Tasks` for misconfigurations or tasks running with elevated privileges.

3. **Network and Sharing Configurations**:

   * Check for weak shared network drives or improperly configured SMB (Server Message Block) shares that can be exploited.
   * Use tools like `smbclient` or `rpcclient` to enumerate shares and identify potential attack vectors.

4. **Log Files**:

   * Review Windows Event Logs (e.g., `Event Viewer` -> `Windows Logs`) for signs of unusual activity, failed login attempts, privilege escalation, or system errors that may indicate ongoing exploitation.
   * Important logs include `Security` (authentication attempts), `System` (system-related events), and `Application` logs.

5. **Persistence Mechanisms**:

   * Investigate user startup folders, scheduled tasks, registry entries, and other locations where malicious actors may establish persistence mechanisms.
   * Use `Autoruns` to check for any suspicious auto-starting programs or services.

---

## Conclusion

Understanding the Windows directory structure is crucial for effective penetration testing. During an assessment, key areas to focus on include user directories, system directories (`System32`, `Program Files`), temporary files, and registry settings. Always consider file permissions, privilege escalation vectors, and persistence mechanisms while investigating the system.

By being aware of the structure and relevant files, penetration testers can efficiently identify vulnerabilities and misconfigurations, making this knowledge vital for both real-world testing and exams like the eJPT.

