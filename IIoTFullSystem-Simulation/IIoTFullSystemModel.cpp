// -----------------------------------------------------------------------------
/// \file                  IIoTFullSystemModel.cpp
/// \brief                 Fieldbus system simulator class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#include "IIoTFullSystemModel.h"
#include "Item.h"
#include "FieldBus.h"
#include "FieldBusDevice.h"
#include "CalculateKPIs.h"
#include "MsgBatch.h"

#include <iostream>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

// Static variables
static pthread_t RTThread1 = 0;
int Item::CreatedCount = 0;
int Item::DestroyedCount = 0;

// -----------------------------------------------------------------------------
/// \class                 RTTimer
/// \brief                 RTTimer structure holds the timer attributes
// -----------------------------------------------------------------------------
struct RTTimer
{
   struct timespec TimeSpec;
   unsigned long TimePeriod;
};

// -----------------------------------------------------------------------------
/// \class                 SimulationArgs
/// \brief                 SimulationArgs structure holds the simulation
///                        command line arguments
// -----------------------------------------------------------------------------
struct SimulationArgs {
   unsigned long arg1;
   int arg2;
   int arg3;
   bool arg4;
};

// -----------------------------------------------------------------------------
/// \brief                 Function to make the thread periodic using
///                        a monotonically increasing system-wide clock in Linux 
/// \param                 period is a time Period to before next clock tick
/// \param                 timer is the timer structure to hold next clock tick
/// \return                -1 if get time function failed
// -----------------------------------------------------------------------------
int RTMakePeriodic(unsigned long period, struct RTTimer* timer)
{
   int ret;
   ret = clock_gettime(CLOCK_MONOTONIC, &(timer->TimeSpec));
   if(ret == -1)
   {
      std::cout<<"Error: clock_gettime returned an error!"<<std::endl;
   }

   // First cycle start after a second
   timer->TimeSpec.tv_sec++;
   timer->TimePeriod = period;
   return ret;
}

// -----------------------------------------------------------------------------
/// \brief                 Function to make the thread periodic using
///                        a special sleep function which has nano second resolution 
/// \param                 timer is the timer structure to hold next clock tick
/// \return                void
// -----------------------------------------------------------------------------
void RTWaitPeriod(struct RTTimer* timer)
{
   int ret;

   ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(timer->TimeSpec), NULL);
   if(ret != 0)
   {
      std::cout<<"Error"<<std::endl;
      return;
   }

   timer->TimeSpec.tv_nsec += (timer->TimePeriod) * 1000;

   // Convert the above nano second time to seconds and nano seconds.
   while(timer->TimeSpec.tv_nsec >= 1000000000)
   {
      timer->TimeSpec.tv_nsec -= 1000000000;
      timer->TimeSpec.tv_sec++;
   }
}

// -----------------------------------------------------------------------------
/// \brief                 Real time thread function which implements a pipeline 
///                        approach to simulate a field bus system. 
/// \param                 args contain the simulation parameters to initialize it 
/// \return                Nothing
// -----------------------------------------------------------------------------
static void* RTThread1Function(void *args)
{
   struct RTTimer timer;
   struct SimulationArgs* arguments = (struct SimulationArgs*) args; 

   unsigned long period = arguments->arg1; // in micro seconds
   std::cout<<"Simulation unit of time (cycle) in us: "<<period<<std::endl;
   int NumDevices = arguments->arg2; // Number of devices in each bus
   std::cout<<"Number of IoT layers in Simulation: "<<NumDevices<<std::endl;
   int Steps = arguments->arg3; // Number of simulation cycles to run
   std::cout<<"Number of steps to run Simulation: "<<Steps<<std::endl;
   bool RandomizeBatch = arguments->arg4; // Randomize messages in batch
   std::cout<<"Randomize messages in batch: "<<RandomizeBatch<<std::endl;
   
   //Create a simulation with required number of devices per bus.
   IIoTFullSystemModel Network(NumDevices, RandomizeBatch);

   // Get current monotonic clock value and initialize the timer
   RTMakePeriodic(period, &timer);
   do
   {
      // Cycle accurate fieldbus simulation, implemented as a pipeline.
      // Each stage below modifies the state of the object "Network".
      // In each unit of time(cycle), one iteration through this loop is executed.
      std::cout<<"\n\nSimulation cycle:     "<<((arguments->arg3)-Steps)<<std::endl;

      Network.ProtocolMsgBatchBuilderStage();
      Network.PushMsgBatchToFieldBusStage();
      Network.FieldBusDeviceProcessMsgStage();
      Network.FieldBusProtocolMsgInspectionStage();
      Network.KPICalculationStage(arguments->arg3, Steps, period);
      Network.MoveForwardFieldBusProtocolStage();

      std::cout<<"\n\n"<<std::endl;

      // Wait for next cycle (Deterministic sleep)
      RTWaitPeriod(&timer);
   }while(--Steps);

   // We expect all Items destroyed at this point.
   std::cout<<"Number of items created: "<<Item::CreatedCount<<std::endl;
   std::cout<<"Number of items destroyed: "<<Item::DestroyedCount<<std::endl;

   return 0;
}

