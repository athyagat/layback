INTRODUCTION
===========================
This repository provides the code base for a general discrete-event simulation framework for the LayBack architecture so as to support other researchers in conducting studies in the LayBack context. The case study code for the MEC based RAN function split is included in the public release as a demonstration model for a completed LayBack case study.
This simulation framework has been implemented in OMNET++, www.omnetpp.org, version 5.0. The case study implements a function split fronthaul network within the context of the Layered Backhaul (LayBack) framework. The fundamental components include call generator, functional blocks, and SDN compute. src.cc implements the call generator, fb.cc implements the functional block, and SDN.cc implements the SDN based LayBack framework.

SIMULATION SETUP
===========================
This simulation setup is configured as follows:
1.	Call generator (src.cc) generates the call type of a call, with a prescribed mean call generation rate (\lambda) according to a Poisson process.
2.	Function block (fb.cc) forwards the request to the SDN orchestrator with the fb-id such that the computing and link resources can be assigned by the SDN orchestrator and tracked.
3.	SDN block (SDN.cc) is the core of the simulation which implements most of the functions of LayBack framework, such as allocation, tracking, decrementing the resources once the call expires, and rejecting a call.
4.	sink.cc collects all the necessary data and helps in the post processing of the results.
5.	MATLAB script (matlab_parsing.m) extracts the data from the out/results folder of omnetpp workspace.

OPERATIONAL PROCEDURE
===========================
The call generator originates three types of calls, with the following compute demands and bitrate requirements:

	call[low].compute_data[numBlock-1]      = 1;
	call[low].bit_rate_data[numBlock-1]     = 0.005;

	call[medium].compute_data[numBlock-1]   = 2;
	call[medium].bit_rate_data[numBlock-1]  = 0.030;

	call[high].compute_data[numBlock-1]     = 4;
	call[high].bit_rate_data[numBlock-1]    = 0.100;

Low, medium, and high loads are randomly chosen in a uniform fashion.
The fb nodes id's are placed as below:

              SDN

            fb0   fb1  fb2   fb3 
	user -> fb4   fb5  fb6   fb7  -> Sink 
            fb8   fb9  fb10  fb11
		
For a 4-layer and 3-operator network, the compute and bitrate requirements are verified in the order,
for with-sharing:

	int scheduleMap[MAX_NUM_SRC_VALUE][MAX_NUM_BLOCK_VALUE*MAX_NUM_SRC_VALUE] = 
			{ {0, 1, 4, 8, 2, 5, 9, 3, 6, 10, 7, 11},
			  {4, 5, 0, 8, 6, 1, 9, 7, 2, 10, 3, 11}, 
			  {8, 9, 0, 4, 10, 1, 5, 11, 2, 6, 3, 7}
			}; 

effectively in a zig-zag pattern prioritizing the nodes closer to the user.
	for no-sharing:
	
	int scheduleMap[MAX_NUM_SRC_VALUE][MAX_NUM_BLOCK_VALUE*MAX_NUM_SRC_VALUE] =
		    { {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3},  
			  {4, 5, 0, 8, 6, 1, 9, 7, 2, 10, 3, 11}, 
			  {8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11}  
		    };
effectively on the same function chain.

EXECUTION
===========================
1.	Install the OMNET++ version 5.0 or greater,
2.	Import the project from local folder, and change your workspace to LayBack folder
3.	"Clean" the project
4.	"Build" the project
5.	"Run" to run each experiment as defined in omnetpp.ini, change ini file to reflect your simulation scenario.
6.	The entire process can be automated with a omnetpp "opp runall gcc debug-results" command on command-prompt window.

