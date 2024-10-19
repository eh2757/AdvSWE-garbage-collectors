#include <iostream>
#include "../src/health_records.pb.h"
#include "../src/health_records.grpc.pb.h"
#include "../src/health_records_server.cpp"

// Test for adding a health record
bool testAddHealthRecord() {
    health::PatientData request;
    request.set_patient_id(1);
    request.add_medications()->set_name("Aspirin");

    health::HealthRecordResponse response;

    HealthRecordsServiceImpl service;
    grpc::ServerContext context;
    grpc::Status status = service.AddHealthRecord(&context, &request, &response);

    return status.ok() && 
           response.patient_data().patient_id() == 1 &&
           response.patient_data().medications(0).name() == "Aspirin";
}

// Test for retrieving a health record
bool testGetHealthRecord() {
    health::HealthRecordRequest request;
    request.set_patient_id(1);

    health::HealthRecordResponse response;

    HealthRecordsServiceImpl service;
    grpc::ServerContext context;
    grpc::Status status = service.GetHealthRecord(&context, &request, &response);

    return status.ok() && 
           response.patient_data().patient_id() == 1 &&
           response.patient_data().medications(0).name() == "Aspirin";
}

// Test for updating a health record
bool testUpdateHealthRecord() {
    health::UpdateHealthRecordRequest request;
    request.mutable_patient_data()->set_patient_id(1);
    request.mutable_patient_data()->add_medications()->set_name("Ibuprofen");

    health::HealthRecordResponse response;

    HealthRecordsServiceImpl service;
    grpc::ServerContext context;
    grpc::Status status = service.UpdateHealthRecord(&context, &request, &response);

    return status.ok() && 
           response.patient_data().patient_id() == 1 &&
           response.patient_data().medications(0).name() == "Ibuprofen";
}

int main() {
    // Run all the tests
    bool allTestsPassed = true;

    if (!testAddHealthRecord()) {
        std::cout << "testAddHealthRecord FAILED" << std::endl;
        allTestsPassed = false;
    } else {
        std::cout << "testAddHealthRecord PASSED" << std::endl;
    }

    if (!testGetHealthRecord()) {
        std::cout << "testGetHealthRecord FAILED" << std::endl;
        allTestsPassed = false;
    } else {
        std::cout << "testGetHealthRecord PASSED" << std::endl;
    }

    if (!testUpdateHealthRecord()) {
        std::cout << "testUpdateHealthRecord FAILED" << std::endl;
        allTestsPassed = false;
    } else {
        std::cout << "testUpdateHealthRecord PASSED" << std::endl;
    }

    // Print the final result
    if (allTestsPassed) {
        std::cout << "All tests PASSED" << std::endl;
    } else {
        std::cout << "Some tests FAILED" << std::endl;
    }

    return allTestsPassed ? 0 : 1;
}
