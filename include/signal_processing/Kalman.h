#ifndef SIGPRO_KALMAN_H
#define SIGPRO_KALMAN_H

#include <functional>
#include "number/Matrix.h"

namespace SigPro {
    class KalmanFilter {
    using DT = double;
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
        Number::Matrix<DT> _controlling;
    };

    class EKF {
    using DT = double;
    public:
        EKF(const Number::Matrix<DT>& init_state, const Number::Matrix<DT>& state_covariance,
            std::function<Number::Matrix<DT>(Number::Matrix<DT>, 
                Number::Matrix<DT>)> transition,
            std::function<Number::Matrix<DT>(Number::Matrix<DT>, 
                Number::Matrix<DT>)> transition_derivative,
            const Number::Matrix<DT>& process_covariance,
            std::function<Number::Matrix<DT>(Number::Matrix<DT>)> observe, 
            std::function<Number::Matrix<DT>(Number::Matrix<DT>)> observe_derivative, 
            const Number::Matrix<DT>& observation_covariance);
    
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

        std::function<Number::Matrix<DT>(
            Number::Matrix<DT>,
            Number::Matrix<DT>)> _transition;
        std::function<Number::Matrix<DT>(
            Number::Matrix<DT>,
            Number::Matrix<DT>)> _transition_derivative;
        Number::Matrix<DT> _process_covariance;
        std::function<Number::Matrix<DT>(
            Number::Matrix<DT>)> _observe;
        std::function<Number::Matrix<DT>(
            Number::Matrix<DT>)> _observe_derivative;
        Number::Matrix<DT> _observation_covariance;
    };
}

#endif // SIGPRO_KALMAN_H