// -----------------------------------------------------------------------------
/// \file                  KPIReport.cpp
/// \brief                 KPIReport class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#include "KPIReport.h"
#include "SimulationDataTypes.h"

#include <iostream>
#include <chrono>

// -----------------------------------------------------------------------------
/// \brief                 Constructor.
///                        Get data from real-time thread 
/// \param                 fut is the future object which has the data 
/// \return                Nothing
// -----------------------------------------------------------------------------
KPIReport::KPIReport(future<struct KeyPerformanceIndicators>&& fut)
   :Future(move(fut))
{
   // Get data from the real-time thread for further processing
   Data = Future.get();
}

// -----------------------------------------------------------------------------
/// \brief                 Method to output data to console
/// \param                 Nothing 
/// \return                Nothing
// -----------------------------------------------------------------------------
void KPIReport::Show(void)
{
   // do someting with data
   Data.UpTime = (Data.NumOfSteps - Data.CurrentStep) * (Data.Period / 1000000);

   cout <<"\nUptime (secs): "<<Data.UpTime
        <<"    "<<"TypeA: "<<Data.UnUsedInventoryOfTypeA
        <<"    "<<"TypeB: "<<Data.UnUsedInventoryOfTypeB
        <<"    "<<"TypeC: "<<Data.UnUsedInventoryOfTypeC
        <<"    "<<"TypeP: "<<Data.UnUsedInventoryOfTypeP
        <<"    "<<"TypeQ: "<<Data.UnUsedInventoryOfTypeQ
        <<"    "<<"Inventory: "<<(Data.TotalComponentInventoryCount * 4) + Data.CurrentBatchPending
        <<"    "<<"Throughput: "<<Data.Throughput
        << flush;
   cout<<endl;
   //this_thread::sleep_for(chrono::seconds(1));
}
