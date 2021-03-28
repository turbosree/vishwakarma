// -----------------------------------------------------------------------------
/// \file                  SimulationDataTypes.h
/// \brief                 Simulator data types
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#ifndef _SimulationDataTypes
#define _SimulationDataTypes


// Defines different types of items (Eg: messages) managed in the system
// TODO: Explain a simple protocol by using below generic types
enum class Type{A, B, C, P, Q, X};


// -----------------------------------------------------------------------------
/// \class                 KeyPerformanceIndicators
/// \brief                 KeyPerformanceIndicators structure holds the KPIs to
///                        report as part of results
// -----------------------------------------------------------------------------
struct KeyPerformanceIndicators
{
   int Throughput;
   int UpTime;
   int NoMessagesOfTypeA;
   int NoMessagesOfTypeB;
   int NoMessagesOfTypeC;
   int NoMessagesOfTypeP;
   int NoMessagesOfTypeQ;
   unsigned long Period;
   int NumOfSteps;
   int CurrentStep;
   int TotalMessageInventoryCount;
   int CurrentBatchPending;
};

#endif // _SimulationDataTypes
