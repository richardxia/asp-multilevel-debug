#ifndef CPP_ANALYSIS_H_
#define CPP_ANALYSIS_H_

#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <assert.h>

#include <vector>
#include <map>
#include <set>
#include <list>

#include <boost/filesystem.hpp>

#include <glog/logging.h>
#define _LOG_ LOG(INFO)

#include <omp.h>

/********************************************************************************/

#define DECL_FIELD(type, name) \
		protected: \
		type name##_; \
		public: \
		inline type& name() { return name##_; } \
		inline void set_##name(type value) { name##_ = value; } \
		private:	\

#define DECL_FIELD_REF(type, name) \
		protected: \
		type name##_; \
		public: \
		inline type* name() { return &name##_; } \
		inline void set_##name(type& value) { name##_ = value; } \
		private:	\

/********************************************************************************/

const char* ASP_ROOT = getenv("ASP_ROOT");

#define REPORT_FILE			"report.txt"
#define TRACE_FILE			"trace.txt"

/********************************************************************************/

#define THREADID int
#define NUM_THREADS 8
#define INVALID_THREADID (-1)
#define CurrentThreadId()	omp_get_thread_num()

#define INVALID_INDEX int(-1)

#define ADDRINT	uintptr_t

namespace asp {


/************************************************************************/

#define NANOSECS_TO_SLEEP 1000
inline void NanoSleep()	{
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = (rand() % NANOSECS_TO_SLEEP);
	nanosleep (&ts, NULL);
}
#define SLEEP() NanoSleep()


/************************************************************************/

// Helper stack operations

template<typename T>
T peek(std::vector<T>& v) {
	assert(v.size() > 0);
	return v.back();
}

template<typename T>
void push(std::vector<T>& v, T e) {
	v.push_back(e);
}

template<typename T>
T pop(std::vector<T>& v) {
	assert(v.size() > 0);
	T e = v.back();
	v.pop_back();
	return e;
}

template<typename T>
T pop_push(std::vector<T>& v, T e) {
	assert(v.size() > 0);
	T t = pop(v);
	push(v, e);
	return t;
}

/************************************************************************/

typedef bool AccessType;
#define READ true
#define WRITE false

/************************************************************************/

typedef std::vector<int> TXID;
extern const TXID INVALID_TXID;

extern std::string TXID_ToString(TXID txid);

class AccessBase;
typedef std::vector<AccessBase*> AccessBasePtrList;

typedef std::pair<AccessBase*, AccessBase*> Edge;
typedef std::vector<Edge> EdgeList;
#define make_edge(access1, access2) std::pair<AccessBase*, AccessBase*>(access1, access2)

enum dfs_state_t {WHITE = 1, GRAY = 2, BLACK = 3};

class Transaction {
public:
	Transaction();
	~Transaction();

	void Begin(TXID txid);
	void End();

	void AddConflictEdge(AccessBase* access1, AccessBase* access2);

	inline bool conflicts_with(Transaction* that) {
		return this->tid_ != that->tid_;
	}

	void PrettyPrint() {
		printf("TXID: %s, TID: %d", TXID_ToString(txid_).c_str(), tid_);
	}

	void PrettyLog() {
		_LOG_ << "TXID: " << TXID_ToString(txid_) << ", TID: " << tid_;
	}
private:
	DECL_FIELD(TXID, txid)
	DECL_FIELD(THREADID, tid)
	DECL_FIELD(bool, active)
	DECL_FIELD_REF(EdgeList, conflict_edges)
	DECL_FIELD_REF(AccessBasePtrList, accesses)

	// DFS state
	DECL_FIELD(dfs_state_t, dfs_state)
};

typedef std::list<Transaction*> TransactionPtrList;

/************************************************************************/

class AccessBase {
public:
	AccessBase(AccessType type,
			Transaction* tx,
			ADDRINT mem,
			ADDRINT base_mem,
			const char* base_name = NULL,
			int offset = -1,
			const char* offset_name = NULL)
	: type_(type), tx_(tx), mem_(mem), base_mem_(base_mem), base_name_(base_name), offset_(offset), offset_name_(offset_name) {
		assert(offset_ >= -1);
	}
	virtual ~AccessBase(){}

	virtual void PrettyPrint();
	virtual void PrettyLog();
	virtual std::string ToString();

	inline bool is_array() { return offset_ >= 0; }
	inline bool is_read() { return type_ == READ; }
	inline bool is_write() { return type_ == WRITE; }

	virtual bool conflicts_with(AccessBase* that) {
		return this->is_write() || that->is_write();
	}

	virtual double double_value() = 0;

private:
	DECL_FIELD(AccessType, type)
	DECL_FIELD(Transaction*, tx)
	DECL_FIELD(ADDRINT, mem)
	DECL_FIELD(ADDRINT, base_mem)
	DECL_FIELD(const char*, base_name)
	DECL_FIELD(int, offset)
	DECL_FIELD(const char*, offset_name)
};

/************************************************************************/

template<typename T>
class Access : public AccessBase {
public:
	Access(AccessType type,
			Transaction* tx,
			T* base,
			const char* base_name = NULL,
			int offset = -1,
			const char* offset_name = NULL)
	: AccessBase(type, tx, int_address(base, offset), int_address(base), base_name, offset, offset_name),
	  base_(base) {}

	~Access(){}

	// perform the actual read from base[offset]
	T DoRead();
	// perform the actual write of value to base[offset]
	void DoWrite(T value);

	static inline ADDRINT int_address(T* base, int offset = 0) {
		if(offset < 0) offset = 0;
		return reinterpret_cast<ADDRINT>(base + offset);
	}

	bool conflicts_with(AccessBase* that) {
		assert(dynamic_cast<Access<T>*>(that) != NULL);
		return AccessBase::conflicts_with(that) &&
				(this->value_ != static_cast<Access<T>*>(that)->value_);
	}

	inline double double_value() {
		return static_cast<double>(value_);
	}

private:
	DECL_FIELD(T*, base)
	DECL_FIELD(T, value)
};

/************************************************************************/

template<typename T>
T Access<T>::DoRead() {
	int offset = offset_ >= 0 ? offset_ : 0;
	assert(type_ == READ);
	value_ = base_[offset];
	return value_;
}

/************************************************************************/

template<typename T>
void Access<T>::DoWrite(T value) {
	int offset = offset_ >= 0 ? offset_ : 0;
	assert(type_ == WRITE);
	value_ = value;
	base_[offset] = value_;
}

/************************************************************************/

// functions to call from kernels to create a new access object

template<typename T>
inline Access<T>* NewReadAccess(Transaction* tx,
						T* base,
						const char* base_name = NULL,
						int offset = -1,
						const char* offset_name = NULL) {
	return new Access<T>(READ, tx, base, base_name, offset, offset_name);
}

template<typename T>
inline Access<T>* NewWriteAccess(Transaction* tx,
						T* base,
						const char* base_name = NULL,
						int offset = -1,
						const char* offset_name = NULL) {
	return new Access<T>(WRITE, tx, base, base_name, offset, offset_name);
}

/************************************************************************/

class ForLoopInfo {
public:
	ForLoopInfo(const char* loopvar, ForLoopInfo* prev = NULL, bool is_parallel = false)
	: loopvar_(loopvar), prev_(prev), index_(0), is_parallel_(is_parallel) {
		if(prev_ != NULL) {
			// compute txid
			txid_ = prev_->txid_;
			txid_.push_back(prev_->index_);
		} else {
			txid_.clear();
		}
	}

	// do not call Iter for parallel loops
	inline void Iter(int index) {
		assert(!is_parallel_);
		index_ = index;
	}

	inline TXID GetTXID(int index) {
		TXID txid = txid_;
		txid.push_back(index);
		return txid;
	}

	inline TXID GetTXID() {
		return GetTXID(index_);
	}

	std::string ToString(bool iter = false) {
		std::stringstream s;
		if(iter) {
			ForLoopInfo* f = prev_;
			if(f != NULL) {
				s << f->ToString(true) << " >> ";
			}
		}
		s << loopvar_ << ":" << index_;
		return s.str();
	}

protected:
	DECL_FIELD(const char*, loopvar)
	DECL_FIELD(TXID, txid) // cached transaction id
	DECL_FIELD(ForLoopInfo*, prev)
	DECL_FIELD(int, index)
	DECL_FIELD(bool, is_parallel)
};

/************************************************************************/

struct DFSStackElement {
	Transaction* tx_;
	EdgeList::iterator begin_;
	EdgeList::iterator end_;

	DFSStackElement(Transaction* tx,
				EdgeList::iterator begin,
				EdgeList::iterator end)
	: tx_(tx), begin_(begin), end_(end)
	{}
};

typedef std::vector<DFSStackElement> DFSStack;

/********************************************************************************/

typedef std::vector<AccessBase*> Cycle;
typedef std::vector<Cycle*> CyclePtrList;

/********************************************************************************/

class AspEventHandler {

public:
	AspEventHandler(const char* test_name, const char* variant_name);
	virtual ~AspEventHandler();

	template<typename T>
	T OnRead(Access<T>* access);

	template<typename T>
	void OnWrite(Access<T>* access, T value);

	Transaction* BeginTransaction(int loopindex);
	void EndTransaction(Transaction* tx);

	void BeginParallel(ForLoopInfo* f);
	void EndParallel(ForLoopInfo* f);

	void EnterForLoop(const char* loopvar, bool is_parallel = false);
	void IterForLoop(int loopindex);
	void ExitForLoop();

protected:

	template<typename T>
	void BeforeAccess(Access<T>* access);

	template<typename T>
	void AfterAccess(Access<T>* access);

	inline void Lock() { omp_set_lock(&lock_); }
	inline void Unlock() { omp_unset_lock(&lock_); }

private:

	AccessBasePtrList* GetLastAccesses(ADDRINT mem);
	void UpdateLastAccesses(AccessBase* access, AccessBasePtrList* accesses, bool conflicting);
	bool UpdateConflictEdges(AccessBase* access, AccessBasePtrList* accesses);
	void RecordAccess(AccessBase* access);

	void check_serializability();

	void dfs(DFSStackElement start_elt);
	Cycle* extract_cycle(DFSStack& stack, Transaction* tx);
	void report_cycles(CyclePtrList cycles);
	void report_topsortedtx();

	DECL_FIELD(const char*, test_name)
	DECL_FIELD(const char*, variant_name)
	DECL_FIELD(omp_lock_t, lock)
	DECL_FIELD(ForLoopInfo*, current_for)
	DECL_FIELD(TransactionPtrList, txlist)

	DECL_FIELD(unsigned int, num_cycles)
	DECL_FIELD(unsigned int, num_unique_cycles)
	DECL_FIELD(CyclePtrList, cycles)
	DECL_FIELD(TransactionPtrList, txlist_sorted)

	DECL_FIELD(FILE*, report_file)

//	static std::map<THREADID, Transaction*> tid_to_tx_;
//	static Transaction* GetThreadLocalTransaction(THREADID tid = INVALID_THREADID);

	std::map<ADDRINT, AccessBasePtrList*> memToLastAccess_;

};

// header text to use when printing or logging information (valid only in AspEventHandler methods)
#define _HANDLER_TITLE_	"[[" << variant_name_ << "]] "

/************************************************************************/

template<typename T>
T AspEventHandler::OnRead(Access<T>* access) {
	SLEEP();

	Lock();

	BeforeAccess(access);

	T value = access->DoRead();

	AfterAccess(access);

	Unlock();

	return value;
}

/************************************************************************/

template<typename T>
void AspEventHandler::OnWrite(Access<T>* access, T value) {
	SLEEP();

	Lock();

	BeforeAccess(access);

	access->DoWrite(value);

	AfterAccess(access);

	Unlock();

}

/************************************************************************/

template<typename T>
void AspEventHandler::BeforeAccess(Access<T>* access) {
	access->PrettyLog();
}

/************************************************************************/

template<typename T>
void AspEventHandler::AfterAccess(Access<T>* access) {
	RecordAccess(access);
}

/************************************************************************/



} // end namespace

#endif // CPP_ANALYSIS_H_
