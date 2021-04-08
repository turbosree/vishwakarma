// -----------------------------------------------------------------------------
/// \file                  KPIReport.h
/// \brief                 KPIReport class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#ifndef _KPIReport
#define _KPIReport

#include "SimulationDataTypes.h"

#include <future>
#include <iostream>

// -----------------------------------------------------------------------------
/// \class                 KPIReport
/// \brief                 KPIReport class implements capability to output
///                        results to standard console using a separate thread
// -----------------------------------------------------------------------------
class KPIReport
{
 public:
   std::future<struct KeyPerformanceIndicators> Future;
   struct KeyPerformanceIndicators Data = {0,0,0,0,0,0,0,0,0,0,0,0};
   KPIReport(std::future<struct KeyPerformanceIndicators>&& fut);
   void Show(void);
};

#endif // _KPIReport
