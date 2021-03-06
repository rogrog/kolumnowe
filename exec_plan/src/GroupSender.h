#ifndef GROUP_SENDER
#define GROUP_SENDER

#include <vector>

#include "Operation.h"
#include "operations.pb.h"
#include "UniversalHashmap.h"
#include "RealUniversalHashmap.h"
#include "layers.h"

namespace Engine {

class GroupSender : public Operation {

	private:
		NodeEnvironmentInterface *nei_;
		Operation *source_;
		std::vector<OperationTree::ScanOperation_Type> source_types_;
		std::vector<OperationTree::ScanOperation_Type> hash_column_types_;
    const OperationTree::GroupByOperation &node_;
		
		// we use hash function to scatter data into buckets
		// when there is enough rows in a bucket, data is sent to 
		// corresponding worker in second group
		std::vector<std::vector<void*> > buckets_;
		std::vector<int> buckets_load_;

		void scatter_data_into_buckets(vector<void*> data, int rows, int32* hashes);	
		int32* count_hashes(const vector<void*> &dataToHash, const vector<OperationTree::ScanOperation_Type> &typesToHash, int rows);
		void cast_to_hash_columns(const std::vector<void*> &data, std::vector<void*> &result);
		bool bucket_ready_to_send(int bucket);
		void send_bucket(int bucket_number);


	public:
		GroupSender(NodeEnvironmentInterface *nei, Operation *source, std::vector<OperationTree::ScanOperation_Type> source_types, std::vector<OperationTree::ScanOperation_Type> hash_column_types, const OperationTree::GroupByOperation &node);
		


		std::vector<void*> pull(int &rows);
};

}

#endif // GROUP_SENDER
