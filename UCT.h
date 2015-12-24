#ifndef HUFFTREE_H
#define HUFFTREE_H

#include <vector> 
#include <set> 
#include <string>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
#define VITALITY_COEFFICIENT 50
#define TERMINAL_REWARD 100.0
#define CONFLICT_PUNISH_INDEX 1

//class UCTTree;
template<class ActionNode>
class StateNode{
	public:
    // default constructor
    StateNode(vector<int> _slot, vector<vector<int> > _block,double _defaultreward)
        : slot(_slot), block(_block),ns(0),isteminal(false),reward(_defaultreward),actionNum(0),isTreeNode(false),parentAction(0){}  
    vector<int> slot;//loadedCntrNum of each slot
    vector<vector<int> > block;//remainCntrNum of each group in each block 
    ActionNode **actions;//legal actions
    int actionNum;
    int ns;//visitied count
    double reward;//immediate reward
    bool isteminal;
    bool isTreeNode;
    ActionNode *parentAction;
    //bool expandable;
    //vector<int> unvisitedactions;    
    
    void clear(){
		if(actionNum>0){
			for(int i=0;i<actionNum;i++){
				actions[i]->clear();
				delete actions[i];
			}
			delete [] actions;
		}
	};
};

class ActionNode{
	public:
    // default constructor
    ActionNode(StateNode<ActionNode> ** _states, double *_transitionProbility,vector<int> _qcaction,int _stateNum,StateNode<ActionNode> * _parentState)
        : states(_states),transitionProbility(_transitionProbility),qcaction(_qcaction),
        stateNum(_stateNum),qvalue(0.0),eqvalue(0.0),nsa(0),parentState(_parentState),expandable(true),notexpandStateNum(_stateNum){}
     vector<int> qcaction;//action of each QC, next slotNo
    StateNode<ActionNode> ** states;  // statenodes
    int stateNum;
    double *transitionProbility;//transitionfunction
    double qvalue;//Q
    double eqvalue;//Q^
    int nsa;//visited count
    StateNode<ActionNode> *parentState;
    bool expandable;
    int notexpandStateNum;
    //map<StateNode *, double> unexpandstates;   
    
    void clear(){
		if(stateNum>0){
			for(int i=0;i<stateNum;i++){			
				states[i]->clear();	
				delete states[i];			
			}
			delete [] states;
			delete [] transitionProbility;
		}
	};
};

class UCTTree{
   
private:
     StateNode<ActionNode> * _root; // root of the Huffman tree    
    vector<QC> QCList;   
    vector<VesselSlot> slotList;
    vector<YardBlock> blockList;
    vector<VesselContainer> containerList;
    int groupNum;
    int UCTTreeNodeNum;
    
