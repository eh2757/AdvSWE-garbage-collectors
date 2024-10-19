#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <grpcpp/grpcpp.h>
#include "health_records.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using health::PatientData;
using health::HealthRecordRequest;
using health::HealthRecordResponse;
using health::UpdateHealthRecordRequest;
using health::HealthRecordsService;

// Health records database. A map with patient_id and patient_data.
std::map<int, PatientData> health_records_db;

// Service implementation inheriting from gRPC generated class
class HealthRecordsServiceImpl final : public HealthRecordsService::Service {
public:
    
    // Add a health record for a patient
    Status AddHealthRecord(ServerContext* context, const PatientData* request,
                           HealthRecordResponse* reply) override {
        int patient_id = request->patient_id();
        health_records_db[patient_id] = *request;
        reply->mutable_patient_data()->CopyFrom(*request);
        std::cout << "Added health record for patient: " << patient_id << std::endl;
        return Status::OK;
    }

    // Get a health record for a patient
    Status GetHealthRecord(ServerContext* context, const HealthRecordRequest* request,
                           HealthRecordResponse* reply) override {
        int patient_id = request->patient_id();
        if (health_records_db.find(patient_id) != health_records_db.end()) {
            reply->mutable_patient_data()->CopyFrom(health_records_db[patient_id]);
            std::cout << "Retrieved health record for patient: " << patient_id << std::endl;
            return Status::OK;
        } else {
            std::cout << "No health record found for patient: " << patient_id << std::endl;
            return grpc::Status(grpc::StatusCode::NOT_FOUND, "Record not found");
        }
    }

    // Update a health record for a patient
    Status UpdateHealthRecord(ServerContext* context, const UpdateHealthRecordRequest* request,
                              HealthRecordResponse* reply) override {
        int patient_id = request->patient_data().patient_id();
        health_records_db[patient_id] = request->patient_data();
        reply->mutable_patient_data()->CopyFrom(health_records_db[patient_id]);
        std::cout << "Updated health record for patient: " << patient_id << std::endl;
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    HealthRecordsServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

#ifndef TEST_BUILD
int main(int argc, char** argv) {
        RunServer();
    return 0;
}
#endif

