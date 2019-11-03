//
// Created by Amos on 01/11/2019.
//

#include <iostream>
#include "Eigen/Dense"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ostream>
#include <iomanip>
#include "Polarisation/pol_utils.h"


#define IMAGE_PATH ("D:/Dataset/Pol/TwoViewAt100cm/Light2/GoatSameCam/R/")
#define IMAGE_NUM 7
#define IMAGE_START_NUM 64

#define NUM_DOWNSIZE (2)

int main(int argc, char *argv[]) {
    // Prepare matrices
    std::vector<double> polAng = {0, 30, 60, 90, 120, 150, 180};
    std::vector<Eigen::MatrixXd> Ipol(IMAGE_NUM);  // Store the polarisation images

    std::ostringstream strStream;
    for (int i = 0; i < IMAGE_NUM; ++i) {
        strStream.str("");
        strStream << IMAGE_PATH << "IMGREAL_" << std::setfill('0') << std::setw(4) << IMAGE_START_NUM + i << ".BMP";
        std::cout << strStream.str() << std::endl;
        cv::Mat img = cv::imread(strStream.str(), 0);
        if (img.empty()) {
            std::cout << strStream.str() << " not found!" << std::endl;
            return -1;
        }

        // Resize to 3 times smaller
        for (int j = 0; j < NUM_DOWNSIZE; ++j) {
            cv::pyrDown(img, img, cv::Size(img.cols / 2, img.rows / 2));
        }

        img.convertTo(img, CV_64F, 1.0 / 255.0);
        cv::cv2eigen(img, Ipol[i]);
//        std::cout << Ipol[i].coeff(1000, 1000) << std::endl;
    }
    // Load mask
    strStream.str("");
    strStream << IMAGE_PATH << "mask.png";
    cv::Mat mask_img = cv::imread(strStream.str(), 0);
    if (mask_img.empty()) {
        std::cout << "mask image not found" << std::endl;
        return -1;
    }

    for (int j = 0; j < NUM_DOWNSIZE; ++j) {
        cv::pyrDown(mask_img, mask_img, cv::Size(mask_img.cols / 2, mask_img.rows / 2));
    }

    Eigen::MatrixXi mask;
    cv::cv2eigen(mask_img, mask);

    auto info = DDD::pol::compute_polInfo(Ipol, polAng, mask);
    cv::Mat Iun, rho, phi;
    cv::eigen2cv(std::get<0>(info), Iun);
    cv::eigen2cv(std::get<1>(info), rho);
    cv::eigen2cv(std::get<2>(info), phi);

    cv::imshow("unpolarised intesnity", Iun);

    cv::Mat rho_disp;
    cv::applyColorMap(rho, rho_disp, cv::COLORMAP_JET);
    cv::imshow("degree of polarisation", rho_disp);
    cv::imshow("phase angle", phi);

    cv::waitKey();
    cv::destroyAllWindows();

    return 0;
}