    // generate action for state
    void GenerateAction( StateNode<ActionNode>* state){
		vector<vector<int> > legalactiongenerator;//all legal slot of each QC;
		//int legalactionNum;
		bool isteminalsign = true;
		for(int i=0;i<QCList.size();i++){
			vector<int> legalslot;
			for(int j=0;j<QCList[i].slotSet.size();j++){
				if(state->slot[QCList[i].slotSet[j]]<slotList[QCList[i].slotSet[j]].slotCntrNum){
					legalslot.push_back(QCList[i].slotSet[j]);
				}
			}
			if(legalslot.size()==0) legalslot.push_back(-1);//if there are not legal slot for this qc than sign as -1;
			else isteminalsign=false;//if there exist legal slot, means this is not a terminal state
			legalactiongenerator.push_back(legalslot);
			//legalactionNum*=legalslot.size();
		}
		if(isteminalsign)  {
			state->isteminal=true;
			state->reward= TERMINAL_REWARD;
			 return;
		}
		
		vector<vector<int> > legalactions;//for each action, give QC an slot
		vector<int> currentaction;
		GetAllcombinationBylegalsets(0,currentaction,legalactiongenerator,legalactions);
		
		//for(int i=0;i<legalactions.size();i++){
			//for(int j=0;j<legalactions[i].size();j++){
				//cout<<legalactions[i][j]<<" ";
			//}
			//cout<<endl;
		//}
		//cout<<"action combinition size:"<<legalactions.size()<<endl;
		
		//当一个action没有合法state，则纳入action队列
		vector<ActionNode *> sactions;
		for(int i=0;i<legalactions.size();i++){
			vector<vector<int> > legalstategenerator;//all legal block for each qcslot
			vector<int> nextstateslot = state->slot;
			
			vector<int> cntrgrouplist;//第一个qc是装哪个类型箱，第二个...(中间不动的qc跳过)
			for(int k=0;k<legalactions[i].size();k++){
				if(legalactions[i][k]!=-1){
					int slotID =  legalactions[i][k];
					int rowNum = state->slot[slotID];
					int cntrID = slotList[slotID].slot[rowNum];
					int cntrgroup = containerList[cntrID].groupID;
					cntrgrouplist.push_back(cntrgroup);
					vector<int> legalblock;
					for(int b=0;b<state->block.size();b++){						
						if(state->block[b][cntrgroup]>0){
							legalblock.push_back(b);
						}
					}
					legalstategenerator.push_back(legalblock);
					nextstateslot[legalactions[i][k]]++;
				}
			}
			
			vector<vector<int> > legalstate;// for each nextstate, give qcslot a block
			vector<int> currentstate;
			GetAllcombinationBylegalsets(0,currentstate,legalstategenerator,legalstate);
			
			
			//for(int x=0;x<legalstategenerator.size();x++){
				//for(int j=0;j<legalstategenerator[x].size();j++){
					//cout<<legalstategenerator[x][j]<<" ";
				//}
				//cout<<endl;
			//}
			//cout<<"legalstategenerator size:"<<legalstategenerator.size()<<endl;
			
			//for(int x=0;x<legalstate.size();x++){
				//for(int j=0;j<legalstate[x].size();j++){
					//cout<<legalstate[x][j]<<" ";
				//}
				//cout<<endl;
			//}
			//cout<<"state combinition size:"<<legalstate.size()<<endl;
			
			//当出现2个选到qc选到同一个block的同一个group上，并且该group仅剩一个箱子。则该state非法。
			 vector<StateNode<ActionNode> *> legalactionstates;		
			vector<int> conflictindex;
			for(int k=0;k<legalstate.size();k++){
				vector<vector<int> > nextstateblock = state->block;
				std::set<int> conflictblock;			
				bool isillegal=false;	
				int confindex=0;
				for(int b=0;b<legalstate[k].size();b++){					
					nextstateblock[legalstate[k][b]][cntrgrouplist[b]]--;
					if(nextstateblock[legalstate[k][b]][cntrgrouplist[b]]<0){
						isillegal=true; 						
					}
					if(!(conflictblock.insert(legalstate[k][b])).second) confindex++;
				}			
				if(!isillegal){
					StateNode<ActionNode> * astate= new StateNode<ActionNode>(nextstateslot,nextstateblock,(double)-confindex*CONFLICT_PUNISH_INDEX);
					legalactionstates.push_back(astate);
					conflictindex.push_back(confindex);
				}	
			}			
			
			int legalstatesize = legalactionstates.size();
			if(legalstatesize>0){
				//cout<<"legalstatesize"<<legalstatesize<<endl;
				StateNode<ActionNode> **  actionstates= new StateNode<ActionNode>*[legalstatesize];
				for(int k=0;k<legalstatesize;k++){
					actionstates[k] = legalactionstates[k];
				}
				
				int minconflict=10000;
				int highestrankstate;
				double *transitionProbility = new double[legalstatesize];
				int minindex=-1;
				for(int k=0;k<legalstatesize;k++){
					if(minconflict>conflictindex[k]){
						minconflict=conflictindex[k];
						transitionProbility[k]=0.9;
						if(minindex!=-1) transitionProbility[minindex] = 0.1/(double)(legalstatesize-1);
						minindex = k;
					}
					else{
						transitionProbility[k]=0.1/(double)(legalstatesize-1);
					}
				}
				if(legalstatesize==1)  transitionProbility[0] =1.0;
				ActionNode * aaction = new ActionNode(actionstates, transitionProbility ,legalactions[i],legalstatesize,state);
				sactions.push_back(aaction);
			}
		}
		ActionNode ** sactions_array = new ActionNode*[sactions.size()];
		for(int k=0;k<sactions.size();k++){
				sactions_array[k] = sactions[k];
			}
		state->actions = sactions_array;
		state->actionNum = sactions.size();
	}
	
