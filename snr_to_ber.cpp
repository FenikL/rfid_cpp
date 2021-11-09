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

double wavelen = SpeedOfLight / Frequency;
double x_reader = 0.0;
double y_reader = 0.0;
double z_reader = 5.0;

double a = Pi / 4;
std::vector<double> forward_dir_reader = {-(std::sin(a)), 0, -(std::cos(a))};
std::vector<double> position_reader = {-x_reader, y_reader, z_reader};
double pola_polarization_reader = 0.5;
double pola_polarization_tag = 1;
double reader_tx_power = 31.5;


double GetDbmToWatt(double value_dbm)
{
    double value = (value_dbm / 10) - 3;
    return std::pow(10, value);
}

double GetWattToDbm(double value_watt)
{
    if (value_watt >= 1e-15)
    {
        return 30 + 10*(std::log10(value_watt));
    }
    else
    {
        return -(std::numeric_limits<double>::max());
    }
}

double GetLinToDb(double value_linear)
{
    if (value_linear >= 1e-15)
    {
        return 10 * std::log10(value_linear);
    }
    else
    {
        return -(std::numeric_limits<double>::max());
    }
}

double GetDipoleRp(double azimuth)
{
    double c = std::cos(azimuth);
    double s = std::sin(azimuth);

    if (c > 1e-9)
    {
        return std::abs(std::cos(s * Pi / 2) / c);
    }
    else
    {
        return 0.0;
    }
}

std::complex<double> GetReflectionCParallel(double grazing_angle)
{
    std::complex<double> eta(15, (-60 * SpeedOfLight * -3e-2) / Frequency);
    std::complex<double> c(std::cos(grazing_angle), 0);
    return std::pow((eta - c*c), 0.5);
}

std::complex<double> GetReflectionCPerpendicular(double grazing_angle)
{
    std::complex<double> eta(15, (-60 * SpeedOfLight * -3e-2) / Frequency);
    std::complex<double> c(std::cos(grazing_angle), 0);
    return std::pow((eta - c*c), 0.5) / eta;
}

std::complex<double> GetReflection(double grazing_angle, double polarization)
{
    std::complex<double> reflection_parallel(0, 0);
    std::complex<double> c_perpendicular;
    std::complex<double> s(std::sin(grazing_angle), 0);

    if (polarization != 0)
    {
        std::complex<double> c_parallel = GetReflectionCParallel(grazing_angle);
        reflection_parallel = (s - c_parallel) / (s + c_parallel);
    }
    else
    {
        reflection_parallel =  (0, 0);
    }

    if (polarization != 1)
    {
        c_perpendicular = GetReflectionCPerpendicular(grazing_angle);
        std::complex<double> reflection_perpendicular = (s - c_perpendicular) / (s + c_perpendicular);
    }
    else
    {
        std::complex<double> c_perpendicular(0, 0);
    }

    return polarization * reflection_parallel + (1 - polarization) * reflection_perpendicular;
}