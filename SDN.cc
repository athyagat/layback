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
 * SDN.cc
 *
 *  Created on: Apr 11, 2018
 *      Author: Prateek Shantharama, Akhilesh Thyagaturu, and Martin Reisslein
 */

#include <vector>
#include <string.h>
#include <omnetpp.h>
#include "dataPkt_m.h"

using namespace omnetpp;

#define MAX_NUM_BLOCK_VALUE 5
#define MAX_NUM_SRC_VALUE 5
#define MAX(a,b) a>b?a:b

typedef enum {low, medium, high} call_type;

typedef struct {
   double max_compute;
   double current_compute;
   double act_current_compute;
} fb_data;


typedef struct bit_rate_data {
    double max_bitrate_inter[MAX_NUM_BLOCK_VALUE];
    double max_bitrate_intra[MAX_NUM_BLOCK_VALUE];
    double current_inter[MAX_NUM_SRC_VALUE];
    double current_intra[MAX_NUM_SRC_VALUE];
    double act_current_inter[MAX_NUM_SRC_VALUE];
    double act_current_intra[MAX_NUM_SRC_VALUE];
}bit_rate_data;


typedef struct {
   call_type   call_type_data;
   double      compute_data[MAX_NUM_BLOCK_VALUE];
   double      bit_rate_data[MAX_NUM_BLOCK_VALUE];
   double      sum_compute;
   double      sum_bitrate;
} call_data;

class SDN : public cSimpleModule
{
protected:
public:
    void      initialize();
    void      handleMessage(cMessage *msg);
    void      finish();
              SDN();
             ~SDN();
    void      computeMaxValues(int* var, int size);
    void      drop_call(dataPkt *ttmsg);
    int       inc_bitrate(int first_index, int second_index, int beta_index, int call_index, int src_index);

    void      assign_actual_compute(double call_duration);
    void      revert_actual_compute();

    void      assign_actual_bitrate(double call_duration);
    void      revert_actual_bitrate();

    void      compute_expiry(dataPkt *ttmsg);
    void      bitrate_expiry(dataPkt *ttmsg);
    void      save_utilization();

    int            numBlock;
    int            numSrc;
    int            sharingMode;

    long double    drop_count;

    long double    drop_count_low;
    long double    drop_count_med;
    long double    drop_count_high;

    long double    drop_bit_rate;

    long double    num_calls;

    long double    num_calls_low;
    long double    num_calls_med;
    long double    num_calls_high;

    cStdDev        intra_utilization[4];
    cStdDev        inter_utilization[3];
    cStdDev        l_utilization[4];

    fb_data          fb_context[MAX_NUM_BLOCK_VALUE*MAX_NUM_SRC_VALUE];
    bit_rate_data    bitrate_context;

    int scheduleMap[MAX_NUM_SRC_VALUE][MAX_NUM_BLOCK_VALUE*MAX_NUM_SRC_VALUE] =
                  { {0, 1, 4, 8, 2, 5, 9, 3, 6, 10, 7, 11},  // For traffic from fb_0
                    {4, 5, 0, 8, 6, 1, 9, 7, 2, 10, 3, 11},  // For traffic from fb_4
                    {8, 9, 0, 4, 10, 1, 5, 11, 2, 6, 3, 7}   // For traffic from fb_8
                  };
/*
    int scheduleMap[MAX_NUM_SRC_VALUE][MAX_NUM_BLOCK_VALUE*MAX_NUM_SRC_VALUE] =
                  { {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3},  // For traffic from fb_0
                    {4, 5, 0, 8, 6, 1, 9, 7, 2, 10, 3, 11},  // For traffic from fb_4
                    {8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11}   // For traffic from fb_8
                  };
*/
    call_data        call[4];

    int call_split_data[4] = {-1, -1, -1, -1};
};

Define_Module(SDN);

SDN::SDN()
{
}

SDN::~SDN()
{
}


