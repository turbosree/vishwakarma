// -----------------------------------------------------------------------------
/// \file                  FactoryProductionLine.cpp
/// \brief                 Factory production line simulator class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#include "FactoryProductionLine.h"
#include "Item.h"
#include "ConveyorBeltSlot.h"
#include "WorkerStation.h"
#include "CalculateKPIs.h"
#include "ComponentBatch.h"

#include <iostream>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

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
      cout<<"Error: clock_gettime returned an error!"<<endl;
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
      cout<<"Error"<<endl;
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
///                        approach to simulate a assembly line. 
/// \param                 args contain the simulation parameters to initialize it 
/// \return                Nothing
// -----------------------------------------------------------------------------
static void* RTThread1Function(void *args)
{
   struct RTTimer timer;
   struct SimulationArgs* arguments = (struct SimulationArgs*) args; 

   unsigned long period = arguments->arg1; // in micro seconds
   cout<<"Simulation unit of time (cycle) in us: "<<period<<endl;
   int NumPairsOfWorkers = arguments->arg2; // 3 as per problem
   cout<<"Number of pairs of workers in Simulation: "<<NumPairsOfWorkers<<endl;
   int Steps = arguments->arg3; // 100 as per problem
   cout<<"Number of steps to run Simulation: "<<Steps<<endl;
   bool RandomizeBatch = arguments->arg4; // Randomize components in batch
   cout<<"Randomize components in batch: "<<RandomizeBatch<<endl;
   
   //Create a simulation with required pairs of workers.
   FactoryProductionLine factory(NumPairsOfWorkers, RandomizeBatch);

   // Get current monotonic clock value and initialize the timer
   RTMakePeriodic(period, &timer);
   do
   {
      // Cycle accurate factory simulation, implemented as a pipeline.
      // Each stage below modifies the state of the object "factory".
      // In each unit of time(cycle), one iteration through this loop is executed.
      cout<<"\n\nSimulation cycle:     "<<((arguments->arg3)-Steps)<<endl;

      factory.ComponentBatchBuilderStage();
      factory.LoadComponentBatchToConveyorBeltStage();
      factory.ConveyorBeltSlotWorkerStage();
      factory.ConveyorBeltSlotInspectionStage();
      factory.KPICalculationStage(arguments->arg3, Steps, period);
      factory.MoveForwardBeltStage();

      cout<<"\n\n"<<endl;

      // Wait for next cycle (Deterministic sleep)
      RTWaitPeriod(&timer);
   }while(--Steps);

   // We expect all Items destroyed at this point.
   cout<<"Number of items created: "<<Item::CreatedCount<<endl;
   cout<<"Number of items destroyed: "<<Item::DestroyedCount<<endl;

   return 0;
}

// -----------------------------------------------------------------------------
/// \brief                 Constructor
/// \param                 slots is the number of slots needed in the conveyor belt
/// \return                randomize decides whether the components in a batch is randomized
// -----------------------------------------------------------------------------
FactoryProductionLine::FactoryProductionLine(int slots, bool randomize)
   :NumOfConveyorBeltSlots(slots),
    RandomizeCurrentBatch(randomize)
{
   // 2 additional slots: one for pre-fetching at start of the line and another for inspection at the end of line
   for(auto i=0; i<NumOfConveyorBeltSlots+2; ++i)
   {
      auto slot = make_unique<ConveyorBeltSlot>();
      ConveyorBelt.emplace_back(move(slot));

      for(auto i : {0,1}) // 2 workers per slot. 
      {
         auto station = make_unique<WorkerStation>();
         Stations.emplace_back(move(station));
      }
   }
}

// -----------------------------------------------------------------------------
/// \brief                 1st stage: Build a batch with randomly coming
///                        components of type: A, B, C
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void FactoryProductionLine::ComponentBatchBuilderStage(void)
{
   auto batch = make_unique<ComponentBatch>();
   Batches.emplace_back(move(batch));
}

