#include "benchmark/tpcc/Database.h"
#include "core/Coordinator.h"
#include "core/Macros.h"
#include <unistd.h>

DEFINE_bool(operation_replication, false, "use operation replication");
DEFINE_string(query, "neworder", "tpcc query, mixed, neworder, payment");
DEFINE_int32(neworder_dist, 10, "new order distributed.");
DEFINE_int32(payment_dist, 15, "payment distributed.");

// ./main --logtostderr=1 --id=1 --servers="127.0.0.1:10010;127.0.0.1:10011"
// cmake -DCMAKE_BUILD_TYPE=Release

int main(int argc, char *argv[]) {

  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  google::ParseCommandLineFlags(&argc, &argv, true);

  std::string log_path;
  char *buffer;
  if ((buffer = getcwd(NULL, 0)) == NULL) {
    printf("getcwd error!\n");
    return 0;
  }
  log_path = buffer;
  free(buffer);
  log_path += "/log/";
  printf("%s\n", log_path.c_str());

  google::SetLogDestination(google::GLOG_INFO, (log_path + "INFO_").c_str());
	google::SetLogDestination(google::GLOG_WARNING, (log_path + "WARNING_").c_str());
	google::SetLogDestination(google::GLOG_ERROR, (log_path + "ERROR_").c_str());
	google::SetLogDestination(google::GLOG_FATAL, (log_path + "FATAL_").c_str());


  star::tpcc::Context context;
  SETUP_CONTEXT(context);

  context.operation_replication = FLAGS_operation_replication;

  if (FLAGS_query == "mixed") {
    context.workloadType = star::tpcc::TPCCWorkloadType::MIXED;
  } else if (FLAGS_query == "neworder") {
    context.workloadType = star::tpcc::TPCCWorkloadType::NEW_ORDER_ONLY;
  } else if (FLAGS_query == "payment") {
    context.workloadType = star::tpcc::TPCCWorkloadType::PAYMENT_ONLY;
  } else {
    CHECK(false);
  }

  context.newOrderCrossPartitionProbability = FLAGS_neworder_dist;
  context.paymentCrossPartitionProbability = FLAGS_payment_dist;

  star::tpcc::Database db;
  db.initialize(context);

  star::Coordinator c(FLAGS_id, db, context);
  c.connectToPeers();
  c.start();
  return 0;
}