void SDN::initialize()
{
       numSrc          = (int) par("numSrc");
       numBlock        = (int) par("numBlock");
       sharingMode    = (int) par("sharingMode");

       drop_count      = 0;
       drop_count_low  = 0;
       drop_count_med  = 0;
       drop_count_high = 0;

       drop_bit_rate   = 0;

       num_calls      =  0;
       num_calls_low  =  0;
       num_calls_med  =  0;
       num_calls_high =  0;

       WATCH(drop_count);
       WATCH(num_calls);

       bitrate_context = {0};

	   // static allocation of bit rates

       call[low].compute_data[numBlock-1]      = 1;
       call[low].bit_rate_data[numBlock-1]     = 0.005;
       call[low].sum_bitrate                   = 0;
       call[low].sum_compute                   = 0;

       call[medium].compute_data[numBlock-1]   = 2; // 7.5s;
       call[medium].bit_rate_data[numBlock-1]  = 0.030;
       call[low].sum_bitrate                   = 0;
       call[low].sum_compute                   = 0;

       call[high].compute_data[numBlock-1]     = 4;
       call[high].bit_rate_data[numBlock-1]    = 0.100;
       call[low].sum_bitrate                   = 0;
       call[low].sum_compute                   = 0;


       for(int i = 0; i<4; i++)
       {
          // for debugging
		   WATCH(call_split_data[i]);
       }


       // Data rate: low, medium, and high call
       /*  j=0 means low call
        *  j=1 means med call
        *  j=2 means high call
        */

       for (int j=0; j<3; j++)
       {
           for (int i=numBlock-1; i>=0; i--)
           {
               call[j].bit_rate_data[i-1]     =  call[j].bit_rate_data[i]*3;
               call[j].compute_data[i-1]      =  call[j].compute_data[i]*2; // 7.5
           }

           for (int i=0; i<numBlock; i++)
           {
               call[j].sum_bitrate            =  call[j].sum_bitrate + call[j].bit_rate_data[i];
           }

           WATCH(call[j].sum_bitrate);
       }

       // Load: calculate low, medium, and high call
       for (int j=0; j<3; j++)
       {

           for (int i=0; i<numBlock; i++)
           {
               call[j].sum_compute             =  call[j].sum_compute + call[j].compute_data[i];
           }

           WATCH(call[j].sum_compute);
       }

       // Assign max computing data
       for (int i=0; i<numBlock*numSrc; i++)
       {


// change here

     int assign = 0;

     if(sharingMode == 0)
         assign = 4;
     else
         assign = numSrc*numBlock;

     if(i<assign)    // Change this to if(i<4) for no sharing, and if(numBlock*numSrc) for sharing
     {

         fb_context[i].max_compute         = 200;
         fb_context[i].current_compute     = 0;
         fb_context[i].act_current_compute = 0;

     }
     else
     {
         fb_context[i].max_compute         = 0;
         fb_context[i].current_compute     = 0;
         fb_context[i].act_current_compute = 0;
     }
           WATCH(fb_context[i].current_compute);     // for debugging
           WATCH(fb_context[i].act_current_compute);
       //  WATCH(fb_context[i].act_current_compute);
           EV<<"Maxcompute value [i="<<i<<"]"<<fb_context[i].max_compute<<"\n";
       }

       EV<<endl;

       // Assign max bitrate data
       for (int i=0; i <= (numBlock-1); i++)
       {
           bitrate_context.max_bitrate_intra[i] = 1000; // In Gbps

           WATCH(bitrate_context.current_intra[i]);
       //  WATCH(bitrate_context.act_current_intra[i]);
           WATCH(bitrate_context.max_bitrate_intra[i]);
           EV<<"Intra bitrate: "<<bitrate_context.max_bitrate_intra[i]<<endl;

       }

       EV<<endl;

       for (int i=0; i < (numBlock-1); i++)
       {
           bitrate_context.max_bitrate_inter[i] = 1000; // In Gbps
           WATCH(bitrate_context.current_inter[i]);
       //  WATCH(bitrate_context.act_current_inter[i]);
           WATCH(bitrate_context.max_bitrate_intra[i]);
           EV<<"Inter bitrate: "<<bitrate_context.max_bitrate_inter[i]<<endl;
       }

       EV<<endl;

      // Debug print
       for (int j=0; j<3; j++)
       {
           for (int i=0; i<numBlock; i++)
           {
               EV<<"call load ["<<i+1<<"]: "<<call[j].compute_data[i] <<"  Bit Rate ["<<i+1<<"]: "<<call[j].bit_rate_data[i]<<endl;
           }
           EV<<endl;
       }
}


