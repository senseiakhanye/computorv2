#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "../computorv.h"

class Functions {
    private:
        string      variableName;
        polynomial  *equation;
        float       tempValue;

    public:
        Functions();
        void        setVariableName(string variableName);
        void        setEquation(polynomial *equation);
        string      getVariableName() const;
        string      toString();
        float       getFunctionValue(float paramater);
};

#endif