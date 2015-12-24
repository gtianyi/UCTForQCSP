/** CS980 UCT for QCSP
 File: QC.h
 Name: Tianyi Gu
Date: 11/30/2015
*/ 

#include <vector> 
//#include "VesselSlot.h"
using namespace std;

class QC{
private:
   
public:
    int qcID;	
	vector<int> slotSet; 
    QC(){};
    
   //CopyConstructor
    QC (const QC &rhs){
		qcID=rhs.qcID;
		slotSet=rhs.slotSet;
	};
	
	
	
    // assignment operator
	QC& operator=(const QC &rhs){
		qcID=rhs.qcID;
		slotSet=rhs.slotSet;
		return *this;
	}
};
