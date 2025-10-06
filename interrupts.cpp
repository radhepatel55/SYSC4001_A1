/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include "interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elements is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0;
    int device_num;
    int delay;
    /******************************************************************/

    //parse each line of the input trace file
    while (std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU burst\n";
            current_time += duration_intr;
        }

        else if (activity == "SYSCALL" || activity == "END_IO") {
            // Use device number for lookup
            device_num = duration_intr;
            delay = delays[device_num];
            std::string vector_addr = vectors[device_num];

            // Step 1: Switch to kernel mode
            execution += std::to_string(current_time) + ", 1, switch to kernel mode\n";
            current_time += 1;

            // Step 2: Context save
            execution += std::to_string(current_time) + ", 10, context saved\n";
            current_time += 10;

            // Step 3: Find vector in memory position
            {
                char pos_hex[16];
                std::snprintf(pos_hex, sizeof(pos_hex), "0x%04X", (ADDR_BASE + device_num * VECTOR_SIZE));
                execution += std::to_string(current_time) + ", 1, find vector " + std::to_string(device_num)
                    + " in memory position " + pos_hex + "\n";
                current_time += 1;
            }

            // Step 4: Load ISR address into PC
            execution += std::to_string(current_time) + ", 1, load address " + vector_addr + " into the PC\n";
            current_time += 1;

            // Step 5: Execute ISR body (simulate device service routine)
            int overhead = 1 + 10 + 1 + 1 + 1;   // (steps 1, 2, 3, 4, 6)
            int body_ms = delay - overhead;
            if (body_ms < 0) body_ms = 0;

            std::string label = (activity == "SYSCALL")
                ? ("call device driver (SYSCALL dev " + std::to_string(device_num) + ")")
                : ("store information in memory (END_IO dev " + std::to_string(device_num) + ")");
            execution += std::to_string(current_time) + ", " + std::to_string(body_ms) + ", " + label + "\n";
            current_time += body_ms;

            // Step 6: IRET
            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time += 1;
        }

        /************************************************************************/
    }

    input_file.close();
    write_output(execution);
    return 0;
}
