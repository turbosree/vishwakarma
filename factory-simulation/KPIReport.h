// -----------------------------------------------------------------------------
/// \file                  KPIReport.h
/// \brief                 KPIReport class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#ifndef _KPIReport
#define _KPIReport

#include "SimulationDataTypes.h"

#include <future>
#include <iostream>

using namespace std;

// -----------------------------------------------------------------------------
/// \class                 KPIReport
/// \brief                 KPIReport class implements capability to output
///                        results to standard console using a separate thread
// -----------------------------------------------------------------------------
class KPIReport
{
 public:
   future<struct KeyPerformanceIndicators> Future;
   struct KeyPerformanceIndicators Data = {0,0,0,0,0,0,0,0,0,0,0,0};
   KPIReport(future<struct KeyPerformanceIndicators>&& fut);
   void Show(void);
};

#endif // _KPIReport