	//get all combination of legal sets//组合完成的顺序需要确保第一个slot是第一个qc被选slot中的一个的
	void GetAllcombinationBylegalsets(int currentqc, vector<int> &currentaction,
	vector<vector<int> > &legalactiongenerator, vector<vector<int> > & legalactions){
		if(currentqc>=legalactiongenerator.size()){
			 vector<int> temp = currentaction;
			legalactions.push_back(temp);			
			return;
		}		
		for(int i=0;i<legalactiongenerator[currentqc].size();i++){
			currentaction.push_back(legalactiongenerator[currentqc][i]);
			GetAllcombinationBylegalsets(currentqc+1,currentaction,legalactiongenerator,legalactions);
			currentaction.pop_back();
		}
	};
    	
	//TreePolicy Function
	StateNode<ActionNode> * TreePolicy(StateNode<ActionNode> * _state){
		while(_state->isTreeNode){
			if(_state->isteminal){
				return _state;
			}
			ActionNode* a = BestAction(_state);
			//if(a->expandable){
				//for(int i=0;i<a->stateNum;i++){
					//if(!a->states[i]->isTreeNode){
						//_state=a->states[i];
						//break;
					//}
				//}
				//a->notexpandStateNum--;
				//if(a->notexpandStateNum==0) a->expandable = false;					
			//}
			//else{				
				//_state =SampleStateByTran(a);				
			//}
			int s = rand()%a->stateNum;
			_state = a->states[s];	
		}
		_state->isTreeNode = true;
		UCTTreeNodeNum++;
		return _state;
	}
	
	//BestChild Function
	ActionNode *BestAction(StateNode<ActionNode> * _state){
		ActionNode *best;
		double maxUCBValue=-10000;
		double c=VITALITY_COEFFICIENT;
		for(int i=0;i<_state->actionNum;i++){
			double tempUCBValue;
			if(_state->actions[i]->nsa!=0)
				tempUCBValue=_state->actions[i]->eqvalue+
			sqrtl(logl(_state->ns)/_state->actions[i]->nsa)*c;
			else
				tempUCBValue = 99999;
			if(tempUCBValue>maxUCBValue||(tempUCBValue==maxUCBValue&&rand()%2==0)){
				maxUCBValue=tempUCBValue;
				best=_state->actions[i];
			}
		}
		return best;
	}
	
	//SimulateAction Function
	double SimulateAction(ActionNode *_action){		
		StateNode<ActionNode> * nextstate = SampleStateByTran(_action);
		//cout<<"STATE:"<<endl<<"slot:";		
		//for(int i=0;i<nextstate->slot.size();i++){
			//cout<<nextstate->slot[i]<<" ";
		//}
		//cout<<endl;
		//for(int i=0;i<nextstate->block.size();i++){
			//cout<<"block"<<i<<":";
			//for(int j=0;j<nextstate->block[i].size();j++){
				//cout<<nextstate->block[i][j]<<" ";
			//}
			//cout<<endl;
		//}		
		if(nextstate->actionNum==0) GenerateAction(nextstate);
		if(nextstate->actionNum==0||nextstate->isteminal){
			//cout<<"terminal"<<endl;
			return nextstate->reward;			
		}
		else{			
			int a = rand()%nextstate->actionNum;
			ActionNode *nextaction = nextstate->actions[a];		
			
			//cout<<"ACTION:"<<endl;	
			//for(int i=0;i<nextaction->qcaction.size();i++){
				//cout<<nextaction->qcaction[i]<<" ";
			//}
			//cout<<endl;
			//cout<<"----------------------------";
			//cout<<endl;
			return nextstate->reward+SimulateAction(nextaction);
		}
	}
	
