#ifndef RFID_SNR_TO_BER_H
#define RFID_SNR_TO_BER_H

std::vector<double> GetDifferenceVector(const std::vector<double>& v1,
                                        const std::vector<double>& v2);

double GetDotProduct(const std::vector<double>& v1,
                     const std::vector<double>& v2);

std::vector<double> GetProductVectorAndNumber(
        const std::vector<double>& v, double n);

double GetDbmToWatt(double value_dbm);

double GetWattToDbm(double value_watt);

double GetDbToLin(double value_db);
double GetLinToDb(double value_linear);
double GetDipoleRp(double azimuth);

std::complex<double> GetReflectionCParallel(double grazing_angle);

std::complex<double> GetReflectionCPerpendicular(double grazing_angle);
std::complex<double> GetReflection(double grazing_angle, double polarization);
double GetTwoRayPathLoss3d(const std::vector<double>& tx_pos,
                           const std::vector<double>& tx_dir_theta,
                           const std::vector<double>& rx_pos,
                           const std::vector<double>& rx_dir_theta,
                           double polarization);
double GetPathLossForReader(double x);
double GetPathLossForTag(double x);

double GetRxPower(double x, double path_loss,
                  double tx_power, double cable_loss,
                  double tag_modulation_loss);
double GetReaderPower(double x);

double GetTagPower(double x);

double GetSignalToNoise(double rx_power, double noise_power);

double GetSyncAngle(double snr, double preamble_duration);

double GetSnrExtended(double snr, double sync_phi, int miller, double symbol_duration);
double GetSnr(double rx_power, int miller, double preamble_duration, double blf);

double GetBerOverRayleigh(double snr);

#endif //RFID_SNR_TO_BER_H