// -----------------------------------------------------------------------------
/// \brief                 2nd stage: Load a batch to conveyor one component at
///                        a time each cycle
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void FactoryProductionLine::LoadComponentBatchToConveyorBeltStage(void)
{
   // ConveyorBelt[0].Ready()?
   if(!Batches.empty())
   {
      if(CurrentBatch != nullptr)
      {
         if(CurrentBatch->Components.empty())
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

      // Randomize component batch (of type A, B & C) if configured
      if(RandomizeCurrentBatch)
      {
         random_shuffle(CurrentBatch->Components.begin(), CurrentBatch->Components.end());
      }

      // Show current batch
      cout<<"Current batch:        "<<flush;
      for(auto& item : CurrentBatch->Components)
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
            cout<<"| "<<c<<" "<<flush;
         }
         else
         {
            cout<<"| "<<" "<<" "<<flush;
         }
      }
      cout<<"| -->"<<flush;
      cout<<endl;
      
      ConveyorBelt[0]->LoadItem(move(CurrentBatch->Components.back()));
      CurrentBatch->Components.pop_back();
   }
   else
   {
      cout<<"No Batch to lead!"<<endl;
   }
}

// -----------------------------------------------------------------------------
/// \brief                 3rd stage: Worker station in each side of belt
///                        execute assembly line procedure
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void FactoryProductionLine::ConveyorBeltSlotWorkerStage(void)
{
   // Run all slots and their respective worker stations
   int NumWorkersPerStation = 2;
   // 2 workers per slot.
   vector<int> workers = {0,1};
   for(auto& slot : ConveyorBelt)
   {
      // First and last slots have no stations
      if((slot == ConveyorBelt[0]) || (slot == ConveyorBelt[NumOfConveyorBeltSlots+1]))
      {
         continue;
      }

      // Choose the first station to assemble product in a random fashion
      random_shuffle(workers.begin(), workers.end());
      for(auto i : workers) 
      {
         // slot.pCurrentItem will be nullptr if this station successfully picks up the current item.
         // slot.pCurrentItem will be the finished product of this station finish assembling product in this cycle.
         slot->pCurrentItem = Stations[i+NumWorkersPerStation]->AssembleProduct(move(slot->pCurrentItem));
      }
      // Type::X is placed on the belt to satisfy following condition in Simulation problem:
      // "So you can't have one worker picking something from a slot while their counterpart puts something down in the same place"
      // Now that both workers on either side of slot is finished, we can delete the object.
      if(slot->pCurrentItem != nullptr && slot->pCurrentItem->Name == Type::X)
      {
         slot->pCurrentItem.reset();
      }
      NumWorkersPerStation += 2;
   }
   ShowConveyorBelt();
   ShowWorkerStations();
}

// -----------------------------------------------------------------------------
/// \brief                 4th stage: Inspect the slot at the end of the line to
///                        to categorize items coming off the production line
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void FactoryProductionLine::ConveyorBeltSlotInspectionStage(void)
{
   if(ConveyorBelt[NumOfConveyorBeltSlots+1]->pCurrentItem != nullptr)
   {
      // Inspect the last slot at the end of line
      //cout<<"Type of item in the end of line in this cycle: "<<static_cast<std::underlying_type<Type>::type>(ConveyorBelt[NumOfConveyorBeltSlots+1]->pCurrentItem->Name)<<endl;

      // Categorize items and update throughput and KPIs
      switch(ConveyorBelt[NumOfConveyorBeltSlots+1]->pCurrentItem->Name)
      {
      case Type::A:
         KPIs.UnUsedInventoryOfTypeA++;
         break;
      case Type::B:
         KPIs.UnUsedInventoryOfTypeB++;
         break;
      case Type::C:
         KPIs.UnUsedInventoryOfTypeC++;
         break;
      case Type::P:
         KPIs.UnUsedInventoryOfTypeP++;
         KPIs.Throughput++;
         break;
      case Type::Q:
         KPIs.UnUsedInventoryOfTypeQ++;
         KPIs.Throughput++;
         break;
      default:
         break;
      }
   }
}

