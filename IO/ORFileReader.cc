// ORFileReader.cc

#include "ORFileReader.hh"

#include "ORLogger.hh"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>

using namespace std;

ORFileReader::ORFileReader(string filename)
{
  if (filename != "") AddFileToProcess(filename);
}

size_t ORFileReader::Read(char* buffer, size_t nBytesMax)
{
  read(buffer, nBytesMax);
  size_t bytesLeft = nBytesMax - gcount();
  if(bytesLeft > 0) {
    Close();
    if(Open()) bytesLeft -= Read(buffer, bytesLeft);
  }
  return nBytesMax-bytesLeft;
}

bool ORFileReader::OpenDataStream()
{
  if(fFileList.size() > 0) {
    ORLog(kDebug) << "OpenDataStream(): opening file " << fFileList[0] << endl;
    open(fFileList[0].c_str());
    if(!OKToRead()) {
      ORLog(kError) << "Could not open file " << fFileList[0] << endl;
      return false;
    }
    fCurrentFileName = fFileList[0];
    fFileList.erase(fFileList.begin());
    return true;
  }
  else {
    ORLog(kDebug) << "OpenDataStream(): no more files to open. " << endl;
    return false;
  }
}

string ORFileReader::GetFileDate()
{
  struct tm* clock;
  struct tm* gmclock;
  struct stat attrib;
  stat(fCurrentFileName.c_str(), &attrib);
        
  clock = localtime(&(attrib.st_mtime));
  gmclock = gmtime(&(attrib.st_mtime));
  int timeZone = clock->tm_hour - gmclock->tm_hour;
  if(clock->tm_mday > gmclock->tm_mday) {
    timeZone += 24;
  }
  else if(clock->tm_mday < gmclock->tm_mday) {
    timeZone -= 24;
  }

  // return in the format used by the ORCA header, 
  // e.g. 2006-07-25 14:42:23 -0700
  ostringstream os;
  os << clock->tm_year + 1900 << "-" << clock->tm_mon+1 << "-" << clock->tm_mday << " "
     << clock->tm_hour << ":" << clock->tm_min << ":" << clock->tm_sec  << " "
     << setw(2) << setfill('0') << timeZone << "00";
  return os.str();
}

string ORFileReader::GetFilePath()
{
  size_t firstSlashPos = fCurrentFileName.find("/");
  size_t lastSlashPos = fCurrentFileName.rfind("/");

  // If the file name starts with /, the full path is specified
  if(firstSlashPos == 0) {
    if(lastSlashPos == 0) lastSlashPos++; // keep / if it is the full path
    return fCurrentFileName.substr(0, lastSlashPos);
  }

  // Otherwise, start from the current working directory. 
  char cwd[MAXPATHLEN];
  getcwd(cwd, MAXPATHLEN);
  string globalPath = cwd;
  // Chop off any trailing "/"
  if(globalPath.rfind("/") == globalPath.size()-1) {
    globalPath = globalPath.substr(0, globalPath.size()-1);
  }
  // If no slash was found, the file is in cwd.
  if(lastSlashPos == string::npos) return globalPath;
  
  // Remove a directory from the end of globalPath for each instance of ../ in
  // fCurrentFileName
  string localPath = fCurrentFileName.substr(0, lastSlashPos);
  while(localPath.substr(0, 3) != "../") {
    lastSlashPos = globalPath.rfind("/");
    if(lastSlashPos == 0) {
      ORLog(kError) << "Can't ../ past / for " << fCurrentFileName << endl;
      return "";
    }
    globalPath = globalPath.substr(0, lastSlashPos);
    localPath = localPath.substr(3);
  }

  // We are left with a concatenatable global and local path
  return globalPath + "/" + localPath;
}

int ORFileReader::GetFileTime() 
{
  struct stat attrib;
  stat(fCurrentFileName.c_str(), &attrib);
  return (int) attrib.st_mtime;
}

int ORFileReader::GetFileRefTime()
{
  struct tm refTm; // 1 Jan 2001 GMT
  refTm.tm_sec = 0;
  refTm.tm_min = 0;
  refTm.tm_hour = 0;
  refTm.tm_mday = 1;
  refTm.tm_mon = 0;
  refTm.tm_year = 101;
  time_t refTime = mktime(&refTm);

  struct stat attrib;
  stat(fCurrentFileName.c_str(), &attrib);
  return (int) difftime(attrib.st_mtime, refTime);
}