//Routines to handle the messages
void SDN::handleMessage(cMessage *msg)
{
     dataPkt *ttmsg = check_and_cast<dataPkt *>(msg);

     if(msg->isSelfMessage())
     {
        if(ttmsg->getSelfMsgType()==1)
        {
            EV<<"In compute expiry"<<endl;
            compute_expiry(ttmsg);
        }
        else if(ttmsg->getSelfMsgType()==2)
        {
            EV<<"In bitrate expiry"<<endl;
            bitrate_expiry(ttmsg);
        }
        else{
            while(1)
            {
            EV<<"I am in a loop "<<endl;
            }
        }
     }
     else
     {
        int    previous_index       =  0;
        bool   compute_passed       = false;
        bool   bitrate_passed       = false;

        num_calls++;

        if(ttmsg->getCall_load()==0)
        {
            num_calls_low++;
        }
        else if(ttmsg->getCall_load()==1)
        {
            num_calls_med++;
        }
        else if(ttmsg->getCall_load()==2)
        {
            num_calls_high++;
        }

        /*
         * If the call is coming in the mode of
         * */


        /* Call split data
         * -1 corresponds to no computing installed*/
        for(int i = 0; i<4; i++)
        {
           call_split_data[i]=-1;
        }


        // begin the computing search

        int    fb_count   =  0;
        int    map_index  =  (ttmsg->getSenderModule()->getIndex())/numBlock;

        double temp_current_compute    =  0;
        int    beta                    =  0;

        while(fb_count < (numSrc*numBlock))
        {
                  temp_current_compute  =  fb_context[scheduleMap[map_index][fb_count]].current_compute;

                  if(temp_current_compute + call[ttmsg->getCall_load()].compute_data[beta] <= fb_context[scheduleMap[map_index][fb_count]].max_compute)
                  {
                      fb_context[scheduleMap[map_index][fb_count]].current_compute = call[ttmsg->getCall_load()].compute_data[beta] + temp_current_compute;
                      EV<<"beta "<<beta<<" is accommodated in fb["<<scheduleMap[map_index][fb_count]<<"]\n";
                      EV<<"needed "<<call[ttmsg->getCall_load()].compute_data[beta]<<" was available in fb["<<scheduleMap[map_index][fb_count]<<"]\n";

                      call_split_data[beta] = scheduleMap[map_index][fb_count];
                      beta++;

                      if(beta >= numBlock)
                      {
                          beta=0;
                          break;
                      }
                  }
                  else
                  {
                      fb_count++;
                  }
        }
      fb_count = 0;

        // End of computing search

        /* Check here any -1 in the call_split_data
         * if -1 exists then drop the call
         */

      compute_passed   =   true;

      for(int i = 0; i<numBlock; i++)
      {
         if(call_split_data[i]==-1)
         {
             EV<<"\n The call not possible for fb"<<i<<endl;
             compute_passed  =  false;
         }
      }

      if(compute_passed)   // check data
      {
        previous_index  = ttmsg->getSenderModule()->getIndex();

        int ret=0;

        bitrate_passed = true;

        for (int i = 0; i<numBlock; i++)
        {
            if(previous_index == call_split_data[i] && i != (numBlock-1)) 
			// this means call is current block & the we need the last implementation
            {
                previous_index = call_split_data[i];
            }
            else
            {
                ret = inc_bitrate(previous_index, call_split_data[i], i,  ttmsg->getCall_load(), ttmsg->getSenderModule()->getIndex());

                if(ret == -1)
                {
                   revert_actual_compute();
                   revert_actual_bitrate();
                   bitrate_passed = false;
                   drop_bit_rate++;
                   break;
                }
                previous_index = call_split_data[i];
            }

        }

        if(bitrate_passed == true)
        {
            assign_actual_compute(ttmsg->getCallDuration()); //assigning all computing
            assign_actual_bitrate(ttmsg->getCallDuration()); //assigning all data rate
        }
      }
      else // compute check is not passed
      {
         revert_actual_compute();
      }


      if((bitrate_passed == false) || (compute_passed == false))
      {
        drop_count++;
            if(ttmsg->getCall_load()==0)
            {
                drop_count_low++;
            }
            else if(ttmsg->getCall_load()==1)
            {
                drop_count_med++;
            }
            else if(ttmsg->getCall_load()==2)
            {
                drop_count_high++;
            }
      }
       delete ttmsg;
       save_utilization();
     }
}


void SDN::drop_call(dataPkt *ttmsg)
{

}

