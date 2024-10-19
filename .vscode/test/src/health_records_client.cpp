#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "health_records.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using health::HealthRecordsService;
using health::PatientData;
using health::Medication;
using health::HealthRecordRequest;
using health::HealthRecordResponse;
using health::UpdateHealthRecordRequest;

class HealthRecordsClient {
public:
    HealthRecordsClient(std::shared_ptr<Channel> channel)
        : stub_(HealthRecordsService::NewStub(channel)) {}

    // Add a health record for a patient
    void AddHealthRecord(int patient_id, const std::string& med_name, const std::string& dosage) {
        PatientData patient_data;
        patient_data.set_patient_id(patient_id);
        Medication* med = patient_data.add_medications();
        med->set_medication_id(1);  // Simplified for demo
        med->set_name(med_name);
        med->set_dosage(dosage);

        HealthRecordResponse reply;
        ClientContext context;
        Status status = stub_->AddHealthRecord(&context, patient_data, &reply);
        if (status.ok()) {
            std::cout << "Added record for patient " << patient_id << std::endl;
        } else {
            std::cout << "Failed to add record: " << status.error_message() << std::endl;
        }
    }

    // Get a health record for a patient
    void GetHealthRecord(int patient_id) {
        HealthRecordRequest request;
        request.set_patient_id(patient_id);

        HealthRecordResponse reply;
        ClientContext context;
        Status status = stub_->GetHealthRecord(&context, request, &reply);
        if (status.ok()) {
            std::cout << "Patient ID: " << reply.patient_data().patient_id() << std::endl;
            for (const auto& med : reply.patient_data().medications()) {
                std::cout << "Medication: " << med.name() << ", Dosage: " << med.dosage() << std::endl;
            }
        } else {
            std::cout << "Failed to retrieve record: " << status.error_message() << std::endl;
        }
    }

private:
    std::unique_ptr<HealthRecordsService::Stub> stub_;
};

int main(int argc, char** argv) {
    HealthRecordsClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    // Add a health record
    client.AddHealthRecord(1, "Aspirin", "50mg");

    // Retrieve the health record
    client.GetHealthRecord(1);

    return 0;
}
