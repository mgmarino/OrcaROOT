// ORRunDecoder.cc

#include "ORRunDecoder.hh"
#include "ORLogger.hh"


std::string ORRunDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "isRemoteControlRun";
    case 1: return "isQuickStartRun";
    case 2: return "runStart";
    case 3: return "heartBeat";
    case 4: return "runStop";
    case 5: return "utime";
    case 6: return "timeToNextHeartBeat";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORRunDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return IsRemoteControlRun(record);
    case 1: return IsQuickStartRun(record);
    case 2: return IsRunStart(record);
    case 3: return IsHeartBeat(record);
    case 4: return IsRunStop(record);
    case 5: return UtimeOf(record);
    case 6: return TimeToNextHeartBeat(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}

