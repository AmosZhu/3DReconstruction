//
// Created by Amos on 01/11/2019.
//

#ifndef INC_3DRECONSTRUCTION_POL_UTILS_H
#define INC_3DRECONSTRUCTION_POL_UTILS_H

#include <tuple>
#include <vector>
#include "Eigen/Dense"

namespace DDD {
    class pol {

    public:
        /*
         * Get the unpolarised intensity, degree of polarisation and phase angle from polarimetric images;
         */
        static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
        compute_polInfo(std::vector<Eigen::MatrixXd> &_img, const std::vector<double> &_angles,
                        const Eigen::MatrixXi &_mask = Eigen::MatrixXi::Ones());
    };

}

#endif //INC_3DRECONSTRUCTION_POL_UTILS_H
