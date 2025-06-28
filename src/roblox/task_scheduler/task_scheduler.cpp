#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <wininet.h>
#include <ShlObj.h>

#include "../rbx_engine.h"
#include "../../misc/output/output.hpp"
#include "../../misc/globals/globals.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include "../../driver/driver_impl.hpp"

uintptr_t RBX::TaskScheduler::GetScheduler() {
    //   utils::output::info("Fetching Scheduler. Virtual Address: " + std::to_string(virtualaddy));
    uintptr_t result = virtualaddy ? read<uintptr_t>(virtualaddy + 0x6829508) : 0;
    utils::output::info("Scheduler Address: " + std::to_string(result));
    return result;
}

std::vector<RBX::Instance> RBX::TaskScheduler::GetJobs() {
    std::vector<RBX::Instance> jobs;

    if (!GetScheduler()) {
        utils::output::warning("No scheduler found. Returning empty job list.");
        return jobs;
    }

    uintptr_t jobStart = read<uintptr_t>(GetScheduler() + 0x1D0);
    uintptr_t jobEnd = read<uintptr_t>(GetScheduler() + 0x1D8);
    utils::output::info("Job Start: " + std::to_string(jobStart) + " Job End: " + std::to_string(jobEnd));

    if (jobStart && jobEnd && jobStart < jobEnd) {
        for (uintptr_t job = jobStart; job < jobEnd; job += 0x10) {
            uintptr_t jobAddress = read<uintptr_t>(job);
            utils::output::info("Job Address: " + std::to_string(jobAddress));
            if (jobAddress) jobs.emplace_back(jobAddress);
        }
    }
    return jobs;
}

// Helper function to display bytes as hex for debugging
std::string dumpHexBytes(uintptr_t address, size_t length) {
    std::stringstream ss;
    ss << "HEX[";
    
    for (size_t i = 0; i < length; i++) {
        uint8_t byte = read<uint8_t>(address + i);
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        if (i < length - 1) ss << " ";
    }
    
    ss << "]";
    return ss.str();
}

std::string RBX::TaskScheduler::GetJobName(RBX::Instance instance) {
    if (!instance.address) {
        utils::output::warning("GetJobName: Received null instance address");
        return "";
    }
    
    uintptr_t nameAddress = instance.address + 0x18;
    
    // First, check if we can read the name pointer and length
    std::string jobName;
    
    try {
        // Read the string directly using readString
        jobName = mem::readString(nameAddress);
        
        // Debug output
        utils::output::info("Job Address: " + std::to_string(instance.address) + 
                           " Name: '" + jobName + "' (Length: " + std::to_string(jobName.length()) + ")");
        
        // If we got an empty string, try to manually inspect the memory
        if (jobName.empty()) {
            // Try to read the string metadata
            int stringLength = read<int>(nameAddress + 0x18);
            uintptr_t stringPtr = read<uintptr_t>(nameAddress);
            
            utils::output::info("String debug - Address: " + std::to_string(nameAddress) + 
                               " Length: " + std::to_string(stringLength) + 
                               " StringPtr: " + std::to_string(stringPtr));
            
            // Dump some bytes for inspection
            utils::output::info("First 16 bytes at name address: " + 
                               dumpHexBytes(nameAddress, 16));
            
            if (stringPtr && stringLength > 0 && stringLength < 1000) {
                utils::output::info("First 16 bytes at string pointer: " + 
                                   dumpHexBytes(stringPtr, (std::min)(16, stringLength)));
            }
        }
    }
    catch (const std::exception& e) {
        utils::output::warning("Exception in GetJobName: " + std::string(e.what()));
        jobName = "ERROR_READING";
    }
    
    return jobName;
}

uint64_t RBX::TaskScheduler::GetJobByName(const std::string& targetName) {
    utils::output::info("Searching for Job: " + targetName);
    for (const auto& job : GetJobs()) {
        std::string jobName = GetJobName(job);
        if (jobName == targetName) {
            utils::output::info("Found Job " + targetName + " at address: " + std::to_string(job.address));
            return job.address;
        }
    }
    utils::output::warning("Job " + targetName + " not found.");
    return 0;
}

