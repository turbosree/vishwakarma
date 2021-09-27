// -----------------------------------------------------------------------------
/// \file                  FactoryProductionLine.h
/// \brief                 Factory production line simulator
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#ifndef _FactoryProductionLine
#define _FactoryProductionLine

#include "SimulationDataTypes.h"

#include <iostream>
#include <algorithm>
#include <memory>

using namespace std;

class WorkerStation;
class ConveyorBeltSlot;
class ComponentBatch;

// -----------------------------------------------------------------------------
/// \class                 FactoryProductionLine
/// \brief                 FactoryProductionLine main class represents the 
///                        Factory production line
// -----------------------------------------------------------------------------
class FactoryProductionLine
{
 public:
   // Various stages of the assembly line
   void ComponentBatchBuilderStage(void);
   void LoadComponentBatchToConveyorBeltStage(void);
   void ConveyorBeltSlotWorkerStage(void);
   void ConveyorBeltSlotInspectionStage(void);
   void KPICalculationStage(int& NumOfSteps, int& CurrentStep, unsigned long& period);
   void MoveForwardBeltStage(void);
   void ShowConveyorBelt(void);
   void ShowWorkerStations(void);
   // Various stages of the assembly line
   FactoryProductionLine(int slots, bool randomize);
   
 private:
   vector<unique_ptr<WorkerStation>> Stations;
   vector<unique_ptr<ConveyorBeltSlot>> ConveyorBelt;
   vector<unique_ptr<ComponentBatch>> Batches;
   unique_ptr<ComponentBatch> CurrentBatch;
 
   struct KeyPerformanceIndicators KPIs = {0,0,0,0,0,0,0,0,0,0,0,0};
   int NumOfConveyorBeltSlots;
   bool RandomizeCurrentBatch;
};

#endif // _FactoryProductionLine
