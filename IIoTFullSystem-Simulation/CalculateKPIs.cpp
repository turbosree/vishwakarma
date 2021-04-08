// -----------------------------------------------------------------------------
/// \file                  CalculateKPIs.cpp
/// \brief                 CalculateKPIs class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#include "CalculateKPIs.h"
#include "SimulationDataTypes.h"
#include "KPIReport.h"

#include <thread>
#include <iostream>
#include <chrono>

// -----------------------------------------------------------------------------
/// \brief                 Constructor.
///                        Create the report thread and pass the data to it 
/// \param                 kpis is the key performance indicators data
/// \return                Nothing
// -----------------------------------------------------------------------------
CalculateKPIs::CalculateKPIs(struct KeyPerformanceIndicators kpis)
{
   Future = Promise.get_future();
   Promise.set_value(kpis);
   KPIReport report(move(Future));
   std::thread th(&KPIReport::Show, &report);
   th.join(); // TODO: Use th.detach(); to separate from real-time thread     
}

