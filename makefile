CXX=g++

all: UCT

UCT: VesselSlot.h VesselContainer.h YardBlock.h QC.h  UCT.h QCSP.cpp
	$(CXX)  QCSP.cpp -o QCSP

clean:
	rm -f *.o  UCT
