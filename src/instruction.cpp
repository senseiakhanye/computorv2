#include "../computorv.h"

vector<Instruction> Instruction::instructions;

Instruction::Instruction() {
    this->valid = false;
}

Instruction::Instruction(string str) {
    this->splitString(str, '=', this->commands);
    this->isPrint = false;
    if (this->commands.size() != 2) {
        this->valid = false;
        return ;
    }
    this->valid = this->verifyInstruction();
}

Instruction& Instruction::operator=(Instruction const &rhs) {
    this->setInstructionData(rhs);
    return (*this);
}

bool    Instruction::setEquation(vector<string> rhs, string rhs_string) {
    polynomial *equation = new polynomial();
    Validate validator;

    if (!validator.isPolynomialValid(rhs.at(0), equation, *this)) {
        return (false);
    }
    equation->calculate();
    if (equation->counter == 1) {
        tempInstruction = new Instruction();
        tempInstruction->setInstruction(equation->getEquationType());
        if (equation->getEquationType() == VARIABLE || equation->getEquationType() == FUNCTION) {
            tempInstruction->setFloatValue(equation->getTerm(0)->getCorrectValue());
        }
        else {
            tempInstruction->setCommand(rhs_string);
        }
        return (true);
    }
    return (false);
}

bool    Instruction::checkOneValue(vector<string> rhs, string rhs_string) {
    Validate validator;

    if (!rhs.at(0).compare("i") || !rhs.at(0).compare("I")) {
        tempInstruction = new Instruction();
        tempInstruction->setInstruction(IMAGINERY);
        tempInstruction->setCommand("i");
        return (true);
    }
    else if (Validate::isNumeric(rhs.at(0))) {
        tempInstruction = new Instruction();
        tempInstruction->setInstruction(VARIABLE);
        tempInstruction->setFloatValue(atof(rhs.at(0).c_str()));
        return (true);
    }
    else if (!rhs.at(0).compare("?")) {
        tempInstruction = new Instruction();
        this->isPrint = true;
        return (true);
    }
    else if (Validate::isValidVariable(rhs.at(0), false)) {
        cout << "Found variable" << endl;
        return (this->setEquation(rhs, rhs_string));
    }
    else if (validator.foundOperator(rhs.at(0)) || validator.foundMixedTerm(rhs.at(0))) {
        cout << "Mixed term found : " << rhs.at(0) << endl;
        return (this->setEquation(rhs, rhs_string));
    }
    cout << "Got here" << endl;
    return (false);
}

bool    Instruction::checkRightHandSide(vector<string> rhs, bool isFunction, string rhs_str) {
    if (!isFunction) {
        if (rhs.size() == 1) {
            return (this->checkOneValue(rhs, rhs_str));
        }
        else {
            return (this->setEquation(rhs, rhs_str));
        }
    }
    return (false);
}

bool    Instruction::setVariableData(vector<string> rightInstructions, string str, string rhs) {
    tempInstruction = NULL;
    if (!this->checkRightHandSide(rightInstructions, false, rhs)) {
        return (false);
    }
    if (tempInstruction == NULL) {
        return (false);
    }
    if (this->isPrint) {
        if (findInstruction(str) == NULL) {
            return (false);
        }
        else {
            this->setInstructionData(*findInstruction(str));
            this->isPrint = true;
        }
        return (true);
    }
    tempInstruction->setInstructionHead(str);
    if (this->findInstruction(str) == NULL) {
        this->setInstructionData(*tempInstruction);
        instructions.push_back(*this);
        return (true);
    }
    else {
        findInstruction(str)->setInstructionData(*tempInstruction);
        this->setInstructionData(*tempInstruction);
        return (true);
    }
    return (false);
}

bool    Instruction::verifyInstruction() {
    vector<string>  leftInstructions;
    vector<string>  rightInstructions;
    string          str;

    splitString(commands.at(0), ' ', leftInstructions);
    splitString(commands.at(1), ' ', rightInstructions);
    if (leftInstructions.size() == 1) {
        str = leftInstructions.at(0);
        if (!Validate::isValidVariable(str, true) && !Validate::isValidFunction(str)) {
            return (false);
        }
        if (Validate::isValidVariable(str, true)) {
            return (setVariableData(rightInstructions, str, commands.at(1)));
        }
    }
    else if (rightInstructions.size() == 1) {
        if (rightInstructions.at(0).compare("?")) {
            return (false);
        }
    }
    return (true);
}

void    Instruction::setInstructionData(Instruction data) {
    this->value = data.getValue();
    this->floatValue= data.getfloatValue();
    this->instructionType = data.getType();
    this->instruction = data.getInstruction();
    this->command = data.getCommand();
}

string  Instruction::getCommand() const {
    return (this->command);
}

string  trimString(string str) {
    string  temp = str;
    int     startPos = -1;
    int     endPos = str.length();

    while (str[++startPos] && str[startPos] == ' ');
    while (--endPos > 0 && str[endPos] == ' ');
    temp = temp.substr(startPos, (endPos - startPos) + 1);
    return (temp);
}

void	Instruction::splitString(string poly, char deliminator, vector<string> &tempVector) {
	string  temp;
	size_t  pos;

	while ((pos = poly.find(deliminator)) != string::npos) {
        temp = trimString(poly.substr(0, pos));
		poly = poly.substr(pos + 1);
        if (temp.length() > 0)
		    tempVector.push_back(temp);
	}
	tempVector.push_back(trimString(poly));
}

string      Instruction::getValue() const {
    return (this->value);
}

int         Instruction::getType() const {
    return (this->instructionType);
}

void        Instruction::setInstruction(int instructionType) {
    this->instructionType = instructionType;
}

bool        Instruction::isValid() const {
    return (this->valid);
}

bool        Instruction::compareCommand(string command) const {
    int index = -1;
    int len = (int)command.length();

    while (++index < len && index < (int)this->instruction.length()) {
        if (tolower(this->getInstruction()[index]) != tolower(command[index])) {
            return (false);
        }
    }
    return (index == (int)command.length() && index == (int)this->instruction.length());
}

string      Instruction::getInstruction() const {
    return (this->instruction);
}

float       Instruction::getfloatValue() const {
    return (this->floatValue);
}

 void        Instruction::setFloatValue(float val) {
     this->floatValue = val;
 }

Instruction *Instruction::findInstruction(string str) {
    int index = -1;
    int len = (int)instructions.size();

    while (++index < len) {
        if (instructions.at(index).compareCommand(str)) {
            return (&instructions.at(index));
        }
    }
    return (NULL);
}

void        Instruction::showAllInstructions() {
    int index = -1;
    int len = (int)instructions.size();

    while (++index < len) {
        cout << "Instruction : " << instructions.at(index).getInstruction();
        cout << " " << instructions.at(index).getfloatValue() << endl;
    }
}

void        Instruction::setInstructionHead(string head) {
    this->instruction = head;
}

void        Instruction::setCommand(string commandType) {
    this->command = commandType;
}