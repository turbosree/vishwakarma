// -----------------------------------------------------------------------------
/// \file                  CalculateKPIs.h
/// \brief                 CalculateKPIs class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#ifndef _CalculateKPIs
#define _CalculateKPIs

#include <future>

using namespace std;

// -----------------------------------------------------------------------------
/// \class                 CalculateKPIs
/// \brief                 CalculateKPIs class implements capability to output
///                        results to standard console using a separate thread
// -----------------------------------------------------------------------------
class CalculateKPIs
{
 public:
   promise<struct KeyPerformanceIndicators> Promise;
   future<struct KeyPerformanceIndicators> Future;
   // Move data from real-time thread to normal thread
   CalculateKPIs(struct KeyPerformanceIndicators kpis);
};

#endif // _CalculateKPIs
