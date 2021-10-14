#include <iostream>
#include <tuple>

const double MICRO = 1e-6;
const double DR = 64.0 / 3;

std::tuple<double, double, double, double, double> getVariablesFromTari(double tari)
{
    double trcal = 3 * tari * MICRO;
    double rtcal = 2 * tari * MICRO;
    double blf = DR / trcal;
    double tpri = 1 / blf;
    double t1_and_t2 = 1.1*(std::max(rtcal, 10*tpri)) + 2*MICRO + 20*tpri;
    double t1_and_t3 = 2 * 1.1 * (std::max(rtcal, 10*tpri));
    return std::make_tuple(trcal, rtcal, blf, t1_and_t2, t1_and_t3);
}


int main() {
    auto [ trcal, rtcal, blf, t1_and_t2, t1_and_t3 ] = getVariablesFromTari(6.25);
    std::cout << "trcal=" << trcal << ", "
              << "rtcal= " << rtcal << ", "
              << "blf= " << blf << ", "
              << "t1_and_t2= " << t1_and_t2 << ", "
              << "t1_and_t3= " << t1_and_t3 << '\n';
}
