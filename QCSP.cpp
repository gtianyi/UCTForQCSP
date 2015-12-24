#include "VesselContainer.h"
#include "VesselSlot.h"
#include "YardBlock.h"
#include "QC.h"
#include "UCT.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <stack> 
#include <set> 
#include <vector>
#include <time.h>
#include <ctime>
using namespace std;

int main(){
    //input parameters
    int slotNum = 3;
    int slotHeight=5;
    int blockNum=2;
    int QCNum=2;
    int groupNum=3;
    
    //generate problem
	vector<QC> QCList;   
    vector<VesselSlot> slotList;
    vector<YardBlock> blockList;
    vector<VesselContainer> blockcontainerList[blockNum];
    vector<VesselContainer> containerList;
  
    //generate slots
    int tempCntrId=0;
    vector<int> qcslot;
    for(int i=0;i<slotNum;i++){
		VesselSlot aslot;	
		aslot.slotID=i;	
		aslot.slotCntrNum=slotHeight;//if the slotCntrNum is different, change code here
		for(int j=0;j<slotHeight;j++){
			VesselContainer acontainer;
			acontainer.containerID = tempCntrId++;
			acontainer.groupID = (tempCntrId)%groupNum;
			//aslot.slot[j]=acontainer;
			aslot.slot.push_back(acontainer.containerID);
			blockcontainerList[j%blockNum].push_back(acontainer)	;//add to a block
			containerList.push_back(acontainer);		
		}
		slotList.push_back(aslot);
		//slotList[i]=aslot;
		//assign to QC
		if(i%(slotNum/QCNum+1)+1>=slotNum/QCNum+1||i+1==slotNum){			
			if(i+1==slotNum) qcslot.push_back(i);
			QC aQC;
			aQC.slotSet = qcslot;
			QCList.push_back(aQC);
			qcslot.clear();
			qcslot.push_back(i);
		}
		else{
			qcslot.push_back(i);
		}
		//QCList[i%QCNum].slotSet.push_back(i);//add slot to qc
	}
	
	//generate blocks
	for(int i=0;i<blockNum;i++){
		YardBlock ablock;
		ablock.blockID=i;
		for(int g=0;g<groupNum;g++){
			ablock.groupNum.push_back(0);
		}		
		for(int j=0;j<blockcontainerList[i].size();j++){
			ablock.groupNum[blockcontainerList[i][j].groupID]++;//calulate group container number;
		}
		blockList.push_back(ablock);
	}	
	
    
   //output setup
   cout<<"container number:"<<containerList.size()<<endl;
   cout<<"slot:"<<endl;   
   for(int i=0;i<slotNum;i++){
	   for(int j=0;j<slotList[i].slot.size();j++){
		   cout<< slotList[i].slot[j]<<"("<<containerList[slotList[i].slot[j]].groupID<<")"<<" ";
	   }
	   cout<<endl;
   }
   cout<<"block:"<<endl;   
   for(int i=0;i<blockNum;i++){
	   for(int j=0;j<blockList[i].groupNum.size();j++){
		   cout<<j<< "("<<blockList[i].groupNum[j]<<")"<<" ";
	   }
	   cout<<endl;
   }
   cout<<"QC:"<<endl;   
   for(int i=0;i<QCList.size();i++){
	   for(int j=0;j<QCList[i].slotSet.size();j++){
		   cout<<QCList[i].slotSet[j]<<" ";
	   }
	   cout<<endl;
   }
   cout<<"-----------------------------"<<endl; 
   //int popular = 10;
	//double avgconfilt[popular];	
	////int minconfilt[200];
   //for(int i=0;i<popular;i++){  
	   //UCTTree utree(QCList,slotList,blockList,groupNum,containerList); 
	   //utree.UCTSearch(i);
	   //double avgcft = 0.0;
	   //int mincft=10000;
	   //for(int k=1;k<50;k++){
		   //int res = utree.doSimulate();
		   //avgcft=(avgcft*double(k-1)+(double)res)/k;
		   ////if(mincft>res) mincft=res;
	   //}
	   //avgconfilt[i] = avgcft;
	   ////minconfilt[i] = mincft;
	   //cout<<"progress:"<<i<<endl;
   //}
   
   //ofstream myfile;
	//myfile.open ("result.csv");
	//for(int i=0;i<popular;i++){
		//myfile <<avgconfilt[i]<<endl;
		////myfile <<avgconfilt[i];
		////myfile << ",";
		////myfile <<minconfilt[i]<<endl;
	//}
	//myfile.close();
   
   //UCTTree utree(QCList,slotList,blockList,groupNum,containerList); 
   //utree.UCTSearch(700);
   //int res = utree.doSimulate();
   //cout<<"Total conflict:"<<res<<endl;
   //int UCTnode = utree.getNodeNum();
   //cout<<"Total Nodes:"<<UCTnode<<endl;
   
    //UCTTree utree(QCList,slotList,blockList,groupNum,containerList); 
   //utree.UCTSearch(100);
   //int UCTnode = utree.getNodeNum();
   //double avgcft = 0.0;
   //int mincft=10000;
   //for(int k=1;k<10;k++){
	   //int res = utree.doSimulate();
	   //avgcft=(avgcft*double(k-1)+(double)res)/k;
	   //if(mincft>res) mincft=res;
   //}
   //cout<<"AVG conflict:"<<avgcft<<endl;
   //cout<<"Total Nodes:"<<UCTnode<<endl;
    time_t start, finish; 
    start=time(NULL);
   UCTTree utree(QCList,slotList,blockList,groupNum,containerList); 
   utree.UCTSearch(100);
   int UCTnode = utree.getNodeNum();
   int res = utree.doSimulate();   
   finish=time(NULL);
   cout <<"computation time:"<<difftime(start,finish)<<endl;
   cout<<"Conflict Num:"<<res<<endl;
   cout<<"Total Nodes:"<<UCTnode<<endl;
   
   
   
    
}
