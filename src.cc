/*
All licenses in this repository are copyrighted by their respective authors.

Everything else is released under CC0.

------------------------------------------------------------------------------

No Copyright

The person who associated a work with this deed has dedicated the work to the
public domain by waiving all of his or her rights to the work worldwide under
copyright law, including all related and neighboring rights,
to the extent allowed by law.

You can copy, modify, distribute and perform the work, even for commercial
purposes, all without asking permission. See Other Information below.

Other Information:

	* In no way are the patent or trademark rights of any person affected
	by CC0, nor are the rights that other persons may have in the work or in
	how the work is used, such as publicity or privacy rights.

	* Unless expressly stated otherwise, the person who associated a work with
	this deed makes no warranties about the work, and disclaims liability for
	all uses of the work, to the fullest extent permitted by applicable law.

	* When using or citing the work, you should not imply endorsement
	by the author or the affirmer.

http://creativecommons.org/publicdomain/zero/1.0/legalcode
*/


/*
 * src.cc
 *
 *  Created on: Apr 7, 2018
 *      Author: Prateek Shantharama, Akhilesh Thyagaturu, and Martin Reisslein 
 */

#include <vector>
#include <string.h>
#include <omnetpp.h>
#include "dataPkt_m.h"

using namespace omnetpp;

enum load {low, med, high};

class src : public cSimpleModule
{
protected:
public:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    src();
   ~src();
    double   randomVar, expLowPar;
    double   effectiveMeanCallRate;
    double   meanCallRate;
    int      numBlock;
    int      callLoad;
    double   computeCallDuration();
    double   meanCallDuration;
    long int count = 0;
    double   rv;
    bool     send_var;
    int      sharingMode ;
};

Define_Module(src);

src::src()
{
}

src::~src()
{
}

void src::initialize()
{
	// "par" denotes that variables are fetched from ini file
	meanCallDuration      = (double) par("meanCallDuration");  
    meanCallRate          = (double) par("meanCallRate");
    numBlock              = (int)    par("numBlock");
    callLoad              = (int)    par("callLoad");

   // Variable "sharingMode" denotes the resource sharing, where 0, 1 enables and disables the transmissions
   // rv controls
    sharingMode           = (int) par("sharingMode");    

    rv                    = 0;
    send_var              = false;

 if(getIndex()==0 || getIndex()==1 || getIndex()==2) 
   // Denotes an initiation packet is generated for each src index
   // Which results in traffic generated from all the three three traffic generators
    {
        dataPkt *msg = new dataPkt("Call_request");
        scheduleAt(simTime(), msg);
    }
}

void src::handleMessage(cMessage *msg)
{
    rv = uniform(0,1);

     switch(getIndex())
     {
         case 0:   	// "0" indicates the behavior of first traffic generator
         {
            if(sharingMode == 0)
            {
                send_var = true;
            }
            else if(sharingMode == 1)
            {
                send_var = true;
            }
            else    
            {
                    if(rv <= 1)           // For Zipf (0<rv<1, 0<rv<2/3, 0<rv<6/11)
                        send_var = true;
                    else
                        send_var = false;
            }
            break;
         }
         case 1:     // "1" indicates the behavior of first traffic generator
         {
             if(sharingMode == 0)
             {
                 send_var = false;
             }
             else if(sharingMode == 1)
             {
                 send_var = true;
             }
             else
             {
                 if(rv <= 1)              // For Zipf (None,    2/3<rv<1, 6/11<rv<9/11)
                     send_var = true;
                 else
                     send_var = false;
             }
             break;
         }
         case 2:     // "1" indicates the behavior of first traffic generator       
         {
             if(sharingMode == 0)
             {
                 send_var = false;
             }
             else if(sharingMode ==1)
             {
                 send_var = true;
             }
             else
             {
                 if(rv <= 1)     // Zipf (None,   None,     9/11<rv<1)
                     send_var = true;
                 else
                     send_var = false;
             }
             break;
         }
     }

    if(count++ < 10000000) // to put zero
    {
        if(msg->isSelfMessage())
        {
            dataPkt *ttmsg = check_and_cast<dataPkt *>(msg);
            if(send_var == true)
            {
                // Create, set fields and transmit
                dataPkt *msg2 = new dataPkt("DataTraffic");
                msg2->setTimestamp(simTime());
                msg2->setCallDuration(computeCallDuration());

                int rv = intuniform(low, high);

                // Assign the call load low, medium, and high
                msg2->setCall_load(rv);
                send(msg2,"src_to_fb$o");
            }
         // exp (1/lambda) of the Poisson traffic
         // Modify the below inter-packet timing to match the Zipf distribution 
         if(getIndex()==0)
              scheduleAt(simTime()+exponential(1/(3*0.545*meanCallRate)),ttmsg); // 3*0.545*, Z=2, 0.66  // Z=3 6/11
         else if(getIndex()==1)
             scheduleAt(simTime()+exponential(1/(3*0.272*meanCallRate)),ttmsg);  // 3*(0.272)*Z=2, 0.33  // (9-6)/11
         else if(getIndex()==2)
              scheduleAt(simTime()+exponential(1/(3*0.1818*meanCallRate)),ttmsg); //  3*(0.1818)*, 2/11
        }
        else
        {
            delete msg;
        }
    }
    else
    {
        delete msg;
    }

}

// Method to compute call duration. This function is seperated to extend the functionality.

double src::computeCallDuration()
{
    return exponential(meanCallDuration);
}
