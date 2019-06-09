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
 * sink.cc
 *
 *  Created on: Apr 7, 2018
 *      Author: Prateek Shantharama, Akhilesh Thyagaturu, and Martin Reisslein
 */
#include <vector>
#include <string.h>
#include <omnetpp.h>
#include "dataPkt_m.h"

using namespace omnetpp;

class sink : public cSimpleModule
{
protected:
public:
    void initialize();
    void handleMessage(cMessage *msg);
    void finish();
    double callDuration;
    sink();
   ~sink();
};

Define_Module(sink);

sink::sink()
{
}

sink::~sink()
{
}


void sink::initialize()
{
    callDuration = 0;
    WATCH(callDuration);
}

void sink::handleMessage(cMessage *msg)
{
    dataPkt *ttmsg = check_and_cast<dataPkt *>(msg);
    callDuration = ttmsg->getCallDuration();
//    EV<<"The call duration from exponential RV = "<<ttmsg->getCallDuration()<<"\n";
//    EV<<"The call load from uniform RV = "<<ttmsg->getCall_load()<<"\n";
    delete ttmsg;
}

void sink::finish()
{
}