int  SDN::inc_bitrate(int first_index, int second_index, int beta_index, int call_index, int src_index)
{
    EV <<"first index: "<< first_index <<"  second index: "<<second_index
       << " beta index: "<<beta_index<<"  call index: "<< beta_index
       << " src index: "<< src_index
       << endl;

    int sec_ind = second_index;

    int k = first_index%numBlock, l=second_index%numBlock;

    int inter_count = 0 ;
    int intra_count = 0 ;

    for (int i = k; i<l; i++)
    {
        EV <<"inter index: "<< i << endl;
        if(bitrate_context.current_inter[i] + call[call_index].bit_rate_data[beta_index] <= bitrate_context.max_bitrate_inter[i])
        {
            bitrate_context.current_inter[i] = call[call_index].bit_rate_data[beta_index] + bitrate_context.current_inter[i];
        }
        else
        {
            EV<<"Bit rate test failed\n";
            return -1;
        }
        inter_count=i+1;
    }

    if(first_index > second_index)
    {
        for (int i = k; i>l; --i)
            {
           //     EV<<"debug: k="<<k<<", l="<<l<<endl;
           //     EV <<"inter index: "<< i << endl;
                EV<<"needed "<<bitrate_context.current_inter[l] + call[call_index].bit_rate_data[beta_index]<<", max value = "<<bitrate_context.max_bitrate_inter[l]<<" at "<<l<<endl;

                if(bitrate_context.current_inter[l] + call[call_index].bit_rate_data[beta_index] <= bitrate_context.max_bitrate_inter[l])
                {
                    bitrate_context.current_inter[l] = call[call_index].bit_rate_data[beta_index] + bitrate_context.current_inter[l];
                }
                else
                {
                    EV<<"Bit rate test failed\n";
                    return -1;
                }
                inter_count=i;
            }
    }

    if(first_index/numBlock != second_index/numBlock)
    {
        EV <<"intra index: "<< inter_count << endl;
        if(bitrate_context.current_intra[inter_count] + call[call_index].bit_rate_data[beta_index] <= bitrate_context.max_bitrate_intra[inter_count])
        {
            bitrate_context.current_intra[inter_count] = call[call_index].bit_rate_data[beta_index] + bitrate_context.current_intra[inter_count];
        }
        else
        {
            EV<<"Bit rate test failed\n";
            return -1;
        }
    }

    if(beta_index==numBlock-1)  // i.e., call_index is 3 (final)
    {
        // check from first/second index, and calculate for final destination
        for(int i = second_index%numBlock; i < numBlock-1; i++)
        {
            EV<<"This is the final call processing, install from "<<sec_ind<<" to "<< sec_ind+1<< " i = "<<i<<endl;

            if(bitrate_context.current_inter[i] + call[call_index].bit_rate_data[beta_index] <= bitrate_context.max_bitrate_inter[i])
            {
                bitrate_context.current_inter[i] = call[call_index].bit_rate_data[beta_index] + bitrate_context.current_inter[i];
            }
            else
            {
                EV<<"Bit rate test failed\n";
                return -1;
            }
            sec_ind = sec_ind+1;
        }
    }

    return 0;
}

void SDN::assign_actual_compute(double call_duration)
{
    EV<<"assigning actual compute value"<<endl;

    dataPkt *call_expiry_compute = new dataPkt("call_expiry");
    call_expiry_compute->setSelfMsgType(1);                        // decrement_compute

    for(int i=0; i< numSrc*numBlock; i++)
    {
        call_expiry_compute->setDecCompute(i, fb_context[i].current_compute - fb_context[i].act_current_compute);
        fb_context[i].act_current_compute  =  fb_context[i].current_compute;
    }

    scheduleAt(simTime()+call_duration, call_expiry_compute);
}

void SDN::assign_actual_bitrate(double call_duration)
{
    EV<<"assigning to actual bitrate value"<<endl;

    dataPkt *call_expiry_bit_rate = new dataPkt("call_expiry");
    call_expiry_bit_rate->setSelfMsgType(2);                      // decrement_bit_rate

    for (int i=0; i <= (numBlock-1); i++)
    {

        call_expiry_bit_rate->setDecBitRate_intra(i, bitrate_context.current_intra[i] - bitrate_context.act_current_intra[i]);
        bitrate_context.act_current_intra[i] = bitrate_context.current_intra[i]; // In Gbps

        if(i <= (numBlock-2))
        {
            call_expiry_bit_rate->setDecBitRate_inter(i, bitrate_context.current_inter[i] - bitrate_context.act_current_inter[i]);
            bitrate_context.act_current_inter[i] = bitrate_context.current_inter[i];
        }
    }
    scheduleAt(simTime()+call_duration, call_expiry_bit_rate);
}


void  SDN::revert_actual_compute()
{
    EV<<"Reverting computing values"<<endl;
    for(int i=0; i< numSrc*numBlock; i++)
    {
       fb_context[i].current_compute =   fb_context[i].act_current_compute;
    }
}


void  SDN::revert_actual_bitrate()
{
    EV<<"Reverting bitrate values"<<endl;
    for (int i=0; i <= (numBlock-1); i++)
    {
        bitrate_context.current_intra[i] = bitrate_context.act_current_intra[i];
        if(i <= (numBlock-2))
        {
            bitrate_context.current_inter[i] = bitrate_context.act_current_inter[i]; // In Gbps
        }
    }
}


