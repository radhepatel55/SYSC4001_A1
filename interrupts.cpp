/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include"interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
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
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if(activity == "CPU") {
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU burst\n";
            current_time += duration_intr;
        } 

        else if(activity == "SYSCALL" || activity == "END_IO") {
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

            // Step 3: Find vector in memory

            // Step 4: Obtain ISR address

            // Step 5: Call device driver

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
