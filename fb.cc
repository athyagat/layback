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
 * fb.cc
 *
 *  Created on: Apr 7, 2018
 *      Author: Prateek Shantharama, Akhilesh Thyagaturu, and Martin Reisslein
 */


/* The various loads indicates the compute load levels at low, medium and high
 * load 1 => low     =>   5
 * load 2 => medium  =>  10
 * load 3 => high    =>  30
 */


#include <vector>
#include <string.h>
#include <omnetpp.h>
#include "dataPkt_m.h"

using namespace omnetpp;

typedef enum {low, med, high}         load;
typedef enum {SUCCESS, FAILURE}       feedback;
typedef enum {fb_sink, fb_fb}         dimension;
typedef enum {dequeue, call_duration_expiry}   selfMsg;

class fb : public cSimpleModule
{
protected:
public:
    virtual void  initialize();
    virtual void  handleMessage(cMessage *msg);
    void          finish();
                  fb(); // constructor
                 ~fb(); // destructor
    int           numBlock;
};

Define_Module(fb);

fb::fb(){
//This is the constructor
// We can define any initialization variables here
}

fb::~fb(){
//This is the destructor
}

void fb::initialize()
{

    numBlock = par("numBlock");
    if(!(getIndex()%numBlock))
    {
        EV<<"\n Index is"<<getIndex()<<endl; // for debugging
    }
}

void fb::handleMessage(cMessage *msg)
{
    dataPkt *ttmsg = check_and_cast<dataPkt *>(msg);

    if(ttmsg->isSelfMessage())
    {
        delete ttmsg;
    }

    else if(ttmsg->getArrivalGateId() == 0) // 8 for SDN
    {
        send(ttmsg, "fb_to_SDN$o");
    }
    else
    {
        delete ttmsg;
    }
}

void fb::finish()
{
}
