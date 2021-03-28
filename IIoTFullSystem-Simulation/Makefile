## -----------------------------------------------------------------------------
### \file						Makefile
### \brief						Makefile to build simualation in using g++ (Ubuntu
###								5.4.0-6ubuntu1~16.04.12) 5.4.0 20160609
### \author						Sreejith.Naarakathil@gmail.com
### \date						27/3/2021
## ----------------------------------------------------------------------------
CC=g++
CFLAGS=--std=c++14 -g3 -I. -lpthread -lrt
DEPS = IIoTFullSystemModel.h CalculateKPIs.h MsgBatch.h FieldBus.h IIoTFullSystemModel.h Item.h KPIReport.h SimulationDataTypes.h FieldBusDevice.h
OBJ = IIoTFullSystemModel.o CalculateKPIs.o MsgBatch.o FieldBus.o KPIReport.o FieldBusDevice.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

RunSimulation: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ core a.out RunSimulation
