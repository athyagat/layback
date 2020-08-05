INTRODUCTION
==================
1. This repository provides the code base for a general discrete-event simulation framework for the LayBack architecture so as to support other researchers in conducting studies in the LayBack context. The case study code for the MEC based RAN function split is included in the public release as a demonstration model for a completed LayBack case study. This simulation framework has been implemented in OMNET++, www.omnetpp.org, version 5.0. The case study implements a function split fronthaul network within the context of the Layered Backhaul (LayBack) framework. The fundamental components include call generator, functional blocks, and SDN compute. src.cc implements the call generator, fb.cc implements the functional block, and SDN.cc implements the SDN based LayBack framework.

2. Second part of the code changes include the changes for (Moby) Docker push and pull for compression and decompression acceleration using a hardware accelerator.

PART 1: OMNET++ Code for LayBack
==================

1.1. SIMULATION SETUP
--------------------
This simulation setup is configured as follows:

1. Call generator (src.cc) generates the call type of a call, with a prescribed mean call generation rate (\lambda) according to a Poisson process.
2. Function block (fb.cc) forwards the request to the SDN orchestrator with the fb-id such that the computing and link resources can be assigned by the SDN orchestrator and tracked.
3. SDN block (SDN.cc) is the core of the simulation which implements most of the functions of LayBack framework, such as allocation, tracking, decrementing the resources once the call expires, and rejecting a call.
4. Sink block (sink.cc) collects all the necessary data and helps in the post processing of the results.
5. MATLAB script (matlab_parsing.m) extracts the data from the out/results folder of omnetpp workspace.

1.2 OPERATIONAL PROCEDURE
--------------------

The call generator originates three types of calls, with the following compute demands and bitrate requirements:

    call[low].compute_data[numBlock-1]      = 1;
    call[low].bit_rate_data[numBlock-1]     = 0.005;

    call[medium].compute_data[numBlock-1]   = 2;
    call[medium].bit_rate_data[numBlock-1]  = 0.030;

    call[high].compute_data[numBlock-1]     = 4;
    call[high].bit_rate_data[numBlock-1]    = 0.100;
Low, medium, and high loads are randomly chosen in a uniform fashion. The fb nodes id's are placed as below:

              SDN

            fb0   fb1  fb2   fb3 
    user -> fb4   fb5  fb6   fb7  -> Sink 
            fb8   fb9  fb10  fb11
For a 4-layer and 3-operator network, the compute and bitrate requirements are verified in the order, for with-sharing:

    int scheduleMap[MAX_NUM_SRC_VALUE][MAX_NUM_BLOCK_VALUE*MAX_NUM_SRC_VALUE] = 
        { {0, 1, 4, 8, 2, 5, 9, 3, 6, 10, 7, 11},
          {4, 5, 0, 8, 6, 1, 9, 7, 2, 10, 3, 11}, 
          {8, 9, 0, 4, 10, 1, 5, 11, 2, 6, 3, 7}
        }; 
effectively in a zig-zag pattern prioritizing the nodes closer to the user. for no-sharing:

    int scheduleMap[MAX_NUM_SRC_VALUE][MAX_NUM_BLOCK_VALUE*MAX_NUM_SRC_VALUE] =
        { {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3},  
          {4, 5, 0, 8, 6, 1, 9, 7, 2, 10, 3, 11}, 
          {8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11}  
        };
effectively on the same function chain.

1.3 EXECUTION
--------------------
1. Install the OMNET++ version 5.0 or greater,
2. Import the project from local folder, and change your workspace to LayBack folder
3. "Clean" the project
4. "Build" the project
5.  "Run" to run each experiment as defined in omnetpp.ini, change ini file to reflect your simulation scenario.
6.  The entire process can be automated with a omnetpp "opp runall gcc debug-results" command on command-prompt window.


PART 2: Service Migration Acceleration in LayBack
===========================

2.1 DOCKER CHANGES SUMMARY
--------------------
We modify following files for adopting the hardware accelerations for container migration Docker push and pull.

    Dockerfile
    Makefile
    cmd/dockerd/docker.go
    distribution/push.go
    distribution/push_v2.go
    layer/layer_store.go
    pkg/archive/archive.go
    
2.2 DOCKER CHANGES DETAILS
--------------------

1. Folder /moby-fork includes the actual changes over default moby Docker code base.
2. In default docker setup, GZIP and PIGZ is used for compression and decompression, respectively.
3. However, for the container image migration and instantiation compression and decompression is a compute intensive task, especially for a network function instantiation in the containerized RAN framework accross the LayBack infrastructure.
4. Service Function Chaining (SFC) and Network Function instantiation and migration can be accelerated.
5. More specifically, our changes will improve the overall container (network function) application instantiation, migration, and tear-down operations of the functions.
6. IO streamer is used to pass data between Docker and QZIP application, hence there involves memory copies, and context switching overheads.
7. File: changes_for_qat.patch includes the git_patch of changes that can be applied to the existing Docker code-base to enable the hardware acceleration.


2.3 Disclaimers for the Evaluation on Intel's Platforms
--------------------

Software and workloads used in performance tests may have been optimized for performance only on Intel microprocessors. Performance tests, such as SYSmark and MobileMark, are measured using specific computer systems, components, software, operations and functions. Any change to any of those factors may cause the results to vary. You should consult other information and performance tests to assist you in fully evaluating your contemplated purchases, including the performance of that product when combined with other products. For more information go to www.intel.com/benchmarks.
 

Performance results are based on testing as of July 15, 2020 (San Diego), disclosed in the system configuration and may not reflect all publicly available security updates.  See configuration disclosure for details. No product or component can be absolutely secure.

Intel technologies’ features and benefits depend on system configuration and may require enabled hardware, software or service activation. Performance varies depending on system configuration. Check with your system manufacturer or retailer or learn more at www.intel.com.

Optimization Notice: Intel's compilers may or may not optimize to the same degree for non-Intel microprocessors for optimizations that are not unique to Intel microprocessors. These optimizations include SSE2, SSE3, and SSSE3 instruction sets and other optimizations. Intel does not guarantee the availability, functionality, or effectiveness of any optimization on microprocessors not manufactured by Intel. Microprocessor-dependent optimizations in this product are intended for use with Intel microprocessors. Certain optimizations not specific to Intel microarchitecture are reserved for Intel microprocessors. Please refer to the applicable product User and Reference Guides for more information regarding the specific instruction sets covered by this notice.  Notice Revision #20110804.

<END>
