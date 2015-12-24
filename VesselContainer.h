/** CS980 UCT for QCSP
 File: VesselContainer.h
 Name: Tianyi Gu
Date: 11/30/2015
*/ 
using namespace std;

class VesselContainer{
private:
    //std::stack<VesselContainer> slot; 
public:
    int containerID;
	int groupID;	
    VesselContainer(){};
    //CopyConstructor
    VesselContainer (const VesselContainer &rhs){
				containerID=rhs.containerID;
				groupID=rhs.groupID;
	};
    
    // assignment operator
	VesselContainer& operator=(const VesselContainer &rhs){
		containerID=rhs.containerID;
		groupID=rhs.groupID;
		return *this;
	}
};