// -----------------------------------------------------------------------------
/// \brief                 5th stage: Capture state of the assembly line stages
///                        and pass it to a non-real time thread for processing and
///                        live reporting
/// \param                 NumOfSteps is the total number of steps scheduled for simulation
/// \param                 CurrentStep is the number of steps elaped counting down
/// \param                 period is the cycle period of the running simulation
/// \return                Nothing
// -----------------------------------------------------------------------------
void FactoryProductionLine::KPICalculationStage(int& NumOfSteps, int& CurrentStep, unsigned long& period)
{
   // Detached thread so that the calculations and report do not impact real-time simulation thread
   KPIs.NumOfSteps = NumOfSteps;
   KPIs.CurrentStep = CurrentStep;
   KPIs.Period = period;
   KPIs.TotalComponentInventoryCount = Batches.size();
   KPIs.CurrentBatchPending = CurrentBatch->Components.size();
   CalculateKPIs calc(KPIs);
}

// -----------------------------------------------------------------------------
/// \brief                 6th stage: Moce the conveyor belt one step at a time
///                        every simulation cycle
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
void FactoryProductionLine::MoveForwardBeltStage(void)
{
   // The last slot at the end of the line is already inspected, we can remove it now.
   // Here we also destroy all the unused components and finished product objects.
   // Alternatively it could be stored in a container for further processing. 
   ConveyorBelt.pop_back();

   // Since we poped a slot we need to put a new one.
   auto slot = make_unique<ConveyorBeltSlot>();
   ConveyorBelt.emplace(ConveyorBelt.begin(), move(slot));
   // ShowConveyorBelt();
   // ShowWorkerStations();
}

void FactoryProductionLine::ShowConveyorBelt(void)
{
   // Print items on the conveyor belt
   cout<<"Conveyor belt status: ";
   for(auto& slot : ConveyorBelt)
   {
      if(slot->pCurrentItem != nullptr)
      {
         char c = 'X';
         switch(slot->pCurrentItem->Name)
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
         cout<<"| "<<c<<" "<<flush;
      }
      else
      {
         cout<<"| "<<" "<<" "<<flush;
      }
   }
   cout<<"|"<<flush;
   cout<<endl;
}

void FactoryProductionLine::ShowWorkerStations(void)
{
   // Print items on the conveyor belt
   cout<<"Work station status:  ";
   for(auto& station : Stations)
   {
      if(station->pItem1 != nullptr)
      {
         char c = 'X';
         switch(station->pItem1->Name)
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
         cout<<"[ "<<c<<" "<<flush;
      }
      else
      {
         cout<<"[ "<<" "<<" "<<flush;
      }
      cout<<"]"<<flush;
      if(station->pItem2 != nullptr)
      {
         char c = 'X';
         switch(station->pItem2->Name)
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
         cout<<"[ "<<c<<" "<<flush;
      }
      else
      {
         cout<<"[ "<<" "<<" "<<flush;
      }
      cout<<"]---"<<flush;
   }
   cout<<endl;
}

// -----------------------------------------------------------------------------
/// \brief                 Main application thread and main function
/// \param                 Nothing
/// \return                Nothing
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
   cout << "\n\nRunning Simulation with " << argc 
        << " arguments: "; 
   for (int i = 0; i < argc; ++i) 
      cout << argv[i] << " ";
   cout<<"\n\n"<<endl;
   cout<<"Results:"<<endl;

   struct SimulationArgs args;
   args.arg1 = atoi(argv[1]);
   args.arg2 = atoi(argv[2]);
   args.arg3 = atoi(argv[3]);
   args.arg4 = atoi(argv[4]);


   // Create a real-time simulation thread using real-time extentions of Linux Kernel
   int result = pthread_create(&RTThread1, NULL, &RTThread1Function, (void*)&args);
   if(result != 0)
   {
      cout<<"Error: Thread creation failed!"<<endl;
   }
   struct sched_param SchedParam;
   SchedParam.sched_priority = 1;
   if(RTThread1)
   {
      pthread_setschedparam(RTThread1, SCHED_FIFO, &SchedParam);
   }
   if(result != 0)
   {
      cout<<"Error: Setting real-time scheduling poilicy failed!"<<endl;
   }

   // Wait for real-time thread to join for gracefully exiting the Simulation
   pthread_join(RTThread1, NULL);
   cout<<"\n\n"<<endl;
   
   return 0;
}
