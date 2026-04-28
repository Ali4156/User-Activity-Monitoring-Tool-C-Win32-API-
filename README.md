# 🖥️ User Activity Monitoring Tool

A Windows-based **User Activity Monitoring Tool** developed in C++ using the Win32 API.
This application tracks all running processes in real time and logs their start time, end time, and execution duration.

---

## 📌 Project Overview

This tool monitors system processes by taking periodic snapshots and comparing them to detect:

* Process start events
* Process termination events
* Execution duration of each process

All logs are displayed in a GUI and saved into a file for future analysis.

📄 Based on project report: 

---

## 🎯 Features

* ✅ Real-time process monitoring
* ✅ Detects process start and stop
* ✅ Calculates execution duration
* ✅ GUI-based log display
* ✅ Auto-scrolling log window
* ✅ File logging (activity_log.txt)
* ✅ Start and Stop monitoring buttons

---

## 🛠️ Technologies Used

* **Language:** C++
* **Platform:** Windows
* **API:** Win32 API
* **Libraries:**

  * windows.h
  * tlhelp32.h
  * STL (set, map, string)
  * fstream
  * ctime

---

## ⚙️ How It Works

1. Takes a snapshot of running processes using Windows API
2. Compares current snapshot with previous snapshot
3. Detects:

   * New process → logs **PROCESS STARTED**
   * Ended process → logs **PROCESS ENDED**
4. Calculates duration using system time
5. Displays logs in GUI and saves to file

📄 Detailed working explained in report: 

---

## 🖼️ Example Output

```
PROCESS STARTED: chrome.exe | 21:51:18
PROCESS ENDED: chrome.exe | 21:53:10 (Duration: 112 sec)
```

---

## 📂 Project Structure

```
📁 User-Activity-Monitor
 ┣ 📄 User Activity Monitor GUI.cpp
 ┣ 📄 activity_log.txt (generated)
 ┣ 📄 Project_Report.pdf
 ┗ 📄 README.md
```

---

## ▶️ How to Run

1. Open project in Visual Studio / CodeBlocks
2. Compile the C++ file
3. Run the executable
4. Click:

   * **Start Monitoring**
   * **Stop Monitoring**

---

## 📊 Advantages

* Simple and lightweight
* Real-time monitoring
* Persistent log storage
* Useful for cybersecurity and system tracking

---

## ⚠️ Limitations

* Works only on Windows
* No CPU or RAM usage tracking
* Uses fixed time interval (may miss very short processes)

---

## 🚀 Future Improvements

* CPU & RAM usage tracking
* Export logs to PDF/Excel
* Process filtering
* Alert system
* Improved GUI

---

## 🔐 Cyber Security Importance

This tool helps in:

* Detecting suspicious processes
* Tracking user activity
* Supporting forensic analysis

(Explained in detail in report pages 8–9: )

---

## 👨‍💻 Author

**Raja Ali Shahid**
SAP ID: 63158

---

## 📜 License

This project is for academic purposes.

---
