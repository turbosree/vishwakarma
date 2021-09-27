## -----------------------------------------------------------------------------
### \file						Makefile
### \brief						Makefile to build simualation in using g++ (Ubuntu
###								5.4.0-6ubuntu1~16.04.12) 5.4.0 20160609
### \author						Sreejith.Naarakathil@gmail.com
### \date						23/1/2021
## ----------------------------------------------------------------------------
CC=g++
CFLAGS=--std=c++14 -g3 -I. -lpthread -lrt
DEPS = FactoryProductionLine.h CalculateKPIs.h ComponentBatch.h ConveyorBeltSlot.h FactoryProductionLine.h Item.h KPIReport.h SimulationDataTypes.h WorkerStation.h
OBJ = FactoryProductionLine.o CalculateKPIs.o ComponentBatch.o ConveyorBeltSlot.o KPIReport.o WorkerStation.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

RunSimulation: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ core a.out RunSimulation