	double SimulateState(StateNode<ActionNode> *_state){
		//cout<<"initialSTATE:"<<endl<<"slot:";		
		//for(int i=0;i<_root->slot.size();i++){
			//cout<<_root->slot[i]<<" ";
		//}
		//cout<<endl;
		//for(int i=0;i<_root->block.size();i++){
			//cout<<"block"<<i<<":";
			//for(int j=0;j<_root->block[i].size();j++){
				//cout<<_root->block[i][j]<<" ";
			//}
			//cout<<endl;
		//}		
		//cout<<"initialACTION:"<<endl;	
			//for(int i=0;i<_root->actions[0]->qcaction.size();i++){
				//cout<<_root->actions[0]->qcaction[i]<<" ";
			//}
			//cout<<endl;
			//cout<<"----------------------------";
			//cout<<endl;
		if(_state->actionNum==0) GenerateAction(_state);
		if(_state->actionNum==0) return _state->reward;			
		int a = rand()%_state->actionNum;
			ActionNode *nextaction = _state->actions[a];	
		return SimulateAction(nextaction);
	}
	
	StateNode<ActionNode>* SampleStateByTran(ActionNode *_action){
		double curdie=(double)(rand()%100)/100.0;
		StateNode<ActionNode> * nextrandstate = _action->states[0];
		StateNode<ActionNode> * nextbeststate = _action->states[0];
		StateNode<ActionNode> * nextstate;
		
		for(int i=0;i<_action->stateNum;i++){
			if(_action->transitionProbility[i]>0.8){
				nextbeststate = _action->states[i];				
			}
			else if(rand()%2==0){
				nextrandstate = _action->states[i];
			}
		}
		if(curdie<=0.2){
			nextstate =  nextrandstate;
		}
		else{			
			nextstate =  nextbeststate;
		}
		nextstate->parentAction = _action;
		return nextstate;
	}
	
	void BackUp(StateNode<ActionNode>* state, double q){
		while(state->parentAction){
			ActionNode* a = state->parentAction;
			state = a->parentState;
			
			a->eqvalue = (a->nsa*(a->eqvalue)+q)/ (a->nsa+1);
			q = q+state->reward;
			state->ns = state->ns+1;
			a->nsa = a->nsa+1;
		}
	}
	
	ActionNode *SimulaterBestAction(StateNode<ActionNode> * _state){
	
		ActionNode *best;
		if(_state->isTreeNode){
			double maxUCBValue=-10000;		
			for(int i=0;i<_state->actionNum;i++){
				double tempUCBValue = _state->actions[i]->eqvalue;		
				if(tempUCBValue>maxUCBValue){
					maxUCBValue=tempUCBValue;
					best=_state->actions[i];
				}
				//cout<<"aQvaule:"<<_state->actions[i]->eqvalue<<endl;
			}
			//cout<<"BEST Qvaule:"<<maxUCBValue<<endl;
		}
		else{
				if(_state->actionNum==0) GenerateAction(_state);
				if(_state->isteminal) return 0;
				int a = rand()%_state->actionNum;
				best = _state->actions[a];	
		}
		return best;
	}
		
public:    
      //UCTSearch Function
     void UCTSearch(int maxloop){
		while(maxloop>0){
			maxloop--;
			StateNode<ActionNode> * s = TreePolicy(_root);
			double q = SimulateState(s);
			BackUp(s,q);
		}	
	}
	
