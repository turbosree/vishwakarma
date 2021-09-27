// -----------------------------------------------------------------------------
/// \file                  SimulationDataTypes.h
/// \brief                 Factory production line simulator data types
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#ifndef _SimulationDataTypes
#define _SimulationDataTypes


// Defines different types of items managed in assembly line
enum class Type{A, B, C, P, Q, X}; // X denotes, do no touch conveyer belt


// -----------------------------------------------------------------------------
/// \class                 KeyPerformanceIndicators
/// \brief                 KeyPerformanceIndicators structure holds the KPIs to
///                        report as part of results
// -----------------------------------------------------------------------------
struct KeyPerformanceIndicators
{
   int Throughput;
   int UpTime;
   int UnUsedInventoryOfTypeA;
   int UnUsedInventoryOfTypeB;
   int UnUsedInventoryOfTypeC;
   int UnUsedInventoryOfTypeP;
   int UnUsedInventoryOfTypeQ;
   unsigned long Period;
   int NumOfSteps;
   int CurrentStep;
   int TotalComponentInventoryCount;
   int CurrentBatchPending;
};

#endif // _SimulationDataTypes
