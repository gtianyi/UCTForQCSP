/** CS980 UCT for QCSP
 File: VesselSlot.h
 Name: Tianyi Gu
Date: 11/30/2015
*/ 

#include <vector> 
//#include "VesselContainer.h"
using namespace std;

class VesselSlot{
private:
   
public:
	int slotID;
	vector<int> slot;
	int slotCntrNum;
	
	 //defaultConstructor
     VesselSlot(){};
	
	//CopyConstructor
    VesselSlot (const VesselSlot &rhs){
		slotID=rhs.slotID;
		slot=rhs.slot;
		slotCntrNum=rhs.slotCntrNum;
	};
	
	
	
    // assignment operator
	VesselSlot& operator=(const VesselSlot &rhs){
		slotID=rhs.slotID;
		slot=rhs.slot;
		slotCntrNum=rhs.slotCntrNum;
		return *this;
	}
	
	//int slotID;
    //VesselContainer *slot; 
    //int slotsize;
    
    ////defaultConstructor
     //VesselSlot(){
		 //slotsize=0;
		 //slot= new VesselContainer[1];		 
	 //};
    
    ////initialConstructor
    //VesselSlot(int size){
		//slotsize=size;
		//slot= new VesselContainer[slotsize];
	//};
	
	////CopyConstructor
    //VesselSlot(VesselSlot rhs){
		//slotID=rhs.slotID;
		//slotsize=rhs.slotsize;
		//slot= new VesselContainer[slotsize];
		//for(int i=0;i<slotsize;i++){
			//slot[i]=rhs.slot[i];
		//}
	//};
	
	
	
    //// assignment operator
	//VesselSlot& operator=(const VesselSlot &rhs){
		//delete [] slot;
		//slotID=rhs.slotID;
		//slotsize=rhs.slotsize;
		//slot= new VesselContainer[slotsize];
		//for(int i=0;i<slotsize;i++){
			//slot[i]=rhs.slot[i];
		//}
		//return *this;
	//}
	
	////destructor
	//~VesselSlot(){
		//delete [] slot;
	//}
	
};