// -----------------------------------------------------------------------------
/// \brief                 Constructor
/// \param                 ports is the number of buses in the control network
/// \return                randomize decides whether the messages in a batch is randomized
// -----------------------------------------------------------------------------
IIoTFullSystemModel::IIoTFullSystemModel(int ports, bool randomize)
   :NumOfFieldBuses(ports),
    RandomizeCurrentBatch(randomize)
{
   // 2 additional ports: one for pre-fetching at start of the simulation stages and another for inspection at the end of simulation stages
   for(auto i=0; i<NumOfFieldBuses+2; ++i)
   {
      auto port = std::make_unique<FieldBus>();
      Buses.emplace_back(move(port));

      for(auto i : {0,1}) // 2 devices per bus. [TODO]
      {
         auto device = std::make_unique<FieldBusDevice>();
         Devices.emplace_back(move(device));
      }
   }
}

// -----------------------------------------------------------------------------
/// \brief                 1st stage: Build a batch with randomly coming
///                        messages of type: A, B, C
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void IIoTFullSystemModel::ProtocolMsgBatchBuilderStage(void)
{
   // A batch here represent a sequence of messages in a specific bus protocol
   // Eg: A diagnostics protocol to collect error codes from all connected devices
   auto batch = std::make_unique<MsgBatch>();
   Batches.emplace_back(std::move(batch));
}

// -----------------------------------------------------------------------------
/// \brief                 2nd stage: Push a batch to the bus one message at
///                        a time each cycle
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void IIoTFullSystemModel::PushMsgBatchToFieldBusStage(void)
{
   // Buses[0].Ready()?
   if(!Batches.empty())
   {
      if(CurrentBatch != nullptr)
      {
         if(CurrentBatch->Msgs.empty())
         {
            CurrentBatch = move(Batches.back());
            Batches.pop_back();
         }
      }
      else
      {
         CurrentBatch = move(Batches.back());
         Batches.pop_back();
      }

      // Randomize message batch (of type A, B & C) if configured.
      // It makes sense to randomize messages in diagnostic protocols for examples,
      // where order of the message is not important.
      if(RandomizeCurrentBatch)
      {
         random_shuffle(CurrentBatch->Msgs.begin(), CurrentBatch->Msgs.end());
      }

      // Show current batch
      std::cout<<"Current batch:        "<<std::flush;
      for(auto& item : CurrentBatch->Msgs)
      {
         if(item != nullptr)
         {
            char c = 'X';
            switch(item->Name)
            {
            case Type::A:
               c = 'A';
               break;
            case Type::B:
               c = 'B';
               break;
            case Type::C:
               c = 'C';
               break;
            default:
               break;
            }
            std::cout<<"| "<<c<<" "<<std::flush;
         }
         else
         {
            std::cout<<"| "<<" "<<" "<<std::flush;
         }
      }
      std::cout<<"| -->"<<std::flush;
      std::cout<<std::endl;
      
      Buses[0]->LoadItem(move(CurrentBatch->Msgs.back()));
      CurrentBatch->Msgs.pop_back();
   }
   else
   {
      std::cout<<"No Batch to push!"<<std::endl;
   }
}

