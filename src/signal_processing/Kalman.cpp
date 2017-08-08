#include "Kalman.h"
#include "number/Matrix.h"

using namespace std;
using namespace Number;

namespace SigPro {
    KalmanFilter::KalmanFilter(const Number::Matrix<DT>& init_state,
        const Number::Matrix<DT>& state_covariance,
        const Number::Matrix<DT>& transition, const Number::Matrix<DT>& process_covariance,
        const Number::Matrix<DT>& observation, const Number::Matrix<DT>& observation_covariance,
        const Number::Matrix<DT>& controlling): _state(init_state),
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

    void KalmanFilter::step(const Number::Matrix<DT>& controlling, const Number::Matrix<DT>& observation) {
        predict(controlling);
        observe(observation);
    }

    void KalmanFilter::predict(const Number::Matrix<DT>& controlling) {
        _state = _transition * _state; // x = Ax
        // P = A P A' + Q
        _state_covariance = _transition * _state_covariance * _transition.transpose() + _process_covariance;
        if (_have_control)
            _state = _state + _controlling * controlling;
    }
    void KalmanFilter::observe(const Number::Matrix<DT>& observation){
        // K = P H' (H P H' + R)^-1
        Matrix<DT> K = _state_covariance * _observation.transpose()
                        * (_observation * _state_covariance 
                        * _observation.transpose() + _observation_covariance).reverse();
        // x = x + K (z - H x)
        _state = _state + K * (observation - _observation * _state);
        // P = P - K H P
        _state_covariance = _state_covariance - K * _observation * _state_covariance;
    }

    Number::Matrix<DT> KalmanFilter::state() const {
        return _state;
    }

    Number::Matrix<DT> KalmanFilter::covariance() const {
        return _state_covariance;
    }
} //namespace SigPro