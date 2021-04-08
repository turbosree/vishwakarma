// -----------------------------------------------------------------------------
/// \file                  IIoTFullSystemModel.h
/// \brief                 Fieldbus control network simulator
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#ifndef _IIoTFullSystemModel
#define _IIoTFullSystemModel

#include "SimulationDataTypes.h"

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

class FieldBusDevice;
class FieldBus;
class MsgBatch;

// -----------------------------------------------------------------------------
/// \class                 IIoTFullSystemModel
/// \brief                 IIoTFullSystemModel main class represents the 
///                        IIoT full system model as below,
///                        <edge>-----(1)-----<gateway>-----(2)------<cloud-native-env>-----(3)------<services>
///                        Current implementation only model the filed bus system (1) and just run dummy systems for (2) and (3)
///                        TODO: Connected gateway
///                        TODO: Scalable services
// -----------------------------------------------------------------------------
class IIoTFullSystemModel
{
 public:
   // Various stages of the simulation
   void ProtocolMsgBatchBuilderStage(void);
   void PushMsgBatchToFieldBusStage(void);
   void FieldBusDeviceProcessMsgStage(void);
   void FieldBusProtocolMsgInspectionStage(void);
   void KPICalculationStage(int& NumOfSteps, int& CurrentStep, unsigned long& period);
   void MoveForwardFieldBusProtocolStage(void);
   void ShowFieldBus(void);
   void ShowFieldBusDevices(void);
   // Various stages of the simulation
   IIoTFullSystemModel(int NumDevices, bool RandomizeBatch);
   
 private:
   std::vector<std::unique_ptr<FieldBusDevice>> Devices;
   std::vector<std::unique_ptr<FieldBus>> Buses;
   std::vector<std::unique_ptr<MsgBatch>> Batches;
   std::unique_ptr<MsgBatch> CurrentBatch;
 
   struct KeyPerformanceIndicators KPIs = {0,0,0,0,0,0,0,0,0,0,0,0};
   int NumOfFieldBuses;
   bool RandomizeCurrentBatch;
};

#endif // _IIoTFullSystemModel