// -----------------------------------------------------------------------------
/// \brief                 3rd stage: Bus devices process the message they received.
///                        Devices can implement their own behaviours.
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void IIoTFullSystemModel::FieldBusDeviceProcessMsgStage(void)
{
   // Run all buses and their the respective devices connected [TODO]
   int NumDevicesPerBus = 2;
   // 2 devices per bus. [TODO]
   std::vector<int> devices = {0,1};
   for(auto& port : Buses)
   {
      // First and last buses are considered as ingress points and have no devices connected to them.
      // It is for internal use in the simulation
      if((port == Buses[0]) || (port == Buses[NumOfFieldBuses+1]))
      {
         continue;
      }

      // Choose the devices in a random fashion to process the messages
      std::random_shuffle(devices.begin(), devices.end());
      for(auto i : devices) 
      {
         // Message arrive at all devices connected to the bus. But only the device to which the message is destined
         // will process it and produce a data response. 
         // port.pCurrentItem will be nullptr if this device successfully process the current message.
         // port.pCurrentItem will be a type of data response message if the device finish it's task.
         port->pCurrentItem = Devices[i+NumDevicesPerBus]->ProcessMessages(move(port->pCurrentItem));
      }
      // Type::X is send to the bus to satisfy following condition:
      // Message arrive at all devices connected to the bus. But only the device to which the message is destined will process it.
      // It can be used to model bus arbitration behaviours [TODO]
      // Now that all devices have finished their task, we can delete the object.
      if(port->pCurrentItem != nullptr && port->pCurrentItem->Name == Type::X)
      {
         port->pCurrentItem.reset();
      }
      NumDevicesPerBus += 2; // [TODO]
   }
   ShowFieldBus();
   ShowFieldBusDevices();
}

// -----------------------------------------------------------------------------
/// \brief                 4th stage: Inspect the bus at the end of a simulation cycle to
///                        to categorize messages on the bus during the protocol communication
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void IIoTFullSystemModel::FieldBusProtocolMsgInspectionStage(void)
{
   if(Buses[NumOfFieldBuses+1]->pCurrentItem != nullptr)
   {
      // Inspect the messages moved to the inspection port
      //cout<<"Type of item in the inspection port in this cycle: "<<static_cast<std::underlying_type<Type>::type>(Buses[NumOfFieldBuses+1]->pCurrentItem->Name)<<endl;

      // Categorize items and update throughput and KPIs
      switch(Buses[NumOfFieldBuses+1]->pCurrentItem->Name)
      {
      case Type::A:
         KPIs.NoMessagesOfTypeA++;
         break;
      case Type::B:
         KPIs.NoMessagesOfTypeB++;
         break;
      case Type::C:
         KPIs.NoMessagesOfTypeC++;
         break;
      case Type::P:
         KPIs.NoMessagesOfTypeP++;
         KPIs.Throughput++;
         break;
      case Type::Q:
         KPIs.NoMessagesOfTypeQ++;
         KPIs.Throughput++;
         break;
      default:
         break;
      }
   }
}

// -----------------------------------------------------------------------------
/// \brief                 5th stage: Collect all the data, organize it
///                        and pass it to a non-real time thread for processing and
///                        live reporting
/// \param                 NumOfSteps is the total number of steps scheduled for simulation
/// \param                 CurrentStep is the number of steps elaped counting down
/// \param                 period is the cycle period of the running simulation
/// \return                Nothing
// -----------------------------------------------------------------------------
void IIoTFullSystemModel::KPICalculationStage(int& NumOfSteps, int& CurrentStep, unsigned long& period)
{
   // Detached thread so that the calculations and report do not impact real-time simulation thread
   KPIs.NumOfSteps = NumOfSteps;
   KPIs.CurrentStep = CurrentStep;
   KPIs.Period = period;
   KPIs.TotalMessageInventoryCount = Batches.size();
   KPIs.CurrentBatchPending = CurrentBatch->Msgs.size();
   CalculateKPIs calc(KPIs);
}

// -----------------------------------------------------------------------------
/// \brief                 6th stage: Run the protocol stack one step forward at a time
///                        every simulation cycle
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void IIoTFullSystemModel::MoveForwardFieldBusProtocolStage(void)
{
   // Data arrived at the inspection port is already inspected, we can remove it now.
   // Here we also destroy all the messages that was pushed to the bus.
   // Alternatively it could be stored in a container for further processing. 
   Buses.pop_back();

   // Since we poped a port we need to put a new one. Beauty of simulation, don't attempt 
   // in your workplace, it will distroy the hardware!! :D
   auto port = std::make_unique<FieldBus>();
   Buses.emplace(Buses.begin(), std::move(port));
   // ShowFieldBus();
   // ShowFieldBusDevices();
}

