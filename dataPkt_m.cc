//
// Generated file, do not edit! Created by nedtool 5.0 from dataPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "dataPkt_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(dataPkt);

dataPkt::dataPkt(const char *name, int kind) : ::omnetpp::cPacket(name,kind)
{
    this->callDuration = 20;
    this->call_load = 0;
    this->selfMsgType = 0;
    for (unsigned int i=0; i<12; i++)
        this->decCompute[i] = 0;
    for (unsigned int i=0; i<5; i++)
        this->decBitRate_intra[i] = 0;
    for (unsigned int i=0; i<5; i++)
        this->decBitRate_inter[i] = 0;
}

dataPkt::dataPkt(const dataPkt& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

dataPkt::~dataPkt()
{
}

dataPkt& dataPkt::operator=(const dataPkt& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void dataPkt::copy(const dataPkt& other)
{
    this->callDuration = other.callDuration;
    this->call_load = other.call_load;
    this->selfMsgType = other.selfMsgType;
    for (unsigned int i=0; i<12; i++)
        this->decCompute[i] = other.decCompute[i];
    for (unsigned int i=0; i<5; i++)
        this->decBitRate_intra[i] = other.decBitRate_intra[i];
    for (unsigned int i=0; i<5; i++)
        this->decBitRate_inter[i] = other.decBitRate_inter[i];
}

void dataPkt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->callDuration);
    doParsimPacking(b,this->call_load);
    doParsimPacking(b,this->selfMsgType);
    doParsimArrayPacking(b,this->decCompute,12);
    doParsimArrayPacking(b,this->decBitRate_intra,5);
    doParsimArrayPacking(b,this->decBitRate_inter,5);
}

void dataPkt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->callDuration);
    doParsimUnpacking(b,this->call_load);
    doParsimUnpacking(b,this->selfMsgType);
    doParsimArrayUnpacking(b,this->decCompute,12);
    doParsimArrayUnpacking(b,this->decBitRate_intra,5);
    doParsimArrayUnpacking(b,this->decBitRate_inter,5);
}

double dataPkt::getCallDuration() const
{
    return this->callDuration;
}

void dataPkt::setCallDuration(double callDuration)
{
    this->callDuration = callDuration;
}

int dataPkt::getCall_load() const
{
    return this->call_load;
}

void dataPkt::setCall_load(int call_load)
{
    this->call_load = call_load;
}

int dataPkt::getSelfMsgType() const
{
    return this->selfMsgType;
}

void dataPkt::setSelfMsgType(int selfMsgType)
{
    this->selfMsgType = selfMsgType;
}

unsigned int dataPkt::getDecComputeArraySize() const
{
    return 12;
}

double dataPkt::getDecCompute(unsigned int k) const
{
    if (k>=12) throw omnetpp::cRuntimeError("Array of size 12 indexed by %lu", (unsigned long)k);
    return this->decCompute[k];
}

void dataPkt::setDecCompute(unsigned int k, double decCompute)
{
    if (k>=12) throw omnetpp::cRuntimeError("Array of size 12 indexed by %lu", (unsigned long)k);
    this->decCompute[k] = decCompute;
}

unsigned int dataPkt::getDecBitRate_intraArraySize() const
{
    return 5;
}

double dataPkt::getDecBitRate_intra(unsigned int k) const
{
    if (k>=5) throw omnetpp::cRuntimeError("Array of size 5 indexed by %lu", (unsigned long)k);
    return this->decBitRate_intra[k];
}

void dataPkt::setDecBitRate_intra(unsigned int k, double decBitRate_intra)
{
    if (k>=5) throw omnetpp::cRuntimeError("Array of size 5 indexed by %lu", (unsigned long)k);
    this->decBitRate_intra[k] = decBitRate_intra;
}

unsigned int dataPkt::getDecBitRate_interArraySize() const
{
    return 5;
}

double dataPkt::getDecBitRate_inter(unsigned int k) const
{
    if (k>=5) throw omnetpp::cRuntimeError("Array of size 5 indexed by %lu", (unsigned long)k);
    return this->decBitRate_inter[k];
}

void dataPkt::setDecBitRate_inter(unsigned int k, double decBitRate_inter)
{
    if (k>=5) throw omnetpp::cRuntimeError("Array of size 5 indexed by %lu", (unsigned long)k);
    this->decBitRate_inter[k] = decBitRate_inter;
}

class dataPktDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    dataPktDescriptor();
    virtual ~dataPktDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(dataPktDescriptor);

dataPktDescriptor::dataPktDescriptor() : omnetpp::cClassDescriptor("dataPkt", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

dataPktDescriptor::~dataPktDescriptor()
{
    delete[] propertynames;
}

bool dataPktDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<dataPkt *>(obj)!=nullptr;
}

const char **dataPktDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *dataPktDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int dataPktDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int dataPktDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *dataPktDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "callDuration",
        "call_load",
        "selfMsgType",
        "decCompute",
        "decBitRate_intra",
        "decBitRate_inter",
    };
    return (field>=0 && field<6) ? fieldNames[field] : nullptr;
}

int dataPktDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "callDuration")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "call_load")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfMsgType")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "decCompute")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "decBitRate_intra")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "decBitRate_inter")==0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *dataPktDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "double",
        "int",
        "int",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **dataPktDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *dataPktDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int dataPktDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    dataPkt *pp = (dataPkt *)object; (void)pp;
    switch (field) {
        case 3: return 12;
        case 4: return 5;
        case 5: return 5;
        default: return 0;
    }
}

std::string dataPktDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    dataPkt *pp = (dataPkt *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getCallDuration());
        case 1: return long2string(pp->getCall_load());
        case 2: return long2string(pp->getSelfMsgType());
        case 3: return double2string(pp->getDecCompute(i));
        case 4: return double2string(pp->getDecBitRate_intra(i));
        case 5: return double2string(pp->getDecBitRate_inter(i));
        default: return "";
    }
}

bool dataPktDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    dataPkt *pp = (dataPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setCallDuration(string2double(value)); return true;
        case 1: pp->setCall_load(string2long(value)); return true;
        case 2: pp->setSelfMsgType(string2long(value)); return true;
        case 3: pp->setDecCompute(i,string2double(value)); return true;
        case 4: pp->setDecBitRate_intra(i,string2double(value)); return true;
        case 5: pp->setDecBitRate_inter(i,string2double(value)); return true;
        default: return false;
    }
}

const char *dataPktDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *dataPktDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    dataPkt *pp = (dataPkt *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


