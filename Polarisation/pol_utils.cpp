//
// Created by Amos on 01/11/2019.
//

#include "pol_utils.h"
#include <ctime>
#include <iostream>
#include "Eigen/LU"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace DDD;

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
pol::compute_polInfo(std::vector<Eigen::MatrixXd> &_img, const std::vector<double> &_angles,
                     const Eigen::MatrixXi &_mask) {
    /*
     * 1. Vectorise the input images
     */
    size_t nPolAng = _angles.size();
    Eigen::MatrixXd A(nPolAng, 3);
    for (size_t i = 0; i < nPolAng; ++i) {
        A(i, 0) = 1;
        A(i, 1) = std::cos(2 * _angles[i] * M_PI / 180);
        A(i, 2) = std::sin(2 * _angles[i] * M_PI / 180);
    }

    /*
     * 2. Construct right
     */
    clock_t begin = std::clock();
//    int N = _mask.rows() * _mask.cols();
    Eigen::MatrixXd RHS(nPolAng, _mask.size());
    for (int i = 0; i < nPolAng; ++i) {
        Eigen::Map<Eigen::RowVectorXd> v_imgi(_img[i].data(), _img[i].size());

        Eigen::ArrayXd arrayA;
        Eigen::ArrayXd aa = (arrayA > 5).select(_mask, arrayA);
        RHS.row(i) = v_imgi;
    }


    /*
     * 3. Solve
     */
    Eigen::MatrixXd X = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(RHS);
    Eigen::RowVectorXd x = X.row(0);
    Eigen::RowVectorXd y = X.row(1);
    Eigen::RowVectorXd z = X.row(2);

    Eigen::RowVectorXd rho = y.array().square() + z.array().square();
    rho = rho.array().sqrt();
    rho = rho.array() / z.array();

    Eigen::RowVectorXd phi = z.array().sin().array() / y.array().sin().array();
    phi /= 2;
    phi.array() += M_PI / 2;

    /*
     * 4. Reshape back to image size
     */
    Eigen::Map<Eigen::MatrixXd> Iun_matrix(x.data(), _mask.rows(), _mask.cols());
    Eigen::Map<Eigen::MatrixXd> rho_matrix(rho.data(), _mask.rows(), _mask.cols());
    Eigen::Map<Eigen::MatrixXd> phi_matrix(phi.data(), _mask.rows(), _mask.cols());


    clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "elapsed time: " << elapsed_secs << std::endl;

    return std::make_tuple(Iun_matrix, rho_matrix, phi_matrix);
}