void IIoTFullSystemModel::ShowFieldBus(void)
{
   // Print type of messages appear on the bus
   std::cout<<"Bus status:           ";
   for(auto& port : Buses)
   {
      if(port->pCurrentItem != nullptr)
      {
         char c = 'X';
         switch(port->pCurrentItem->Name)
         {
         case Type::A:
            c = 'A';
            break;
         case Type::B:
            c = 'B';
            break;
         case Type::C:
            c = 'C';
            break;
         case Type::P:
            c = 'P';
            break;
         case Type::Q:
            c = 'Q';
            break;            
         default:
            break;
         }
         std::cout<<"| "<<c<<" "<<std::flush;
      }
      else
      {
         std::cout<<"| "<<" "<<" "<<std::flush;
      }
   }
   std::cout<<"|"<<std::flush;
   std::cout<<std::endl;
}

void IIoTFullSystemModel::ShowFieldBusDevices(void)
{
   // Print items on the bus
   std::cout<<"Device status:        ";
   int i = 0;
   for(auto& device : Devices)
   {
      // if(i < 2 || i > 7)
      // {
      //    i++;
      //    continue;
      // }
      if(device->pItem1 != nullptr)
      {
         char c = 'X';
         switch(device->pItem1->Name)
         {
         case Type::A:
            c = 'A';
            break;
         case Type::B:
            c = 'B';
            break;
         case Type::C:
            c = 'C';
            break;
         case Type::P:
            c = 'P';
            break;
         case Type::Q:
            c = 'Q';
            break;            
         default:
            break;
         }
         std::cout<<"[ "<<c<<" "<<std::flush;
      }
      else
      {
         std::cout<<"[ "<<" "<<" "<<std::flush;
      }
      std::cout<<"]"<<std::flush;
      if(device->pItem2 != nullptr)
      {
         char c = 'X';
         switch(device->pItem2->Name)
         {
         case Type::A:
            c = 'A';
            break;
         case Type::B:
            c = 'B';
            break;
         case Type::C:
            c = 'C';
            break;
         case Type::P:
            c = 'P';
            break;
         case Type::Q:
            c = 'Q';
            break;            
         default:
            break;
         }
         std::cout<<"[ "<<c<<" "<<std::flush;
      }
      else
      {
         std::cout<<"[ "<<" "<<" "<<std::flush;
      }
      // (i++ % 2) ? cout<<"]   "<<flush : cout<<"]---"<<flush;
      std::cout<<"]---"<<std::flush;
   }
   std::cout<<std::endl;
}

// -----------------------------------------------------------------------------
/// \brief                 Main application thread and main function
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
   std::cout << "\n\nRunning Simulation with " << argc 
        << " arguments: "; 
   for (int i = 0; i < argc; ++i) 
      std::cout << argv[i] << " ";
   std::cout<<"\n\n"<<std::endl;
   std::cout<<"Results:"<<std::endl;

   struct SimulationArgs args;
   if(argc >= 5)
   {
      args.arg1 = atoi(argv[1]);
      args.arg2 = atoi(argv[2]);
      args.arg3 = atoi(argv[3]);
      args.arg4 = atoi(argv[4]);
   }
   else
   {
      // default simulation setting
      args.arg1 = 1000000;
      args.arg2 = 3;
      args.arg3 = 100;
      args.arg4 = 0;
   }

   // Create a real-time simulation thread using real-time extentions of Linux Kernel
   int result = pthread_create(&RTThread1, NULL, &RTThread1Function, (void*)&args);
   if(result != 0)
   {
      std::cout<<"Error: Thread creation failed!"<<std::endl;
   }
   struct sched_param SchedParam;
   SchedParam.sched_priority = 1;
   if(RTThread1)
   {
      pthread_setschedparam(RTThread1, SCHED_FIFO, &SchedParam);
   }
   if(result != 0)
   {
      std::cout<<"Error: Setting real-time scheduling poilicy failed!"<<std::endl;
   }

   // Wait for real-time thread to join for gracefully exiting the Simulation
   pthread_join(RTThread1, NULL);
   std::cout<<"\n\n"<<std::endl;
   
   return 0;
}

// TODO:
/*
Ability to add the simulation as a module in Python

*/
