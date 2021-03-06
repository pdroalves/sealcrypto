#include <stdexcept>
#include "util/computation.h"

using namespace seal;
using namespace std;

namespace seal
{
    namespace util
    {
        FreshComputation::FreshComputation()
        {
        }

        FreshComputation::~FreshComputation()
        {
        }

        Simulation FreshComputation::simulate(const EncryptionParameters &parms)
        {
            return Simulation(parms);
        }

        FreshComputation *FreshComputation::clone()
        {
            return new FreshComputation();
        }

        AddComputation::AddComputation(Computation &input1, Computation &input2)
        {
            input1_ = input1.clone();
            input2_ = input2.clone();
        }

        AddComputation::~AddComputation()
        {
            delete input1_;
            delete input2_;
        }

        Simulation AddComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.add(input1_->simulate(parms), input2_->simulate(parms));
        }

        AddComputation *AddComputation::clone()
        {
            return new AddComputation(*input1_, *input2_);
        }

        AddManyComputation::AddManyComputation(vector<Computation*> inputs)
        {
#ifdef _DEBUG
            if (inputs.empty())
            {
                throw invalid_argument("inputs can not be empty");
            }
#endif
            for (vector<Computation*>::size_type i = 0; i < inputs.size(); ++i)
            {
#ifdef _DEBUG
                if (inputs[i] == nullptr)
                {
                    throw invalid_argument("inputs can not contain null pointers");
                }
#endif
                inputs_.push_back(inputs[i]->clone());
            }
        }

        AddManyComputation::~AddManyComputation()
        {
            for (vector<Computation*>::size_type i = 0; i < inputs_.size(); ++i)
            {
                delete inputs_[i];
            }
        }

        Simulation AddManyComputation::simulate(const EncryptionParameters &parms)
        {
            vector<Simulation> inputs;
            for (vector<Computation*>::size_type i = 0; i < inputs_.size(); ++i)
            {
                inputs.push_back(inputs_[i]->simulate(parms));
            }
            return simulation_evaluator_.add_many(inputs);
        }

        AddManyComputation *AddManyComputation::clone()
        {
            return new AddManyComputation(inputs_);
        }

        SubComputation::SubComputation(Computation &input1, Computation &input2)
        {
            input1_ = input1.clone();
            input2_ = input2.clone();
        }

        SubComputation::~SubComputation()
        {
            delete input1_;
            delete input2_;
        }