uint64_t RBX::TaskScheduler::GetRenderJob() {
    utils::output::info("Fetching RenderJob");
    return GetJobByName("RenderJob");
}

unsigned int RBX::TaskScheduler::GetTargetFPS() {
    double rawFps = read<double>(GetScheduler() + 0x178);
    return static_cast<unsigned int>(1.0 / rawFps);
}

void RBX::TaskScheduler::SetTargetFPS(unsigned int targetFps) {
    if (targetFps > 0) {
        double newFps = 1.0 / static_cast<double>(targetFps);
        write<double>(GetScheduler() + 0x178, newFps);
    }
}

void RBX::TaskScheduler::PauseTask(uintptr_t jobAddress) {
    if (jobAddress) {
        write<bool>(jobAddress + 0x10, true);
        utils::output::print("Task at " + std::to_string(jobAddress) + " paused.");
    }
}

void RBX::TaskScheduler::ResumeTask(uintptr_t jobAddress) {
    if (jobAddress) {
        write<bool>(jobAddress + 0x10, false);
        utils::output::print("Task at " + std::to_string(jobAddress) + " resumed.");
    }
}

bool RBX::TaskScheduler::RemoveTaskByName(const std::string& targetName) {
    uintptr_t jobListStart = read<uintptr_t>(GetScheduler() + 0x1D0);
    for (uintptr_t currentJob = jobListStart; ; currentJob += 0x10) {
        uintptr_t jobAddress = read<uintptr_t>(currentJob);
        if (jobAddress == 0) break;

        std::string jobName = mem::readString(jobAddress + 0x18);
        if (jobName == targetName) {
            write<uintptr_t>(currentJob, 0);
            utils::output::print("Removed job with name: " + jobName);
            return true;
        }
    }
    return false;
}

unsigned int RBX::TaskScheduler::GetJobCount() {
    unsigned int jobCount = 0;
    uintptr_t jobListStart = read<uintptr_t>(GetScheduler() + 0x1D0);
    for (uintptr_t currentJob = jobListStart; ; currentJob += 0x10) {
        uintptr_t jobAddress = read<uintptr_t>(currentJob);
        if (jobAddress == 0) break;
        ++jobCount;
    }
    return jobCount;
}

void RBX::TaskScheduler::GetJobDetails(uintptr_t jobAddress) {
    if (jobAddress) {
        std::string jobName = mem::readString(jobAddress + 0x18);
        unsigned int jobPriority = read<unsigned int>(jobAddress + 0xC);
        utils::output::print("Job Name: " + jobName + ", Job Priority: " + std::to_string(jobPriority));
    }
}

void RBX::TaskScheduler::QueueTask(uintptr_t taskAddress) {
    uintptr_t schedulerAddress = GetScheduler();
    uintptr_t jobListStart = read<uintptr_t>(schedulerAddress + 0x1D0);
    write<uintptr_t>(jobListStart, taskAddress);
    utils::output::print("Task queued at address: " + std::to_string(taskAddress));
}

void RBX::TaskScheduler::ClearScheduler() {
    uintptr_t schedulerAddress = GetScheduler();
    uintptr_t jobListStart = read<uintptr_t>(schedulerAddress + 0x1D0);
    write<uintptr_t>(jobListStart, 0);
    utils::output::print("Scheduler cleared.");
}

bool RBX::TaskScheduler::IsTaskRunning(uintptr_t jobAddress) {
    return read<bool>(jobAddress + 0x14);
}

void RBX::TaskScheduler::UpdateJobPriority(uintptr_t jobAddress, unsigned int newPriority) {
    if (jobAddress) {
        write<unsigned int>(jobAddress + 0xC, newPriority);
        utils::output::print("Updated priority of job at " + std::to_string(jobAddress) + " to " + std::to_string(newPriority));
    }
}