void  SDN::compute_expiry(dataPkt *ttmsg)
{
// Decrementing the compute resources from the available, and track it
    for(int i=0; i< numSrc*numBlock; i++)
    {
      EV<<"\nCall expired; decrementing compute for fb :"<<i<<endl;
      EV<<"act_current_compute: "<<fb_context[i].act_current_compute <<", to be dec "<<ttmsg->getDecCompute(i)<<endl;
      EV<< "current_compute: " <<fb_context[i].current_compute <<endl;
      EV<<"\nNew value after should be: "<< fb_context[i].current_compute -  (double)ttmsg->getDecCompute(i)<<endl;
      fb_context[i].act_current_compute = fb_context[i].act_current_compute - ttmsg->getDecCompute(i);
      fb_context[i].current_compute     = fb_context[i].act_current_compute;
    }

    for(int i=0; i< numSrc*numBlock; i++)
    {
        EV<< "\nPrinting values after decrementing :"<<i<<endl;
        EV<< "act_current_compute: "<<fb_context[i].act_current_compute <<", to be dec "<<ttmsg->getDecCompute(i)<<endl;
        EV<< "current_compute: " <<fb_context[i].current_compute     <<endl;
    }

delete ttmsg;
}

void  SDN::bitrate_expiry(dataPkt *ttmsg)
{
    EV<<"\nCall expired decrementing bitrate"<<endl;
    for (int i=0; i <= (numBlock-1); i++)
    {
        EV<<"act_intra_intra: "<<i<<", value = "<<bitrate_context.act_current_intra[i]<<", to be dec "<<ttmsg->getDecBitRate_intra(i)<<endl;
        bitrate_context.act_current_intra[i] = bitrate_context.act_current_intra[i]- ttmsg->getDecBitRate_intra(i);
        bitrate_context.current_intra[i]     = bitrate_context.current_intra[i] - ttmsg->getDecBitRate_intra(i);

        if(i <= (numBlock-2))
        {
            EV<<"act_intra_inter: "<<i<<", value = "<<bitrate_context.act_current_inter[i]<<", to be dec "<<ttmsg->getDecBitRate_inter(i)<<endl;
            bitrate_context.act_current_inter[i] = bitrate_context.act_current_inter[i]- ttmsg->getDecBitRate_inter(i);
            bitrate_context.current_inter[i]     = bitrate_context.current_inter[i] - ttmsg->getDecBitRate_inter(i);
        }
    }
delete ttmsg;
}

void SDN::save_utilization()
{
	// This method saves the utilization of bitrates 

	double sum_bit_rate_intra[4] = {0};
	double sum_bit_rate_inter[3] = {0};
	double sum_compute[4]  = {0};

    for (int i=0; i <= (numBlock-1); i++)
    {
        intra_utilization[i].collect(bitrate_context.current_intra[i]);

        if(i <= (numBlock-2))
        {
           inter_utilization[i].collect(bitrate_context.current_inter[i]);
        }
    }

    for (int i=0; i< numSrc*numBlock; i++)
    {
        sum_compute[i%numBlock] = sum_compute[i%numBlock]+fb_context[i].current_compute;
    }

    for (int i=0; i< numBlock; i++)
    {
       l_utilization[i].collect(sum_compute[i]);
    }

}

void SDN::finish()
{
	// This methods saves all the data

	recordScalar("Blocking Probability:",          100*(drop_count/num_calls));
    recordScalar("Blocking Probability_low:",      100*(drop_count_low/num_calls_low));
    recordScalar("Blocking Probability_med:",      100*(drop_count_med/num_calls_med));
    recordScalar("Blocking Probability_high:",     100*(drop_count_high/num_calls_high));
    recordScalar("Blocking Probability_bit_rate:", 100*(drop_bit_rate/num_calls));

    recordScalar("Intra[0] utilization:", intra_utilization[0].getMean());
    recordScalar("Intra[1] utilization:", intra_utilization[1].getMean());
    recordScalar("Intra[2] utilization:", intra_utilization[2].getMean());
    recordScalar("Intra[3] utilization:", intra_utilization[3].getMean());

    recordScalar("Inter[0] utilization:", intra_utilization[0].getMean());
    recordScalar("Inter[1] utilization:", intra_utilization[1].getMean());
    recordScalar("Inter[2] utilization:", intra_utilization[2].getMean());

    recordScalar("Compute L1 utilization:", l_utilization[0].getMean());
    recordScalar("Compute L2 utilization:", l_utilization[1].getMean());
    recordScalar("Compute L3 utilization:", l_utilization[2].getMean());
    recordScalar("Compute L4 utilization:", l_utilization[3].getMean());
}
