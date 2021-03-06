#include "operations.pb.h"
#include <google/protobuf/text_format.h>
#include "server.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include "MEngine.h"

const int BUFF_SIZE = 10000;

using namespace std;

int main(int argc, char ** argv) {
    
      GOOGLE_PROTOBUF_VERIFY_VERSION; 

      if (argc != 3) {
          cerr << "Usage " << argv[0] << " query_number path_to_protouff_file." << endl;
          return -1;
      }

      int query_no = atoi(argv[1]);
      //int max_rows = atoi(argv[3]); // TODO remove this shit before submitting
      int max_rows = 20;

      cerr << "Engine run with parameters query_no = " << query_no << " path_to_protobuff = " << argv[2] << endl;

      OperationTree::Operation operation; 
      std::fstream input(argv[2], ios::in);
      if (!input) {
          cerr << argv[2] << ": File not found." << endl;
          return -1;
      } else {
          char data[BUFF_SIZE];
          input.read(data, BUFF_SIZE);
          cout << string(data) << endl;
          if (!google::protobuf::TextFormat::ParseFromString(string(data), &operation)) {
              cerr << "Failed to parse operation." << endl;
              return -1;
          }
      }

      Server * server = CreateServer(query_no);
      Engine::MEngine engine(operation, server, max_rows);
      cerr << "DONE" << endl;
      engine.run();
      

      google::protobuf::ShutdownProtobufLibrary();

      return 0;
}
