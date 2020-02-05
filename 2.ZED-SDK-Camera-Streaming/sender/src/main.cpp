// ZED includes
#include <sl/Camera.hpp>

// Sample includes
#include "utils.hpp"

// Using namespace
using namespace sl;

int main(int argc, char **argv) {
    // Create a ZED camera
    Camera zed;

    // Set configuration parameters for the ZED
    InitParameters initParameters;
    initParameters.camera_resolution = RESOLUTION::HD720;
    initParameters.camera_fps = 60;
    initParameters.depth_mode =	DEPTH_MODE::ULTRA;
    initParameters.sdk_verbose = true;

    // Open the camera
    ERROR_CODE err = zed.open(initParameters);
    if (err != ERROR_CODE::SUCCESS) {
        std::cout << toString(err) << std::endl;
        zed.close();
        return -1; // Quit if an error occurred
    }

    sl::StreamingParameters stream_params;
    stream_params.codec = STREAMING_CODEC::H265;
    stream_params.bitrate = 6000;
    if(argc > 1) stream_params.port = atoi(argv[1]);

    err = zed.enableStreaming(stream_params);
    if (err != ERROR_CODE::SUCCESS) {
        std::cout << "Streaming initialization error. " << toString(err) << std::endl;
        zed.close();
        return -2;
    }

    std::cout << "Streaming on port " << stream_params.port << std::endl;

    SetCtrlHandler();

    int fc = 0;
    while (!exit_app) {
        if (zed.grab() == ERROR_CODE::SUCCESS) {
            sl::sleep_ms(1);
            fc++;
        }
    }

    // Stop recording
    zed.disableStreaming();
    zed.close();
    return 0;
}