	int doSimulate(){
		ofstream myfile;
		myfile.open ("resultdet");		
		int r = SystemSimulator(_root,myfile);
		myfile.close();
		return  r;
		
	}
	//SystemSimulator
	//return TotalConflictNum
	int SystemSimulator(StateNode<ActionNode> *state,ofstream &myfile){
		
		myfile<<"STATE:"<<endl<<"slot:";		
		//cout<<"STATE:"<<endl<<"slot:";		
		for(int i=0;i<state->slot.size();i++){
			//cout<<state->slot[i]<<" ";
			myfile<<state->slot[i]<<" ";
		}
		//cout<<endl;
		myfile<<endl;
		for(int i=0;i<state->block.size();i++){
			//cout<<"block"<<i<<":";
			myfile<<"block"<<i<<":";
			for(int j=0;j<state->block[i].size();j++){
				//cout<<state->block[i][j]<<" ";
				myfile<<state->block[i][j]<<" ";
			}
			//cout<<endl;
			myfile<<endl;
		}		
		
		if(state->isteminal){
		//cout<<"terminal"<<endl;
		myfile<<"terminal"<<endl;
		return 0;			
		}
		else{			
			ActionNode *a = SimulaterBestAction(state);			
			if(!a){
			//cout<<"terminal"<<endl;
			myfile<<"terminal"<<endl;
			return 0;			
			}
							
			//cout<<"ACTION:"<<endl;
			myfile<<"ACTION:"<<endl;
			for(int i=0;i<a->qcaction.size();i++){
				//cout<<a->qcaction[i]<<" ";
				myfile<<a->qcaction[i]<<" ";
			}
			//cout<<endl;
			//cout<<"----------------------------";
			//cout<<endl;			
			myfile<<endl;
			myfile<<"----------------------------";
			myfile<<endl;					
			StateNode<ActionNode> *nextstate;
			nextstate = SampleStateByTran(a);
			//int s = rand()%a->stateNum;
			//if(state->isTreeNode) 
				//nextstate = SampleStateByTran(a);
			//else{
				//int s = rand()%a->stateNum;
				//nextstate = a->states[s];
			//}
			int conflictNum = 0;
			for(int b=0;b<blockList.size();b++){
				int prevBlockCntrNum=0;
				int nextBlockCntrNum=0;
				for(int g=0;g<groupNum;g++){
					prevBlockCntrNum+=state->block[b][g];
					nextBlockCntrNum+=nextstate->block[b][g];
				}
				if(prevBlockCntrNum > nextBlockCntrNum){
					conflictNum += prevBlockCntrNum - nextBlockCntrNum -1;
				}
			}					
			return conflictNum+SystemSimulator(nextstate,myfile);
		}
		
	}
	
    // constructor 
    UCTTree(vector<QC> _QCList, vector<VesselSlot> _slotList,vector<YardBlock> _blockList,int _groupNum, vector<VesselContainer> _containerList){
		QCList = _QCList;		
		slotList= _slotList;
		blockList=_blockList;
		containerList =_containerList;		
		groupNum = _groupNum;
		UCTTreeNodeNum = 0;
		vector<int> stateslot;
		 vector<vector<int> > stateblock;
		 for(int i=0;i<slotList.size();i++){
			 stateslot.push_back(0);
		 }
		  for(int i=0;i<blockList.size();i++){
			  vector<int> groupCntrNum = blockList[i].groupNum;	
			  stateblock.push_back(groupCntrNum);
		 }
		_root = new StateNode<ActionNode>(stateslot,stateblock,0);
		//GenerateAction(_root);		
	};

	//getNodeNUm
	int getNodeNum(){
		return UCTTreeNodeNum;
	}
	// destructor
    ~UCTTree(){
		_root->clear();
		delete _root;
	};
};
	
#endif
