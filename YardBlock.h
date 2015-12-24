/** CS980 UCT for QCSP
 File: YardBlock.h
 Name: Tianyi Gu
Date: 11/30/2015
*/ 

#include <stack> 
//#include "VesselContainer.h"
using namespace std;

class YardBlock{
private:
   
public:
	int blockID;
	vector<int> groupNum;
	
	 //defaultConstructor
     YardBlock(){};
	
	//CopyConstructor
    YardBlock (const YardBlock &rhs){
			blockID=rhs.blockID;
			groupNum=rhs.groupNum;
	};
	
	YardBlock& operator=(const YardBlock &rhs){
		blockID=rhs.blockID;
		groupNum=rhs.groupNum;
		return *this;
	}
	
    //int BlockID;	
    //int groupNum;
	//int *groupcontainerNum; 
	
    ////defaultConstructor
     //YardBlock(){
		 //BlockID=0;
		 //groupcontainerNum= new int[1];		 
	 //};
    
    ////initialConstructor
    //YardBlock(int size){
		//groupNum=size;
		//groupcontainerNum= new int[groupNum];
	//};
	
    //// assignment operator
	//YardBlock& operator=(const YardBlock &rhs){
		//delete [] groupcontainerNum;
		//groupNum=rhs.groupNum;
		//BlockID=rhs.BlockID;
		//groupcontainerNum= new int[groupNum];
		//for(int i=0;i<groupNum;i++){
			//groupcontainerNum[i]=rhs.groupcontainerNum[i];
		//}
		//return *this;
	//}
	
	////destructor
	//~YardBlock(){
		//delete [] groupcontainerNum;
	//}
};
