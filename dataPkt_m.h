//
// Generated file, do not edit! Created by nedtool 5.0 from dataPkt.msg.
//

#ifndef __DATAPKT_M_H
#define __DATAPKT_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0500
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>dataPkt.msg:17</tt> by nedtool.
 * <pre>
 * packet dataPkt
 * {
 *     double callDuration = 20;
 *     int call_load = 0;
 *     int selfMsgType = 0;
 *     double decCompute[12] = 0;
 *     double decBitRate_intra[5] = 0;
 *     double decBitRate_inter[5] = 0;
 * }
 * </pre>
 */
class dataPkt : public ::omnetpp::cPacket
{
  protected:
    double callDuration;
    int call_load;
    int selfMsgType;
    double decCompute[12];
    double decBitRate_intra[5];
    double decBitRate_inter[5];

  private:
    void copy(const dataPkt& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const dataPkt&);

  public:
    dataPkt(const char *name=nullptr, int kind=0);
    dataPkt(const dataPkt& other);
    virtual ~dataPkt();
    dataPkt& operator=(const dataPkt& other);
    virtual dataPkt *dup() const {return new dataPkt(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual double getCallDuration() const;
    virtual void setCallDuration(double callDuration);
    virtual int getCall_load() const;
    virtual void setCall_load(int call_load);
    virtual int getSelfMsgType() const;
    virtual void setSelfMsgType(int selfMsgType);
    virtual unsigned int getDecComputeArraySize() const;
    virtual double getDecCompute(unsigned int k) const;
    virtual void setDecCompute(unsigned int k, double decCompute);
    virtual unsigned int getDecBitRate_intraArraySize() const;
    virtual double getDecBitRate_intra(unsigned int k) const;
    virtual void setDecBitRate_intra(unsigned int k, double decBitRate_intra);
    virtual unsigned int getDecBitRate_interArraySize() const;
    virtual double getDecBitRate_inter(unsigned int k) const;
    virtual void setDecBitRate_inter(unsigned int k, double decBitRate_inter);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const dataPkt& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, dataPkt& obj) {obj.parsimUnpack(b);}


#endif // ifndef __DATAPKT_M_H

