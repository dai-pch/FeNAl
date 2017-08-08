#ifndef SIGPRO_KALMAN_H
#define SIGPRO_KALMAN_H

#include "number/Matrix.h"

namespace SigPro {
    using DT = double;
    class KalmanFilter {

    public:
        KalmanFilter(const Number::Matrix<DT>& init_state, const Number::Matrix<DT>& state_covariance,
            const Number::Matrix<DT>& transition, const Number::Matrix<DT>& process_covariance,
            const Number::Matrix<DT>& observation, const Number::Matrix<DT>& observation_covariance,
            const Number::Matrix<DT>& controlling = Number::Matrix<DT>());
    
    public:
        void step(const Number::Matrix<DT>& controlling, const Number::Matrix<DT>& observation);
        void predict(const Number::Matrix<DT>& controlling);
        void observe(const Number::Matrix<DT>& observation);
        Number::Matrix<DT> state() const;
        Number::Matrix<DT> covariance() const;

    private:
        // size_t _dim;
        Number::Matrix<DT> _state;
        Number::Matrix<DT> _state_covariance;

        Number::Matrix<DT> _transition;
        Number::Matrix<DT> _process_covariance;
        Number::Matrix<DT> _observation;
        Number::Matrix<DT> _observation_covariance;
        bool _have_control;
        Number::Matrix<double> _controlling;
    };
}

#endif // SIGPRO_KALMAN_H