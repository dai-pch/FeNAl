#include "Kalman.h"
#include "number/Matrix.h"

using namespace std;
using namespace Number;

namespace SigPro {
    KalmanFilter::KalmanFilter(const Matrix<KalmanFilter::DT>& init_state,
        const Matrix<KalmanFilter::DT>& state_covariance,
        const Matrix<KalmanFilter::DT>& transition, 
        const Matrix<KalmanFilter::DT>& process_covariance,
        const Matrix<KalmanFilter::DT>& observation, 
        const Matrix<KalmanFilter::DT>& observation_covariance,
        const Matrix<KalmanFilter::DT>& controlling): _state(init_state),
        _state_covariance(state_covariance), _transition(transition),
        _process_covariance(process_covariance), _observation(observation),
        _observation_covariance(observation_covariance), _controlling(controlling) {
        size_t sd = _state.rows();
        assert(_state.cols() == 1);
        assert(_state_covariance.rows() == sd && _state_covariance.cols() == sd);
        assert(sd == _transition.cols() && sd == _transition.rows());
        assert(sd == _process_covariance.cols() && sd == _process_covariance.rows());
        assert(sd == _observation.cols());
        assert(_observation.rows() == _observation_covariance.rows() 
            && _observation.rows() == _observation_covariance.cols());
        if (controlling.rows() * controlling.cols() == 0)
            _have_control = false;
        else {
            _have_control = true;
            assert(sd == _controlling.rows());
        }
    }

    void KalmanFilter::step(const Matrix<DT>& controlling, const Matrix<DT>& observation) {
        predict(controlling);
        observe(observation);
    }

    void KalmanFilter::predict(const Matrix<DT>& controlling) {
        _state = _transition * _state; // x = Ax
        // P = A P A' + Q
        _state_covariance = _transition * _state_covariance * _transition.transpose() + _process_covariance;
        if (_have_control)
            _state = _state + _controlling * controlling;
    }
    void KalmanFilter::observe(const Matrix<DT>& observation){
        // K = P H' (H P H' + R)^-1
        Matrix<DT> K = _state_covariance * _observation.transpose()
                        * (_observation * _state_covariance 
                        * _observation.transpose() + _observation_covariance).reverse();
        // x = x + K (z - H x)
        _state = _state + K * (observation - _observation * _state);
        // P = P - K H P
        _state_covariance = _state_covariance - K * _observation * _state_covariance;
    }

    Matrix<KalmanFilter::DT> KalmanFilter::state() const {
        return _state;
    }

    Matrix<KalmanFilter::DT> KalmanFilter::covariance() const {
        return _state_covariance;
    }

    EKF::EKF(const Matrix<DT>& init_state, const Matrix<DT>& state_covariance,
        std::function<Matrix<DT>(Matrix<DT>, Matrix<DT>)> transition,
        std::function<Matrix<DT>(Matrix<DT>, Matrix<DT>)> transition_derivative,
        const Matrix<DT>& process_covariance,
        std::function<Matrix<DT>(Matrix<DT>)> observe,  
        std::function<Number::Matrix<DT>(Number::Matrix<DT>)> observe_derivative,
        const Matrix<DT>& observation_covariance)
        : _state(init_state), _state_covariance(state_covariance), 
        _transition(transition), _transition_derivative(transition_derivative),
        _process_covariance(process_covariance), _observe(observe),
        _observe_derivative(observe_derivative), 
        _observation_covariance(observation_covariance) {
        size_t sd = _state.rows();
        assert(_state.cols() == 1);
        assert(_state_covariance.rows() == sd && _state_covariance.cols() == sd);
        assert(sd == _process_covariance.cols() && sd == _process_covariance.rows());
    }

    void EKF::step(const Matrix<DT>& controlling, const Matrix<DT>& observation) {
        predict(controlling);
        observe(observation);
    }

    void EKF::predict(const Matrix<DT>& controlling) {
        size_t sd = _state.rows();
        // x = f(x)
        _state = _transition(_state, controlling); 
        assert(_state.rows() == sd && _state.cols() == 1);
        auto F = _transition_derivative(_state, controlling);
        assert(F.rows() == sd && F.cols() == sd);
        // P = F P F' + Q
        _state_covariance = F * _state_covariance 
                        * F.transpose() + _process_covariance;
    }

    void EKF::observe(const Matrix<DT>& observation){
        auto H = _observe_derivative(_state);
        // K = P H' (H P H' + R)^-1
        Matrix<DT> K = _state_covariance * H.transpose() * (H * _state_covariance 
                        * H.transpose() + _observation_covariance).reverse();
        // x = x + K (z - h(x))
        _state = _state + K * (observation - _observe(_state));
        // P = P - K H P
        _state_covariance = _state_covariance - K * H * _state_covariance;
    }

    Matrix<EKF::DT> EKF::state() const {
        return _state;
    }

    Matrix<EKF::DT> EKF::covariance() const {
        return _state_covariance;
    }
} //namespace SigPro