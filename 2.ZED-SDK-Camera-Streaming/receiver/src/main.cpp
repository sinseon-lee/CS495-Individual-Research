// ZED includes
#include <sl/Camera.hpp>

// OpenCV include (for display)
#include <opencv2/opencv.hpp>

// Using std and sl namespaces
using namespace std;
using namespace sl;

std::vector<std::string> split(const std::string& s, char seperator) {
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos) {
        std::string substring(s.substr(prev_pos, pos - prev_pos));
        output.push_back(substring);
        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos));
    return output;
}

int main(int argc, char **argv) {
    Camera zed;

    // Set configuration parameters for the ZED
    InitParameters initParameters;
    initParameters.depth_mode = DEPTH_MODE::ULTRA;
    initParameters.camera_fps = 60;
    initParameters.sdk_verbose = true;

    if (argc > 1) {
        std::vector<std::string> configStream = split(std::string(argv[1]), ':');        
        sl::String ip = sl::String(configStream.at(0).c_str());
        if (configStream.size() == 2) {
            initParameters.input.setFromStream(ip, atoi(configStream.at(1).c_str()));
        } else initParameters.input.setFromStream(ip);
    } else {
        std::cout << "Opening the stream requires the IP of the sender\n Usage : ./ZED_Streaming_Receiver IP:[port]" << std::endl;
        return 1;
    }

    // Open the camera
    ERROR_CODE zed_error = zed.open(initParameters);

    if (zed_error != ERROR_CODE::SUCCESS) {// Quit if an error occurred
        cout << zed_error << endl;
        zed.close();
        return 1;
    }

    // Create a Mat to store images
    Mat zed_image;

    char key = ' ';
    while (key != 'q') {
	// Check that grab() is successful
        if (zed.grab() == ERROR_CODE::SUCCESS) {
	    // Retrieve image
            zed.retrieveImage(zed_image, VIEW::SIDE_BY_SIDE);

            // Display image with OpenCV
            cv::imshow("VIEW", cv::Mat((int) zed_image.getHeight(), (int) zed_image.getWidth(), CV_8UC4, zed_image.getPtr<sl::uchar1>(sl::MEM::CPU)));

            key = cv::waitKey(10);

            printf("fps : %f\n", zed.getCurrentFPS());
        }
        else
        {
            key = cv::waitKey(20);
        }
    }

    // Exit
    zed.close();
    return 0;
}
