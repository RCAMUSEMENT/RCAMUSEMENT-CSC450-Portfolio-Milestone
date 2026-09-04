/**
 * @file main.cpp
 * @brief Submersible Ballast and Hydrostatic Pressure Concurrency Simulator
 * @details Establishes an ironclad, race-condition-free real-time telemetry pipeline.
 * @author Ryley Carlson
 * @course CSC450: Programming III
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Independent mutex to guarantee the atomicity of standard console output operations
std::mutex consoleIOMutex;

/**
 * @struct SubmersibleTelemetry
 * @brief Encapsulates synchronized telemetry variables inside an aligned block.
 */
struct alignas(64) SubmersibleTelemetry {
    std::mutex hullMutex;
    std::condition_variable ascentDescentBarrier;
    int depthPressurePSI = 0;
    bool divingPhaseComplete = false;
};

/**
 * @brief Simulates ballast tanks flooding, increasing hydrostatic pressure up to 20 PSI.
 * @param telemetry Reference to the encapsulated submersible telemetry state.
 */
void executeBallastDive(SubmersibleTelemetry& telemetry) {
    for (int psiIndex = 1; psiIndex <= 20; ++psiIndex) {
        {
            std::lock_guard<std::mutex> lock(telemetry.hullMutex);
            telemetry.depthPressurePSI = psiIndex;
        }

        {
            std::lock_guard<std::mutex> ioLock(consoleIOMutex);
            std::cout << "[Telemetry Thread 1] Ballast Intake Active - Hull Hydrostatic Pressure: "
                    << psiIndex << " PSI\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    {
        std::lock_guard<std::mutex> lock(telemetry.hullMutex);
        telemetry.divingPhaseComplete = true;
    }
    telemetry.ascentDescentBarrier.notify_one();
}

/**
 * @brief Simulates blowing ballast tanks, venting pressure back down to 0 PSI.
 * @param telemetry Reference to the encapsulated submersible telemetry state.
 */
void executeBallastAscent(SubmersibleTelemetry& telemetry) {
    {
        std::unique_lock<std::mutex> lock(telemetry.hullMutex);
        telemetry.ascentDescentBarrier.wait(lock, [&telemetry] { 
            return telemetry.divingPhaseComplete; 
        });
    }

    while (true) {
        int localCurrentPressure = 0;

        {
            std::lock_guard<std::mutex> lock(telemetry.hullMutex);
            if (telemetry.depthPressurePSI < 0) {
                break;
            }
            localCurrentPressure = telemetry.depthPressurePSI;
            --telemetry.depthPressurePSI;
        }

        {
            std::lock_guard<std::mutex> ioLock(consoleIOMutex);
            std::cout << "[Telemetry Thread 2] Ascent Initiated - Blowing Ballast - Hull Hydrostatic Pressure: " 
                    << localCurrentPressure << " PSI\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}

int main() {
    {
        std::lock_guard<std::mutex> ioLock(consoleIOMutex);
        std::cout << "=========================================================\n";
        std::cout << "  LAUNCHING SUBMERSIBLE CONCURRENT TELEMETRY SIMULATOR  \n";
        std::cout << "=========================================================\n";
    }

    SubmersibleTelemetry submersibleInstance;

    std::thread diveThread(executeBallastDive, std::ref(submersibleInstance));
    std::thread ascentThread(executeBallastAscent, std::ref(submersibleInstance));

    diveThread.join();
    ascentThread.join();

    {
        std::lock_guard<std::mutex> ioLock(consoleIOMutex);
        std::cout << "=========================================================\n";
        std::cout << "  MISSION-CRITICAL SYSTEMS TERMINATED CLEANLY            \n";
        std::cout << "=========================================================\n";
    }

    return 0;
}