
#include "cpp_analysis.h"

namespace asp {

static bool initialize();
static bool initialized_ = initialize();
static bool initialize() {
	// init logging
	if(!initialized_) {
		google::InitGoogleLogging("asp");
	}
	return true;
}

/********************************************************************************/

const TXID INVALID_TXID;

std::string TXID_ToString(TXID txid) {
	assert(txid.size() > 0);
	std::stringstream s;
	s << txid[0];
	for(unsigned int i = 1; i < txid.size(); ++i) {
		s << "::" << txid[i];
	}
	return s.str();
}

/********************************************************************************/

Transaction::Transaction() {
	tid_ = INVALID_THREADID;
	txid_ = INVALID_TXID;
	active_ = false;
	dfs_state_ = WHITE;
}

Transaction::~Transaction() {
	for(AccessBasePtrList::iterator iter = accesses_.begin(); iter < accesses_.end(); iter = accesses_.erase(iter)) {
		AccessBase* access = *iter;
		assert(access != NULL);
		delete access;
	}
}

void Transaction::Begin(TXID txid) {
	assert(!active_);

	assert(conflict_edges_.empty());
	assert(accesses_.empty());

	txid_ = txid;
	tid_ = CurrentThreadId();
	active_ = true;
	dfs_state_ = WHITE;

	_LOG_ << "Begin Transaction...";
	PrettyLog();
}

void Transaction::End() {
	assert(active_);

	_LOG_ << "End Transaction...";
	PrettyLog();

	active_ = false;
}

/********************************************************************************/

//std::map<THREADID, Transaction*> AspEventHandler::tid_to_tx_;

/************************************************************************/

AspEventHandler::AspEventHandler(const char* test_name, const char* variant_name)
: test_name_(test_name), variant_name_(variant_name) {
	assert(variant_name_ != NULL);

	_LOG_ << _HANDLER_TITLE_ << "Initializing AspEventHandler for variant " << variant_name_;


	int num_threads = NUM_THREADS;
	char* env_num_threads = getenv("NUM_THREADS");
	if(env_num_threads != NULL) {
		num_threads = atoi(env_num_threads);
	}
	assert(num_threads > 0);

	_LOG_ << _HANDLER_TITLE_ << "Setting number of threads to " << num_threads;
	omp_set_num_threads(num_threads);

	_LOG_ << _HANDLER_TITLE_ << "Initializing the lock";
	omp_init_lock(&lock_);

	current_for_ = NULL;

	num_cycles_ = 0;
	num_unique_cycles_ = 0;

	char buff[256];
	assert(ASP_ROOT != NULL);
	sprintf(buff, "%s/asp/analysis/work/%s/%s", ASP_ROOT, test_name_, REPORT_FILE);
	report_file_ = fopen(buff, "a");
	if(!report_file_) {
		printf("Error in opening the report file!");
		exit(-1);
	}
	fprintf(report_file_, "Test: %s Variant: %s\n\n", test_name_, variant_name);
}

/************************************************************************/

AspEventHandler::~AspEventHandler() {
	_LOG_ << _HANDLER_TITLE_ << "Finalizing AspEventHandler";

	_LOG_ << _HANDLER_TITLE_ << "Destroying the lock";
	omp_destroy_lock(&lock_);

	if(num_unique_cycles_ > 0) {
	  printf("\n"
		 "****************************************************\n"
		 "****************************************************\n"
		 "***** Active testing found:\n"
		 "***** %d data races and serializability violations in %s/%s!\n"
		 "***** (See the report at work/%s/report.txt for details.)\n"
		 "****************************************************\n"
		 "****************************************************\n",
		 num_unique_cycles_, test_name_, variant_name_, test_name_);
	} else {
	  printf("\n"
		 "****************************************************\n"
		 "****************************************************\n"
		 "***** Active testing found:\n"
		 "***** NO data races or serializability violations.\n"
		 "****************************************************\n"
		 "****************************************************\n");
	}

	fprintf(report_file_, "Number of all cycles: %d\nNumber of unique cycles: %d\n", num_cycles_, num_unique_cycles_);

	fclose(report_file_);
}

/************************************************************************/

void AspEventHandler::EnterForLoop(const char* loopvar, bool is_parallel) {
	// push new for loop
	current_for_ = new ForLoopInfo(loopvar, current_for_, is_parallel);

	if(is_parallel) {
		BeginParallel(current_for_);
	}
}

void AspEventHandler::IterForLoop(int loopindex) {
	// push new for loop
	ForLoopInfo* top = current_for_;
	assert(top != NULL);
	assert(!top->is_parallel());
	top->Iter(loopindex);
}

void AspEventHandler::ExitForLoop() {
	// pop new for loop
	ForLoopInfo* top = current_for_;
	assert(top != NULL);

	if(top->is_parallel()) {
		EndParallel(top);
	}

	current_for_ = top->prev();
	delete top;
}

/********************************************************************************/

Transaction* AspEventHandler::BeginTransaction(int loopindex) {
	Lock();

	// compute txid
	ForLoopInfo* f = current_for_;
	assert(f != NULL && f->is_parallel());
	TXID txid = f->GetTXID(loopindex);

	Transaction* tx = new Transaction();

	// register the transaction
	txlist_.push_back(tx);

	tx->Begin(txid);

	Unlock();

	return tx;
}

void AspEventHandler::EndTransaction(Transaction* tx) {
	Lock();

	assert(tx != NULL);
	tx->End();

	Unlock();
}

/************************************************************************/

void AspEventHandler::BeginParallel(ForLoopInfo* f) {
	assert(txlist_.empty());
	assert(txlist_sorted_.empty());
	assert(memToLastAccess_.empty());

	fprintf(report_file_, "Parallel loop: %s\n\n", f->ToString(true).c_str());
}

void AspEventHandler::EndParallel(ForLoopInfo* f) {

	// do serializability check
	check_serializability();

	if(cycles_.size() > 0) {
		// report and cycles
		report_cycles(cycles_);
	} else {
		// report the topologically sorted transactions
		report_topsortedtx();
	}

	//------------------------------------------------------------
	// CLEAN-UP

	// empty topologically sorted txlist
	txlist_sorted_.clear();

	// delete all transactions from txlist and delete all accesses by transactions
	for(TransactionPtrList::iterator itr = txlist_.begin(); itr != txlist_.end(); itr = txlist_.erase(itr)) {
		Transaction* tx = (*itr);
		delete tx; // deletes all the accesses by tx
	}

	// clear last accesses map
	for(std::map<ADDRINT, AccessBasePtrList*>::iterator itr = memToLastAccess_.begin(); itr != memToLastAccess_.end(); memToLastAccess_.erase(itr++)) {
		AccessBasePtrList* accesses = itr->second;
		delete accesses;
	}
}

/********************************************************************************/

// transactions seen in a cycle
// we use this set to reduce the number of cycles to ones with different number of transactions
static std::set<Transaction*> txset_in_cycle;

void AspEventHandler::check_serializability() {
	assert(cycles_.empty());
	assert(txlist_sorted_.empty());

	txset_in_cycle.clear();

	for(TransactionPtrList::iterator itr = txlist_.begin(); itr != txlist_.end(); ++itr) {
		Transaction* tx = (*itr);
		if(tx->dfs_state() == WHITE) {
			EdgeList* edges = tx->conflict_edges();
			dfs(DFSStackElement(tx, edges->begin(), edges->end()));
		}
	}
}

void AspEventHandler::dfs(DFSStackElement start_elt) {
	DFSStack stack;
	push(stack, start_elt);
	while(stack.size() > 0) {
		DFSStackElement current_elt = peek(stack);
		Transaction* current_tx = current_elt.tx_;
		if(current_elt.begin_ == current_elt.end_) {
			// no out-going edges; done with current transaction; pop it
			current_tx->set_dfs_state(BLACK);
			pop(stack);

			// add tx to the topologically sorted txlist
			txlist_sorted_.push_front(current_tx);

			if(stack.size() > 0) {
				// iterate the new top to next edge
				current_elt = peek(stack);
				current_elt.begin_++;
				pop_push(stack, current_elt);
			}
			continue;
		}

		Edge e = *(current_elt.begin_);
		assert(current_tx == e.first->tx());
		Transaction* next_tx = e.second->tx();
		assert(current_tx != next_tx);

		dfs_state_t next_state = next_tx->dfs_state();
		if(next_state == WHITE) {
			next_tx->set_dfs_state(GRAY);

			EdgeList* edges = next_tx->conflict_edges();
			push(stack, DFSStackElement(next_tx, edges->begin(), edges->end()));
		} else {
			assert(next_state == BLACK || next_state == GRAY);
			// check cycle
			if(next_state == GRAY) {
				// Cycle; extract it from the current dfs stack and save it
				Cycle* cycle = extract_cycle(stack, next_tx);
				// extract_cycle may skip reporting a cycle, so check if cycle is non-null
				if(cycle != NULL) {
					cycles_.push_back(cycle);
				}
			}
			// iterate top to next edge
			current_elt.begin_++;
			pop_push(stack, current_elt);
		}
	}
}

// takes a dfs stack and a transaction that is visited twice, and returns the corresponding cycle
// last access in the dfs stack belongs to tx
// the first and last access in the cycle belong to tx
Cycle* AspEventHandler::extract_cycle(DFSStack& stack, Transaction* tx) {
	num_cycles_++; // all cycles

	// if we have seen another cycle with tx as the revisited transaction, skip reporting this cycle
	if(txset_in_cycle.find(tx) != txset_in_cycle.end()) {
		return NULL;
	}
	// record this transaction, so we do not report cycles with the same revisited transaction
	txset_in_cycle.insert(tx);

	//----------------------------
	Cycle* cycle = new Cycle();
	num_unique_cycles_++; // unique cycles

	DFSStack::iterator itr = stack.begin();
	for(; itr < stack.end(); ++itr) {
		DFSStackElement current_elt = (*itr);
		if(current_elt.tx_ == tx) {
			break;
		}
	}

	for(; itr < stack.end(); ++itr) {
		DFSStackElement current_elt = (*itr);
		Edge edge = *(current_elt.begin_);
		cycle->push_back(edge.first);
		cycle->push_back(edge.second);
	}

	assert(cycle->front()->tx() == tx);
	assert(cycle->back()->tx() == tx);

	return cycle;
}

// TODO(elmas): report more information about the cycle
void AspEventHandler::report_cycles(CyclePtrList cycles) {
	int i = 1;
	fprintf(report_file_, "Number of unique cycles: %d\n\n", cycles.size());
	for(CyclePtrList::iterator itr = cycles_.begin(); itr < cycles_.end(); itr = cycles_.erase(itr)) {
		Cycle* cycle = (*itr);

		// report cycle
		fprintf(report_file_, "Cycle no: %d\n", i);
		for(Cycle::iterator iter = cycle->begin(); iter < cycle->end(); ++iter) {
			AccessBase* access = (*iter);
			fprintf(report_file_, "%s\n", access->ToString().c_str());
		}
		fprintf(report_file_, "\n");

		delete cycle;

		++i;
	}
}

void AspEventHandler::report_topsortedtx() {
	char buff[256];
	assert(ASP_ROOT != NULL);
	sprintf(buff, "%s/asp/analysis/work/%s/%s", ASP_ROOT, test_name_, TRACE_FILE);
	FILE* trace_file = fopen(buff, "w");
	if(!trace_file) {
		printf("Error in opening the trace file /tmp/trace.txt !");
		exit(-1);
	}

	for(TransactionPtrList::iterator itr = txlist_sorted_.begin(); itr != txlist_sorted_.end(); ++itr) {
		Transaction* tx = (*itr);
		fprintf(report_file_, "%s\n", TXID_ToString(tx->txid()).c_str());

		// write to Richard's trace file format
		AccessBasePtrList* accesses = tx->accesses();
		for(AccessBasePtrList::iterator iter = accesses->begin(); iter < accesses->end(); ++iter) {
			AccessBase* access = (*iter);
			if(access->is_array() && access->is_write()) {
				fprintf(trace_file, "_asp_log_write(%s, %d, %s, %.4f)\n",
						access->base_name(), access->offset(), "_my_in_img", access->double_value());
			}
		}
	}

	fclose(trace_file);
}

/********************************************************************************/

AccessBasePtrList* AspEventHandler::GetLastAccesses(ADDRINT mem) {
	AccessBasePtrList* accesses = NULL;
	std::map<ADDRINT, AccessBasePtrList*>::iterator itr = memToLastAccess_.find(mem);
	if(itr != memToLastAccess_.end()) {
		accesses = itr->second;
		assert(accesses != NULL);
		assert(!accesses->empty());
	}
	return accesses;
}

void AspEventHandler::UpdateLastAccesses(AccessBase* access, AccessBasePtrList* accesses, bool conflicting) {
	if(accesses == NULL) {
		accesses = new AccessBasePtrList();
		memToLastAccess_[access->mem()] = accesses;
	} else if(conflicting){
		accesses->clear();
	}
	accesses->push_back(access);
}

bool AspEventHandler::UpdateConflictEdges(AccessBase* access, AccessBasePtrList* accesses) {
	bool conflicting = false;
	ADDRINT mem = access->mem();
	if(accesses != NULL) {
		assert(accesses->size() > 0);
		Transaction* tx = access->tx();
		// record last accesses in the transaction graph
		for(AccessBasePtrList::iterator itr = accesses->begin(); itr < accesses->end(); ++itr) {
			AccessBase* lastAccess = (*itr);
			assert(lastAccess->mem() == mem);
			conflicting = conflicting || lastAccess->conflicts_with(access);
			if(conflicting) {
				// add a conflict edge
				Transaction* lastTx = lastAccess->tx();
				assert(lastTx != NULL);
				if(lastTx->conflicts_with(tx)) {
					lastTx->AddConflictEdge(lastAccess, access);
				}
			} else {
				break;
			}
		}
	}
	return conflicting;
}

void AspEventHandler::RecordAccess(AccessBase* access) {

	// fetch last accesses
	AccessBasePtrList* accesses = GetLastAccesses(access->mem());

	// update transaction conflicts graph
	bool conflicting = UpdateConflictEdges(access, accesses);

	// add the current access
	UpdateLastAccesses(access, accesses, conflicting);

	// add access to its transaction
	access->tx()->accesses()->push_back(access);
}

/********************************************************************************/

void Transaction::AddConflictEdge(AccessBase* access1, AccessBase* access2) {
	assert(access1->tx() == this);
	conflict_edges_.push_back(make_edge(access1, access2));
}

/********************************************************************************/

void AccessBase::PrettyPrint() {
	printf("%s", ToString().c_str());
}

/********************************************************************************/

void AccessBase::PrettyLog() {
	_LOG_ << ToString();
}

/********************************************************************************/

std::string AccessBase::ToString() {
	std::stringstream s;
	s << (type_ == READ ? "Read from " : "Write to ")
	  << (base_name_ != NULL ? base_name_ : "<unknown>");
	if(offset_ >= 0) {
		s << "[" << (offset_name_ != NULL ? offset_name_ : "<unknown>") << "]";
	}
	s << " by ";
	if(tx_ != NULL) {
		s << "Thread " << tx_->tid()
		  << " Iteration " << TXID_ToString(tx_->txid());
	}else {
		s << "Thread " << 0
		  << " Iteration " << "<unknown>";
	}
	s << " -- Address: "
	  << base_mem_;
	if(offset_ >= 0) {
		s << "[" << offset_ << "]";
	}
	return s.str();
}

/********************************************************************************/

} // end namespace