        Simulation SubComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.sub(input1_->simulate(parms), input2_->simulate(parms));
        }

        SubComputation *SubComputation::clone()
        {
            return new SubComputation(*input1_, *input2_);
        }

        MultiplyComputation::MultiplyComputation(Computation &input1, Computation &input2)
        {
            input1_ = input1.clone();
            input2_ = input2.clone();
        }

        MultiplyComputation::~MultiplyComputation()
        {
            delete input1_;
            delete input2_;
        }

        Simulation MultiplyComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.multiply(input1_->simulate(parms), input2_->simulate(parms));
        }

        MultiplyComputation *MultiplyComputation::clone()
        {
            return new MultiplyComputation(*input1_, *input2_);
        }

        /*
        MultiplyNoRelinComputation::MultiplyNoRelinComputation(Computation &input1, Computation &input2)
        {
            input1_ = input1.clone();
            input2_ = input2.clone();
        }

        MultiplyNoRelinComputation::~MultiplyNoRelinComputation()
        {
            delete input1_;
            delete input2_;
        }

        Simulation MultiplyNoRelinComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.multiply_norelin(input1_->simulate(parms), input2_->simulate(parms));
        }

        MultiplyNoRelinComputation *MultiplyNoRelinComputation::clone()
        {
            return new MultiplyNoRelinComputation(*input1_, *input2_);
        }

        RelinearizeComputation::RelinearizeComputation(Computation &input)
        {
            input_ = input.clone();
        }

        RelinearizeComputation::~RelinearizeComputation()
        {
            delete input_;
        }

        Simulation RelinearizeComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.relinearize(input_->simulate(parms));
        }

        RelinearizeComputation *RelinearizeComputation::clone()
        {
            return new RelinearizeComputation(*input_);
        }
        */

        MultiplyPlainComputation::MultiplyPlainComputation(Computation &input, int plain_max_coeff_count, const BigUInt &plain_max_abs_value) :
            plain_max_coeff_count_(plain_max_coeff_count), plain_max_abs_value_(plain_max_abs_value)
        {
#ifdef _DEBUG
            if (plain_max_coeff_count <= 0)
            {
                throw invalid_argument("plain_max_coeff_count");
            }
#endif

            input_ = input.clone();
        }

        MultiplyPlainComputation::MultiplyPlainComputation(Computation &input, int plain_max_coeff_count, uint64_t plain_max_abs_value) :
            plain_max_coeff_count_(plain_max_coeff_count)
        {
#ifdef _DEBUG
            if (plain_max_coeff_count <= 0)
            {
                throw invalid_argument("plain_max_coeff_count");
            }
#endif

            BigUInt plain_max_abs_value_uint;
            plain_max_abs_value_uint = plain_max_abs_value;
            plain_max_abs_value_ = plain_max_abs_value_uint;

            input_ = input.clone();
        }

        MultiplyPlainComputation::~MultiplyPlainComputation()
        {
            delete input_;
        }

        Simulation MultiplyPlainComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.multiply_plain(input_->simulate(parms), plain_max_coeff_count_, plain_max_abs_value_);
        }

        MultiplyPlainComputation *MultiplyPlainComputation::clone()
        {
            return new MultiplyPlainComputation(*input_, plain_max_coeff_count_, plain_max_abs_value_);
        }

        AddPlainComputation::AddPlainComputation(Computation &input)
        {
            input_ = input.clone();
        }

        AddPlainComputation::~AddPlainComputation()
        {
            delete input_;
        }

        Simulation AddPlainComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.add_plain(input_->simulate(parms));
        }

        AddPlainComputation *AddPlainComputation::clone()
        {
            return new AddPlainComputation(*input_);
        }

        SubPlainComputation::SubPlainComputation(Computation &input)
        {
            input_ = input.clone();
        }

        SubPlainComputation::~SubPlainComputation()
        {
            delete input_;
        }

        Simulation SubPlainComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.sub_plain(input_->simulate(parms));
        }

        SubPlainComputation *SubPlainComputation::clone()
        {
            return new SubPlainComputation(*input_);
        }

        NegateComputation::NegateComputation(Computation &input)
        {
            input_ = input.clone();
        }

        NegateComputation::~NegateComputation()
        {
            delete input_;
        }

        Simulation NegateComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.negate(input_->simulate(parms));
        }

        NegateComputation *NegateComputation::clone()
        {
            return new NegateComputation(*input_);
        }

        ExponentiateComputation::ExponentiateComputation(Computation &input, uint64_t exponent) : exponent_(exponent)
        {
            input_ = input.clone();
        }

        ExponentiateComputation::~ExponentiateComputation()
        {
            delete input_;
        }

        Simulation ExponentiateComputation::simulate(const EncryptionParameters &parms)
        {
            return simulation_evaluator_.exponentiate(input_->simulate(parms), exponent_);
        }

        ExponentiateComputation *ExponentiateComputation::clone()
        {
            return new ExponentiateComputation(*input_, exponent_);
        }

        MultiplyManyComputation::MultiplyManyComputation(vector<Computation*> inputs)
        {
#ifdef _DEBUG
            if (inputs.empty())
            {
                throw invalid_argument("inputs can not be empty");
            }
#endif
            for (vector<Computation*>::size_type i = 0; i < inputs.size(); ++i)
            {
#ifdef _DEBUG
                if (inputs[i] == nullptr)
                {
                    throw invalid_argument("inputs can not contain null pointers");
                }
#endif
                inputs_.push_back(inputs[i]->clone());
            }
        }

        MultiplyManyComputation::~MultiplyManyComputation()
        {
            for (vector<Computation*>::size_type i = 0; i < inputs_.size(); ++i)
            {
                delete inputs_[i];
            }
        }

        Simulation MultiplyManyComputation::simulate(const EncryptionParameters &parms)
        {
            vector<Simulation> inputs;
            for (vector<Computation*>::size_type i = 0; i < inputs_.size(); ++i)
            {
                inputs.push_back(inputs_[i]->simulate(parms));
            }
            return simulation_evaluator_.multiply_many(inputs);
        }

        MultiplyManyComputation *MultiplyManyComputation::clone()
        {
            return new MultiplyManyComputation(inputs_);
        }
    }
}