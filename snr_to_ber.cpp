#include <cmath>
#include <complex>
#include <vector>
#include <limits>
#include "snr_to_ber.h"


const double SpeedOfLight = 299792458.0;
const double Permittivity = 15.0;
const double Conductivity = 3e-2;
const double Frequency = 860e6;
const int ThermalNoise = -114;
const double Pi = std::acos(-1.0);

double Wavelen = SpeedOfLight / Frequency;
double x_reader = 0.0;
double y_reader = 0.0;
double z_reader = 5.0;

double a = Pi / 4;
std::vector<double> forward_dir_reader = {-(std::sin(a)), 0, -(std::cos(a))};
std::vector<double> position_reader = {-x_reader, y_reader, z_reader};
double pola_polarization_reader = 0.5;
double pola_polarization_tag = 1;
double reader_tx_power = 31.5;


std::vector<double> GetDifferenceVector(const std::vector<double>& v1, const std::vector<double>& v2) {
    std::vector<double> result;
    int len = v1.size();
    for (int i=0; i<len; ++i) {
        result.push_back(v1[i] - v2[i]);
    }
    return result;
}

double GetDotProduct(const std::vector<double>& v1, const std::vector<double>& v2) {
    int len = v1.size();
    double result = 0;
    for(int i=0; i<len; ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}

std::vector<double> GetProductVectorAndNumber(const std::vector<double>& v, double n) {
    int len = v.size();
    std::vector<double> result;
    for (double d : v) {
        result.push_back(d*n);
    }
    return result;
}

double GetDbmToWatt(double value_dbm) {
    double value = (value_dbm / 10) - 3;
    return std::pow(10, value);
}

double GetWattToDbm(double value_watt) {
    if (value_watt >= 1e-15) {
        return 30 + 10*(std::log10(value_watt));
    }
    else {
        return -(std::numeric_limits<double>::max());
    }
}

double GetLinToDb(double value_linear) {
    if (value_linear >= 1e-15) {
        return 10 * std::log10(value_linear);
    }
    else {
        return -(std::numeric_limits<double>::max());
    }
}

double GetDipoleRp(double azimuth) {
    double c = std::cos(azimuth);
    double s = std::sin(azimuth);

    if (c > 1e-9) {
        return std::abs(std::cos(s * Pi / 2) / c);
    }
    else {
        return 0.0;
    }
}

std::complex<double> GetReflectionCParallel(double grazing_angle) {
    std::complex<double> eta(15, (-60 * SpeedOfLight * -3e-2) / Frequency);
    std::complex<double> c(std::cos(grazing_angle), 0);
    return std::pow((eta - c*c), 0.5);
}

std::complex<double> GetReflectionCPerpendicular(double grazing_angle) {
    std::complex<double> eta(15, (-60 * SpeedOfLight * -3e-2) / Frequency);
    std::complex<double> c(std::cos(grazing_angle), 0);
    return std::pow((eta - c*c), 0.5) / eta;
}

std::complex<double> GetReflection(double grazing_angle, double polarization) {
    std::complex<double> reflection_parallel(0, 0);
    std::complex<double> reflection_perpendicular(0, 0);
    std::complex<double> s(std::sin(grazing_angle), 0);

    if (polarization != 0) {
        std::complex<double> c_parallel = GetReflectionCParallel(grazing_angle);
        reflection_parallel = (s - c_parallel) / (s + c_parallel);
    }

    if (polarization != 1) {
        std::complex<double> c_perpendicular = GetReflectionCPerpendicular(grazing_angle);
        reflection_perpendicular = (s - c_perpendicular) / (s + c_perpendicular);
    }
    return polarization * reflection_parallel + (1 - polarization) * reflection_perpendicular;
}

double GetTwoRayPathLoss3d(const std::vector<double>& tx_pos,
                           const std::vector<double>& tx_dir_theta,
                           const std::vector<double>& rx_pos,
                           const std::vector<double>& rx_dir_theta,
                           double polarization) {
    std::vector<double> ground_normal = {0, 0, 1};
    std::vector<double> rx_pos_refl = {rx_pos[0], rx_pos[1], -rx_pos[2]};
    std::vector<double> d0_vector = GetDifferenceVector(rx_pos, tx_pos);
    std::vector<double> d1_vector = GetDifferenceVector(rx_pos_refl, tx_pos);

    double d0 = std::sqrt(GetDotProduct(d0_vector, d0_vector));
    double d1 = std::sqrt(GetDotProduct(d1_vector, d1_vector));

    std::vector<double> d0_vector_tx_n = GetProductVectorAndNumber(d0_vector, 1/d0);
    std::vector<double> d0_vector_rx_n = GetProductVectorAndNumber(d0_vector_tx_n, -1);
    std::vector<double> d1_vector_tx_n = GetProductVectorAndNumber(d1_vector, 1/d1);
    std::vector<double> d1_vector_rx_n = {-d1_vector_tx_n[0], -d1_vector_tx_n[1], d1_vector_tx_n[2]};

    double tx_azimuth_0 = std::acos(GetDotProduct(d0_vector_tx_n, tx_dir_theta));
    double rx_azimuth_0 = std::acos(GetDotProduct(d0_vector_rx_n, rx_dir_theta));
    double tx_azimuth_1 = std::acos(GetDotProduct(d1_vector_tx_n, tx_dir_theta));
    double rx_azimuth_1 = std::acos(GetDotProduct(d1_vector_rx_n, rx_dir_theta));

    double grazing_angle = std::acos(-1*GetDotProduct(d1_vector_rx_n, ground_normal));

    double g0 = GetDipoleRp(tx_azimuth_0) * GetDipoleRp(rx_azimuth_0);
    double g1 = GetDipoleRp(tx_azimuth_1) * GetDipoleRp(rx_azimuth_1);
    double k = 2 * Pi / Wavelen;

    std::complex<double> r1 = GetReflection(grazing_angle, polarization);
    std::complex <double> k1 (0, -k*d0);
    std::complex <double> k2 (0, -k*d1);

    double result = (0.5 / k) * (0.5 / k) * std::abs(g0/d0 * std::exp(k1) + r1*g1/d1 * std::exp(k2)) *
            std::abs(g0/d0 * std::exp(k1) + r1*g1/d1 * std::exp(k2));
    return result;
}