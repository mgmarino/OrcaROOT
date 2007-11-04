#include "ORLogger.hh"
#include "ORHeader.hh"

using namespace std;


int main(int argc, char** argv)
{
  if(argc !=2) {
    cout << "Usage: " << argv[0] << " [file]" << endl;
    return 1;
  }

  ORLogger::SetSeverity(ORLogger::kDebug);

  ORHeader header;
  header.LoadHeaderFile(argv[1]);
  ORLog(kRoutine) << endl;

  return 0;
}

